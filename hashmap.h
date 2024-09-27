#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdbool.h>

#include "node.h"

#define TRUE (1)
#define FALSE (0)

typedef struct hashmap {
    unsigned int (*hash_func)(const char* key);
    node_t** plist;
    size_t length;
} hashmap_t;

hashmap_t* init_hashmap_malloc(size_t length, unsigned int (*p_hash_func)(const char* key));

int add_key(hashmap_t* hashmap, const char* key, const int value);

int get_value(const hashmap_t* hashmap, const char* key);

int update_value(hashmap_t* hashmap, const char* key, int value);

int remove_key(hashmap_t* hashmap, const char* key);

void destroy(hashmap_t* hashmap);


inline const size_t make_start_index(const hashmap_t* hashmap, const int hash_id);

typedef struct _report {
    node_t* prev;
    node_t* target;
} _report_t;

_report_t _inspect_node(const hashmap_t* hashmap, const size_t hash_id, const char* key);

void print_hashmap(hashmap_t* hashmap);

#endif /* HASHMAP_H */
