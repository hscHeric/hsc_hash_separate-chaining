
# Implementação de Hash Table com Separate Chaining em C
Esta implementação em C oferece uma estrutura de dados de tabela de hash usando a técnica de open hashing (ou hashing com encadeamento separado). Esse método lida com colisões permitindo múltiplos elementos em uma mesma posição da tabela por meio de uma lista encadeada. Além disso, a implementação inclui um iterador que permite percorrer os elementos armazenados na tabela de hash.

## Arquivos

A estrutura da implementação consiste em dois arquivos:

- `hash_tab.h`: Contém as definições das estruturas, protótipos de funções, documentação e a estrutura do iterador para manipulação da tabela de hash.
- `hash_tab.c`: Implementações das funções definidas em `hash_tab.h`.


## Estruturas de dados

As estruturas de dados utilizadas são três:

### node_t

```c
typedef struct node_t {
  void *key;
  size_t key_len;
  void *value;
  size_t value_len;
  struct node_t *next;
} node_t;
```

A estrutura node_t representa um nó na tabela de hash com encadeamento separado. Ela foi projetada com flexibilidade para permitir o uso de qualquer tipo de dado como chave e valor, graças ao uso de ponteiros void *. Essa abordagem permite que diferentes tipos de dados, incluindo tipos personalizados, sejam usados como chave e valor na tabela de hash.

Esta flexibilidade é proporcionada pela utilização de ponteiros void * para os campos key (chave) e value (valor). Isso significa que qualquer tipo de dado da linguagem C pode ser utilizado como chave e valor na tabela de hash. Por exemplo, é possível empregar tipos básicos de dados (como inteiros, caracteres, etc.) ou até mesmo tipos de dados personalizados definidos pelo usuário.

### hash_tab_t

A estrutura hash_tab_t é a estrutura principal que representa a tabela de hash com encadeamento separado. Essa estrutura foi projetada para oferecer flexibilidade e eficiência no armazenamento e recuperação de dados, permitindo o uso de qualquer tipo de dado como chave e valor.

```c
typedef struct hash_tab_t {
  node_t **buckets;
  size_t size;
  int count;
  int (*hash_func) (void *, size_t, size_t);
} hash_tab_t;
```

- `node_t **buckets:` É um vetor de ponteiros para nós (node_t) que serve como os buckets (compartimentos) da tabela de hash. Cada bucket armazena um ponteiro para o primeiro nó de uma lista encadeada, permitindo a manipulação eficiente de colisões.

- `size_t size:` Indica o tamanho total da tabela de hash, determinando o número de buckets disponíveis para distribuir os elementos.

- `int count:` Representa o número atual de elementos armazenados na tabela de hash. Esse valor é atualizado dinamicamente durante a inserção e remoção de elementos.

- `int (*hash_func) (void *, size_t, size_t):` É um ponteiro para uma função de hash personalizada ou a função de hash padrão. Essa função é responsável por calcular o índice onde um elemento será armazenado na tabela de hash, com base na chave fornecida. A flexibilidade dessa função permite a adaptação da tabela de hash para diferentes tipos de dados e tamanhos de tabela.

## hash_tab_iterator_t
A estrutura hash_tab_iterator_t representa um iterador utilizado para percorrer os elementos armazenados na tabela de hash.

```c
typedef struct hash_tab_iterator_t {
  void *key;
  void *value;
  size_t key_len;
  size_t value_len;

  struct hash_tab_internal_t {
    hash_tab_t *hash_table;
    node_t *node;
    int index;
  } hash_tab_internal_t;

} hash_tab_iterator_t;
```

- `void *key:` Ponteiro para a chave do nó atual no iterador.
void *value: Ponteiro para o valor do nó atual no iterador.
- `size_t key_len:` Tamanho da chave.
- `size_t value_len:` Tamanho do valor.

Além dos campos convencionais de chave, valor e seus tamanhos, a estrutura `hash_tab_iterator_t` possui um campo adicional:

Estrutura interna hash_tab_internal_t: Essa estrutura interna contém informações essenciais para o funcionamento do iterador:
``hash_tab_t *hash_table:` Ponteiro para a tabela de hash associada ao iterador.
- `node_t *node:` Ponteiro para o nó atual dentro da tabela de hash.
- `int index:` Índice do nó atual na tabela de hash.

## Exemplos 
Aqui estão exemplos de uso funções disponíveis nesta implementação:


### hash_tab_init
A função hash_tab_init é responsável por criar e inicializar uma tabela de hash para armazenar elementos, sendo uma operação fundamental para utilizar a estrutura de dados de tabela de hash oferecida neste projeto.


```c
//Assinatura da função: hash_tab_t *hash_tab_init(size_t size, int (*hash_func)(void *key, size_t keylen, size_t hash_tab_size));


hash_tab_t *ht = hash_tab_init(100, NULL);
if (ht == NULL) {
    printf("Erro ao inicializar a tabela de hash!\n");
    // Lidar com o erro, se necessário
}
``` 

Nesse caso NULL é passado no lugar da função hash, logo a função de hashing padrão é utilizada, mas pode ser usado uma funções implementada pelo usuário

void *hash_tab_insert(hash_tab_t *hash_table, void *key, size_t key_len, void *value, size_t value_len);

### hash_tab_insert
A função hash_tab_insert é responsável por inserir um novo elemento na tabela de hash, associando uma chave a um valor. Esta operação é essencial para adicionar e armazenar dados na estrutura de tabela de hash.

Supondo que a hash_tab_t de nome ht já foi criada, podemos adicionar um par chave, valor na hash table da seguinte forma:

```c
//Assinatura da função: void *hash_tab_insert(hash_tab_t *hash_table, void *key, size_t key_len, void *value, size_t value_len);

hash_tab_t *ht = hash_tab_init(100, NULL);
if (ht == NULL) {
    printf("Erro ao inicializar a tabela de hash!\n");
    // Lidar com o erro, se necessário
}
  char *key1 = "chave1";
  char *value1 = "valor1";
  hash_tab_insert(ht, key1, strlen(key1) + 1, value1, strlen(value1) + 1);
```

### hash_tab_search
A função hash_tab_search é utilizada para buscar um elemento na tabela de hash, localizando o valor associado a uma chave específica.

```c
//Assinatura da função: void *hash_tab_search(hash_tab_t *hash_table, void *key, size_t key_len);

char *key1 = "chave1";
char *value1 = "valor1";
hash_tab_insert(ht, key1, strlen(key1) + 1, value1, strlen(value1) + 1);

char *result = (char*) hash_tab_search(hash_table, key1, strlen(key1) + 1);
if (result != NULL) {
    printf("Valor encontrado: %s\n", (char *)result);
} else {
    printf("Chave não encontrada na tabela de hash!\n");
    //Tratar o erro se necessario
}
```

### hash_tab_remove
A função hash_tab_remove é utilizada para remover um elemento (nó) específico da tabela de hash, identificado pela chave correspondente.


```c
//Assinatura da função: void hash_tab_remove(hash_tab_t *hash_table, void *key, size_t key_len);

char *key1 = "chave1";
char *value1 = "valor1";
hash_tab_insert(ht, key1, strlen(key1) + 1, value1, strlen(value1) + 1);

hash_tab_remove(hash_table, key1, strlen(key1) + 1); //Removeu o membro de chave == key1
```
### hash_tab_grow
A função hash_tab_grow é utilizada para aumentar o tamanho da tabela de hash, reorganizando os elementos existentes para acomodar uma capacidade maior de armazenamento.


```c
//Assinatura da função: void *hash_tab_grow(hash_tab_t *hash_table, size_t size);


hash_tab_t *ht = hash_tab_init(10, NULL);
if (ht == NULL) {
    printf("Erro ao inicializar a tabela de hash!\n");
    // Lidar com o erro, se necessário
}

hash_tab_t *ht2 = (hash_tab_t *)hash_tab_grow(ht, 15);
if(ht2 == NULL) {
    // Lidar com o erro, se necessário
}
```
### hash_tab_free
A função hash_tab_free é utilizada para liberar a memória alocada pela tabela de hash, incluindo todos os seus elementos.

```c
//Assinatura da função: void hash_tab_free(hash_tab_t *hash_table;

hash_tab_t *ht = hash_tab_init(100, NULL);
if (ht == NULL) {
    printf("Erro ao inicializar a tabela de hash!\n");
    // Lidar com o erro, se necessário
}
  char *key1 = "chave1";
  char *value1 = "valor1";
  hash_tab_insert(ht, key1, strlen(key1) + 1, value1, strlen(value1) + 1);

  hash_tab_free(ht);
```

### Exemplo de uso do iterador
Considerando uma tabela de hash já inicializada de nome ht com N buckets e X valores armazenados tal que X <= N, Vamos mostrar um exemplo em que se imprime a par chave e valor para cada um dos X elementos da hash table.


```c
//Assinatura da função de inicialização: void hash_tab_iterator_init(hash_tab_t *hash_table, hash_tab_iterator_t *iterator);
//Assinatura de função de incremento do iterador: void hash_tab_iterator_increment(hash_tab_iterator_t *iterator);

  hash_tab_iterator_t i; // Cria um iterador i
  hash_tab_iterator_init(ht, &i); //Inicializa o iterador passando a hash table e o endereço do iterador
  for (; i.key != NULL; hash_tab_iterator_increment(&i)) {
    printf("key: %s, value: %s\n", (char*)i.key, (char*)i.value); 
  }

```
Aqui partimos do pré-suposto que tanto a key, quanto value são strings, mas basta mudar o printf para a formatação desejada. Vale também lembrar que não estamos em uma linguagem orientada a objetos, logos quando iteramos usando esse iterador estamos apenas usando copias dos valores presentes na hash table. Caso queria por exemplo remover todos os elementos da hash table precisara usar as demais funções analizadas anteriormente.

## **Contribuindo**

Se você deseja contribuir para este projeto, sinta-se à vontade para abrir uma issue ou enviar um pull request. Será um prazer receber feedbacks e melhorar a implementação mais este programa.

## **Licença**

Este projeto está licenciado sob a licença MIT. Consulte o arquivo LICENSE para obter mais informações.
