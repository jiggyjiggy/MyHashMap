#ifndef NODE_H
#define NODE_H

typedef struct node {
    char* key;
    int value;
    struct node* next;
} node_t;

enum { NODE_NOT_FOUND = -1 };

#endif /* NODE_H */
