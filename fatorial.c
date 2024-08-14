#include <stdio.h>

// funcao recursiva que calcula o fatorial de n
unsigned long long int fatorial(int inicio, int n) {
    unsigned long long int resultado = 1;
    for (int i = inicio; i <= n; i++) {
        resultado *= i;
    }
    return resultado;
}

int main() {
    int numero = 1000000000; // defina AQUI o valor de N
    printf("%llu\n",fatorial(1,numero));
    return 0;
}
