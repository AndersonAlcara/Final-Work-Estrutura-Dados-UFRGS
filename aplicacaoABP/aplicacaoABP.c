/* Arquivo de implementacao das funcoes a auxiliarem na criacao de um arquivo de saida automatico, com as avalicaoes das
   opinioes contidas nas sentencas juntamente com a qtd de comparacoes necessarias para isso. */

/*---bibliotecas necessarias-----*/

#include "aplicacaoABP.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Obj : inicializar com null ponteiro para raiz da arvore .
   ent : vazia
   saida : ponteiro null. */
ptNodoABP* inicializaArvoreABP(void){
    return NULL;
}

/* Obj: posicionar adequademente cada nodo em sua posicao na abp
   ent: raiz e nodo folha, ja alocado
   saida: raiz de uma arvore atualizada com o novo nodo */
ptNodoABP* insereNodoABP(ptNodoABP* raiz, ptNodoABP* folha){
    // se a raiz eh nula, entao folha é raiz
    if(raiz==NULL){
        raiz = folha;
        return raiz;
    }
    // senao, procurar posicao na abp
    else{
        // se verdade, folha vai a esquerda
        if(strcmp(raiz->palavra,folha->palavra)>0){
            raiz->esq = insereNodoABP(raiz->esq, folha);
            return raiz;
        }
        // senao, folha vai a direita
        else{
            raiz->dir = insereNodoABP(raiz->dir, folha);
            return raiz;
        }
    }
}

/* Obj: Criar uma abp a partir de um arquivo .txt, com um formato tal que temos palavra e escore na mesma linha.
   Formamos um nodo da arvore com cada linha que tenha esse formato.
   Ent: arquivo .txt aberto para leitura, em formato especifico, e ponteiro para arvore já inicializado na main
   Sai: ponteiro para raiz da arvore com todos os nodos possiveeis. */
ptNodoABP* criaArvoreABP(FILE* arq_lexico, ptNodoABP* a){
    char separador[]= {" ,?!;'@\"$#=><()][}{:\n\t"}; // usado na strtok
    char linha[50];     // armazena palavra e polaridade
    ptNodoABP *raiz;
    char *palavra_aux;  //Para evitar colocar null como palavra na arvore ABP

    // temos de inicializar a raiz
    raiz = inicializaArvoreABP();
    // a funcao insere nodo nao aloca memoria para tal, logo devemos fazer aqui
    while(fgets(linha,50,arq_lexico)!=NULL){
        palavra_aux = strtok(linha, separador);
        // temos de evitar inserir vazio como palavra
        if(palavra_aux!=NULL){
            a=(ptNodoABP*)malloc(sizeof(ptNodoABP));
            /* QUEREMOS FIXAR EM MEMORIA O CONTEUDO APONTADO, PARA NAO PERDER NA SOBREESCRICAO DO VETOR LINHA */
            strcpy(a->palavra,palavra_aux); // Assim mantemos em memoria numa posicao que nao sera sobreescrito o valor da string
            a->pontuacao = atof(strtok(NULL, separador)); // pegando a polaridade da segunda palavra da linha e convertendo.
            // isso vamos deixar para o insere, entao colocamos null
            a->dir=NULL;
            a->esq=NULL;
            // verifica se esgotou o vetor linha
            if(strtok(NULL, separador)==NULL)
                raiz = insereNodoABP(raiz, a);
        }
    }
    return raiz;
}

/* Obj : achar um palavra especifica na arvore e retornar sua pontuacao caso exista, senao retorna zero
   Ent : string procurada, raiz abp e o ponteiro de um contador de acessos declarado na main
   sai : um float */
float buscaABP(char *palavra, ptNodoABP* a, int *comp){

    if(a==NULL) return 0; // caso que nao encontramos a palavra na arvore.
    else{
        *comp = *comp + 1; // conta o numero de acessos a nodos até achar ou nao a um nodo com a palavra
        // verifica se nodo contem a palavra do texto
        if(strcmp(palavra,a->palavra)==0)
            return a->pontuacao;
        else
            // senao, verifica se a palavra procurado eh lexicograficamente maior que o nodo
            if(strcmp(palavra,a->palavra)>0)
                return buscaABP(palavra, a->dir, comp);
            else
                // senao, ela eh menor.
                return buscaABP(palavra, a->esq, comp);
    }
}

/* obj: inicializar com null ponteiro para inicio de uma lse
   ent: vazio
   sai: ponteiro lse para null */
ptLSE* inicializaLSE(void){ return NULL; }

/* obj: inserir na lista, sempre na ultima posicao possivel.
   ent: ponteiro para inicio da lista e pontuacao a ser inserida na lista
   sai: ponteiro para inicio da lista, agora atualizada com mais um nodo ao seu final */
ptLSE* insereNoFimLSE(ptLSE* ptlista, float escore){
    // se ptlista==null
    if(ptlista==NULL){
        ptlista = (ptLSE*)malloc(sizeof(ptLSE));
        ptlista->escore = escore;
        ptlista->prox = NULL;
        return ptlista;
    }else{
        ptlista->prox = insereNoFimLSE(ptlista->prox, escore);
        return ptlista;
    }
}

/* obj: remover nodo do inicio da lista
   ent: ponteiro para inicio da lista
   sai: ponteiro para proximo elemento da lista, sendo desalocado o elemento anterior */
ptLSE* removeDoInicio(ptLSE* ptlista){
    ptLSE* ptaux;
    if(ptlista==NULL) return NULL;
    else{
        ptaux = ptlista->prox;
        free(ptlista);
        return ptaux;
    }
}

/* obj: desalocar todos os nodos da lista
   ent: ponteiro para inicio da lista
   sai: ponteiro para null indicando a destruicao da lista */
ptLSE* destroiLSE(ptLSE* ptlista){
    do{
        ptlista = removeDoInicio(ptlista);
    }while(ptlista!=NULL);
    return ptlista;
}

/* obj: gerar uma lista com os escores de cada sentenca em um nodo, na mesma ordem dos sentencas
   ent: arquivo txt de sentencas em cada linha, aberto para leitura, uma arvore de lexico pronta,
   uma lista ja inicializada e por fim o contador de acessos
   sai: ponteiro para inicio da lista, lista que tem os escores de cada avaliacao e alem disso conseguimos
   o numero de comparacooes total */
ptLSE* leTXT(FILE* arq_entrada, ptNodoABP* a, ptLSE* lista, int* comp){
    float escore = 0; // cada linha do arquivo terá um escore.
    char separador[]= {" ,.?!;'@\"$#=><()][}{:\n\t"}; // usado na strtok
    char linha[1000]; // contem a sentenca por inteiro.
    char *palavra;    // evitar palavra vazia
    //percorre todo o arquivo lendo linha por linha
    while (fgets(linha,1000,arq_entrada))
    {
        palavra = strtok(linha, separador);
        while (palavra != NULL)
        {
            // observe a funcao strlwr(), vamos usá-la para enviar para busca e contornar o case sensitive
            // buscaABP retorna um float com o valor de cada palavra a ser acumulado
            escore = escore + buscaABP(strlwr(palavra), a, comp);
            palavra = strtok(NULL, separador);
        }
            // temos de armazenar o escore numa lista para depois colocar no  txt_saida
            lista = insereNoFimLSE(lista, escore);
            // setar escore para zero, pois é uma nova linha
            escore = 0;
    }
    return lista;
}

/* obj: desalocar todos os nodos da abp
   ent: ponteiro para raiz
   sai: ponteiro para null, indicando que foi toda desalocada*/
ptNodoABP* destroiABP(ptNodoABP* a){
    if(a==NULL) return NULL; // caso da raiz de entrada ser vazia;
    else{
        // se a eh folha, remove a e retorna pro pai de a, se houver.
        if(a->esq==NULL&&a->dir==NULL){
            free(a); // libera a.
            return NULL; // nodo pai passa a ter nodo filho null.
        }
        // se nodo nao eh folha temos de percorrer a arvore
        else{
            // se nodo, tem 2 filhos
            if(a->dir!=NULL&&a->esq!=NULL){
                a->dir = destroiABP(a->dir);
                a->esq = destroiABP(a->esq);
                return destroiABP(a);
            }
            // com certeza um tem filho.
            else
                if(a->dir!=NULL){
                    a->dir = destroiABP(a->dir);
                    return destroiABP(a);
                }
                // nesse caso, eh necessariamente a esquerda
                //(a->esq!=NULL)
                else{
                    a->esq = destroiABP(a->esq);
                    return destroiABP(a);
                }
        }
    }
}

/* obj: escrever os escores da lse na mesma sequencia das linhas do arquivo de comentarios em um novo arquivo
   criado na main, gerando um arquivo identico ao de comentarios porem com os escores em cada linha
   ent: arquivom de comentarios com ponteiro de arquivo desde o inicio, arquivo criado para receber os comentarios
   junto com suas avaliacoes e por fim o ponteiro para inicio da lista com os escores.
   sai: arquivo criado com conteudo do original mais avaliacoes passadas tudo num formato adequado.*/
void escreveEscoresNova(FILE* txt_entrada,FILE* txt_saida ,ptLSE* lista){
    char linha[1000]; // guarda sentencas de cada linha do arquivo

    while(fgets(linha,1000,txt_entrada)){
        // escreve no inicio de cada nova linha com esse formato.
        fprintf(txt_saida, "(Avaliacao:%.2f) - %s", lista->escore, linha);
        // proxima linha, proximo escore
        lista=lista->prox;
        // acha o final da linha e coloca indicador de ponteiro de arquivo na proxima linha.
    }
}
/* obj: escrever em uma linha extra ao final do arquivo, ja pronto com as avaliacoes em cada linha,
   o numero de comparacoe feitas na arvore a fim de obter os escores adequados
   ent: arquivo txt saida com as avaliacoes, aberto em formato "a" e o numero de comparacoes total do arquivo
   sai : arquivo txt saida com numero total de comparacoes ao seu final */
void escreveComparacao(FILE* txt_saida, int comp){
    // escrevemos a numero de comparacoes
        fprintf(txt_saida,"\n");
        fprintf(txt_saida,"(Total de comparacoes:%d)", comp);
}
