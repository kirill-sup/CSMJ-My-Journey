#include <stdio.h>
#include <stdlib.h>

struct result{

    union {

    int i;
    float f;
    char* s;
    } data;
};
struct Entry
{
    char* key;

    union {

    int i;
    float f;
    char* s;
        } value;
    
    enum {

    INT,
    STRING,
    FLOAT,
    } value_type;
	struct Entry *next;
};

struct HashTable
{
	int capacity;
	int expand;
	struct Entry **buckets;
};

int str_length(char* word){

        int length = 0;
        for (int i = 0; word[i] != '\0'; i++){
        length++;
                }
        return length;
        }

int str_cmp(char *first_word,char *second_word){
	
	int first_word_length = str_length(first_word);

	if (first_word_length == str_length(second_word)){
		
		for (int i = 0; i < first_word_length; i++){

			if (first_word[i] != second_word[i]){
				
				return 1;
				}	
			}	
		}
	return 0;
	}

int hash(char* letter, int capacity_of_hashtable)
{
	unsigned int sum = 0;
	for (int i = 0; letter[i] != '\0'; i++){
		sum += letter[i];
	    }
	return sum % capacity_of_hashtable;
}

void hashmap_init(struct HashTable *hashmap, int size){
	hashmap->capacity = size;
	
	hashmap->expand = 0;
	
	hashmap->buckets = calloc(hashmap->capacity, sizeof(struct Entry*));
    }

struct Entry* entry_init(char* name, void* value, char value_type){
	struct Entry *new_node;
	
	new_node = malloc(sizeof(struct Entry)); 
	
    new_node->key = name;     
	
    switch(value_type){

        case 'i':
            new_node->value_type = INT; 
            new_node->value.i = *(int*) value;
            break;
        case 'f':   
            new_node->value_type = FLOAT;
            new_node->value.f = *(float*) value;
            break;
        case 's':
            new_node->value_type = STRING;
            new_node->value.s = *(char**) value;
            break;
               
        }	
	new_node->next = NULL;
	
	return new_node;
}

void entry_rellocation(struct HashTable *hashmap, struct Entry **entryptr){

    struct Entry *temp = (*entryptr), *previous;

    while (temp != NULL){

        previous = temp->next;

        int index = hash(temp->key, hashmap->capacity);
      
        temp->next = hashmap->buckets[index];
        hashmap->buckets[index] = temp;
        hashmap->expand += 1;
        
        temp = previous;
        }
    }
    
void hashmap_realloc(struct HashTable *hashmap){

    struct Entry **old_buckets = hashmap->buckets;
    int old_capacity = hashmap->capacity; 
    
    
    hashmap->capacity *= 2;
    hashmap->buckets = calloc(hashmap->capacity, sizeof(struct Entry*));
    hashmap->expand = 0;

    for (int i = 0; i < old_capacity; i++){

        if (old_buckets[i] != NULL){
            entry_rellocation(hashmap, &old_buckets[i]); 
            }
        } 
    free(old_buckets);
    }

void hashmap_add(struct HashTable *hashmap, char* name, void* value, char value_type){
    
    if (hashmap->expand >= hashmap->capacity * 0.7){

        hashmap_realloc(hashmap);
        }
	
	int index;
	index = hash(name, hashmap->capacity);
	
	struct Entry *new_entry = entry_init(name, value, value_type);
	
		
	new_entry->next = hashmap->buckets[index];

	hashmap->buckets[index] = new_entry;				
		
	hashmap->expand += 1;
	}

int hashmap_delete(struct HashTable *hashmap, char* name){

    int index = hash(name, hashmap->capacity);
    
    if (hashmap->buckets[index] != NULL){
        
        struct Entry *temp = hashmap->buckets[index], *previous;
        
        if (str_cmp(temp->key, name) == 0){

            previous = temp;
            temp = temp->next;

            hashmap->buckets[index] = temp;
            free(previous);
            
            return 1;
            }

        else{
            while (temp->next != NULL){
                
                if (str_cmp(temp->next->key, name) == 0){
    
                    previous = temp;
                    temp = temp->next;
                    previous->next = temp->next;

                    free(temp);

                    return 1;
                    }
                temp = temp->next;
                }             
            }
        }
    return 0;
    }

void hashmap_get(struct HashTable *hashmap, char* name, struct result *res){
	
	int index = hash(name, hashmap->capacity);
	
	if (hashmap->buckets[index] != NULL){
		
		struct Entry *temp = hashmap->buckets[index];
		
		while (temp != NULL){
			
			if (str_cmp(temp->key, name) == 0){
				
                switch(temp->value_type){

                    case INT:
                        res->data.i = temp->value.i;
                        break;

                    case FLOAT:
                        res->data.f = temp->value.f;
                        break;
                    
                    case STRING:
                        res->data.s = temp->value.s;
                        break;
                    }
                return;
				}
				
			temp = temp->next;
			}
		}
	}

void hashmap_free(struct HashTable *hashmap){

    for (int i = 0; i < hashmap->capacity; i++){

        if (hashmap->buckets[i] != NULL){
            
            struct Entry *temp = hashmap->buckets[i];

            while (temp != NULL){
                struct Entry *next = temp->next;
                free(temp);
                temp = next;
            }
        }
    }
    
    free(hashmap->buckets);
    }

int main()
{
    struct HashTable hashmap;
    hashmap_init(&hashmap, 100);

    int num = 120;
    hashmap_add(&hashmap, "bannan", &num, 'i');
    struct result res;
    res.data.i = 0;
    hashmap_get(&hashmap, "bannan", &res);

    printf("this is a bannan: %d\n", res.data.i);
    
    hashmap_free(&hashmap);
    
    return 0;
}
