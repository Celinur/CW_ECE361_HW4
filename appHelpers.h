/**
 * appHelpers.h - Header file for MLS hash table helper functions
 *
 * @brief   This is the header file for "helper" functions for the hash
 * table-based MLS app assignment (ECE 361 HW #4)
*/

#ifndef _APPHELPERS_H
#define _APPHELPERS_H

#include <stdbool.h>
#include "hash_table.h"

// define constants
#define NUMTEAMINFOFIELDS 8	

// function prototypes
TeamInfoPtr_t parseTeamInfo(char *buf);
void printTeamInfo(TeamInfoPtr_t teamInfo);
char* createKey(TeamInfoPtr_t teamInfoPtr);
char* strUpper(char* str);

#endif
