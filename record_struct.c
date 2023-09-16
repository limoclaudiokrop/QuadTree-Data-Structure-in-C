/*
 Data structure and function implementations for a
 csv parsing module intended to convert from CSV files
 into C strings.
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "read.h"
#include "record_struct.h"
#include "record_struct.c"
#define INIT_RECORDS 1
#define NUM_FIELDS 19
/*
Reads a line - removing trailing whitespace, returns a csvRecord or
NULL
if parsing was unsuccessful.
*/
struct csvRecord *parseLine(char *line);
struct csvRecord **readCSV(FILE *csvFile, int *n){
 struct csvRecord **records = NULL;
 int numRecords = 0;
 int spaceRecords = 0;
 char *line = NULL;
 size_t size = 0;
 /*
 We want to drop the header as the reading does not have that
 functionality.
 */
 int droppedFirstLine = 0;
 while(getline(&line, &size, csvFile) > 0){
 if(! droppedFirstLine){
 droppedFirstLine = 1;
 continue;
 }
 if(spaceRecords == 0){
 records = (struct csvRecord **)
 malloc(sizeof(struct csvRecord *) * INIT_RECORDS);
 assert(records);
 spaceRecords = INIT_RECORDS;
 } else if(numRecords >= spaceRecords){
 spaceRecords *= 2;
 records = (struct csvRecord **)
 realloc(records, sizeof(struct csvRecord *) *
spaceRecords);
 assert(records);
 }
 records[numRecords] = parseLine(line);
 if(records[numRecords]){
 numRecords++;
 }
 }
 if(line){
 free(line);
 }
 /* Shrink. */
 records = (struct csvRecord **)
 realloc(records, sizeof(struct csvRecord *) *
numRecords);
 assert(records);
 *n = numRecords;
 return records;
}
/* This function reads CSVs where fields do not contain quotes.
 To handle this, we could add an additional Step 3 which shifted
the
 string one character back for every quote seen. */
struct csvRecord *parseLine(char *line){
 struct csvRecord *ret = NULL;
 char **fields = (char **) malloc(sizeof(char *) * NUM_FIELDS);
 assert(fields);
 int fieldNum = 0;
 int len = strlen(line);
 /* Remove trailing whitespace first. */
 while(len > 0 && (line[len - 1] == '\n' || line[len - 1] ==
'\r')){
 line[len - 1] = '\0';
 len--;
 }
 /* Check for empty lines. */
 if(len == 0){
 return NULL;
 }
 int progress = 0;
 int start = 0;
 /* For simplicity assume quotes only escape comma fields. */
 int inQuotes = 0;
 while(progress <= len){
 if(line[progress] == '\"'){
 if(inQuotes){
 inQuotes = 0;
 } else {
 inQuotes = 1;
 }
 } else if(line[progress] == ',' || line[progress] == '\0'){
 if(inQuotes){
 /* Skip over, comma is escaped. */
 /* Verify CSV not malformed. */
 assert(line[progress] != '\0');
 } else {
 /* Check fields match expected. */
 assert(line[progress] != '\0' || fieldNum ==
(NUM_FIELDS - 1));
 /* Terminate */
 line[progress] = '\0';
 assert(fieldNum < NUM_FIELDS);
 fields[fieldNum] = strdup(line + start);
 assert(fields[fieldNum]);
 fieldNum++;
 start = progress + 1;
 }
 }
 progress++;
 }
 /* Sanity check! Did we get everything? */
 assert(fieldNum == NUM_FIELDS);
 /* Step 2: Clean extraneous quotes. */
 for(int i = 0; i < NUM_FIELDS; i++){
 if(strlen(fields[i]) > 0){
 if(fields[i][0] == '\"'){
 assert(fields[i][strlen(fields[i]) - 1] == '\"');
 fields[i][strlen(fields[i]) - 1] = '\0';
 int len = strlen(fields[i]);
 for(int j = 0; j < len; j++){
 // Shuffle all characters along one character.
 fields[i][j] = fields[i][j + 1];
 }
 }
 }
 }
 ret = (struct csvRecord *) malloc(sizeof(struct csvRecord));
 assert(ret);
 ret->fieldCount = fieldNum;
 ret->fields = fields;
 return ret;
}
char *getQuery(FILE *f){
 char *line = NULL;
 size_t size = 0;
 if(getline(&line, &size, f) > 0){
 while(strlen(line) > 0 && (line[strlen(line) - 1] == '\n'
 || line[strlen(line) - 1] == '\r')){
 line[strlen(line) - 1] = '\0';
 }
 return line;
 } else {
 if(line){
 free(line);
 }
 return NULL;
 }
}
void freeCSV(struct csvRecord **dataset, int n){
 if(! dataset){
 return;
 }
 for(int i = 0; i < n; i++){
 for(int j = 0; j < dataset[i]->fieldCount; j++){
 free(dataset[i]->fields[j]);
 }
 free(dataset[i]->fields);
 free(dataset[i]);
 }
 free(dataset);
}