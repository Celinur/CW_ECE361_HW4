/**
 * hash_table.c - Hash table ADT source code file
 *
 * @brief   This is the source code file for the functionality in the Hash
 * table ADT
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"

// constants, typedefs and global variables
static ht_item HT_DELETED_ITEM = {NULL, NULL};

// prototypes for the Helper functions

// create a new element for the hash table
static ht_item* ht_new_item(const char* k, void*  v);

// delete an element from the hash table
static void ht_del_item(ht_item* i);

  // generic hash function
static int ht_generic_hash(const char* s, const int a, const int m);

// actual hash function
static int ht_get_hash(const char* s, const int num_buckets, const int attempt);

// Hash Table ADT

/**
 * ht_new() - initializes a new Hash table
 *
 * Allocates space for a new hash table.  The size of the array is fixed in
 * this implementation
 *
 * @return a pointer to the new hash table
 *
 * @note We initialize the array of items with `calloc`, which fills the allocated memory
 * with `NULL` bytes. A `NULL` entry in the array indicates that the bucket is empty.
 */
ht_hash_table* ht_new(void) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
	if (ht == NULL) {
		#if (_DEBUG_ > 0)
			fprintf(stderr,
				"ERROR(ht_new()): Could not allocate space for hash table\n");
		#endif
		return NULL;
	}

	// allocated space for hash table, now get space for all of the elements in the hash table
  ht->size = HASH_TABLE_SIZE;
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
	if (ht->items == NULL) {
		#if (_DEBUG_ > 0)
			fprintf(stderr,
				"ERROR(ht_new()): Could not allocate elements for the hash table\n");
		#endif
		return NULL;
	}
  return ht;
}


/**
 * ht_del_hash_table() - deletes an entire hash table
 *
 * Deletes all of the elements in the hash table and frees up their memory
 * After the elements are deleted the table, itself is deleted
 *
 * @param	ht is a pointer to the Hash table that should be deleted
 *
 */
void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if ((item != NULL) && (item != &HT_DELETED_ITEM)) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}


/**
 * ht_insert() - insert a new key-value pair into hash table
 *
 * To insert a new key-value pair, we iterate through indexes until we find an empty bucket.
 * We then insert the item into that bucket and increment the hash table's
 * `count` attribute, to indicate a new item has been added.
 *
 * @param ht is a pointer to a Hash table created with the ht_new() function
 * @param key is a pointer to a string containing the key
 * @param value is a void pointer to what we want to insert into the hash table
 *
 */
void ht_insert(ht_hash_table* ht, const char* key, void* value) {
  ht_item* item = ht_new_item(key, value);
  int index = ht_get_hash(item->key, ht->size, 0);
  ht_item* cur_item = ht->items[index];
  int i = 1;

  while (cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
	if (strcmp(cur_item->key, key) == 0) {  // support updating keys
    ht->items[index] = item;
    return;
  }
  index = ht_get_hash(item->key, ht->size, i);
  cur_item = ht->items[index];
  i++;
  }
ht->items[index] = item;

	#if (_DEBUG_ > 0)
		fprintf(stderr,
			"\tINFO(ht_insert()): Inserted hash table[%d], k:v = %s:%p\n",
			index, item->key, item->value);
	#endif

  ht->count++;
}


/**
 * ht_search() - search the hash table for an element with the specified key
 *
 * Searching is similar to inserting, but at each iteration of the `while` loop,
 * we check whether the item's key matches the key we're searching for. If it does,
 * we return the item's value. If the while loop hits a `NULL` bucket, we return`NULL`
 * to indicate that no value was found.
 *
 * @param ht is a pointer to a Hash table created with the ht_new() function
 * @param key is a pointer to a string containing the key
 *
 * @return value as a generic pointer.  Should be cast to the type of the return data
 */
void* ht_search(ht_hash_table* ht, const char* key) {
	int index;
	ht_item* item;
	int i;

	#if (_DEBUG_ > 0)
		fprintf(stderr,
			"\tINFO(ht_search()): Searching for key: %s\n",
			key);
	#endif

  index = ht_get_hash(key, ht->size, 0);
  item = ht->items[index];
	i = 1;
  while (item != NULL && item != &HT_DELETED_ITEM) {
    if (strcmp(item->key, key) == 0) {
		    #if (_DEBUG_ > 0)
				fprintf(stderr,
					"\tINFO(ht_search()): Found key %s in bucket: %d at address %p\n",
					key, index, item->value);
			  #endif
			  return item->value;
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }

	#if (_DEBUG_ > 0)
		fprintf(stderr,
			"\tINFO(ht_search()): key: %s is not in the hash table\n",
			key);
		#endif
    return NULL;
}


/* ht_delete() - delete an element from the hash table
 *
 * Deleting from an open addressed hash table is more complicated than inserting
 * or searching. The item we wish to delete may be part of a collision chain.
 * Removing it from the table will break that chain, and will make finding items in
 * the tail of the chain impossible.
 *
 * To solve this, instead of deleting the item, we simply mark it as deleted.
 * We mark an item as deleted by replacing it with a pointer to a global sentinel
 * item which represents that a bucket contains a deleted item.
 *
 * @param ht is a pointer to a Hash table created with the ht_new() function
 * @param key is a pointer to a string containing the key
 *
 */
void ht_delete(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;

    while (item != NULL && item != &HT_DELETED_ITEM) {
        if (strcmp(item->key, key) == 0) {
            ht_del_item(item);
            ht->items[index] = &HT_DELETED_ITEM;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
}


/**
 * ht_dump() - dumps (lists) the entire hash table to the console
 *
 * Traverses the entire hash table displaying the key and value for
 * every occupied bucket in the hash table.  Empty ('.') and deleted ("delete")
 * buckets are noted
 *
 * @param	ht is a pointer to the Hash table that should be deleted
 *
 */
void ht_dump(ht_hash_table* ht) {
	printf("Hash table contains:\n");
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item == NULL) {
            printf(".");
        }
		else if (item == &HT_DELETED_ITEM) {
			printf("\n\tHash Table[%02d] has been deleted\n", i);
		}
		else {
			printf("\n\tHash Table[%02d] has k:v = %s:%p", i, item->key, item->value);
		}
    }
	printf("\n");
}



// Helper functions

/**
 * ht_new_item() - allocate a new element for the hash table
 *
 * Allocates a chunk of memory the size of an `ht_item` and saves a copy of the
 * key and value pair in the new chunk of memory.
 *
 * @param k is a pointer to the string containing the key for the element
 * @param v is a pointer to a team Info record
 *
 * @return a pointer to the new element
 *
 * @note The function is declared `static` because it will only be called by code internal to the hash table.
 */
static ht_item* ht_new_item(const char* k, void*  v) {
  ht_item* i = malloc(sizeof(ht_item));
	if (i == NULL) {
		#if (_DEBUG_ > 0)
		fprintf(stderr,
			"ERROR (ht_item()):  Could not allocate a new item\n");
		#endif
		return NULL;
	}

	// alas, if only there was a strdup() function in the string library..do this instead
	char* d = malloc(strlen(k) + 1);
	i->key = strcpy(d, k);

  i->value = v;
  return i;
}



/**
 * ht_del_item() - deletes an element from the hash table
 *
 * Deletes the specified element from the hash table.  Frees up the memory
 * for the key and the value and the hash+item.
 *
 * @param	i is a pointer to the ht_item that should be deleted
 *
 * @note The function is declared `static` because it will only be called by code internal to the hash table
 */
static void ht_del_item(ht_item* i) {
  free(i->key);
  free(i->value);
  free(i);
}


/**
 * ht_generic_hash() -  generic Hash function
 *
 * Applies a generic hash function for character strings.  Produces an index into
 * the Hash table.  The algorithm works by creating a large integer from scanning each
 * character in the string and then using the modulus (%) operator to reduce the number so that it fits into the table.
 *
 * @param s is the key for the hash table element
 * @param a is a prime number larger than the size of the alphabet (> 128).
 * @param m is the number of elements (the size) in the Hash table
 *
 * @return the value calculated by the function.  This generic function is used
 * in this implementation for generating the primary hash and the secondary hash that is
 * used if the first hash results in collision.
 *
 */
static int ht_generic_hash(const char* s, const int a, const int m) {
  long hash = 0;

  const int len_s = strlen(s);
  for (int i = 0; i < len_s; i++) {
      hash += (long)pow(a, len_s - (i+1)) * s[i];
      hash = hash % m;
  }
  return (int)hash;
}


/**
 * ht_get_hash() -  gets the index for a key by employing a double hashing algorithm
 *
 * The hashing function that returns the index into the hash table.  Collision handling
 * is handled by double hashing.  That is, a secondary hashing function is used to select
 * the next bucket in the hash table for that index if applying the first hashing
 * function results in a collision.
 *
 * @param s is the key for the Hash table element
 * @param num_buckets is the number of buckets in the Hash table
 * @param attempt is the number of attempts to find the correct bucket for the key
 *
 * @returns the value calculated by the function.
 *
 */
static int ht_get_hash(const char* s, const int num_buckets, const int attempt) {
    const int hash_a = ht_generic_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_generic_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}
