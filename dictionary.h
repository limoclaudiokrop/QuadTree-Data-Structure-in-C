/*
 Data structure declarations and prototypes for a
 dictionary module intended to allow lookup
 for particular values.
*/
#include "record_struct.h"
#include <stdio.h>
#define LOOKUPSTAGE 1
#define PROXIMITYSTAGE 2
/* Result of a query. */
struct queryResult;
/* Dictionary. */
struct dictionary;
/* Returns an empty dictionary. */
struct dictionary *newDict();
/* Insert a given record into the dictionary. */
void insertRecord(struct dictionary *dict, struct csvRecord
*record);
/* Search for a given key in the dictionary. */
struct queryResult *lookupRecord(struct dictionary *dict, char
*query);
/* Search for the closest record in the dictionary to the query
string in the given
 field index. Assumes the field selected is double type. */
struct queryResult *searchClosestDouble(struct dictionary *dict,
char *query,
 int fieldIndex);
/* Output the given query result.
 If stage is LOOKUPSTAGE, information retrieved by the
lookupRecord
 function can be printed.

 If stage is PROXIMITYSTAGE, information retrieved by the
searchClosestDouble
 function can be printed.

 The formats of these print outs is described in the COMP20003
Assignment 1
 specification where summaryFile refers to stdout and
outputFile refers
 to the output file.
*/
void printQueryResult(struct queryResult *r, FILE *summaryFile,
 FILE *outputFile, int stage);
/* Free the given query result. */
void freeQueryResult(struct queryResult *r);
/* Free a given dictionary. */
void freeDict(struct dictionary *dict);