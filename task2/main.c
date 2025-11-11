#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


typedef struct {
    long iterations; 
} ThreadArgs;


void cleanup(void *arg) {
    double *pi = (double *)arg;
    fprintf(stderr, "\n🧹 [Cleanup] Потік завершується. Поточне значення π ≈ %.10f\n", *pi);
}


void *calc_pi(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    double pi = 0.0;

  
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    
    pthread_cleanup_push(cleanup, &pi);

    for (long i = 0; i < args->iterations; i++) {
        pthread_testcancel(); 
        double term = (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
        pi += term;

        if (i % 1000000 == 0) {
            printf("Потік [%lu]: ітерація %ld, π ≈ %.10f\n",
                   pthread_self(), i, 4 * pi);
        }
    }

    pthread_cleanup_pop(1); 
    return NULL;
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Використання: %s <час_затримки_сек> <кількість_ітерацій>\n", argv[0]);
        return 1;
    }

    int delay = atoi(argv[1]);
    long iterations = atol(argv[2]);

    pthread_t thread;
    ThreadArgs args = { iterations };

   
    if (pthread_create(&thread, NULL, calc_pi, &args) != 0) {
        perror("pthread_create");
        return 1;
    }

    printf("Головний потік очікує %d секунд...\n", delay);
    sleep(delay);

  
    printf("Надсилається запит на скасування потоку...\n");
    pthread_cancel(thread);

    void *status;
    pthread_join(thread, &status);

    if (status == PTHREAD_CANCELED)
        printf("Потік був успішно скасований!\n");
    else
        printf("Потік завершився природно.\n");

    return 0;
}
