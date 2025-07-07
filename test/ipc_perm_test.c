#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main() {
    int shmid = shmget(IPC_PRIVATE, 128, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    }

    // attach then detach to trigger permission checks
    void *shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void *) -1) {
        perror("shmat");
        return 1;
    }

    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);  // cleanup
    return 0;
}
