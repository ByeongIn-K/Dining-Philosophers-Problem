#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM 5
#define ITERATIONS 100000

sem_t forks[NUM];

pthread_mutex_t count_mutex;

long long eat_count = 0;

void pickup(int philosopher_num)
{
    sem_wait(&forks[philosopher_num % NUM]);
}

void putdown(int philosopher_num)
{
    sem_post(&forks[philosopher_num % NUM]);
}

void eating(int philosopher_num)
{
    pthread_mutex_lock(&count_mutex);
    eat_count++;
    pthread_mutex_unlock(&count_mutex);
}

void thinking(int philosopher_num)
{
}

void* philosopher(void* arg)
{
    int philosopher_num;

    philosopher_num = (unsigned long int)arg;

    for(int i = 0; i < ITERATIONS; i++)
    {
        if(philosopher_num < 4)
        {
            pickup(philosopher_num);
            pickup(philosopher_num + 1);
        }
        else
        {
            pickup(philosopher_num + 1);
            pickup(philosopher_num);
        }

        eating(philosopher_num);

        putdown(philosopher_num + 1);
        putdown(philosopher_num);

        thinking(philosopher_num);
    }

    return NULL;
}

int main()
{
    pthread_t threads[NUM];

    struct timespec start, end;

    for(int i = 0; i < NUM; i++)
    {
        sem_init(&forks[i], 0, 1);
    }
    
    pthread_mutex_init(&count_mutex, NULL);

    clock_gettime(CLOCK_MONOTONIC, &start);

    for(unsigned long int i = 0; i < NUM; i++)
    {
        pthread_create(&threads[i], NULL, philosopher, (void*)i);
    }

    for(int i = 0; i < NUM; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("===== Resource Ordering Result =====\n");
    printf("Total Eating Count : %lld\n", eat_count);
    printf("Execution Time : %.6f sec\n", elapsed);

    for(int i = 0; i < NUM; i++)
    {
        sem_destroy(&forks[i]);
    }
    
    pthread_mutex_destroy(&count_mutex);

    return 0;
}
