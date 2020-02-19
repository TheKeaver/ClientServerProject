#include<stdio.h>
#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

#include<strings.h>
#include<stdlib.h>
#include<unistd.h>

#define SERVER_PORT 1234
#define MAX_PENDING 5
#define MAX_LINE 256

struct word {
	char name[50];
	char part[50];
	char def[100];
};

struct constant {
	char name[50];
	char value[50];
};

struct conversion {
	char name[50];
	char unit1[50];
	char unit2[50];
	char factor[50];
};

// ****************** CONSTRUCTOR METHODS ********************* //
struct word *createWord(char n[],char p[],char d[]) {
	struct word *w = malloc(sizeof(struct word));
	strcpy(w->name,n);
	strcpy(w->part,p);
	strcpy(w->def,d);
	return w;
}

struct constant *createConstant(char n[],char v[]) {
	struct constant *cs = malloc(sizeof(struct constant));
	strcpy(cs->name,n);
	strcpy(cs->value,v);
	return cs;
}

struct conversion *createConversion(char n[],char u1[],char u2[],char f[]) {
	struct conversion *cv = malloc(sizeof(struct conversion));
	strcpy(cv->name,n);
	strcpy(cv->unit1,u1);
	strcpy(cv->unit2,u2);
	strcpy(cv->factor,f);
	return cv;
}

struct word allWords[200]; // Maximum number of words in dictionary
struct constant allConstants[50]; // Maximum number of constants
struct conversion allConversions[50]; // Maximum number of conversions

int MAX_INDEX_CONV = 0;
int MAX_INDEX_CONS = 0;
int MAX_INDEX_WORD = 0;

// ************** DELETE FUNCTIONS **************** //
void deleteWord(char *response, char *del) {
	int counter = 0;
	int store = -1;
	char delimit[] = " \n\t\r\f\v\0";

	strtok(del,delimit);
	while(counter < MAX_INDEX_WORD) {
		strtok(allWords[counter].name,delimit);
		if(strcmp(del,allWords[counter].name) == 0) {
			store = counter;
		}
		counter++;
	}
	if(store >= 0) {
		while(store<(MAX_INDEX_WORD-1)) {
			allWords[store] = allWords[store+1];
			store++;
		}
		MAX_INDEX_WORD--;
		strcat(response,"(202) The DELETE request succeeded\n");
	}
	else {
		strcat(response,"Error (500): definition not deleted\n");
	}
}

void deleteConstant(char *response,char *del) {
	int counter = 0;
	int store = -1;
	char delimit[] = " \n\t\r\f\v\0";

	strtok(del,delimit);
	while(counter < MAX_INDEX_CONS) {
		strtok(allConstants[counter].name,delimit);
		if(strcmp(del,allConstants[counter].name) == 0) {
			store = counter;
		}
		counter++;
	}
	if(store >= 0) {
		while(store<(MAX_INDEX_CONS-1)) {
			allConstants[store] = allConstants[store+1];
			store++;
		}
		MAX_INDEX_CONS--;
		strcat(response,"(202) The DELETE request succeeded\n");
	}
	else {
		strcat(response,"Error (500): constant not deleted\n");
	}
}

void deleteConversion(char *response,char *del) {
	int counter = 0;
	int store = -1;
	char delimit[] = " \n\t\r\f\v\0";

	strtok(del,delimit);
	while(counter < MAX_INDEX_CONV) {
		strtok(allConversions[counter].name,delimit);
		if(strcmp(del,allConversions[counter].name) == 0) {
			store = counter;
		}
		counter++;
	}
	if(store >= 0) {
		while(store<(MAX_INDEX_CONV-1)) {
			allConversions[store] = allConversions[store+1];
			store++;
		}
		MAX_INDEX_CONV--;
		strcat(response,"(202) The DELETE request succeeded");
	}
	else {
		strcat(response,"Error (500): conversion not deleted");
	}
}

// ****************** PUT FUNCTIONS ******************** //
void putWord(char *response,char n[],char p[],char d[]) {
	int store = -1;
	int index = 0;
	char delimit[] = " \n\t\r\f\v\0";
	char delim[] = "\n\t\r\f\v\0";

	strtok(n,delimit);
	while(index <= MAX_INDEX_WORD) {
		strtok(allWords[index].name,delimit);
		if(strcmp(n,allWords[index].name) == 0) {
			store = index;
		}
		index++;
	}
	if(store >= 0) {
		strcat(response,"Error (404): Syntax error, word already exists\n");
	}
	else if (MAX_INDEX_WORD < 48) {
		strtok(p,delimit);
		strtok(d,delim);
		struct word w = *createWord(n,p,d);
		allWords[MAX_INDEX_WORD] = w;
		strcat(response,"(201) PUT request succeeded\n");
		MAX_INDEX_WORD++;
	}
	else {
		strcat(response,"Error (500): maximum word limit reached, please delete a word\n");
	}
}

void putConstant(char *response, char n[],char v[]) {
	int store = -1;
	int index = 0;
	char delimit[] = " \n\t\r\f\v\0";

	strtok(n,delimit);
	while(index < MAX_INDEX_CONS) {
		strtok(allConstants[index].name,delimit);
		if(strcmp(n,allConstants[index].name) == 0) {
			store = index;
		}
		index++;
	}
	if(store >= 0) {
		strcat(response,"Error (404) Syntax error, constant already exists\n");
	}
	else if(MAX_INDEX_CONS < 48) {
		strtok(v,delimit);
		struct constant cs = *createConstant(n,v);
		allConstants[MAX_INDEX_CONS] = cs;
		strcat(response,"(201) Created: put request succeeded\n");
		MAX_INDEX_CONS++;
	}
	else {
		strcat(response,"Error (500) max constants reached, please delete\n");
	}
}

void putConversion(char *response,char n[],char u1[],char u2[],char f[]) {
	int store = -1;
	int index = 0;
	char delimit[] = " \n\t\r\f\v\0";

	strtok(n,delimit);
	while(index <= MAX_INDEX_CONV) {
		strtok(allConversions[index].name,delimit);
		if(strcmp(n,allConversions[index].name) == 0)
		store = index;
		index++;
	}
	if(store >= 0) {
		strcat(response,"Error (404): Syntax error, conversion already exists\n");
	}
	else if (MAX_INDEX_CONV < 48) {
		strtok(u1,delimit);
		strtok(u2,delimit);
		strtok(f,delimit);
		struct conversion cv = *createConversion(n,u1,u2,f);
		allConversions[MAX_INDEX_CONV] = cv;
		strcat(response,"(201) The PUT request succeeded\n");
		MAX_INDEX_CONV++;
	}
	else {
		strcat(response,"Error (505): maximum conversions reached, please delete\n");
	}
}

int compare_strings(char a[], char b[])
{
	int c = 0;

	while (a[c] == b[c]) {
		if (a[c] == '\0' || b[c] == '\0')
		break;
		c++;
	}

	if (a[c] == '\0' && b[c] == '\0')
	return 0;
	else
	return -1;
}

// ************************ GET METHODS **************************** //
void getWord(char *response,char n[]) {
	int index = 0;
	int store = -1;
	char delimit[] = " \n\t\r\f\v\0";

	strtok(n,delimit);
	while(index <= MAX_INDEX_WORD) {
		strtok(allWords[index].name,delimit);
		if(strcmp(allWords[index].name,n) == 0) {
			store = index;
		}
		index++;
	}

	if(store >= 0) {
		strcat(response,"(200) The GET request succeeded\n");
		strcat(response,"Category: word\n");
		strcat(response,"Name: ");
		strcat(response,allWords[store].name);
		strcat(response,"\nPart: ");
		strcat(response,allWords[store].part);
		strcat(response,"\nDefinition: ");
		strcat(response,allWords[store].def);
	}
	else {
		strcat(response,"Error code: (403) The field name is unknown\n");
	}
}

void getConstant(char *response,char n[]) {
	int store = -1;
	int index = 0;
	char delimit[] = " \n\t\r\f\v\0";

	strtok(n,delimit);
	while(index <= MAX_INDEX_CONS) {
		strtok(allConstants[index].name,delimit);
		if(strcmp(n,allConstants[index].name) == 0) {
			store = index;
		}
		index++;
	}
	if(store >= 0) {
		strcat(response,"(200) The GET request succeeded");
		strcat(response,"\nCategory: constant");
		strcat(response,"\nName: ");
		strcat(response,allConstants[store].name);
		strcat(response, "\nValue: ");
		strcat(response,allConstants[store].value);
	}
	else {
		strcat(response,"Error code: (403) The field name is unknown\n");
	}
}

void getConversion(char *response,char n[]) {
	int index = 0;
	int store = -1;
	while(index <= MAX_INDEX_CONV) {
		if(strcmp(n,allConversions[index].name) == 0) {
			store = index;
		}
		index++;
	}
	if(store >= 0) {
		strcat(response,"(200) The GET request succeeded\n");
		strcat(response,"Category: conversion\n");
		strcat(response,"\nName: ");
		strcat(response,allConversions[store].name);
		strcat(response,"\nUnit1: ");
		strcat(response,allConversions[store].unit1);
		strcat(response,"\nUnit2: ");
		strcat(response,allConversions[store].unit2);
		strcat(response,"\nFactor1to2: ");
		strcat(response,allConversions[store].factor);
	}
	else {
		strcat(response,"Error code: (403) The field name is unknown\n");
	}
}

// ******************** WRITE TO FILE FUNCTIONS ******************** //
void writeToFileConversion() {
	FILE *file = fopen("conversion.txt", "w+"); // clear file if exists, else make file
	int index = 0;

	// test that file exists
	if (file == NULL) {
		printf("Couldn't open file\n");
		exit(-1);
	}
	while(index < MAX_INDEX_CONV) {
		fprintf(file, "%s\r\n%s\r\n%s\r\n%s\r\n",allConversions[index].name, allConversions[index].unit1, allConversions[index].unit2, allConversions[index].factor);
		index++;
	}
	fclose(file);
}

void writeToFileConstant() {
	FILE *file = fopen("constant.txt", "w+"); // a for append
	int index = 0;
	// test that file exists
	if (file == NULL) {
		printf("Couldn't open file\n");
		exit(-1);
	}

	while(index < MAX_INDEX_CONS) {
		fprintf(file, "%s\r\n%s\r\n", allConstants[index].name, allConstants[index].value);
		index++;
	}

	fclose(file);
}

void writeToFileWord() {
	FILE *file = fopen("words.txt", "w+"); // a for append
	int index = 0;
	// test that file exists
	if (file == NULL) {
		printf("Couldn't open file\n");
		exit(-1);
	}
	while(index < MAX_INDEX_WORD) {
		//fprintf(file, "%s\r\n%s\r\n%s\r\n", allWords[index].name, allWords[index].part, allWords[index].def);
		fprintf(file,"%s\r\n",allWords[index].name);
		fprintf(file,"%s\r\n",allWords[index].part);
		fprintf(file,"%s\r\n",allWords[index].def);
		index++;
	}
	fclose(file);
}

// ***************** READ FILE FUNCTIONS ********************* //
void readWord() {
	FILE *file = fopen("words.txt", "r"); // read from FILE
	char *line = NULL;
	char name[256];
	char part[256];
	char def[256];
	char delimit[] = " \n\t\r\f\v\0";
	char delim[] = "\n\t\r\f\v\0";
	size_t len = 0;
	ssize_t read;

	// test that file exists
	if (file == NULL) {
		printf("Couldn't open words.txt\n");
		exit(-1);
	}
	while (getline(&line, &len, file) != -1) { // Get first line of file
		strcpy(name,line);
		strtok(name,delimit);
		getline(&line, &len, file); // next line
		strcpy(part,line);
		strtok(part,delimit);
		getline(&line, &len, file); // next line
		strcpy(def,line);
		strtok(def,delim);
		struct word *wd = createWord(name,part,def);
		allWords[MAX_INDEX_WORD] = *wd;
		MAX_INDEX_WORD++;
	}
	free(line);
	fclose(file);
}

void readConversion() {
	FILE *file = fopen("conversion.txt", "r"); // read from FILE
	char *line = NULL;
	char name[256];
	char unit1[256];
	char unit2[256];
	char factor[256];
	size_t len = 0;
	char delimit[] = " \n\t\r\f\v\0";

	// test that file exists
	if (file == NULL) {
		printf("Couldn't open file\n");
		exit(-1);
	}
	while (getline(&line, &len, file) != -1) {
		//getline(&line, &len, file);
		strtok(line,delimit);
		strcpy(name,line); // Line 1 = Name
		getline(&line, &len, file);
		strtok(line,delimit);
		strcpy(unit1,line); // Line 2 = Unit1
		getline(&line, &len, file);
		strtok(line,delimit);
		strcpy(unit2,line); // Line 3 = Unit2
		getline(&line, &len, file);
		strtok(line,delimit);
		strcpy(factor,line); // Line 4 = Factor
		struct conversion *cv = createConversion(name,unit1,unit2,factor);
		allConversions[MAX_INDEX_CONV] = *cv;
		MAX_INDEX_CONV++;
	}
	free(line);
	fclose(file);
}

void readConstant() {
	FILE *file = fopen("constant.txt", "r"); // read from FILE
	char *line = NULL;
	char name[256];
	char value[256];
	size_t len = 0;
	ssize_t read;
	char delimit[] = " \n\t\r\f\v\0";

	// test that file exists
	if (file == NULL) {
		printf("Couldn't open file\n");
		exit(-1);
	}
	while ((getline(&line, &len, file)) != -1) {
		strtok(line,delimit);
		strncpy(name,line,len); // Line 1 = name
		getline(&line, &len, file);
		strtok(line,delimit);
		strncpy(value,line,len); // Line 2 = value
		struct constant *cs = createConstant(name,value);
		allConstants[MAX_INDEX_CONS] = *cs;
		MAX_INDEX_CONS++;
	}
	free(line);
	fclose(file);
}

// ***************** MAIN METHOD ****************** //
int main(int argc, char * argv[]) {
	char response[256];
	struct sockaddr_in sin ;
	char buf[MAX_LINE] ;
	int len ;
	int s, new_s ;

	readWord();
	readConstant();
	readConversion();

	printf("Now waiting for connection\n");

	bzero((char *)&sin,sizeof(sin)) ;
	sin.sin_family = AF_INET ;
	sin.sin_addr.s_addr = INADDR_ANY ;
	sin.sin_port = htons( SERVER_PORT) ;

	if ( (s = socket(PF_INET,SOCK_STREAM,0)) < 0) {
		perror("simplex-talk: socket") ;
		exit(1) ;
	}
	if   (bind( s, (struct sockaddr *) &sin, sizeof(sin) ) < 0 ) {
		perror("simplex-talk: bind") ;
		exit(1) ;
	}

	listen( s, MAX_PENDING ) ;

	while(1) {
		if ( (new_s = accept(s,(struct sockaddr *)&sin,
		(socklen_t *) &len) ) < 0 ) {
			perror("simplex-talk: accept") ;
			exit(1) ;
		}
		if ( (len = recv(new_s, buf, sizeof(buf), 0 )) ) {
			printf("what server received %s", buf);

			int i = 0; // counter for looping through token
			char delimit[] = " \n\t\r\f\v\0"; // delimiting characters for searching through what the user entered
			char *p = strtok(buf, delimit);
			char *array[100]; // if this size is too large the program seg faults but I want it to be bigger than it currently is
			int size = 1; // how many tokens there are


			strcpy(response, "FACTP/0.9 "); // preparing the response before adding response code values

			// loop through what client sent and split the whole message into tokens
			while (p != NULL) {
				array[i++] = p;
				p = strtok(NULL, delimit);
				size++;
			}

			if (size < 5) { // client didn't enough tokens for a valid request
			strcat(response, "(404) Syntax error, not enough arguments \n");
		}

		// checks that the first token entered is either "GET", "PUT", or "DELETE"
		if (strcmp("GET", array[0]) != 0 && strcmp("PUT", array[0]) != 0
		&& strcmp("DELETE", array[0]) != 0) {
			strcat(response, "Error code: (401) Method name\n");
		}
		// checks that the second token entered is either "Conversion", "Constant", "Definition"
		if (strcmp("Conversion", array[1]) != 0 && strcmp("Constant", array[1]) != 0
		&& strcmp("Definition", array[1]) != 0) {
			strcat(response, "(402) Category name\n");
		}

		// the main logic of what to do with what the user entered
		// 3 outer most if statements for GET, PUT, and DELETE
		// then in each of those if statements there are 3 more for "Conversion", "Constant", and "Definition"
		if (strcmp("GET", array[0]) == 0) { // GET request
			if (strcmp("Name: ", array[3]) != 0 && array[4] != NULL) {
				if (strcmp("Conversion", array[1]) == 0) {
					getConversion(response, array[4]);
				}
				else if (strcmp("Constant", array[1]) == 0) {
					getConstant(response, array[4]);
				}
				else if (strcmp("Definition\0", array[1]) == 0) {
					getWord(response, array[4]);
				}
			}
			else {
				strcat(response, "(403) The name of field is unknown\n");
			}

		} // end get
		else if (strcmp("PUT\0", array[0]) == 0) { // PUT request
			if (strcmp("Name:\0", array[3]) == 0 && array[4] != NULL) {
				if (strcmp("Conversion\0", array[1]) == 0) {
					if((strcmp("Unit1:\0", array[5]) == 0) && (strcmp("Unit2:\0", array[7]) == 0) && (strcmp("Factor1to2:\0", array[9]) == 0) ) {
						if (array[4] != NULL && array[6] != NULL && array[8] != NULL && array[10] != NULL)
						{
							//writeToFileConversion(response, array[4], array[6], array[8], array[10]);
							putConversion(response,array[4],array[6],array[8],array[10]);
						}
					}
					else {
						strcat(response, "(403) The name field is unknown\n");
					}
				} // conversion

				else 	if (strcmp("Constant\0", array[1]) == 0) {
					if (strcmp("Value:\0", array[5]) == 0 && array[6] != NULL) {
						//writeToFileConstant(response, array[4], array[6]);
						putConstant(response,array[4],array[6]);
					}
					else {
						strcat(response, "(403) The name field is unknown\n");
					}
				} // constant
				else 	if (strcmp("Definition\0", array[1]) == 0) {
					if (strcmp("Part:\0", array[5]) == 0 && strcmp("Definition:\0", array[7]) == 0 && array[8] != NULL) {
						char theDefinition[256];

						if (array[8] != NULL) {
							strcpy(theDefinition, array[8]); // array[8] is the first part of the defintion
							strcat(theDefinition, " ");
							int i = 9;
							for ( i; i < size-1; i++) {
								strcat(theDefinition, array[i]);
								strcat(theDefinition, " ");
							}
						}
						//writeToFileDefinition(response, array[4], array[6], theDefinition);
						putWord(response,array[4],array[6],theDefinition);
					}
					else {
						strcat(response, "(403) The name field is unknown\n");
					}
				} // definition
			} // name comparison
			else {
				strcat(response, "(403) The name field is unknown\n");
			}
		} // end put
		else if (strcmp("DELETE\0", array[0]) == 0) { // DELETE request
			if ((strcmp("Name:\0", array[3]) == 0) && (array[4] != NULL)) {
				if (strcmp("Conversion\0", array[1]) == 0) {
					// CALL DELETE METHOD
					deleteConversion(response,array[4]);
				} // conversion
				else if (strcmp("Constant\0", array[1]) == 0) {
					// CALL DELETE METHOD
					deleteConstant(response,array[4]);
				} // constant
				else if (strcmp("Definition\0", array[1]) == 0) {
					// CALL DELETE METHOD
					deleteWord(response,array[4]);
				} // definition
			}
			else {
				strcat(response, "(403) The name field is unknown\n");
			}
		} // end delete

		printf("sending back: %s\n", response);
		send(new_s, response, sizeof(response), 0); // sends back  to client
		printf("\nclosing connection\n");
	}
	writeToFileWord();
	writeToFileConstant();
	writeToFileConversion();
	close(new_s);
}
}
