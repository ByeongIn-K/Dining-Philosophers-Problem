#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM 5
#define ITERATIONS 100000

pthread_mutex_t banker_mutex;

pthread_mutex_t count_mutex;

int available[NUM];
int allocation[NUM][NUM];
int max_need[NUM][NUM];

long long eat_count = 0;

int is_safe()
{
    int work[NUM];
    int finish[NUM] = {0};

    for(int i = 0; i < NUM; i++)
    {
        work[i] = available[i];
    }

    int progress = 1;

    while(progress)
    {
        progress = 0;

        for(int i = 0; i < NUM; i++)
        {
            if(finish[i])
                continue;

            int possible = 1;

            for(int j = 0; j < NUM; j++)
            {
                int need = max_need[i][j] - allocation[i][j];

                if(need > work[j])
                {
                    possible = 0;
                    break;
                }
            }

            if(possible)
            {
                for(int j = 0; j < NUM; j++)
                {
                    work[j] += allocation[i][j];
                }

                finish[i] = 1;
                progress = 1;
            }
        }
    }

    for(int i = 0; i < NUM; i++)
    {
        if(!finish[i])
            return 0;
    }

    return 1;
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

    int left = philosopher_num;
    int right = (philosopher_num + 1) % NUM;

    for(int i = 0; i < ITERATIONS; i++)
    {
        while(1)
        {
            pthread_mutex_lock(&banker_mutex);

            if(available[left] && available[right])
            {
                available[left] = 0;
                available[right] = 0;

                allocation[philosopher_num][left] = 1;
                allocation[philosopher_num][right] = 1;

                if(is_safe())
                {
                    pthread_mutex_unlock(&banker_mutex);
                    break;
                }

                allocation[philosopher_num][left] = 0;
                allocation[philosopher_num][right] = 0;

                available[left] = 1;
                available[right] = 1;
            }

            pthread_mutex_unlock(&banker_mutex);
        }

        eating(philosopher_num);

        pthread_mutex_lock(&banker_mutex);

        allocation[philosopher_num][left] = 0;
        allocation[philosopher_num][right] = 0;

        available[left] = 1;
        available[right] = 1;

        pthread_mutex_unlock(&banker_mutex);

        thinking(philosopher_num);
    }

    return NULL;
}

int main()
{
    pthread_t threads[NUM];

    struct timespec start, end;

    pthread_mutex_init(&banker_mutex, NULL);
    
    pthread_mutex_init(&count_mutex, NULL);

    for(int i = 0; i < NUM; i++)
    {
        available[i] = 1;
    }

    for(int i = 0; i < NUM; i++)
    {
        max_need[i][i] = 1;
        max_need[i][(i + 1) % NUM] = 1;
    }

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

    printf("===== Banker's Algorithm Result =====\n");
    printf("Total Eating Count : %lld\n", eat_count);
    printf("Execution Time : %.6f sec\n", elapsed);

    pthread_mutex_destroy(&banker_mutex);
    
    pthread_mutex_destroy(&count_mutex);

    return 0;
}
