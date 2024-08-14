#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2
#define CACHE_LINE_SIZE 64  // Tamanho típico de uma linha de cache

typedef struct {
    int inicio;
    int fim;
    unsigned long long resultado;
    char padding[CACHE_LINE_SIZE - sizeof(int) * 2 - sizeof(unsigned long long)]; // Ajuste de padding
} DadosThread;
git remote -v
void* func_thread (void* arg) {
    DadosThread* dados = (DadosThread*)arg;
    dados->resultado = 1;

    for (int i = dados->inicio; i <= dados->fim; i++) {
        dados->resultado *= i;
    }

    return NULL;
}

unsigned long long fatorial(int n) {
    pthread_t threads[NUM_THREADS];
    DadosThread dados_threads[NUM_THREADS];
    int tamanho_segmento = n / NUM_THREADS;
    unsigned long long resultado_final = 1;

    for (int i = 0; i < NUM_THREADS; i++) {
        dados_threads[i].inicio = i * tamanho_segmento + 1;
        if (i == NUM_THREADS - 1) {
    dados_threads[i].fim = n;  // Se for a última thread, ela deve ir até o número n
}       else {
    dados_threads[i].fim = (i + 1) * tamanho_segmento;  // Caso contrário, vai até o final do segmento
}

        pthread_create(&threads[i], NULL, func_thread, (void*)&dados_threads[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        resultado_final *= dados_threads[i].resultado;
    }

    return resultado_final;
}

int main() {
    int n = 1000000000; 
    unsigned long long resultado = fatorial(n);
    printf("Fatorial de %d é %llu\n", n, resultado);
    return 0;
}
