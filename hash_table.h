/**
 * Celina Wong ECE361 HW #4
 * hash_table.h - Hash table ADT header file
 *
 * @brief   This is the header file for constants, variables, structs, and
 * function prototypes for functionality in the Hash table ADT.
*/

#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

// constants
#define NUM_MLS_EAST_TEAMS  15
#define NUM_MLS_WEST_TEAMS  15
#define NUM_NWSL_TEAMS      15
#define NUM_TEAMS           (NUM_MLS_EAST_TEAMS + NUM_MLS_WEST_TEAMS + NUM_NWSL_TEAMS)

#define HASH_TABLE_SIZE		(NUM_TEAMS * 2)
#define HT_PRIME_1			151
#define HT_PRIME_2			193

#define MAX_CONF_NAME       10
#define MAX_CITY_NAME       15
#define MAX_TEAM_NAME       25


// Debug constants
// This is a good way to compile debug code into your program as needed and then
// remove it when you have everything working.
// Be sure you use {} in pieces of code that make use of the approach
#define _DEBUG_ 0		// > 0 compiles in the debug code.

// define conference enum
typedef enum _conf_e {NWSL, EAST, WEST} conf_t;

// define team info struct
typedef struct _TeamInfo_s {
  char    conf[MAX_CONF_NAME + 1];    // conference - leave room for \0 at end of string
  char    city[MAX_CITY_NAME + 1];    // city - leave room for \0 at end of string
	char 		name[MAX_TEAM_NAME + 1];    // team name - leave room for \0 at end of string
	int			pts;
	int			win;
	int			loss;
	int			tie;
  int     gd;
} TeamInfo_t, *TeamInfoPtr_t;

// struct containing key:value (k:v) pairs
typedef struct ht_item {
  char* key;
  void* value;
} ht_item;

// struct containing the hash table
typedef struct {
  int size;
  int count;
  ht_item** items;
} ht_hash_table;


// API function prototypes

// creates a new hash table
ht_hash_table* ht_new(void);

// deletes a hash table
void ht_del_hash_table(ht_hash_table* ht);

// inserts element into hash table
void ht_insert(ht_hash_table* ht, const char* key, void* value);

// searches for element in the hash table
void* ht_search(ht_hash_table* ht, const char* key);

// deletes an element from the hash table
void ht_delete(ht_hash_table* ht, const char* key);

// displays the entire hash table on stdout
void ht_dump(ht_hash_table* ht);

#endif
