/**
 * appHelpers.c - Source code for MLS hash table helper functions
 *
 * @brief   This is the source code file for "helper" functions for the hash
 * table-based MLS app assignment (ECE 361 HW #4)
*/


 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <ctype.h>

 #include "hash_table.h"
 #include "appHelpers.h"

 /**
 * parseTeamInfo() - parses a buffer to create a Team Info record
 *
 * @param	buf			buffer to parseTeamInfo
 * @return	pointer to a TeamInfo struct
 *
 */
 TeamInfoPtr_t parseTeamInfo(char *buf){
	static TeamInfo_t   info;		// will point to a team info record
	int                 numFields;	// number of fields parsed by scanf

	// comments in the file contain // and should be displayed but not parsed
	if ((strstr(buf, "//") != NULL))  {
			printf("%s\n", buf);
			return NULL;
	}
	else {
		// parse the buffer
		if (strlen(buf) > 0) {
			numFields = sscanf(buf, "%[^,],%[^,], %[^,], %d, %d, %d, %d, %d",
				info.conf, info.city, info.name, &info.pts, &info.win,
				&info.loss, &info.tie, &info.gd);
        //printf(buf);  //to check if items are being parsed
			if (numFields != NUMTEAMINFOFIELDS) {
				// parsing error
				printf("ERROR: Could not parse record.");
				printf("\tNumber of fields parsed = %d\n%s\n", numFields, buf);
				return NULL;
			}
			else {
				#if (_DEBUG_ > 1)
					printTeamInfo(&info);
				#endif
				return &info;
			}
		}
	}
	return NULL;
 }

 /**
 * printTeamInfo() - prints a Team Info record
 *
 * @param	TeamInfo record to display.
 *
 */
 void printTeamInfo(TeamInfoPtr_t teamInfo) {
	if (teamInfo != NULL) {
		printf("\n");
		printf("Team Information for the %s:\n", teamInfo->name);
		printf("\tConference: %s\n", teamInfo->conf);
		printf("\tCity: %s\n", teamInfo->city);
		printf("\tRecord (w-l-t): %d-%d-%d\n", teamInfo->win, teamInfo->loss,teamInfo->tie);
    printf("\tGoal differential: %d\n", teamInfo->gd);
	}
	else { //if no team info for selected conference
		printf("ERROR: No team information for the selected team in conference.\n");
	}
	printf("----------------------\n");
}

/**
 * createKey() - Creates a key for a team info record.
 *
 * The function generate a key for a key:value pair that will be inserted into a hash table
 * This is for ECE 361 HW #4.   The key is the concatenation of the city and the
 * conference the team plays in (ex: PORTLANDWEST for the Portland Timbers of the
 * Western conference)
 *
 * @param name      (pointer to a team info record
 *
 * @returns         a pointer to a character string containing the key for a hash table
 */
char* createKey(TeamInfoPtr_t teamInfoPtr) {
	char *key = malloc(MAX_CONF_NAME + MAX_CITY_NAME + 1); // string for key

	strcpy(key, "");
	strcpy(key, teamInfoPtr->city);
	strcat(key, teamInfoPtr->conf);
	strUpper(key);

	#if (_DEBUG_ > 0)
		fprintf(stderr,
			"INFO(createKey()): Generated key is %s\n\n",
			key);
	#endif

	return key;
}

 /**
 * strUpper() - converts the characters of a string to uppercase
 *
 * @param	char* is a pointer to the character string to convert
 *
 * @note  Based on, but not exactly the same as
 * https://stackoverflow.com/questions/33086007/c-string-to-uppercase-in-c-and-c
 *
 */
char* strUpper(char* str) {
    while (*str != '\0')
    {
        *str = (char) toupper((int) *str);
        str++;
    }
    return str;
}
