/* arquivo principal */

/* bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "aplicacaoABP.h"

// argc conta o numero de parametros e argv armazena as strings correspondentes aos paramentros digitados
int main(int argc, char *argv[])
{
    FILE * dicionario;   //para arq de lexico
    FILE * txt_entrada;  // para arq de sentencas
    FILE * txt_saida;    // para arq resultado da aplicacao
    ptNodoABP* a;        // arvore de lexico
    ptLSE* lista;        // para armazenar os escores de cada linha
    int comparacoes = 0; // numero de comparacoes eh inicializado na main

    if (argc!=4)  // verifica numero de parametros de entrada no cmd
    {
        printf ("Número incorreto de parâmetros.\n Para chamar o programa digite: aplicacaoABP <arq_lexico> <arq_sentencas> <arq_sentencas_saida>\n");
        return 1;
    }
    else
    {
        // abre arquivo de lexico
        dicionario = fopen (argv[1], "r"); // abre o arquivo para leitura -- argv[1] � o primeiro par�metro ap�s o nome do arquivo.
        if (dicionario == NULL)
        {
            printf ("Erro ao abrir o arquivo %s",argv[1]);
            return 1;
        }
        else // arquivo de lexico OK
        {
            // inicializar ponteiro da arvore
            a = inicializaArvoreABP();
            // aqui temos de criar uma arvore ABP com o conteudo do arquivo de lexico
            a = criaArvoreABP(dicionario, a);
            // podemos fechar o arquivo de lexico, pois nao sera mais usado
            fclose(dicionario);

            // devemos abrir o arquivo de txt_entrada, das sentencas para leitura
            txt_entrada = fopen (argv[2], "r"); // abre o arquivo para leitura -- argv[2] : o segundo parametro apos o nome do arquivo.
            if (txt_entrada == NULL)
            {
                printf ("Erro ao abrir o arquivo %s",argv[2]);
                return 1;
            }
            // NA VERDADE, VAMOS OBTER FIM DA OPINIAO PELO \n, POIS CADA OPINIAO ESTÁ EM UMA LINHA.
            // abertura txt-entrada, OK.
            else{
                // criando arquivo saida para escrita, nao precisa existir previamente.
                if ((txt_saida = fopen(argv[3], "w")) == NULL)
                    printf("Erro ao abrir arquivo %s \n",argv[3]);
                else
                {
                    // precisamos comparar cada palavra de arquivo de entrada com cada palavra da arvore.
                    // se encontrarmos retornamos o float daquele nodo da arvore, caso nao, retornamos 0(zero).
                    // precisamos das funcoes buscaNaABP que retorna o valor da string(palavra) float, se achar a string, ou zero.
                    // precisamos percorrer o arquivo txt-entrada para obter cada palavra
                    // vamos implementar para isso a funcao leTXT.
                    // precisamos retornar o valor da sentenca ao encontrarmos \n e armazenar numa lista.
                    // cada avaliacao será colocada em uma única linha, logo podemos usar \n.

                    lista = inicializaLSE();

                    lista = leTXT(txt_entrada, a, lista, &comparacoes);

                    /* fazemos um rewind em txt-entrada*/
                    // presisamos do ponteiro de arquivo no inicio do arquivo novamente.
                    rewind(txt_entrada);

                    // nesse ponto não precisamos nem da árvore abp, podemos desaloca-lá se quisernmos.
                    // podemos criar a funcao destroiABP();

                    a = destroiABP(a);
                    if(a!=NULL) printf("erro ao destruir ABP!");

                    // precisamos escrever os valores da lista no inicio de cada sentenca avaliativa no txt-saida, ou seja,
                    // cada inicio de linha inserimos esccore:nota, nota esta que está armazenada na lista, em respectiva ordem.

                        // txt-entrada com ponteiro de arquivo no inicio e aberto para leitura
                        // txt-saida aberto para escrita, recem criado com sua abertura
                        // lista com cada nodo contendo o escore da linha.
                        // ao final teremos o arquivo de saida com os escores no inicio de cada sentenca
                        escreveEscoresNova(txt_entrada,txt_saida,lista);

                        // após terminar o txt devemos fecha-los
                        fclose(txt_saida);
                        fclose(txt_entrada);

                        // adicionar numero de comparacoes, ao final do arquivo, por isso "a".
                        txt_saida = fopen(argv[3], "a");
                        if(txt_saida==NULL) return 1;
                        else{
                            escreveComparacao(txt_saida, comparacoes);
                            fclose(txt_saida);
                        }

                        // destruir a lista para desalocar memoria
                        lista = destroiLSE(lista);
                        if(lista!=NULL) printf("erro ao destruir lista");

                        return 0; // ocorreu tudo ok
                    }
                }
            }
        }
    }
    return 1; // ocorreu algum problema
}
