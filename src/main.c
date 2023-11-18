#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_tab.h"

static void print_hash_tab(hash_tab_t *hash_table) {
  printf("Tabela de hash:\n");
  printf("Tamanho: %zu\n", hash_table->size);
  printf("Contagem: %d\n", hash_table->count);
  printf("Buckets:\n");
  for (int i = 0; i < (int) hash_table->size; i++) {
    node_t *node = hash_table->buckets[i];
    printf("[%d]: ", i);
    while (node != NULL) {
      printf("(%s, %s) -> ", (char *)node->key, (char *)node->value);
      node = node->next;
    }
    printf("NULL\n");
  }
  printf("\n");
}

int main() {
    // Inicializa a tabela hash
    hash_tab_t *ht = hash_tab_init(10, NULL);

    // Insere alguns valores
    char *key1 = "chave1";
    char *value1 = "valor1";
    hash_tab_insert(ht, key1, strlen(key1) + 1, value1, strlen(value1) + 1);

    char *key2 = "chave2";
    char *value2 = "valor2";
    hash_tab_insert(ht, key2, strlen(key2) + 1, value2, strlen(value2) + 1);

    char *key3 = "chave3";
    char *value3 = "valor3";
    hash_tab_insert(ht, key3, strlen(key3) + 1, value3, strlen(value3) + 1);

    // Imprime a tabela hash
    print_hash_tab(ht);

    // Remove alguns valores
    hash_tab_remove(ht, key1, strlen(key1) + 1);
    hash_tab_remove(ht, key2, strlen(key2) + 1);

    hash_tab_t *ht2 = hash_tab_grow(ht, 15); // TODO: ERROR AO COPIAR VALORES (Olha o iterator e a grow)

    print_hash_tab(ht2);

    return 0;
}
