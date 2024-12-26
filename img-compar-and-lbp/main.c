#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagem.h"
#include "lbp.h"
#include "comparacao.h"

int main(int argc, char *argv[]) {
    char *diretorio_base = NULL;
    char *imagem_entrada = NULL;
    char *imagem_saida = NULL;
    int modo = 0;

    //argumentos da linha de comando
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            diretorio_base = argv[++i];
            modo = 1;
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            imagem_entrada = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            imagem_saida = argv[++i];
            modo = 2;
        }
    }

    // parâmetros obrigatórios foram passados
    if (modo == 1 && diretorio_base && imagem_entrada) {
        if (comparar_imagens(diretorio_base, imagem_entrada) != 0) {
            return 1;
        }
    } else if (modo == 2 && imagem_entrada && imagem_saida) {
        if (gerar_imagem_lbp(imagem_entrada, imagem_saida) != 0) {
            return 1;
        }
    } else {
        return 1;
    }

    return 0;
}
