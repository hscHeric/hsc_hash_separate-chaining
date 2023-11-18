#include "hash_tab.h"
#include <stdlib.h>
#include <string.h>

static int default_hash_func(void* key, size_t key_len, size_t hash_tab_size){
    int sum = 0;
    for(int i = 0; i < (int) key_len; i++){
         sum += ((unsigned char *) key)[i] * (i + 1);
    }

    return (sum % (int)hash_tab_size);
}


hash_tab_t *hash_tab_init(size_t size, int (*hash_func)(void *key, size_t keylen, size_t hash_tab_size)) {
  hash_tab_t *hash_tab = (hash_tab_t *)malloc(sizeof(hash_tab_t));

  hash_tab->buckets = (node_t**)malloc(sizeof(node_t*) * size);

  hash_tab->size = size;
  hash_tab->count = 0;

  for (int i = 0; i < (int) size; i++) {
    hash_tab->buckets[i] = NULL;
  }

  if(hash_func == NULL){
    hash_tab->hash_func = &default_hash_func;
  } else {
    hash_tab->hash_func = hash_func;
  }

  return hash_tab;
}

void *hash_tab_search(hash_tab_t *hash_table, void *key, size_t key_len) {
  int index = default_hash_func(key, key_len, hash_table->size);
  
  if(hash_table->buckets[index] != NULL) {
    node_t *last_node = hash_table->buckets[index];
    while (last_node != NULL) {
      if(last_node->key_len == key_len) {
        if (memcmp(key, last_node->key, key_len) == 0) {
          return last_node->value;
        }
      }

      last_node = last_node->next;
    }
  }

  return NULL;
}


void *hash_tab_insert(hash_tab_t *hash_table, void *key, size_t key_len, void *value, size_t value_len) {
  int index = default_hash_func(key, key_len, hash_table->size);

  node_t *next_node, *last_node;
  next_node = hash_table->buckets[index];
  last_node = NULL;

  while (next_node != NULL) {
    if (next_node->key_len == key_len) {
      if (memcmp(key, next_node->key, key_len) == 0) {
        if (next_node->value_len != value_len) {
          free(next_node->value);
          next_node->value = malloc(key_len);
          if (next_node->value == NULL) {
            return NULL;
          }
        }
        memcpy(next_node->value, value, value_len);
        next_node->value_len = value_len;
        return next_node;
      }


    }
    last_node = next_node;
    next_node = next_node->next;
  }

  node_t *p_node = (node_t *)malloc(sizeof(node_t));
  if(p_node == NULL) {
    return NULL;
  }

  p_node->key = malloc(key_len);
  p_node->value = malloc(value_len);
  if (p_node->key == NULL || p_node->value == NULL) {
    free(p_node->key);
    free(p_node->value);
    free(p_node);
    return NULL;
  }

  memcpy(p_node->key, key, key_len);
  memcpy(p_node->value, value, value_len);
  p_node->key_len = key_len;
  p_node->value_len = value_len;
  p_node->next = NULL;

  if (last_node != NULL) {
    last_node->next = p_node;
  } else {
    hash_table->buckets[index] = p_node;
  }

  hash_table->count++;
  return p_node->value;
}

void hash_tab_remove(hash_tab_t *hash_table, void *key, size_t key_len) {
  node_t *next_node, *last_node;
  int index = default_hash_func(key, key_len, hash_table->size);

  next_node = hash_table->buckets[index];
  last_node = NULL;

  while (next_node != NULL) {
    if (next_node->key_len == key_len) {
      if (memcmp(key, next_node->key, key_len) == 0) {
        free(next_node->value);
        free(next_node->key);

        if (last_node != NULL) {
          last_node->next = next_node->next;
        } else {
          hash_table->buckets[index] = next_node->next;
        }

        free(next_node);
        break;
      }
    }

    last_node = next_node;
    next_node = next_node->next;
  }
}

void *hash_tab_grow(hash_tab_t *hash_table, size_t size) {
  hash_tab_t *p_hash_table = hash_tab_init(size, hash_table->hash_func);

  if (p_hash_table == NULL) {
    return NULL;
  }

  void *iterator_return_values;
  hash_tab_iterator_t iterator;
  hash_tab_iterator_init(p_hash_table, &iterator);

  for (; iterator.key != NULL; hash_tab_iterator_increment(&iterator)) {
    iterator_return_values = hash_tab_insert(p_hash_table, iterator.key, iterator.key_len, iterator.value, iterator.value_len);
    if (iterator_return_values == NULL) {
      hash_tab_free(p_hash_table);
      return NULL;
    }
  }

  hash_tab_free(hash_table);
  return p_hash_table;
}

void hash_tab_free(hash_tab_t *hash_table) {
  node_t *next_node, *last_node;

  for (int i = 0; i < (int) hash_table->size; i++) {
    next_node = hash_table->buckets[i];
    while (next_node != NULL) {
      free(next_node->key);
      free(next_node->value);
      last_node = next_node;
      next_node = next_node->next;
      free(last_node);
    }
  }
  free(hash_table->buckets);
  free(hash_table);
}

void hash_tab_iterator_init(hash_tab_t *hash_table, hash_tab_iterator_t *hash_tab_iterator){
  hash_tab_iterator->hash_tab_internal_t.hash_table = hash_table;
  hash_tab_iterator->hash_tab_internal_t.node = NULL;
  hash_tab_iterator->hash_tab_internal_t.index = -1;

  hash_tab_iterator_increment(hash_tab_iterator);
}

void hash_tab_iterator_increment(hash_tab_iterator_t *hash_tab_iterator) {
  hash_tab_t *hash_table = hash_tab_iterator->hash_tab_internal_t.hash_table;
  int index = hash_tab_iterator->hash_tab_internal_t.index;

  if (hash_tab_iterator->hash_tab_internal_t.node == NULL || hash_tab_iterator->hash_tab_internal_t.node->next == NULL) {
    index++;
  } else {
    hash_tab_iterator->hash_tab_internal_t.node = hash_tab_iterator->hash_tab_internal_t.node->next;
    hash_tab_iterator->key = hash_tab_iterator->hash_tab_internal_t.node->key;
    hash_tab_iterator->key_len = hash_tab_iterator->hash_tab_internal_t.node->key_len;
    hash_tab_iterator->value = hash_tab_iterator->hash_tab_internal_t.node->value;
    hash_tab_iterator->value_len = hash_tab_iterator->hash_tab_internal_t.node->value_len;
    return;
  }

  while (hash_table->buckets[index] == NULL && index < (int)hash_table->size) {
    index++;
  }

  if (index >= (int) hash_table->size) {
    hash_tab_iterator->hash_tab_internal_t.node = NULL;
    hash_tab_iterator->hash_tab_internal_t.index = (int) hash_table->size;
    hash_tab_iterator->key = NULL;
    hash_tab_iterator->value = NULL;
    hash_tab_iterator->value_len = 0;
    hash_tab_iterator->key_len = 0;

    return;
  }

  hash_tab_iterator->hash_tab_internal_t.node = hash_table->buckets[index];
  hash_tab_iterator->hash_tab_internal_t.index = index;
  hash_tab_iterator->key = hash_tab_iterator->hash_tab_internal_t.node->key;
  hash_tab_iterator->value = hash_tab_iterator->hash_tab_internal_t.node->value;
  hash_tab_iterator->key_len = hash_tab_iterator->hash_tab_internal_t.node->key_len;
  hash_tab_iterator->value_len = hash_tab_iterator->hash_tab_internal_t.node->value_len;
}
