#ifndef _HASH_TAB_H_
#define _HASH_TAB_H_


#include <stddef.h>

/**
 * @brief Estrutura de dados que representa um nó da tabela de hash.
 * @param key Chave do nó.
 * @param key_len Tamanho da chave.
 * @param value Valor do nó.
 * @param value_len Tamanho do valor.
 * @param next Ponteiro para o próximo nó.
 */
typedef struct node_t {
  void *key;
  size_t key_len;
  void *value;
  size_t value_len;
  struct node_t *next;
} node_t;

/**
 * @brief Estrutura de dados que representa uma tabela de hash.
 * @param buckets Vetor de nós.
 * @param size Tamanho da tabela.
 * @param count Número de nós na tabela.
 * @param hash_func Função de hash.
 */
typedef struct hash_tab_t {
  node_t **buckets;
  size_t size;
  int count;
  int (*hash_func) (void *, size_t, size_t);
} hash_tab_t;

/**
 * @brief Estrutura de dados que representa um iterador da tabela de hash.
 * @param key Chave do nó.
 * @param key_len Tamanho da chave.
 * @param value Valor do nó.
 * @param value_len Tamanho do valor.
 * @param hash_table Tabela de hash.
 * @param node Nó atual.
 * @param index Índice do nó atual.
 */
typedef struct hash_tab_iterator_t {
  void *key;
  void *value;
  size_t key_len;
  size_t value_len;

  /**
   * @brief Estrutura de dados interna do iterador da tabela de hash.
   * @param hash_table Tabela de hash.
   * @param node Nó atual.
   * @param index Índice do nó atual.
  */
  struct hash_tab_internal_t {
    hash_tab_t *hash_table;
    node_t *node;
    int index;
  } hash_tab_internal_t;

} hash_tab_iterator_t;

/**
 * @brief Inicializa uma tabela de hash.
 * 
 * @param size Tamanho da tabela.
 * @param hash_func Função de hash, se NULL, usa a função de hash padrão.
 * @return hash_tab_t* Ponteiro para a tabela de hash.
 */
hash_tab_t *hash_tab_init(size_t size, int (*hash_func)(void *key, size_t keylen, size_t hash_tab_size));

/**
 * @brief Busca um nó na tabela de hash.
 * 
 * @param hash_table Tabela de hash.
 * @param key Chave do nó.
 * @param key_len Tamanho da chave.
 * @return void* Ponteiro para o valor do nó.
 */
void *hash_tab_search(hash_tab_t *hash_table, void *key, size_t key_len);

/**
 * @brief Insere um nó na tabela de hash.
 * 
 * @param hash_table Tabela de hash.
 * @param key Chave do nó.
 * @param key_len Tamanho da chave.
 * @param value Valor do nó.
 * @param value_len Tamanho do valor.
 * @return void* Ponteiro para o valor do nó.
 */
void *hash_tab_insert(hash_tab_t *hash_table, void *key, size_t key_len, void *value, size_t value_len);

/**
 * @brief Remover um nó da tabela de hash.
 * 
 * @param hash_table Tabela de hash.
 * @param key Chave do nó.
 * @param key_len Tamanho da chave.
 */
void hash_tab_remove(hash_tab_t *hash_table, void *key, size_t key_len);

/**
 * @brief Aumenta o tamanho da tabela de hash.
 * 
 * @param hash_table Tabela de hash.
 * @param size Tamanho da tabela.
 * @return void* Ponteiro para a nova tabela de hash.
 */
void *hash_tab_grow(hash_tab_t *hash_table, size_t size);

/**
 * @brief Libera a memória alocada pela tabela de hash.
 * 
 * @param hash_table Tabela de hash.
 */
void hash_tab_free(hash_tab_t *hash_table);

/**
 * @brief Inicializa um iterador da tabela de hash.
 * 
 * @param hash_table Tabela de hash.
 * @param hash_tab_iterator Iterador da tabela de hash.
 */
void hash_tab_iterator_init(hash_tab_t *hash_table, hash_tab_iterator_t *iterator);

/**
 * @brief Incrementa o iterador da tabela de hash.
 * 
 * @param hash_tab_iterator Iterador da tabela de hash.
 */
void hash_tab_iterator_increment(hash_tab_iterator_t *iterator);

#endif // !_HASH_TAB_H_
