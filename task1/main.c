#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct {
    int n;         
    int *primes;    
} PrimeArgs;


void *catalan_thread(void *arg) {
    int n = *(int *)arg;
    unsigned long long *catalans = malloc(n * sizeof(unsigned long long));
    if (!catalans) {
        perror("malloc");
        pthread_exit(NULL);
    }

    catalans[0] = 1;
    for (int i = 1; i < n; i++) {
        catalans[i] = (2 * (2 * i - 1) * catalans[i - 1]) / (i + 1);
    }

    pthread_exit(catalans); 
}


int is_prime(int x) {
    if (x < 2) return 0;
    for (int i = 2; i <= sqrt(x); i++) {
        if (x % i == 0) return 0;
    }
    return 1;
}


void *prime_thread(void *arg) {
    PrimeArgs *args = (PrimeArgs *)arg;
    int count = 0, num = 2;
    while (count < args->n) {
        if (is_prime(num)) {
            args->primes[count++] = num;
        }
        num++;
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Використання: %s <кількість чисел>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Кількість повинна бути > 0\n");
        return 1;
    }

    pthread_t t1, t2;
    unsigned long long *catalans;
    int *primes = malloc(n * sizeof(int));
    if (!primes) {
        perror("malloc");
        return 1;
    }

    PrimeArgs args = { n, primes };

  
    pthread_create(&t1, NULL, catalan_thread, &n);
    pthread_create(&t2, NULL, prime_thread, &args);

  
    pthread_join(t1, (void **)&catalans);
    pthread_join(t2, NULL);

 
    printf("\n--- Числа Каталана ---\n");
    for (int i = 0; i < n; i++) {
        printf("%d: %llu\n", i, catalans[i]);
    }

    printf("\n--- Прості числа ---\n");
    for (int i = 0; i < n; i++) {
        printf("%d: %d\n", i, primes[i]);
    }

    
    free(catalans);
    free(primes);

    printf("\nПрограма завершила роботу успішно!\n");
    return 0;
}
