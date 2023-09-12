/* funcoes a auxiliarem na criacao de um arquivo de saida automatico, com as avalicaoes das
   opinioes contidas nas sentencas */

#include "aplicacaoAVL.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Obj: inicializar com null ponteiro para raiz da arvore .
   saida: ponteiro null. */
ptNodoAVL* inicializaArvoreAVL(void){
    return NULL;
}

/* Obj: posicionar adequademente cada nodo em sua posicao na avl
   ent: raiz e nodo folha, ja alocado
   saida : raiz de uma arvore atualizada com o novo nodo */
ptNodoAVL* insereNodoAVL(ptNodoAVL* raiz, ptNodoAVL* folha){
    // se a raiz eh nula, entao folha é raiz
    if(raiz==NULL){
        raiz = folha;
        return raiz;
    }
    // senao, procurar posicao na abp
    else{
        // se verdade, folha vai a esquerda
        if(strcmp(raiz->palavra,folha->palavra)>0){
            raiz->esq = insereNodoAVL(raiz->esq, folha);
            return raiz;
        }
        // senao, folha vai a direita
        else{
            raiz->dir = insereNodoAVL(raiz->dir, folha);
            return raiz;
        }
    }
}

/* Obj: Criar uma avl a partir de um arquivo .txt, com um formato tal que temos palavra e escore da mesma
   formamos um nodo da arvore com cada linha que tenha esse formato.
   Ent: arquivo .txt aberto para leitura, em formato especifico, e ponteiro para arvore já inicializado na main
   Sai: ponteiro para raiz da arvore com todos os nodos possiveeis. */
ptNodoAVL* criaArvoreAVL(FILE* arq_lexico, ptNodoAVL* a){
    char separador[]= {" ,?!;'@\"$#=><()][}{:\n\t"};
    char linha[50];
    ptNodoAVL *raiz;
    ptNodoAVL *nodo_desbalanceado;
    char *palavra_aux; // Para evitar colocar null como palavra na arvore ABP

    // temos de inicializar a raiz
    raiz = inicializaArvoreAVL();
    nodo_desbalanceado = inicializaArvoreAVL();
    // a funcao insere nodo nao aloca memoria para tal, logo devemos fazer aqui
    while(fgets(linha,50,arq_lexico)!=NULL){
        palavra_aux = strtok(linha, separador);
        // criacao do nodo
        if(palavra_aux!=NULL){
            a=(ptNodoAVL*)malloc(sizeof(ptNodoAVL));
            /* QUEREMOS FIXAR EM MEMORIA O CONTEUDO APONTADO, PARA NAO PERDER NA SOBREESCRICAO DO VETOR LINHA */
            strcpy(a->palavra,palavra_aux);
            a->pontuacao = atof(strtok(NULL, separador)); // pegando a polaridade da segunda palavra da linha.
            // isso vamos deixar para o insere, entao colocamos null
            a->dir=NULL;
            a->esq=NULL;
            // todo nodo a inserir, iniciamos seu fator de balanceamento como zero por convencao
            // apos inserir devemos fazer uma atualizacao com os fatores de todos os nodos da avl.
            a->fb = 0;
            // devemos inserir normalmente o nodo e depois verificar se houve um desbalanceamento em algum
            // dos nodos da arvore.
            if(strtok(NULL, separador)==NULL){
                raiz = insereNodoAVL(raiz, a);
                // aqui começa as diferencas fundamentais entre abp e avl
                // a cada insercao na arvore devemos verificar se ha desbalanceamento
                // primeiramente, devemos atualizar todos os fatores de balaceamento da arvore para depois verificarmos
                // se há pelo menos um nodo desbalanceado.
                raiz = atualizaFB(raiz);
                // se eh verdade, pelo menos um nodo esta desbalanceado.
                if(abs(fatorAVL(raiz))>1){
                    // devemos descobrir qual nodo eh o nodo desbalanceado de menor altura
                    do{
                        nodo_desbalanceado = menorDesbalanceado(raiz);
                        // aplicamos a rotacao adequada ao nodo
                        raiz = aplicandoRotacao(nodo_desbalanceado, raiz);
                        // atualizamos os fatores de toda a arvore
                        raiz = atualizaFB(raiz);
                    }while(abs(fatorAVL(raiz))>1); // verifica se arvore esta balanceada apos as operacoes
                }
                // caso nao esteja desbalanceada continuamos o processo de insercao normalmente.
            }
        }
    }
    return raiz;
}

/* Obj : achar um palavra especifica na arvore e retornar sua pontuacao caso exista, senao retorna zero
   Ent : string procurada, raiz abp e contador de acessos
   sai : um float */
float buscaAVL(char *palavra, ptNodoAVL* a, int *comp){

    if(a==NULL) return 0; // caso que nao encontramos a palavra na arvore.
    else{
        *comp = *comp + 1; // conta o numero de acessos a nodos até achar ou nao um nodo com a palavra
        // verifica se nodo contem a palavra do texto
        if(strcmp(palavra,a->palavra)==0)
            return a->pontuacao;
        else
            // senao, verifica se a palavra procurado eh lexicograficamente maior que o nodo
            if(strcmp(palavra,a->palavra)>0)
                return buscaAVL(palavra, a->dir, comp);
            else
                // senao, ela eh menor.
                return buscaAVL(palavra, a->esq, comp);
    }
}

/* obj: inicializar com null ponteiro para inicio de uma lse
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

/* obj: gerar uma lista com os escores de cada comentario em um nodo, na mesma ordem dos comentarios
   ent: arquivo txt de comentarios em cada linha, aberto para leitura, uma arvore de lexico pronta,
   uma lista ja inicializada e por fim o contador de acessos
   sai: ponteiro para inicio da lista, lista que tem os escores de cada avaliacao e alem disso conseguimos
   o numero de comparacooes total */
ptLSE* leTXT(FILE* arq_entrada, ptNodoAVL* a, ptLSE* lista, int* comp){
    float escore = 0; // cada linha do arquivo terá um escore.
    char separador[]= {" ,.?!;'@\"$#=><()][}{:\n\t"};
    char linha[1000];
    char *palavra;
    //percorre todo o arquivo lendo linha por linha
    while (fgets(linha,1000,arq_entrada))
    {
        palavra = strtok(linha, separador);
        while (palavra != NULL)
        {
            // observe a funcao strlwr(), vamos usá-la para enviar para busca e contornar o case sensitive
            // buscaABP retorna um float com o valor de cada palavra a ser acumulado
            escore = escore + buscaAVL(strlwr(palavra), a, comp);
            palavra = strtok(NULL, separador);
        }
            // temos de armazenar o escore numa lista para depois colocar no  txt_saida
            lista = insereNoFimLSE(lista, escore);
            // setar escore para zero, pois é uma nova linha
            escore = 0;
    }
    return lista;
}

/* obj: desalocar todos os nodos da avl
   ent: ponteiro para raiz
   sai: ponteiro para null, indicando que foi toda desalocada*/
ptNodoAVL* destroiAVL(ptNodoAVL* a){
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
                a->dir = destroiAVL(a->dir);
                a->esq = destroiAVL(a->esq);
                return destroiAVL(a);
            }
            // com certeza um tem filho.
            else
                if(a->dir!=NULL){
                    a->dir = destroiAVL(a->dir);
                    return destroiAVL(a);
                }
                // nesse caso, eh necessariamente a esquerda
                //(a->esq!=NULL)
                else{
                    a->esq = destroiAVL(a->esq);
                    return destroiAVL(a);
                }
        }
    }
}

/* obj: escrever os escores da lse na mesma sequencia das linhas do arquivo de comentarios em um novo arquivo
   criado na main, gernaod um arquivo identico ao de comentarios porem com os escores em cada linha
   ent: arquivom de comentarios com ponteiro de arquivo desde o inicio, arquivo criado para receber os comentarios
   junto com suas avaliacoes e por fim o ponteiro para inicio da lista com os escores.
   sai: arquivo criado com conteudo do original mais avaliacoes passadas tudo num formato adequado.*/
void escreveEscoresNova(FILE* txt_entrada,FILE* txt_saida ,ptLSE* lista){
    char linha[1000];

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

/* obj : calcular a altura do respectivo nodo
   ent : nodo
   sai : maior altura entre as alturas em relacao ao nodo inicial */
int alturaNodo(ptNodoAVL* a){
    int Alt_Esq, Alt_Dir;
    if (a == NULL)
        return 0;
    else
    {
        Alt_Esq = alturaNodo(a->esq);
        Alt_Dir = alturaNodo(a->dir);
        if (Alt_Esq > Alt_Dir)
            return (1 + Alt_Esq);
        else
            return (1 + Alt_Dir);
    }
}

/* obj : calcular o fator de balaceamento do nodo
   ent : ponteiro do nodo
   sai : inteiro que representa o fator de balanceamento do nodo */
int fatorBalaceamento(ptNodoAVL* nodo){ return alturaNodo(nodo->dir) - alturaNodo(nodo->esq);}

/* obj : atualizar os fatores de balaceamento de toda a arvore, aplicamos essa funcao apos inserir um novo nodo
   ent : ponteiro para raiz da avl
   sai : ponteiro para raiz da avl, agora com os fb atualizados */
ptNodoAVL* atualizaFB(ptNodoAVL* a){
    if(a!=NULL){
        a->fb = fatorBalaceamento(a); // o fator de balanceamento eh um numero inteiro, ou seja, pode ser negativo.
        a->esq = atualizaFB(a->esq);
        a->dir = atualizaFB(a->dir);
        return a;
    }else
        return NULL;
}


/* retorna o maior fator da avl, com ele podemos verificar se a avl esta desbalanceada */
/* obj : verificar qual o maior fator da avl, depois podemos verificar se precisa de balanceamento
   ent : ponteiro para a raiz da arvore
   saida : valor do maior fator em modulo */
int fatorAVL(ptNodoAVL* a){

    int fator;

    if(a!=NULL){
        fator = abs(alturaNodo(a->esq)-alturaNodo(a->dir));
        if(fator<fatorAVL(a->esq)){
            fator=fatorAVL(a->esq);
            return fator;
        }else if(fator<fatorAVL(a->dir)){
            fator=fatorAVL(a->dir);
            return fator;
        }else
         return fator;
    }else
        return 0;
}

/* Os nodos desbalanceados com menor altura devem sofrer as operacoes de rotacao primeiro que os nodos de maior altura */
/* devemos criar uma funcao que retorna o endereco do nodo desbalanceado com menor altura */
/* obj : retornar endereco de nodo desbalanceado com menor altura entre os desbalanceados
   ent : raiz da avl desbalanceada
   sai : ponteiro para nodo de menor altura entre os desbalanceados */
ptNodoAVL* menorDesbalanceado(ptNodoAVL* a){

    ptNodoAVL* aux;

    /* corrigindo um erro, sempre devemos verificar primeiro se é nulo */
    if(a==NULL) return NULL;
    else{
        // se (a) tiver algum descendente desbalanceado, o descendente mais distante de (a) eh o que deve ser retornado
        // caso nao haja, retorna o proprio (a) que eh desbalanceado
        if(abs(a->fb)>1){
            aux = menorDesbalanceado(a->dir);
            // se aux é difente de null, logo auxiliar tem altura menor que a, assim devemos retornar aux.
            if(aux!=NULL) return aux;
            else{
                // devemos verificar o outro lado, outra subarvore tambem
                aux = menorDesbalanceado(a->esq);
                if(aux!=NULL) return aux;
                else{
                    //senao, devemos retornar o proprio (a), pois esse nao tem descendente desbalanceado
                    return a;
                }
            }
        }else{ /* caso ele nao eh nulo, porem tambem nao eh desbalanceado */

            // devemos procurar nos outros nodos, observe que eh semelhante a estrutura acima
            // porem, caso nao achamos desbalanceamento nos descendentes de (a) nao devemos
            // retornar (a), pois este nao é desbalanceado.
            aux = menorDesbalanceado(a->dir);
            // se aux é difente de null, logo auxiliar tem altura menor que a, assim devemos retornar aux.
            if(aux!=NULL) return aux;
            else{
                // devemos verificar o outro lado, outra subarvore tambem
                aux = menorDesbalanceado(a->esq);
                if(aux!=NULL) return aux;
                else{
                    //senao, devemos retornar NULL, pois esse nao tem descendente desbalanceado e ele proprio nao eh desbalanceado.
                    return NULL;
                }
            }
        }
    }
}

/* obj : rotar a direita nodo desbalanceado
   ent : nodo desbalanceado
   sai : nodo balanceado*/
ptNodoAVL* rotacao_direita(ptNodoAVL* p)
{
    ptNodoAVL *u; // é o auxiliar que será o filho esquerdo de p

    u = p->esq;
    p->esq = u->dir;
    u->dir = p;
    //p->fb = 0; NAO PRECISA, POIS TEMOS UMA FUNCAO QUE CALCULA OS FB DE TODA ARVORE
    //p = u; NAO PRECISA, PODEMOS PASSAR O U DIRETO.

    return u;
}

/* obj : rotar a esquerda nodo desbalanceado
   ent : nodo desbalanceado
   sai : nodo balanceado*/
ptNodoAVL* rotacao_esquerda(ptNodoAVL* p)
{
    ptNodoAVL *u; // é o auxiliar que será o filho esquerdo de p

    u = p->dir;
    p->dir = u->esq;
    u->esq = p;
    //p->fb = 0; NAO PRECISA, POIS TEMOS UMA FUNCAO QUE CALCULA OS FB DE TODA ARVORE
    //p = u; NAO PRECISA, PODEMOS PASSAR O U DIRETO.

    return u;
}


/* obj : rotar duplamente a direita o nodo desbalanceado
   ent : nodo desbalanceado
   sai : nodo balanceado*/
ptNodoAVL* rotacao_dupla_direita(ptNodoAVL* p)
{
    ptNodoAVL *u, *v;

    u = p->esq; // recebe filho esquerdo do nodo desbalanceado
    v = u->dir; // recebe filho direito do filho esquerdo de p

    // rotacao simples a esquerda
    u->dir = v->esq;
    v->esq = u;

    // rotacao simples a direita
    p->esq = v->dir;
    v->dir = p;

    p = v;

    return p;
}

/* obj : rotar duplamente a esquerda o nodo desbalanceado
   ent : nodo desbalanceado
   sai : nodo balanceado*/
ptNodoAVL* rotacao_dupla_esquerda(ptNodoAVL* p)
{
    ptNodoAVL *u, *v;

    u = p->dir; // recebe filho direito do nodo desbalanceado
    v = u->esq; // recebe filho esquerdo do filho direito de p

    // rotacao simples a direita
    u->esq = v->dir;
    v->dir = u;

    // rotacao simples a esquerda
    p->dir = v->esq;
    v->esq = p;

    p = v;

    return p;
}

/* obj : dado o nodo desbalanceado e a raiz da avl, percorrer recursivamente até chegar no endereco
   de nodo desbalanceado quando chegar lá, aplicar a rotacao e depois voltar com a raiz de uma
   arvore rotada
   ent : endereco de nodo desbalanceado e raiz da avl
   sai : ponteiro para uma raiz atualizada com desbalaceamento possivelmente corrigido */

ptNodoAVL* aplicandoRotacao(ptNodoAVL* nodo_desbalanceado, ptNodoAVL* raiz){
    ptNodoAVL* filho_esq;
    ptNodoAVL* filho_dir;

    if(raiz == NULL) return NULL;
    else{
        if(raiz == nodo_desbalanceado){
            filho_dir = nodo_desbalanceado->dir;
            filho_esq = nodo_desbalanceado->esq;
            // como o nodo passado é desbalanceado, precisamos somente saber seu sinal de balanceamento
            if(nodo_desbalanceado->fb>0){
                // se é positivo eh a subarvore direita
                // rotacoes nesse caso sao : rotacao a esquerda
                switch(filho_dir->fb){
                    case 1: // rotacao simples a esquerda
                        return rotacao_esquerda(nodo_desbalanceado);
                        break;
                    case -1: // rotacao dupla a esquerda
                        return rotacao_dupla_esquerda(nodo_desbalanceado);
                        break;
                    default:
                        //return 1; // informar erro
                }
            }else{
                // se é negativo eh a subarvore esquerda
                // rotacoes nesse caso sao : rotacao a direita
                /* estava trocado a rotacao*/
                switch(filho_esq->fb){
                    case 1: // rotacao simples a direita
                        return rotacao_dupla_direita(nodo_desbalanceado);
                        break;
                    case -1: // rotacao dupla a direita
                        return rotacao_direita(nodo_desbalanceado);
                        break;
                    default:
                        //return 1; // informar erro
                }
            }
        }
        else{ /* caso de raiz nao ser igual nodo desbalanceado */
            // devemos procurar por esse nodo
            raiz->dir = aplicandoRotacao(nodo_desbalanceado, raiz->dir);
            raiz->esq = aplicandoRotacao(nodo_desbalanceado, raiz->esq);
            return raiz;
        }
    }
    return raiz;
}
