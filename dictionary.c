/*
 Data structure and function implementations for a
 dictionary module intended to allow lookup
 for particular values.
*/
#include "dictionary.h"
#include "record_struct.h"
#include "record_struct.c"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define NUMERIC_BASE 10
#define KEY_FIELD 1
#define NOTFOUND "NOTFOUND"
#define NOTDOUBLE (-1)
#define MAXPRECISION (-2)
#define NUM_FIELDS 19
#define FIELDLOOKUPFAILURE (-1)
#define INDEXINITIAL 1
// Field names by index.
static char *fieldNames[] = {"footpath_id", "address", "clue_sa",
 "asset_type", "deltaz", "distance", "grade1in", "mcc_id",
"mccid_int",
 "rlmax", "rlmin", "segside", "statusid", "streetid",
"street_group",
 "start_lat", "start_lon", "end_lat", "end_lon"};
/*
Field double precision by index. -1 signifies that the value should
not
be printed as a double value.
*/
static int fieldPrecision[] = {NOTDOUBLE, NOTDOUBLE, NOTDOUBLE,
 NOTDOUBLE, 2, 2, 1, NOTDOUBLE, NOTDOUBLE,
 2, 2, NOTDOUBLE, NOTDOUBLE, NOTDOUBLE, NOTDOUBLE,
 MAXPRECISION, MAXPRECISION, MAXPRECISION, MAXPRECISION};
struct queryResult {
 char *searchString;
 int numRecords;
 double closestValue;
 struct data **records;
};
/* CSV records. */
struct data;
struct data {
 int footpath_id;
 char *address;
 char *clue_sa;
 char *asset_type;
 double deltaz;
 double distance;
 double grade1in;
 int mcc_id;
 int mccid_int;
 double rlmax;
 double rlmin;
 char *segside;
 int statusid;
 int streetid;
 int street_group;
 double start_lat;
 double start_lon;
 double end_lat;
 double end_lon;
};
/* A node in the dictionary, used to allow quick lookup. */
struct dictionaryNode;
struct dictionaryNode{
 struct data *record;
 struct dictionaryNode *next;
};
struct index;
struct index {
 int valueCount;
 struct dictionaryNode **nodes;
};
/* Dictionary - concrete data structure is linked list. */
struct dictionary {
 struct dictionaryNode *head;
 struct dictionaryNode *tail;
 struct index **indices;
};
/* Reads a given string as an integer and returns the integer. */
int readIntField(char *fieldString);
/* Reads a given string as a string and returns a copy of the
string. */
char *readStringField(char *fieldString);
/* Reads a given string as a double value and returns the double
value. */
double readDoubleField(char *fieldString);
int readIntField(char *fieldString){
 return strtol(fieldString, NULL, NUMERIC_BASE);
}
char *readStringField(char *fieldString){
 char *str = strdup(fieldString);
 assert(str);
 return str;
}
double readDoubleField(char *fieldString){
 return strtod(fieldString, NULL);
}
/* Sets the relevant field. */
void setField(struct data *record, int fieldIndex, char *fieldVal);
void setField(struct data *record, int fieldIndex, char *fieldVal){
 switch(fieldIndex){
 case 0:
 record->footpath_id = readIntField(fieldVal);
 break;
 case 1:
 record->address = readStringField(fieldVal);
 break;
 case 2:
 record->clue_sa = readStringField(fieldVal);
 break;
 case 3:
 record->asset_type = readStringField(fieldVal);
 break;
 case 4:
 record->deltaz = readDoubleField(fieldVal);
 break;
 case 5:
 record->distance = readDoubleField(fieldVal);
 break;
 case 6:
 record->grade1in = readDoubleField(fieldVal);
 break;
 case 7:
 record->mcc_id = readIntField(fieldVal);
 break;
 case 8:
 record->mccid_int = readIntField(fieldVal);
 break;
 case 9:
 record->rlmax = readDoubleField(fieldVal);
 break;
 case 10:
 record->rlmin = readDoubleField(fieldVal);
 break;
 case 11:
 record->segside = readStringField(fieldVal);
 break;
 case 12:
 record->statusid = readIntField(fieldVal);
 break;
 case 13:
 record->streetid = readIntField(fieldVal);
 break;
 case 14:
 record->street_group = readIntField(fieldVal);
 break;
 case 15:
 record->start_lat = readDoubleField(fieldVal);
 break;
 case 16:
 record->start_lon = readDoubleField(fieldVal);
 break;
 case 17:
 record->end_lat = readDoubleField(fieldVal);
 break;
 case 18:
 record->end_lon = readDoubleField(fieldVal);
 break;
 default:
 fprintf(stderr, "%d: Unhandled field number %d\n",
__LINE__,
 fieldIndex);
 assert(fieldIndex >= 0 && fieldIndex < NUM_FIELDS);
 }
}
/* Returns the double value for the given field index. */
double getDoubleVal(struct data *record, int fieldIndex);
double getDoubleVal(struct data *record, int fieldIndex){
 switch(fieldIndex){
 case 4:
 return record->deltaz;
 break;
 case 5:
 return record->distance;
 break;
 case 6:
 return record->grade1in;
 break;
 case 9:
 return record->rlmax;
 break;
 case 10:
 return record->rlmin;
 break;
 case 15:
 return record->start_lat;
 break;
 case 16:
 return record->start_lon;
 break;
 case 17:
 return record->end_lat;
 break;
 case 18:
 return record->end_lon;
 break;
 case 0:
 case 1:
 case 2:
 case 3:
 case 7:
 case 8:
 case 11:
 case 12:
 case 13:
 case 14:
 fprintf(stderr, "%d: Unhandled non-double field number
%d\n", __LINE__,
 fieldIndex);
 assert(0);
 break;
 default:
 fprintf(stderr, "%d: Unhandled field number %d\n",
__LINE__,
 fieldIndex);
 assert(fieldIndex >= 0 && fieldIndex < NUM_FIELDS && 0);
 }
 return FIELDLOOKUPFAILURE;
}
/* Prints a given value. */
void printIntField(FILE *f, int value);
/* Prints a given value. */
void printStringField(FILE *f, char *value);
/* Prints a given value. */
void printDoubleField(FILE *f, double value, int fieldPrecision);
void printIntField(FILE *f, int value){
 fprintf(f, "%d", value);
}
void printStringField(FILE *f, char *value){
 fprintf(f, "%s", value);
}
void printDoubleField(FILE *f, double value, int fieldPrecision){
 fprintf(f, "%.*f", fieldPrecision, value);
}
/* Prints the relevant field. */
void printField(FILE *f, struct data *record, int fieldIndex);
void printField(FILE *f, struct data *record, int fieldIndex){
 switch(fieldIndex){
 case 0:
 printIntField(f, record->footpath_id);
 break;
 case 1:
 printStringField(f, record->address);
 break;
 case 2:
 printStringField(f, record->clue_sa);
 break;
 case 3:
 printStringField(f, record->asset_type);
 break;
 case 4:
 printDoubleField(f, record->deltaz,
fieldPrecision[fieldIndex]);
 break;
 case 5:
 printDoubleField(f, record->distance,
fieldPrecision[fieldIndex]);
 break;
 case 6:
 printDoubleField(f, record->grade1in,
fieldPrecision[fieldIndex]);
 break;
 case 7:
 printIntField(f, record->mcc_id);
 break;
 case 8:
 printIntField(f, record->mccid_int);
 break;
 case 9:
 printDoubleField(f, record->rlmax,
fieldPrecision[fieldIndex]);
 break;
 case 10:
 printDoubleField(f, record->rlmin,
fieldPrecision[fieldIndex]);
 break;
 case 11:
 printStringField(f, record->segside);
 break;
 case 12:
 printIntField(f, record->statusid);
 break;
 case 13:
 printIntField(f, record->streetid);
 break;
 case 14:
 printIntField(f, record->street_group);
 break;
 case 15:
 printDoubleField(f, record->start_lat,
fieldPrecision[fieldIndex]);
 break;
 case 16:
 printDoubleField(f, record->start_lon,
fieldPrecision[fieldIndex]);
 break;
 case 17:
 printDoubleField(f, record->end_lat,
fieldPrecision[fieldIndex]);
 break;
 case 18:
 printDoubleField(f, record->end_lon,
fieldPrecision[fieldIndex]);
 break;
 default:
 fprintf(stderr, "%d: Unhandled field number %d\n",
__LINE__,
 fieldIndex);
 assert(fieldIndex >= 0 && fieldIndex < NUM_FIELDS);
 }
}
struct dictionary *newDict(){
 struct dictionary *ret = (struct dictionary *)
 malloc(sizeof(struct dictionary));
 assert(ret);
 ret->head = NULL;
 ret->tail = NULL;
 ret->indices = NULL;
 return ret;
}
/* Read a record into a typed data structure. */
struct data *readRecord(struct csvRecord *record);
struct data *readRecord(struct csvRecord *record){
 struct data *ret = (struct data *) malloc(sizeof(struct data));
 assert(ret);
 assert(record->fieldCount == NUM_FIELDS);
 for(int i = 0; i < NUM_FIELDS; i++){
 setField(ret, i, record->fields[i]);
 }
 return ret;
};
void insertRecord(struct dictionary *dict, struct csvRecord *record)
{
 if(! dict){
 return;
 }
 struct dictionaryNode *newNode = (struct dictionaryNode *)
 malloc(sizeof(struct dictionaryNode));
 assert(newNode);
 newNode->record = readRecord(record);
 newNode->next = NULL;
 if(! (dict->head)){
 /* First insertion, insert new node as head and tail. */
 dict->head = newNode;
 dict->tail = newNode;
 } else {
 /* Insert at tail. */
 dict->tail->next = newNode;
 dict->tail = newNode;
 }
}
/* Search for a given key in the dictionary. */
struct queryResult *lookupRecord(struct dictionary *dict, char
*query){
 int numRecords = 0;
 struct data **records = NULL;
 /* Iterate over all records and collect all matching records. */
 struct dictionaryNode *current = dict->head;
 while(current){
 if(strcmp(current->record->address, query) == 0){
 /* NB: Here we use a time inefficient algorithm as we
assume
 the number of records matching is likely to be
small. May fix
 later. */
 /* Match. */
 records = (struct data **) realloc(records,
 sizeof(struct data *) * (numRecords + 1));
 assert(records);
 records[numRecords] = current->record;
 numRecords++;
 }
 current = current->next;
 }
 struct queryResult *qr = (struct queryResult *)
 malloc(sizeof(struct queryResult));
 assert(qr);
 qr->searchString = strdup(query);
 assert(qr->searchString);
 qr->numRecords = numRecords;
 qr->records = records;
 return qr;
}
/* Output the given query result. */
void printQueryResult(struct queryResult *r, FILE *summaryFile,
 FILE *outputFile, int stage){
 /* Print summary. */
 if(stage == LOOKUPSTAGE){
 if(r->numRecords == 0){
 fprintf(summaryFile, "%s --> %s\n", r->searchString,
NOTFOUND);
 } else {
 fprintf(summaryFile, "%s --> %d\n", r->searchString, r-
>numRecords);
 }
 } else {
 assert(stage == PROXIMITYSTAGE);
 if(r->numRecords == 0){
 fprintf(summaryFile, "%s --> %s\n", r->searchString,
NOTFOUND);
 } else {
 fprintf(summaryFile, "%s --> %.1f\n", r->searchString,
r->closestValue);
 }
 }
 /* Print details. */
 fprintf(outputFile, "%s\n", r->searchString);
 for(int i = 0; i < r->numRecords; i++){
 fprintf(outputFile, "--> ");
 for(int j = 0; j < NUM_FIELDS; j++){
 fprintf(outputFile, "%s: ", fieldNames[j]);
 printField(outputFile, r->records[i], j);
 fprintf(outputFile, " || ");
 }
 fprintf(outputFile, "\n");
 }
}
/* Builds an index for the given (double) field in the data
structure. */
struct index *buildDoubleIndex(struct dictionary *dict, int
fieldIndex);
struct index *buildDoubleIndex(struct dictionary *dict, int
fieldIndex){
 struct index *index = (struct index *) malloc(sizeof(struct
index));
 assert(index);
 int valueCount = 0;
 struct dictionaryNode **nodes = NULL;

 int nodeSpace = 0;
 nodes = (struct dictionaryNode **) malloc(sizeof(struct
dictionaryNode *) * INDEXINITIAL);
 assert(nodes);
 nodeSpace = INDEXINITIAL;
 /* Insertion sort. */
 struct dictionaryNode *current = dict->head;
 while(current){
 if(valueCount == nodeSpace){
 nodeSpace *= 2;
 nodes = (struct dictionaryNode **) realloc(nodes,
 sizeof(struct dictionaryNode *) * nodeSpace);
 assert(nodes);
 }
 nodes[valueCount] = current;
 for(int j = valueCount - 1; j >= 0; j--){
 if(getDoubleVal(current->record, fieldIndex) <
getDoubleVal(nodes[j]->record, fieldIndex)){
 nodes[j + 1] = nodes[j];
 nodes[j] = current;
 } else {
 /* Sorted. */
 break;
 }
 }
 valueCount++;
 current = current->next;
 }
 index->valueCount = valueCount;
 index->nodes = nodes;
 return index;
}
/* Searches for the closest value using the index built for the
field. If an index
 has not yet been built, it is */
struct queryResult *searchClosestDouble(struct dictionary *dict,
char *query,
 int fieldIndex){
 if(! dict->indices){
 dict->indices = (struct index **) malloc(sizeof(struct index
*) * NUM_FIELDS);
 assert(dict->indices);
 for(int i = 0; i < NUM_FIELDS; i++){
 (dict->indices)[i] = NULL;
 }
 }
 if(! dict->indices[fieldIndex]){
 dict->indices[fieldIndex] = buildDoubleIndex(dict,
fieldIndex);
 }
 double searchVal = strtod(query, NULL);
 struct index *index = dict->indices[fieldIndex];
 int highIdx = index->valueCount - 1;
 int lowIdx = 0;
 while(highIdx > lowIdx){
 int mid = (highIdx + lowIdx) / 2;
 if(getDoubleVal(index->nodes[mid]->record, fieldIndex) <
searchVal){
 lowIdx = mid + 1;
 } else if (getDoubleVal(index->nodes[mid]->record,
fieldIndex) > searchVal){
 highIdx = mid - 1;
 } else {
 // Exact match. Terminate early.
 lowIdx = mid;
 highIdx = mid;
 }
 }
 int currentIdx = (highIdx + lowIdx) / 2;
 if(currentIdx < 0){
 currentIdx = 0;
 } else if(currentIdx >= index->valueCount){
 currentIdx = index->valueCount - 1;
 }
 double closestProximity = getDoubleVal(index->nodes[currentIdx]-
>record, fieldIndex) - searchVal;
 /* Check surrounding values */
 if(closestProximity < 0){
 /* searchVal is larger than the current index. */
 closestProximity = -closestProximity;
 /* Check if value higher is closer. */
 if((currentIdx + 1) < index->valueCount){
 double candidateProximity = getDoubleVal(index-
>nodes[currentIdx + 1]->record, fieldIndex)
 - searchVal;
 if(candidateProximity < 0){
 candidateProximity = -candidateProximity;
 }
 if(candidateProximity < closestProximity){
 currentIdx = currentIdx + 1;
 }
 }
 } else {
 /* searchVal is smaller or equal to the current index. */
 if((currentIdx - 1) >= 0){
 double candidateProximity = getDoubleVal(index-
>nodes[currentIdx - 1]->record, fieldIndex)
 - searchVal;
 if(candidateProximity < 0){
 candidateProximity = -candidateProximity;
 }
 if(candidateProximity < closestProximity){
 currentIdx = currentIdx - 1;
 }
 }
 }

 struct queryResult *qr = (struct queryResult *)
 malloc(sizeof(struct queryResult));
 assert(qr);
 int numRecords = 1;
 struct data **records = (struct data **)
 malloc(sizeof(struct data *) * (numRecords));
 assert(records);
 records[0] = index->nodes[currentIdx]->record;
 double closestValue = getDoubleVal(index->nodes[currentIdx]-
>record, fieldIndex);
 qr->searchString = strdup(query);
 assert(qr->searchString);
 qr->numRecords = numRecords;
 qr->records = records;
 qr->closestValue = closestValue;

 return qr;
}
/* Free the given query result. */
void freeQueryResult(struct queryResult *r){
 if(! r){
 return;
 }
 free(r->records);
 free(r->searchString);
 free(r);
}
void freeData(struct data *d){
 if(! d){
 return;
 }
 if(d->address){
 free(d->address);
 };
 if(d->clue_sa){
 free(d->clue_sa);
 };
 if(d->asset_type){
 free(d->asset_type);
 };
 if(d->segside){
 free(d->segside);
 };
 free(d);
}
/* Free a given dictionary. */
void freeDict(struct dictionary *dict){
 if(! dict){
 return;
 }
 struct dictionaryNode *current = dict->head;
 struct dictionaryNode *next;
 while(current){
 next = current->next;
 freeData(current->record);
 free(current);
 current = next;
 }
 if(dict->indices){
 for(int i = 0; i < NUM_FIELDS; i++){
 if(dict->indices[i]){
 if(dict->indices[i]->nodes){
 free(dict->indices[i]->nodes);
 }
 free(dict->indices[i]);
 }
 }
 free(dict->indices);
 }
 free(dict);
}