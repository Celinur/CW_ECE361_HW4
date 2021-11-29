/**
 * soccerApp.c - Test program for Hash table ADT
 *
 * @author  Celina Wong (wcelina@pdx.edu)
 * @date    21-Nov-2021
 *
 * @brief  This is the main file for implementing an application that uses
 * hash tables to retrieve information from a database of team information.
 * The user should be prompted to enter a conference and team nickname,
 * and the program should look up the record in a hash table and display the
 * keys for all the conference entries in the table (conference, city, name,
 * points, wins, losses, ties, and goal differential).
 *
 * Information for this database will come from a text file called
 * soccer2021.csv that this program will read. A "helper" function in
 * appHelpers.c will parse a line from the file and place the fields in a
 * struct that this code will operate on.
 *
 * @requirements
 * - The program should loop and prompt the user for additional
 * record to look up until the user enters an empty line (Enter key), or 'q'
 * for the conference.
 * - If user enters '?', the program does an ht_dump() to display the keys
 * for all the conference entries in the hash table.
 * - If user enters an invalid conference and/or city, the program should
 * notify that there is no team info for their selected team.
 *
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
//#include <stdbool.h>

#include "hash_table.h"
#include "appHelpers.h"

int main(){
	TeamInfoPtr_t tir;						// pointers to a Team Info records
  TeamInfoPtr_t tir1;

	char* key = "";							// key for hash table entry
//  char key2 = "";
	ht_hash_table* teams_ht;				// hash table

  //Introduction
	printf("\nHash Table ADT test program (by Celina Wong, 21-Nov-2021)\n\n");
    errno = 0;
    char *buf = getcwd(NULL, 0);    // allocates a buffer large enough to hold the path
    if (buf == NULL) {
        perror("getcwd");
        printf("Could not display the path\n");
    }
    else {
        printf("Current working directory: %s\n", buf);
        free(buf);
    }
    printf("\n");

	// create a hash table
	teams_ht = ht_new();
	if (teams_ht != NULL) {
		printf("\nCreating a new hash table...\n");
	}
	else {
		printf("\nERROR: Could not create a new hash table\n");
		exit(1);
	}

  // insert team info records in the hash table
	printf("\nInserting Team Info records into hash table...\n");

  char line[200];   //to read each row as a string in .csv file
  //open .csv file
  FILE *fp = fopen("soccer2021.csv", "r");
  if (fp == NULL) { //if error in opening file
    printf("Cannot open file.\n");
    exit(1);
  }

  //parse .csv file and put into hash table
  fgets(line, sizeof(line),fp); //get first line

  while ((fgets(line, sizeof(line), fp)) != NULL) {  //loop until end of file
    key++;    //increment key space
    tir1 = malloc(sizeof(TeamInfo_t));
    tir = parseTeamInfo(line);  //parse the line
    memcpy(tir1, tir, sizeof(TeamInfo_t));
    key = createKey(tir1);  //create key for each team
    ht_insert(teams_ht, key, tir1); //insert into hash table
  //  ht_dump(teams_ht);  //just to see what's happening in here
  }

  fclose(fp); //after contents are put into hash table, close file

  //prompt and scan user input
  char user_conf[100];   //user input conference
  char user_city[100];  //user input city

  printf("\nMenu:\n\n");
  printf("Enter '?' to display the keys for all the conference entires.\n");
  printf("Enter 'q' to quit the program at any time.\n");
  printf("\nEnter a conference (NWSL, EAST, or WEST): ");
  fflush(stdout);
  fgets(user_conf, 100, stdin);
  user_conf[strlen(user_conf) - 1]='\0';


  //if user enters "q", exit program
  if (strcmp(user_conf, "q") == 0) {
    printf("Exiting program.  Celina Wong (wcelina@pdx.edu)");
    exit(1);
  }
  else if (strcmp(user_conf, "?") == 0) { //display hash table if user enters '?'
    ht_dump(teams_ht);
  }
  else {
    printf("Enter a city: ");
    fflush(stdout);
    fgets(user_city, 100, stdin);
    user_city[strlen(user_city) - 1]='\0';

    for(;;) { //loop prompt for additional records
      key++;
      strcat(user_city, user_conf); //create key for user input
      strcpy(key, user_city);
      strUpper(key);
      printf("\nSearching hash table for %s\n", key);
      tir = (TeamInfoPtr_t) ht_search(teams_ht, key); //search hash table
      printTeamInfo(tir); //print out selected team info

      //repeat prompt
      printf("\nEnter another conference and team nickname.\n");
      printf("\nConference (NWSL, EAST, or WEST): ");
      fflush(stdout);
      fgets(user_conf, 100, stdin);
      user_conf[strlen(user_conf)-1]='\0';

      //if user wants to exit loop
      if (strcmp(user_conf, "q") == 0) {
        printf("\n\nExiting program.  Celina Wong (wcelina@pdx.edu)\n");
        fflush(stdout);
        exit(0);
      }

      printf("City: ");
      fflush(stdout);
      user_city[strlen(user_city) - 1]='\0';

    }
  }

  printf("\nHash table dump is complete.  Celina Wong (wcelina@pdx.edu)\n");

  exit(0);
}
