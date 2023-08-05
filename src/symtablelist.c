#include<stdlib.h>
#include<assert.h>
#include"symtable.h"
#include<stdio.h>
#include<string.h>

struct SymTable_node{
	char* key;
	void* value;
	struct SymTable_node *next;	
};

typedef struct SymTable_node Sym_node_T;

struct symtable_s{
	struct SymTable_node *head;
};

/*takes no arguments.
 * returns a new empty symtable with no bindings*/
SymTable_T SymTable_new(void){
	SymTable_T new = malloc(sizeof(SymTable_T));
	assert(new != NULL);

	new->head = NULL;	

	return new;
}

/*takes the symtable to free
 * frees all the memory previously allocated
 * by the SymTable_ factions
 * if oSymTable == NULL the function does nothing*/
void SymTable_free(SymTable_T oSymTable){
	Sym_node_T *list;
	Sym_node_T *next_node;

	if(oSymTable == NULL) return ;

	list = oSymTable->head; 

	while(list != NULL){
		next_node = list->next;
		free(list->key);
		free(list);
		list = next_node;	
	}
	free(oSymTable);
	
	return;	
}

/*takes the Symtable of which to take the length 
 * returns the number of bindings that the  given table has
 * checks for runtime error if oSymTable == NULL */
unsigned int SymTable_getLength(SymTable_T oSymTable){
	Sym_node_T *list;
	unsigned int lenght;
	assert(oSymTable != NULL);

	list = oSymTable->head; 
	lenght = 0;
	
	while (list != NULL){lenght++; list = list->next;}

	return lenght;
	
}

/*takes the Symtable in with to add the new binding
 * the key of the binding
 * the value of the binding
 *
 * if the key doesn't mach an existing binding
 * it crates a new binding with the given values
 * and returns 1
 * else if doesn't change the symtable and returns 0
 * it checks for runtime error if oSymTable == NULL or pcKey == NULL*/
int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue){
	Sym_node_T *list;
	Sym_node_T *new_node;

	assert(oSymTable != NULL);
	assert(pcKey != NULL);
	
	if(SymTable_contains(oSymTable, pcKey))return 0;
	
	new_node = malloc(sizeof(Sym_node_T));
	assert(new_node != NULL);

	/*creating copy of the key for stability*/
	new_node->key = malloc(strlen(pcKey) + 1);
	assert(new_node->key != NULL);
	strcpy(new_node->key, pcKey);

	
	/*we could make the value const in the struct
	* but the we would to change some declarations
	* ex the get function to return const void* */
	new_node->value = (void*)pvValue;
	
	list = oSymTable->head;
	new_node->next = list;
	oSymTable->head = new_node;

	return 1;
	
}

/*takes a symtable from which to revove the binding(oSymTable)
 * the key of the binding to revove
 * if the there is a binding with the given key it 
 * removes  the binding and returns 1
 * else it doesn't change the table and returns 0  
 * it checks for runtime error if oSymTable == NULL
 * or pcKey == NULL  */
int SymTable_remove(SymTable_T oSymTable, const char *pcKey){
	Sym_node_T *list;
	Sym_node_T *prev;
	assert(oSymTable != NULL);
	assert(pcKey != NULL);

	if(SymTable_contains(oSymTable, pcKey) == 0)return 0;
	
	list = oSymTable->head;
	
	/*if ist's the first node(using prev as next to avoid 
	 * new declaration*/
	if(strcmp(list->key, pcKey) == 0){
		prev = list->next;
		free(list->key);
		free(list);
		oSymTable->head = prev;
		return 1;
	}
	/*else*/
	while (list != NULL && strcmp(list->key, pcKey) != 0){
			prev = list;
			list = list->next;
	}
	
	if(list != NULL){
		prev->next = list->next;
		free(list->key);
		free(list);
	}else assert(0);

	return 1;
	
}

/*takes the symtable to check if it contains a binding(oSymTable)
 *the key to check if exists(pcKey)
 *
 *it returns 1 if the binding exists else return 0  
 *
 * it checks for runtime error if oSymTable == NULL
 * or pcKey == NULL  */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
	Sym_node_T *list;
	assert(oSymTable != NULL);
	assert(pcKey != NULL);
	
	list = oSymTable->head; 
	while (list != NULL){
		if(strcmp(list->key, pcKey) == 0) return 1;

		list = list->next;
	}
	return 0;
}

/*takes the symtable form which to get a value(oSymTable)
 *the key from which to get the value (pcKey)
 *
 *if the binding exits it return the value of the binding 
 *else it return NULL
 *
 * it checks for runtime error if oSymTable == NULL
 * or pcKey == NULL  */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
	Sym_node_T *list;

	assert(oSymTable != NULL);

	list = oSymTable->head;
	while(list != NULL){
		if(strcmp(list->key, pcKey) == 0)return list->value;

		list = list->next;
	}

	return NULL;

}

/*takes the symtable to aply the function at its bindings(oSymTable)
 * the function to apply(pfApply) 
 * and an extra parameter to pass on the pfApply (pvExtra) 
 * 
 * it applies the giver function with each key and value and the pvExtra
 * it checks for runtime if oSymTable == NULL or pfApply == NULL */
void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),const void *pvExtra){
	Sym_node_T *list;

	assert(oSymTable != NULL);
	assert(pfApply != NULL);
	
	list = oSymTable->head;
	while(list != NULL){
	/*it might be better to change the parameter of pfApply to const void* to 
	avoid the cast*/

		pfApply(list->key, list->value, (void *)pvExtra);

		list = list->next;
	}
	return ;
}
