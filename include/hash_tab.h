#ifndef _HASH_TAB_H_
#define _HASH_TAB_H_


#include <stddef.h>

typedef struct node_t {
  void *key;
  size_t key_len;
  void *value;
  size_t value_len;
  struct node_t *next;
} node_t;

typedef struct hash_tab_t {
  node_t **buckets;
  size_t size;
  int count;
  int (*hash_func) (void *, size_t, size_t);
} hash_tab_t;

hash_tab_t *hash_tab_init(size_t size, int (*hash_func)(void *key, size_t keylen, size_t hash_tab_size));
void *hash_tab_search(hash_tab_t *hash_table, void *key, size_t key_len);
void *hash_tab_insert(hash_tab_t *hash_table, void *key, size_t key_len, void *value, size_t value_len);
void hash_tab_remove(hash_tab_t *hash_table, void *key, size_t key_len);
void *hash_tab_grow(hash_tab_t *hash_table, size_t size);
void hash_tab_free(hash_tab_t *hash_table);


#endif // !_HASH_TAB_H_
