#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>

struct dado {
    unsigned long long int resultado;
    int inicio;
    int fim;
};

int main() {
    int N = 1000000000; 
    pid_t pid1, pid2;
    int status1, status2;

    int shm_id = shmget(IPC_PRIVATE, 2 * sizeof(struct dado), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }
    char shm_id_str[20];
    sprintf(shm_id_str,"%d", shm_id);

    struct dado *shm_ptr = (struct dado *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    int mid = N / 2;

    shm_ptr[0].inicio = 1;
    shm_ptr[0].fim = mid;
    shm_ptr[1].inicio = mid + 1;
    shm_ptr[1].fim = N;

    if ((pid1 = fork()) == 0) {
        execl("./fatorial_auxiliar", "fatorial_auxiliar_1", shm_id_str, "0", (char *)NULL);
        perror("execl");
        exit(1);
    }

    if ((pid2 = fork()) == 0) {
        execl("./fatorial_auxiliar", "fatorial_auxiliar_2", shm_id_str, "1", (char *)NULL);
        perror("execl");
        exit(1);
    }

    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);

    unsigned long long int resultado_final = shm_ptr[0].resultado * shm_ptr[1].resultado;

    printf("%lld\n", resultado_final);

    shmdt(shm_ptr);
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
}
