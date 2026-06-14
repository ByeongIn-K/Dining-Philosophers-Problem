#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 5
#define ITERATIONS 10

sem_t forks[NUM];
sem_t once;

void pickup(int philosopher_num)
{
    sem_wait(&forks[philosopher_num % NUM]);
}

void putdown(int philosopher_num)
{
    sem_post(&forks[philosopher_num % NUM]);
}

void thinking(int philosopher_num)
{
    printf("Philosopher %d is thinking\n", philosopher_num);
}

void eating(int philosopher_num)
{
    printf("Philosopher %d is eating\n", philosopher_num);
}

void* philosopher(void* arg)
{
    int philosopher_num;

    philosopher_num = (unsigned long int)arg;

    for(int i = 0; i < ITERATIONS; i++)
    {
        sem_wait(&once);

        pickup(philosopher_num);

        printf("Philosopher %d picks up fork %d\n", philosopher_num, philosopher_num);

        pickup(philosopher_num + 1);

        printf("Philosopher %d picks up fork %d\n", philosopher_num, (philosopher_num + 1) % NUM);

        sem_post(&once);

        eating(philosopher_num);

        putdown(philosopher_num + 1);

        printf("Philosopher %d puts down fork %d\n", philosopher_num, (philosopher_num + 1) % NUM);

        putdown(philosopher_num);

        printf("Philosopher %d puts down fork %d\n", philosopher_num, philosopher_num);

        thinking(philosopher_num);
    }

    return NULL;
}

int main()
{
    pthread_t threads[NUM];

    for(int i = 0; i < NUM; i++)
    {
        sem_init(&forks[i], 0, 1);
    }

    sem_init(&once, 0, 1);

    for(unsigned long int i = 0; i < NUM; i++)
    {
        pthread_create(&threads[i], NULL, philosopher, (void*)i);
    }

    for(int i = 0; i < NUM; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\nNO DEADLOCK\n");

    for(int i = 0; i < NUM; i++)
    {
        sem_destroy(&forks[i]);
    }

    sem_destroy(&once);

    return 0;
}
