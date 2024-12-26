#ifndef IMAGEM_H
#define IMAGEM_H

unsigned char** ler_pgm(const char *nome_arquivo, int *largura, int *altura, int *max_valor, char *formato); 
void salvar_pgm(const char *nome_arquivo, unsigned char **imagem, int largura, int altura, int max_valor, const char *formato); 

#endif
