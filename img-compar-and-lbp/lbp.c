#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include "lbp.h"
#include "imagem.h"


// Função para calcular o LBP de uma imagem
unsigned char** calcular_lbp(unsigned char **imagem, int largura, int altura) {
    unsigned char **imagem_lbp = (unsigned char**) malloc(altura * sizeof(unsigned char*));
    if (!imagem_lbp) {
        return NULL;  // Falha de alocação de memória
    }

    for (int i = 0; i < altura; i++) {
        imagem_lbp[i] = (unsigned char*) malloc(largura * sizeof(unsigned char));
        if (!imagem_lbp[i]) {
            // Limpar a memória alocada se falhar
            for (int k = 0; k < i; k++) free(imagem_lbp[k]);
            free(imagem_lbp);
            return NULL;
        }

        // Inicializar a linha da imagem LBP com zero para evitar valores não inicializados
        memset(imagem_lbp[i], 0, largura * sizeof(unsigned char));
    }

    // Para cada pixel, calcular o LBP, ignorando as bordas
    for (int i = 1; i < altura - 1; i++) {
        for (int j = 1; j < largura - 1; j++) {
            unsigned char centro = imagem[i][j];
            unsigned char valor_lbp = 0;
            valor_lbp |= (imagem[i-1][j-1] >= centro) << 7;
            valor_lbp |= (imagem[i-1][j] >= centro) << 6;
            valor_lbp |= (imagem[i-1][j+1] >= centro) << 5;
            valor_lbp |= (imagem[i][j+1] >= centro) << 4;
            valor_lbp |= (imagem[i+1][j+1] >= centro) << 3;
            valor_lbp |= (imagem[i+1][j] >= centro) << 2;
            valor_lbp |= (imagem[i+1][j-1] >= centro) << 1;
            valor_lbp |= (imagem[i][j-1] >= centro) << 0;
            imagem_lbp[i][j] = valor_lbp;
        }
    }

    return imagem_lbp;
}






//função para gerar a imagem LBP e salvá-la
int gerar_imagem_lbp(const char *nome_arquivo_entrada, const char *nome_arquivo_saida) {
    int largura, altura, max_valor;

    char formato[3];

    unsigned char **imagem = ler_pgm(nome_arquivo_entrada, &largura, &altura, &max_valor, formato);
    if (!imagem) {
        return 1;
    }

    if (largura < 3 || altura < 3) {
        for (int i = 0; i < altura; i++) free(imagem[i]);
        free(imagem);
        return 1;
    }

    unsigned char **imagem_lbp = calcular_lbp(imagem, largura, altura);
    if (!imagem_lbp) {
        for (int i = 0; i < altura; i++) free(imagem[i]);
        free(imagem);
        return 1;
    }

    //salvar a imagem LBP no mesmo formato que a imagem de entrada (P2 ou P5)
    salvar_pgm(nome_arquivo_saida, imagem_lbp, largura, altura, max_valor, formato);

    //liberar a memória das imagens
    for (int i = 0; i < altura; i++) {
        free(imagem[i]);
        free(imagem_lbp[i]);
    }
    free(imagem);
    free(imagem_lbp);

    return 0;
}







// Função para calcular o histograma LBP (256 bins) com normalização
void calcular_histograma_lbp(unsigned char **imagem_lbp, int largura, int altura, double *histograma) {
    // Inicializa o histograma com zeros (prevenir valores não inicializados)
    for (int i = 0; i < 256; i++) {
        histograma[i] = 0.0;
    }

    // Preencher o histograma com a contagem de cada valor LBP
    int total_pixels = 0;
    for (int i = 1; i < altura - 1; i++) {
        for (int j = 1; j < largura - 1; j++) {
            int valor_lbp = imagem_lbp[i][j];
            histograma[valor_lbp]++;
            total_pixels++;
        }
    }

    // Normalizar o histograma dividindo pelo total de pixels
    if (total_pixels > 0) {
        for (int i = 0; i < 256; i++) {
            histograma[i] /= total_pixels;
        }
    }
}




// Função para calcular a distância Euclidiana entre dois histogramas
double calcular_distancia_euclidiana(double *histograma1, double *histograma2) {
    double soma = 0.0;
    for (int i = 0; i < 256; i++) {
        double diff = histograma1[i] - histograma2[i];
        soma += diff * diff;
    }
    return sqrt(soma);
}






