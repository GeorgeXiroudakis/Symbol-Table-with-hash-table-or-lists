#include<stdio.h>
#include"symtable.h"
#include<string.h>
#include<stdlib.h>

#define BIG_TEST_SIZE 10000

void print(const char *pcKey, void *pvValue, void *pvExtra){
    printf("%s -> %d\n", pcKey, *(int*)pvValue);
}

/*void free_val(const char *pcKey, void *pvValue, void *pvExtra){if(pvValue != NULL)free(pvValue); return ;}*/

int main(){
	int x = 1;
	int y = 2;
	int z = 3;
	char key[50];
	int *temp[BIG_TEST_SIZE];
	int flag;

	SymTable_T table = SymTable_new(); printf("\ncreated new table\n");

	printf("\nsimple test for the function functionality.\n");	

	if(SymTable_put(table, "first", &x)) printf("\nadded key=first with value %d\n", x);
	if(SymTable_put(table, "second", &y)) printf("added key=second with value %d\n", y);
	if(SymTable_put(table, "third", &z)) printf("added key=third with value %d\n", z);
	
	printf("\ncurrent length: %d\n", SymTable_getLength(table));
	
	printf("\ncurrent table:\n");
	SymTable_map(table, print, NULL);
	
	printf("\nthe table %s contain the key second\n", SymTable_contains(table, "second") ? "does" : "doesn't");

	if(SymTable_remove(table, "second")) printf("\nremoved the binding with key=second\n");

	printf("\ncurrent length: %d\n", SymTable_getLength(table));
	
	printf("\nthe table %s contain the key second\n", SymTable_contains(table, "second") ? "does": "doesn't");

	printf("\ncurrent table:\n");
	SymTable_map(table, print, NULL);

	printf("\ngetting the value with key first -> %d\n", *(int*)SymTable_get(table, "first"));

	printf("\npress enter to start testing putting %d bindings\n", BIG_TEST_SIZE);
	
	getchar();
	
	for(x = 0; x < BIG_TEST_SIZE; x++){
		sprintf(key, "keyNum:%d", x);
	        temp[x] = malloc(sizeof(int));
     		*temp[x] = x;
                if( !( SymTable_put(table, key, temp[x]) ) ) printf("\nERROR in :adding key=%s with value %d\n", key, *temp[x]);
 
        }
	flag = 0;
	for(x = 0; x < BIG_TEST_SIZE; x++){
		sprintf(key, "keyNum:%d", x);
		if(*temp[x] != *(int*)SymTable_get(table, key)){
			printf("ERROR: in getting the value of key = %s\n", key);
			flag = 1;
		}
          }

	if(!flag)printf("\nTest passed with %d bindings\n", BIG_TEST_SIZE);
	
	printf("\npress enter to print the full table\n");
	getchar();
	
	printf("\ncurrent table:\n");
	SymTable_map(table, print, NULL);

	
	while(1){
		printf("give the key num to get the value of (e.x. give 5 for key:5) give negative num to exit\n>");
		if(!(scanf("%d", &x))){printf("invalid imput\n"); exit(-1);}
		if(x<0)break;
		if(x>= BIG_TEST_SIZE){printf("max size = %d\n", BIG_TEST_SIZE - 1); continue;}
		sprintf(key, "keyNum:%d", x);
		printf("%s -> %d\n", key, *(int*)SymTable_get(table, key));
	}

	/*SymTable_map(table, free_val, NULL);*/
	
	for (x = 0; x < BIG_TEST_SIZE; x++){
		free(temp[x]);
	}

	SymTable_free(table); printf("\nfreed table\n");

	return 0;
}

