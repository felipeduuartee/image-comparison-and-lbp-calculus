#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "imagem.h"
#include "lbp.h"


//carrega o histograma LBP de um arquivo .lbp ou calcula o LBP se não existir - retorna o histograma LBP (array 256 doubles) ou NULL
double* carregar_ou_calcular_lbp(const char *caminho_imagem, const char *caminho_lbp) {
    
    FILE *arquivo_lbp = fopen(caminho_lbp, "rb"); // abrir arquivo .lbp para leitura binaria
    char formato[3];

    // se o arquivo .lbp existir, carrega o histograma LBP
    if (arquivo_lbp) {
        
        fseek(arquivo_lbp, 0, SEEK_END);
        long tamanho_arquivo = ftell(arquivo_lbp);
        fseek(arquivo_lbp, 0, SEEK_SET);
        

        if (tamanho_arquivo != 256 * sizeof(double)) {
            fclose(arquivo_lbp);
            return NULL;
        }

        double *histograma = (double*) malloc(256 * sizeof(double));
        if (!histograma) {
            fclose(arquivo_lbp);
            return NULL;
        }
        if (fread(histograma, sizeof(double), 256, arquivo_lbp) != 256) {
            free(histograma);
            fclose(arquivo_lbp);
            return NULL;
        }
        fclose(arquivo_lbp);
        return histograma;
    } else {
        int largura, altura, max_valor;
        unsigned char **imagem = ler_pgm(caminho_imagem, &largura, &altura, &max_valor, formato);
        if (!imagem) {
            return NULL;
        }

        unsigned char **imagem_lbp = calcular_lbp(imagem, largura, altura);
        double *histograma = (double*) malloc(256 * sizeof(double));
        if (!histograma) {
            for (int i = 0; i < altura; i++) free(imagem[i]);
            free(imagem);
            free(imagem_lbp);
            return NULL;
        }
        calcular_histograma_lbp(imagem_lbp, largura, altura, histograma);

        FILE *novo_arquivo_lbp = fopen(caminho_lbp, "wb");
        if (!novo_arquivo_lbp) {
            free(histograma);
            for (int i = 0; i < altura; i++) free(imagem[i]);
            free(imagem);
            free(imagem_lbp);
            return NULL;
        }
        fwrite(histograma, sizeof(double), 256, novo_arquivo_lbp);
        fclose(novo_arquivo_lbp);

        for (int i = 0; i < altura; i++) {
            free(imagem[i]);
            free(imagem_lbp[i]);
        }
        free(imagem);
        free(imagem_lbp);

        return histograma;
    }
}


// Função para comparar a imagem de entrada com as imagens da base
int comparar_imagens(const char *diretorio_base, const char *imagem_entrada) {
    char caminho_lbp_entrada[256];
    char formato[3];

    snprintf(caminho_lbp_entrada, sizeof(caminho_lbp_entrada), "%s.lbp", imagem_entrada);

    double *histograma_entrada = carregar_ou_calcular_lbp(imagem_entrada, caminho_lbp_entrada);
    if (!histograma_entrada) {
        return 1;
    }

    DIR *dir = opendir(diretorio_base);
    if (!dir) {
        free(histograma_entrada);
        return 1;
    }

    struct dirent *entrada;
    double menor_distancia = -1;
    char imagem_mais_similar[256] = "";

    // Percorrer as imagens da base
    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        }

        // Ignorar arquivos que não têm extensão .pgm
        if (strstr(entrada->d_name, ".pgm") == NULL) {
            continue; 
        }

        char caminho_imagem[512], caminho_lbp[512];
        snprintf(caminho_imagem, sizeof(caminho_imagem), "%s/%s", diretorio_base, entrada->d_name);
        snprintf(caminho_lbp, sizeof(caminho_lbp), "%s/%s.lbp", diretorio_base, entrada->d_name);

        int largura, altura, max_valor;
        unsigned char **imagem = ler_pgm(caminho_imagem, &largura, &altura, &max_valor, formato);
        if (!imagem || largura < 3 || altura < 3) {
            if (imagem) {
                for (int i = 0; i < altura; i++) free(imagem[i]);
                free(imagem);
            }
            continue;
        }

        double *histograma_base = carregar_ou_calcular_lbp(caminho_imagem, caminho_lbp);
        if (!histograma_base) {
            for (int i = 0; i < altura; i++) free(imagem[i]);
            free(imagem);
            continue;
        }

        double distancia = calcular_distancia_euclidiana(histograma_entrada, histograma_base);

        if (menor_distancia == -1 || distancia < menor_distancia) {
            menor_distancia = distancia;
            strcpy(imagem_mais_similar, entrada->d_name);
        }

        free(histograma_base);
        for (int i = 0; i < altura; i++) free(imagem[i]);
        free(imagem);
    }

    closedir(dir);
    free(histograma_entrada);

    if (menor_distancia >= 0) {
        printf("Imagem mais similar: %s com distância %.6f\n", imagem_mais_similar, menor_distancia);
        return 0;
    } else {
        return 1;
    }
}





