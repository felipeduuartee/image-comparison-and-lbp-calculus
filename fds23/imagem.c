#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagem.h"


// funcao para ler uma imagem PGM - retorna a matriz da imagem ou NULL em caso de erro
unsigned char** ler_pgm(const char *nome_arquivo, int *largura, int *altura, int *max_valor, char *formato) {

    if (!nome_arquivo || !largura || !altura || !max_valor || !formato) {
        return NULL;
    }

    //abre o arquivo em modo binario (rb)
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        return NULL;
    }

    char numero_magico[3]; //armazenar (P2 ou P5)
    if (fscanf(arquivo, "%2s", numero_magico) != 1) {
        fclose(arquivo);
        return NULL;
    }

    //armazenar o formato P2 ou P5
    strcpy(formato, numero_magico);

    //checagem se é uma imagem pgm (numero magico p2 ou p5)
    if (strcmp(numero_magico, "P2") != 0 && strcmp(numero_magico, "P5") != 0) { // pode ser um formato nao suportado (colorida ou outra coisa)
        fclose(arquivo);
        return NULL;
    }

    //ignorar comentarios
    int c;
    while ((c = fgetc(arquivo)) == '#') {
        while (fgetc(arquivo) != '\n');
    }
    ungetc(c, arquivo); //retorna o caracter lido para o fluxo de entrada

    //leitura de largura, altura e valor máximo de cinza
    if (fscanf(arquivo, "%d %d %d", largura, altura, max_valor) != 3) {
        fclose(arquivo);
        return NULL;
    }

    //se o valor máximo de cinza é maior que 255 entao é inválido para imagens PGM
    if (*max_valor > 255) {
        fclose(arquivo);
        return NULL;
    }

    //alocar a matriz de imagem
    unsigned char **imagem = (unsigned char**) malloc((*altura) * sizeof(unsigned char*));
    if (!imagem) {
        fclose(arquivo);
        return NULL;
    }

    //aloca memoria para cada linha da imagem
    for (int i = 0; i < *altura; i++) {
        imagem[i] = (unsigned char*) malloc((*largura) * sizeof(unsigned char));
        if (!imagem[i]) { // se a alocação para alguma linha falhar
            //libera a memoria que ja foi alocada antes de retornar null
            for (int k = 0; k < i; k++) free(imagem[k]);
            free(imagem);
            fclose(arquivo);
            return NULL;
        }

        // leitura dos pixels da imagem no formato P2 (ASCII)
        if (strcmp(numero_magico, "P2") == 0) { //compara o numero magico com o P2
            for (int j = 0; j < *largura; j++) {
                int valor_pixel;
                if (fscanf(arquivo, "%d", &valor_pixel) != 1) {
                    //caso de erro de leitura no pixel, libera a memoria e fecha o arquivo
                    for (int k = 0; k <= i; k++) free(imagem[k]);
                    free(imagem);
                    fclose(arquivo);
                    return NULL;
                }

                //verifica se o valor do pixel passa o valor máximo permitido
                if (valor_pixel < 0 || valor_pixel > *max_valor) {
                    for (int k = 0; k <= i; k++) free(imagem[k]);
                    free(imagem);
                    fclose(arquivo);
                    return NULL;
                }

                // armazenar o valor do pixel na matriz
                imagem[i][j] = (unsigned char) valor_pixel;
            }
        }

        // leitura dos pixels da imagem no formato P5 (Binário)
        else if (strcmp(numero_magico, "P5") == 0) {
            if (fread(imagem[i], sizeof(unsigned char), *largura, arquivo) != *largura) {
                for (int k = 0; k <= i; k++) free(imagem[k]);
                free(imagem);
                fclose(arquivo);
                return NULL;
            }
        }
    }

    fclose(arquivo);
    return imagem;
}







//salva uma imagem PGM no formato P2 ou P5
void salvar_pgm(const char *nome_arquivo, unsigned char **imagem, int largura, int altura, int max_valor, const char *formato) {
    FILE *arquivo = fopen(nome_arquivo, "wb"); 
    if (!arquivo) {
        return;
    }

    //verifica o formato e escreve o cabeçalho correspondente
    if (strcmp(formato, "P2") == 0) {
        // formato P2
        fprintf(arquivo, "P2\n%d %d\n%d\n", largura, altura, max_valor);
        for (int i = 0; i < altura; i++) {
            for (int j = 0; j < largura; j++) {
                fprintf(arquivo, "%d ", imagem[i][j]);
            }
            fprintf(arquivo, "\n");
        }
    } else if (strcmp(formato, "P5") == 0) {
        //formato (P5)
        fprintf(arquivo, "P5\n%d %d\n%d\n", largura, altura, max_valor);
        for (int i = 0; i < altura; i++) {
            fwrite(imagem[i], sizeof(unsigned char), largura, arquivo);
        }
    }

    fclose(arquivo);
}

