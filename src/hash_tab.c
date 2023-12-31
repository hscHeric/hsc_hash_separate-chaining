#include "hash_tab.h"
#include <stdlib.h>
#include <string.h>


static int default_hash_func(void* key, size_t key_len, size_t hash_tab_size){
    int sum = 0;
    for(int i = 0; i < (int) key_len; i++){
         sum += ((unsigned char *) key)[i] * (i + 1); // (i + 1) para evitar colisões entre palavras que são anagramas
    }

    return (sum % (int)hash_tab_size); // Retorna o resto da divisão da soma dos caracteres pela quantidade de buckets
}


hash_tab_t *hash_tab_init(size_t size, int (*hash_func)(void *key, size_t keylen, size_t hash_tab_size)) {
  hash_tab_t *hash_tab = (hash_tab_t *)malloc(sizeof(hash_tab_t));
  if(hash_tab == NULL) {
    return NULL; // Erro ao alocar memória
  }

  hash_tab->buckets = (node_t**)malloc(sizeof(node_t*) * size);
  if(hash_tab->buckets == NULL) {
    free(hash_tab);
    return NULL; // Erro ao alocar memória
  }

  hash_tab->size = size;
  hash_tab->count = 0;

  // Inicializa os buckets com NULL
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
  
  if(hash_table->buckets[index] != NULL) { // Se o bucket não estiver vazio
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

  // Procura se já existe um nó com a mesma chave
  while (next_node != NULL) {
    if (next_node->key_len == key_len) {
      // Se já existe um nó com a mesma chave, atualiza o valor
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

  // Se não existe um nó com a mesma chave, cria um novo nó
  node_t *p_node = (node_t *)malloc(sizeof(node_t));
  if(p_node == NULL) {
    return NULL;
  }

  // Aloca memória para a chave e o valor
  p_node->key = malloc(key_len);
  p_node->value = malloc(value_len);
  if (p_node->key == NULL || p_node->value == NULL) {
    free(p_node->key);
    free(p_node->value);
    free(p_node);
    return NULL;
  }

  // Copia a chave e o valor para o nó
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

  while (next_node != NULL) { // Procura o nó com a chave
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
  hash_tab_t *new_hash = hash_tab_init(size, hash_table->hash_func);
  if (new_hash == NULL) {
    return NULL;
  }

  void* i_rv; //Iterator return values
  hash_tab_iterator_t i;
  hash_tab_iterator_init(hash_table, &i);
  for (; i.key != NULL; hash_tab_iterator_increment(&i)) {
    i_rv = hash_tab_insert(new_hash, i.key, i.key_len, i.value, i.value_len);
    if (i_rv == NULL) { 
      //Erro de inserção
      hash_tab_free(new_hash);
      return NULL;
    }
  }

  //Destroi a hash_tab antiga e retorna a nova hash_table com o novo valor
  hash_tab_free(hash_table);
  return new_hash;
}

void hash_tab_free(hash_tab_t *hash_table) {
  node_t *next_node, *last_node;
  
  // Libera a memória de cada nó
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

void hash_tab_iterator_init(hash_tab_t *hash_table, hash_tab_iterator_t *iterator){
  iterator->hash_tab_internal_t.hash_table = hash_table; //Seta a hash a ser iterada
  iterator->hash_tab_internal_t.node = NULL;
  iterator->hash_tab_internal_t.index = -1;

  hash_tab_iterator_increment(iterator); //incrementa o iterador para 0
}

void hash_tab_iterator_increment(hash_tab_iterator_t *iterator) {
  hash_tab_t *hashtable = iterator->hash_tab_internal_t.hash_table;
  int index = iterator->hash_tab_internal_t.index;
  
  if (iterator->hash_tab_internal_t.node == NULL || iterator->hash_tab_internal_t.node->next == NULL) {
    index++;
  } else {
    //Proximo nó da lista
    iterator->hash_tab_internal_t.node = iterator->hash_tab_internal_t.node->next;
    iterator->key = iterator->hash_tab_internal_t.node->key;
    iterator->value = iterator->hash_tab_internal_t.node->value;
    iterator->key_len = iterator->hash_tab_internal_t.node->key_len;
    iterator->value_len = iterator->hash_tab_internal_t.node->value_len;
    return;
  }

  //Procura o próximo nó
  while (hashtable->buckets[index] == NULL && index < (int) hashtable->size) {
    index++; //Enquanto o valor dos buckets[index] for nulo e não tiver chegado ao final da hash_tab
  }

  if (index >= (int) hashtable->size) {
    //Fim da hash_tab 

    iterator->hash_tab_internal_t.node = NULL;
    iterator->hash_tab_internal_t.index = (int) hashtable->size;

    iterator->key = NULL;
    iterator->value = NULL;
    iterator->value_len = 0;
    iterator->key_len = 0;

    return;
  }

  //Proximo item da hash_tab
  iterator->hash_tab_internal_t.node = hashtable->buckets[index];
  iterator->hash_tab_internal_t.index = index;
  iterator->key = iterator->hash_tab_internal_t.node->key;
  iterator->value = iterator->hash_tab_internal_t.node->value;
  iterator->key_len = iterator->hash_tab_internal_t.node->key_len;
  iterator->value_len = iterator->hash_tab_internal_t.node->value_len;
}
