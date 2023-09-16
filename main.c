#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read.h"
#include "dictionary.h"

// #define MINARGS 8
// #define EXPECTED_STAGE "1"
// #define STAGE (LOOKUPSTAGE)

struct Point {
	char address[100];
	double x;
	double y;
}typedef Point;

struct Rectangle {
	struct Point bottomright;
	struct Point upperleft;
}typedef Rectangle;

struct Node {
	struct Point p;
	struct Rectangle rec;
	char color;
	struct Node* topleft;
	struct Node* topright;
	struct Node* bottomright;
	struct Node* bottomleft;
}typedef Node;

struct Node* createNode(struct Rectangle rec){
	struct Node* node = malloc(sizeof(struct Node));
	node->topleft = NULL;
	node->topright = NULL;
	node->bottomleft = NULL;
	node->bottomright = NULL;
	
	node->rec = rec;
	node->color = 'W';
	return node;
}
int inside(Rectangle rec, Point p){
	return (p.x > rec.upperleft.x &&
			p.x <= rec.bottomright.x &&
			p.y > rec.upperleft.y &&
			p.y <= rec.bottomright.y);
}

void insert(Node* node, struct Point p){
	if(!inside(node->rec, p)){
		return;
	}
	if(!inside(node->rec, p)){
		return;
	}
	if(node == NULL){
		printf("Error\n");
		return;
	}

	//check if node is white
	if (node->color == 'W'){
		node->p = p;
		node->color = 'B';
		//printf("in %f %f\n", p.x, p.y);
		return;
	}
	//if node is gray find the appropriate rectangle
	if(node->color == 'G'){
		if(inside(node->topleft->rec, p)){
			insert(node->topleft, p);
		}
		if(inside(node->topright->rec, p)){
			insert(node->topright, p);
		}
		if(inside(node->bottomleft->rec, p)){
			insert(node->bottomleft, p);
		}
		if(inside(node->bottomright->rec, p)){
			insert(node->bottomright, p);
		}
	}

	//if node is black, subdivide the rectangle
	if(node->color == 'B'){
		node->color = 'G';
		//subdivide rectangles
		struct Rectangle temp;
		struct Point upperleft;
		struct Point bottomright;
		double x1 = node->rec.upperleft.x;
		double x2 = node->rec.bottomright.x;
		double y1 = node->rec.upperleft.y;
		double y2 =node->rec.bottomright.y;

		upperleft.x = x1;
		upperleft.y = y1;
		bottomright.x = (x1+x1)/2;
		bottomright.y = (y1+y1)/2;
		temp.upperleft = upperleft;
		temp.bottomright = bottomright;
		node->topleft = createNode(temp);
		
		upperleft.x = (x1 + x2)/2;
		upperleft.y = y1;
		bottomright.x = x2;
		bottomright.y = (y1+y1)/2;
		temp.upperleft = upperleft;
		temp.bottomright = bottomright;
		node->topright = createNode(temp);
		
		upperleft.x = x1;
		upperleft.y = (y1+y2)/2;
		bottomright.x = (x1+x2)/2;
		bottomright.y = y2;
		temp.upperleft = upperleft;
		temp.bottomright = bottomright;
		node->bottomleft = createNode(temp);
		
		upperleft.x = (x1 + x2)/2;
		upperleft.y = (y1+y2)/2;
		bottomright.x = x2;
		bottomright.y = y2;
		temp.upperleft = upperleft;
		temp.bottomright = bottomright;
		node->bottomright = createNode(temp);
		

		insert(node->topleft, p);
		insert(node->topright, p);
		insert(node->bottomleft, p);
		insert(node->bottomright, p);


	}




}

struct Node* search(struct Node* head, struct Point p){
	if(!inside(head->rec, p)){
		return NULL;
	}
	if(head == NULL){
		return NULL;
	}

	if(head->color == 'B'){
		return head;
	}
	if(inside(head->topleft->rec, p)){
		return search(head->topleft, p);
	}
	if(inside(head->topright->rec, p)){
		return search(head->topright, p);
	}
	if(inside(head->bottomright->rec, p)){
		return search(head->bottomright, p);
	}
	if(inside(head->bottomleft->rec, p)){
		return search(head->bottomleft, p);
	}
}

Point* getCornerPoints(Rectangle rec){
	Point *arr = malloc(sizeof(Point)*4);
	arr[0] = rec.upperleft;
	arr[3] = rec.bottomright;

	Point temp;
	temp.x = rec.upperleft.x;
	temp.y = rec.bottomright.y;
	arr[1] = temp;
	Point temp2;
	temp2.x = rec.bottomright.x;
	temp2.y = rec.upperleft.y;
	arr[2] = temp2;

	return arr;
}
int rectangleOverlap(Rectangle rec1, Rectangle rec2){
	Point arr[4];
	arr[0] = rec1.upperleft;
	arr[3] = rec1.bottomright;

	Point temp;
	temp.x = rec1.upperleft.x;
	temp.y = rec1.bottomright.y;
	arr[1] = temp;
	Point temp2;
	temp2.x = rec1.bottomright.x;
	temp2.y = rec1.upperleft.y;
	arr[2] = temp2;



	for(int i =0; i < 4; i++){
		if(inside(rec2, arr[i])){
			
			return 1;
		}
	}
	arr[0] = rec2.upperleft;
	arr[3] = rec2.bottomright;

	
	temp.x = rec2.upperleft.x;
	temp.y = rec2.bottomright.y;
	arr[1] = temp;
	
	temp2.x = rec2.bottomright.x;
	temp2.y = rec2.upperleft.y;
	arr[2] = temp2;


	for(int i =0; i < 4; i++){
		if(inside(rec1, arr[i])){
			return 1;
		}
	}


	
	return 0;
}

void rangeQuery(Node* node, Rectangle rec, Point* arr, int size){
	if(node == NULL){
		return;
	}
	if(!rectangleOverlap(rec, node->rec)){
		return;
	}

	if(node->color == 'B'){
		arr[size] = node->p;
		size++;
		return;
	}

	rangeQuery(node->topleft, rec, arr, size);
	rangeQuery(node->topright, rec, arr, size);
	rangeQuery(node->bottomright, rec, arr, size);
	rangeQuery(node->bottomleft, rec,arr, size);
}

#define MINARGS 8
int main(int argc, char **argv)
{

	if(argc < MINARGS){
		//fprintf(stderr, "%s\n", "Insufficient arguments ");
		//return 0;
	}
	
	char* stage = argv[1];
	if(strcmp(stage, "4") != 0 && strcmp(stage, "3") !=0){
		fprintf(stderr, "%s\n", "Error: Stage has to be 3 or 4");
		return 0;
	}

	char *inputCSVName = argv[2];
	char *outputFilename = argv[3];

	FILE *csvFile = fopen(inputCSVName, "r");
	assert(csvFile);
	FILE *outputFile = fopen(outputFilename, "w");
	assert(outputFile);

	int n;
	struct csvRecord **dataset = readCSV(csv, &n);
	struct dictionary **dict = newDict();

	for(int i =0; i < n; i++){
		insertRecord(dict, dataset[i]);
	}

	//Insert nodes into the tree
	Point bottomleft;
	bottomleft.x = strold(argv[4]);
	bottomleft.y = strold(argv[5]);
	Point upper_right;
	upper_right.x = strold(argv[6]);
	upper_right.y = strold(argv[7]);

	Rectangle rec;
	rec.upperleft = upper_right;
	rec.bottomright = bottomleft;

	Node * head = createNode(rec);
	for(int i =0; i < n; i++){
		struct csvRecord *curr = dataset[i];
		Point p1;
		p1.x = curr->start_lat;
		p1.y = curr->start_lon;
		strcpy(p1.address, curr->address);
		Point p2;
		p2.x = curr->end_lat;
		p2.y = curr->end_lon;
		strcpy(p2.address, curr->address);
		insert(head, p1);
		insert(head, p2);
	}

	freeCSV(dataset, n);
	char *query = NULL;
	while((query == getQuery(stdin))){
		//get lon and lat
		int result = fputs(query, outputFile);
		fputs('\n', outputFile);
		if(result == EOF){
			printf("Error writing to file: \n");
			return 0;
		}
		char* lat;
		char* lon;
		for(int i =0; i , strlen(query); i++){
			if(query[i] == ' '){
				query[i] = '\0';
				lat = query;
				lon = query+1;
				break;
			}
		}

		Point p;
		p.x = strold(lat);
		p.y = strold(lon);
		Node* r = search(head, p);
		struct queryResult *r = lookupRecord(dict, query);
		printQueryResult(r, stdout, outputFile, stage);
		freeQueryResult(r);
		free(query);

	}
	freeDict(dict);
	dict = NULL;
	fclose(csvFile);
	fclose(outputFile);


	 return 0;
}