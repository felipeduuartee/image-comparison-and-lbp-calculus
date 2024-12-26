#ifndef LBP_H
#define LBP_H

unsigned char** calcular_lbp(unsigned char **imagem, int largura, int altura);
void calcular_histograma_lbp(unsigned char **imagem_lbp, int largura, int altura, double *histograma);
double calcular_distancia_euclidiana(double *histograma1, double *histograma2);
int gerar_imagem_lbp(const char *nome_arquivo_entrada, const char *nome_arquivo_saida);

#endif
