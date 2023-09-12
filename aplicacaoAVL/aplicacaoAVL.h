#include <stdio.h> /* exigencia do compilador por causa do tipo FILE */

/* declaracoes de estruturas e tipos de dados, alem de prototipos a serem
 usados na aplicacao */


/*---------- TIPOS DE DADOS --------------------------------------------------------------*/

/* Mudança do campo palavra, para um vetor de caracteres e nao um ponteiro de caracteres
   isso eh devido a necessidade de reservar um novo espaco em memoria para guardar o conteudo da
   string sem o risco de perde-la por alguma razao de sobreescricao de memoria */

// struct para construcao da avl
struct ptnodo{
    char palavra[20];   // NAO MAIS - recebe retorno da strtok
                         /* AGORA VETOR DE CARACTERES, SEM SE PREOCUPAR COM STRTOK */
    float pontuacao;    // armazena polaridade da palavra
    struct ptnodo* esq; // ponteiros para os nodos filhos
    struct ptnodo* dir;
    int fb;             // campo fundamental para uma avl, fator de balanceamento.
};
typedef struct ptnodo ptNodoAVL;

// struct para construcao da lista simplesmente encadeada
struct ptlse{
    float escore; // para armazenar escore das opinioes
    struct ptlse* prox; // sequencia ordenada de nodos
};
typedef struct ptlse ptLSE;

/*---------------------------------------------------------------------------------------------------*/

/*----------- PROTÓTIPOS-----------------------------------------------------------------------------*/

ptNodoAVL* inicializaArvoreAVL(void);
ptNodoAVL* insereNodoAVL(ptNodoAVL* raiz, ptNodoAVL* folha);
ptNodoAVL* criaArvoreAVL(FILE* arq_lexico, ptNodoAVL* a);
float buscaAVL(char *palavra, ptNodoAVL* a, int *comp);
ptLSE* inicializaLSE(void);
ptLSE* insereNoFimLSE(ptLSE* ptlista, float escore);
ptLSE* removeDoInicio(ptLSE* ptlista);
ptLSE* destroiLSE(ptLSE* ptlista);
ptLSE* leTXT(FILE* arq_entrada, ptNodoAVL* a, ptLSE* lista, int* comp);
ptNodoAVL* destroiAVL(ptNodoAVL* a);
void escreveEscoresNova(FILE* txt_entrada,FILE* txt_saida ,ptLSE* lista);
void escreveComparacao(FILE* txt_saida, int comp);
int alturaNodo(ptNodoAVL* nodo);
int fatorBalaceamento(ptNodoAVL* nodo);
ptNodoAVL* atualizaFB(ptNodoAVL* a);
int fatorAVL(ptNodoAVL* a);
ptNodoAVL* menorDesbalanceado(ptNodoAVL* a);
ptNodoAVL* rotacao_direita(ptNodoAVL* p);
ptNodoAVL* rotacao_esquerda(ptNodoAVL* p);
ptNodoAVL* rotacao_dupla_direita(ptNodoAVL* p);
ptNodoAVL* rotacao_dupla_esquerda(ptNodoAVL* p);
ptNodoAVL* aplicandoRotacao(ptNodoAVL* nodo_desbalanceado, ptNodoAVL* raiz);

