#include "constants.h"
#include "stdlib.h"
#include "string.h"

typedef struct Label {
    char *name;
    char *value;
    struct Label *next;
} Label;

/*  */
Label *init_label(char *name){
    Label *inst=(LineNode *) malloc(sizeof(Label));
    char *name_copy=(char *) malloc(MAX_LABEL_LENGTH * sizeof(char));
    strcpy(name_copy, name);
    if (!*name_copy||!inst) {
        fprintf(stderr, "Fatal: failed to allocate required space");
        exit(1);
    }
    inst->name=name_copy;
    inst->next=NULL;
    return inst;
}