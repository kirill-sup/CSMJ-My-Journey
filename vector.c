#include <stdio.h>
#include <stdlib.h>

enum Type{INT, FLOAT, STRING};


struct value{
        
    enum Type type;
    union {
        
        int i;
        float f;
        char* s;
        } data;
    };
struct vector{
	
    struct value *items;
    int size;
	int capacity;
    };
struct result{

    union {

        int i;
        float f;
        char* s;
        } data;
    };

void value_init(struct value *variable, char type, void* value){
    
    if (type == 'i'){
         variable->data.i = *(int*)value;
        variable->type = INT;
        }
    else if (type == 's'){

        variable->data.s = *(char**)value;
        variable->type = STRING; 
        }
    else if (type == 'f'){
    
        variable->data.f = *(float*)value;
        variable->type = FLOAT;
        }
}

void vector_init(struct vector *vector, int size){

    vector->capacity = size;
    vector->items = malloc(vector->capacity * sizeof(struct value));
    vector->size = 0; 
    }

void vector_resize(struct vector *vector){

    vector->capacity *= 2;
    struct value *temp = realloc(vector->items, vector->capacity * sizeof(struct value));
    if (temp != NULL){
        vector->items = temp;
        }
    else {
        printf("no memory rest");
        }
    }

void vector_push(struct vector *vector, char type, void* value){

    if (vector->size == vector->capacity){
        
        vector_resize(vector);
        }
    
    struct value variable;
 
    value_init(&variable, type, value);
        
    vector->items[vector->size] = variable;

    vector->size += 1;
    }

void vector_get(struct vector *vector, int index, struct result *res){
    
    if (index < vector->size){
        switch(vector->items[index].type){

            case INT:
                res->data.i = vector->items[index].data.i;
                break;
            case FLOAT:

                res->data.f = vector->items[index].data.f;
                break;
            case STRING:

                res->data.s = vector->items[index].data.s;
                break; 
            }
        }
    else {
        printf("there are nothing on this index\n");
        }
    }
void vector_print(struct vector *vector){
    if (vector->size != 0){

        for (int i = 0; i < vector->size; i++){

            switch(vector->items[i].type){

                case INT:

                    printf("int: %d\n", vector->items[i].data.i);
                    break;
                
                case FLOAT:

                    printf("float: %f\n", vector->items[i].data.f);
                    break;
    
                case STRING:
    
                    printf("string %s\n", vector->items[i].data.s);
                    break;
                }
            } 
        }
    }
int main(){
    
    struct vector simple_vector;

    vector_init(&simple_vector, 100);

    int num = 100; 
    
    vector_push(&simple_vector, 'i', &num);

    for (int i = 0; i < 200; i++){
        int temp = i; 
        vector_push(&simple_vector, 'i', &temp);
        }
    struct result res;
    

    vector_get(&simple_vector, 0, &res);
    printf("%d\n", res.data.i);

    vector_print(&simple_vector);
    return 0;
    }
