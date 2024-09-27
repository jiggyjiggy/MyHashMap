#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hashmap.h"

static void free_node(node_t* node);
static void free_hashmap(hashmap_t* hashmap);

hashmap_t* init_hashmap_malloc(size_t length, unsigned int (*p_hash_func)(const char* key))
{
    hashmap_t* pa_hashmap;
    pa_hashmap = malloc(sizeof(hashmap_t));

    pa_hashmap->hash_func = p_hash_func;
    pa_hashmap->length = length;

    pa_hashmap->plist = malloc(sizeof(node_t*) * length);
    node_t** p_plist = pa_hashmap->plist;
    for (register int i = 0; i < length; ++i) {
        p_plist[i] = NULL;
    }

    return pa_hashmap;
}

int add_key(hashmap_t* hashmap, const char* key, const int value)
{
    const size_t hash_id = hashmap->hash_func(key);
    _report_t nodes_info = _inspect_node(hashmap, hash_id, key);
    node_t* prev = nodes_info.prev;
    node_t* target = nodes_info.target;

    if (target != NULL) {
        return FALSE;
    }

    {
        node_t* new_node = malloc(sizeof(node_t));
        {
            size_t key_len = strlen(key);
            new_node->key = malloc(key_len + 1);
            strcpy(new_node->key, key);
            new_node->key[key_len] = '\0';

            new_node->value = value;
            new_node->next = NULL;
        }

        {
            if (prev == NULL) {
                const size_t start_index = make_start_index(hashmap, hash_id);
                hashmap->plist[start_index] = new_node;
            } else {
                prev->next = new_node;
            }
        }
        return TRUE;
    }
}

int get_value(const hashmap_t* hashmap, const char* key)
{
    _report_t nodes_info = _inspect_node(hashmap, hashmap->hash_func(key), key);
    node_t* target = nodes_info.target;

    if (target == NULL) {
        return NODE_NOT_FOUND;
    }

    return target->value;
}

int update_value(hashmap_t* hashmap, const char* key, int value)
{
    _report_t nodes_info = _inspect_node(hashmap, hashmap->hash_func(key), key);
    node_t* target = nodes_info.target;

    if (target == NULL) {
        return FALSE;
    }

    target->value = value;
    return TRUE;
}

int remove_key(hashmap_t* hashmap, const char* key)
{
    size_t hash_id = hashmap->hash_func(key);
    _report_t nodes_info = _inspect_node(hashmap, hash_id, key);
    node_t* target = nodes_info.target;

    if (target == NULL) {
        return FALSE;
    }

    {
        node_t* prev = nodes_info.prev;

        if (prev == NULL) {
            const size_t start_index = make_start_index(hashmap, hash_id);
            hashmap->plist[start_index] = target->next;
        } else {
            prev->next = target->next;
        }

        free_node(target);
        return TRUE;
    }
}

void destroy(hashmap_t* hashmap)
{
    node_t** plist = hashmap->plist;

    int length = hashmap->length;
    for (register int start_index = 0; start_index < length; ++start_index) {
        node_t* node = plist[start_index];

        while (node != NULL) {
            node_t* next = node->next;

            free_node(node);

            node = next;
        }
    }

    free_hashmap(hashmap);
}


inline const size_t make_start_index(const hashmap_t* hashmap, const int hash_id)
{
    return hash_id % hashmap->length;
}

/*
 * hashmap은 동일한 hash_id(해시 충돌)에 대하여 링크드 리스트로 구현되어있다.
 * _inspect_node 함수는 _report_t 구조체에 값을 저장해서 반환한다.
 *
 * 경우의 수는 아래와 같다.
 * 1. 못 찾는 경우 : (target == NULL)
 *      1-1. 해시 충돌이 없는 경우 : prev == NULL
 *      1-2. 해시 충돌이 있는 경우 : prev == &last_node
 * 2. 찾는 경우 : (target != NULL)
 *      2-1. 해시 충돌이 없는 경우 : prev == NULL
 *      2-2. 해시 충돌이 있는 경우 : prev == &prev_node
 */
_report_t _inspect_node(const hashmap_t* hashmap, const size_t hash_id, const char* key)
{
    _report_t report;
    report.prev = NULL;
    report.target = NULL;

    const size_t start_index = make_start_index(hashmap, hash_id);
    node_t* node = hashmap->plist[start_index];
    {
        if (node == NULL) {
            goto inspection_done;
        }
    }
    {
        do {
            if (strcmp(node->key, key) == 0) {
                report.target = node;
                goto inspection_done;
            }
            report.prev = node;

            node = node->next;
        } while (node != NULL);
    }

inspection_done:
    return report;
}

void print_hashmap(hashmap_t* hashmap)
{
    node_t** plist = hashmap->plist;

    int length = hashmap->length;
    for (register int start_index = 0; start_index < length; ++start_index) {
        printf("start_index: %d\n", start_index);

        node_t* node = plist[start_index];

        while (node != NULL) {
            node_t* next = node->next;

            printf("key: %s, value: %d\n", node->key, node->value);

            node = next;
        }

        printf("\n");
    }
}

static void free_node(node_t* node)
{
    {
        char* p_key = node->key;
        free(p_key);
        p_key = NULL;
    }
    free(node);
    node = NULL;
}

static void free_hashmap(hashmap_t* hashmap)
{
    free(hashmap->plist);
    hashmap->plist = NULL;

    free(hashmap);
    hashmap = NULL;
}
