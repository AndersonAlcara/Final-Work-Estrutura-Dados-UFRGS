#include <stdio.h> /* exigencia do compilador por causa do tipo FILE */

/* declaracoes de estruturas e tipos de dados, alem de prototipos a serem
 usados na aplicacao */


/*---------- TIPOS DE DADOS --------------------------------------------------------------*/

/* Mudança do campo palavra para um vetor de caracteres e nao um ponteiro de caracteres ocorre
   devido a necessidade de reservar um novo espaco em memoria para guardar o conteudo da
   string sem o risco de perde-la por alguma razao de sobreescricao de memoria */

// struct para construcao da ABP
struct ptnodo{
    char palavra[20];   // NAO MAIS - recebe retorno da strtok
                        /* AGORA VETOR DE CARACTERES, SEM SE PREOCUPAR COM STRTOK */
    float pontuacao;    // armazena polaridade da palavra
    struct ptnodo* esq; // ponteiros para os nodos filhos
    struct ptnodo* dir;
};
typedef struct ptnodo ptNodoABP;

// struct para construcao da lista simplesmente encadeada
struct ptlse{
    float escore;       // para armazenar escore das opinioes
    struct ptlse* prox; // sequencia ordenada de nodos
};
typedef struct ptlse ptLSE;

/*---------------------------------------------------------------------------------------------------*/

/*----------- PROTÓTIPOS-----------------------------------------------------------------------------*/

ptNodoABP* inicializaArvoreABP(void);
ptNodoABP* insereNodoABP(ptNodoABP* raiz, ptNodoABP* folha);
ptNodoABP* criaArvoreABP(FILE* arq_lexico, ptNodoABP* a);
float buscaABP(char *palavra, ptNodoABP* a, int *comp);
ptLSE* inicializaLSE(void);
ptLSE* insereNoFimLSE(ptLSE* ptlista, float escore);
ptLSE* removeDoInicio(ptLSE* ptlista);
ptLSE* destroiLSE(ptLSE* ptlista);
ptLSE* leTXT(FILE* arq_entrada, ptNodoABP* a, ptLSE* lista, int* comp);
ptNodoABP* destroiABP(ptNodoABP* a);
void escreveEscoresNova(FILE* txt_entrada,FILE* txt_saida ,ptLSE* lista);
void escreveComparacao(FILE* txt_saida, int comp);
