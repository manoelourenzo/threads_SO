#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

struct dado {
    unsigned long long int resultado;
    int inicio;
    int fim;
};

long long int fatorial(int x, int n) {
    unsigned long long int resultado = 1;
    for (int i = x; i <= n; i++) {
        resultado *= i;
    }
    return resultado;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <shm_id> <index>\n", argv[0]);
        exit(1);
    }
    int shm_id = atoi(argv[1]);
    int index = atoi(argv[2]);

    struct dado *shm_ptr = (struct dado *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    shm_ptr[index].resultado = fatorial(shm_ptr[index].inicio, shm_ptr[index].fim);

    shmdt(shm_ptr);

    return 0;
}
