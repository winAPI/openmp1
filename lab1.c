#include <stdio.h>
#include <stdlib.h>     // getenv
#include <windows.h>    // Sleep, QueryPerformanceFrequency
#include <time.h>
#include <omp.h>

// gcc lab1.c -fopenmp -E > lab1_p.c
// Компиляция: gcc lab1.c -o lab1 -fopenmp -std=c99
// Установка переменных окружения set OMP_NUM_THREADS=10
// 500, 700, 900, 1000, 1500, 2000
#define SIZE_N 1000
#define COUNT_THREADS 5 // for example
#define DEBUG_
#define SAFE_PRINTF
#define SLEEP_THREAD_

void omp_info();
void safe_print(const char * txt, int threa_num);

double mul(double a[][SIZE_N], double b[][SIZE_N], int i, int j);
void mul_matrix(double a[][SIZE_N], double b[][SIZE_N], double c[][SIZE_N]);
void mul_matrix_parallel(double a[][SIZE_N], double b[][SIZE_N], double c[][SIZE_N], int count_threads);
void print_matrix(double m[][SIZE_N]);
void random_matrix(double m[][SIZE_N], int seed);

double m_a[SIZE_N][SIZE_N] = {0};
double m_b[SIZE_N][SIZE_N] = {0};
double m_res[SIZE_N][SIZE_N] = {0};

void benchmark()
{
    printf("N=%d\n", SIZE_N);
    
    LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER t1, t2;           // ticks
    double elapsedTime;

    // get ticks per second
    QueryPerformanceFrequency(&frequency);
    
    
    random_matrix(m_a, 15);
    random_matrix(m_b, 20);
    QueryPerformanceCounter(&t1);
    //
    mul_matrix(m_a, m_b, m_res);
    //
    QueryPerformanceCounter(&t2);
    elapsedTime = (t2.QuadPart - t1.QuadPart) * 1.0 / frequency.QuadPart;
    printf("[%.2f, ", elapsedTime);
    
    int count_threads[] = {5, 10, 20};
    for(int i = 0; i < 3; ++i)
    {
        QueryPerformanceCounter(&t1);
        //
        mul_matrix_parallel(m_a, m_b, m_res, count_threads[i]);
        //
        QueryPerformanceCounter(&t2);
        elapsedTime = (t2.QuadPart - t1.QuadPart) * 1.0 / frequency.QuadPart; // (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart; // ms
        printf("%.2f, ", elapsedTime);
    }
    printf("]");
}

void example()
{
    random_matrix(m_a, 15);
    print_matrix(m_a);
    random_matrix(m_b, 20);
    print_matrix(m_b);
    //mul_matrix(m_a, m_b, m_res); 
    mul_matrix_parallel(m_a, m_b, m_res, COUNT_THREADS);
    print_matrix(m_res);
}

int main(int argc, char** argv)
{
    benchmark();
    //example();
    
    return 0;
}

void mul_matrix_parallel(double a[][SIZE_N], double b[][SIZE_N], double c[][SIZE_N], int count_threads)
{
    int i, j;
    int thread_num;
    
    omp_set_num_threads(count_threads);
    
    #pragma omp parallel shared(a, b, c)
    {
        #pragma omp for private(thread_num, i, j)
        for(j = 0; j < SIZE_N; ++j)
        {
            thread_num = omp_get_thread_num();
            
            #ifdef DEBUG
            safe_print("in", thread_num);
            #endif
            
            for(i = 0; i < SIZE_N; ++i)
            {
                #ifdef DEBUG
                safe_print("computing...", thread_num);
                #endif
                
                c[i][j] = mul(a, b, i, j);
            }
            
            #ifdef DEBUG
            safe_print("out", thread_num);
            #endif
        }
    }
}

void mul_matrix(double a[][SIZE_N], double b[][SIZE_N], double c[][SIZE_N])
{
    for(int i = 0; i < SIZE_N; ++i)
    {
        for(int j = 0; j < SIZE_N; ++j)
        {
            c[i][j] = mul(a, b, i, j);
        }
    }
}

double mul(double a[][SIZE_N], double b[][SIZE_N], int i, int j)
{
    #ifdef SLEEP_THREAD
        time_t t;
        srand((unsigned) time(&t));
        Sleep((rand() % 50));
    #endif
    
    double sum = 0;
    for(int r = 0; r < SIZE_N; ++r)
    {
        sum += a[i][r] * b[r][j];
    }
    return sum;
}

void random_matrix(double m[][SIZE_N], int seed)
{
    time_t t;
    srand((unsigned) time(&t) + seed);
    
    for(int i = 0; i < SIZE_N; ++i)
    {
        for(int j = 0; j < SIZE_N; ++j)
        {
            m[i][j] = (rand() % 100) / 10.0;
        }
    }
}

void print_matrix(double m[][SIZE_N])
{
    for(int i = 0; i < SIZE_N; ++i)
    {
        for(int j = 0; j < SIZE_N; ++j)
        {
            printf("%.2f ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void safe_print(const char * txt, int thread_num)
{
    #ifdef SAFE_PRINTF
        #pragma omp critical(printf)
    #else
        #warning "Not define SAFE_PRINTF. "
    #endif
    printf("Thread#%d %s\n", thread_num, txt);
}

void omp_info()
{
    char* pEnv = getenv("OMP_NUM_THREADS");
    if (pEnv != NULL)
        printf("OMP_NUM_THREADS: %s\n", pEnv);
    pEnv = getenv("OMP_NUM_THREADS");
    if (pEnv != NULL)
        printf("Max threads in env: %s\n", pEnv);

    printf("Max threads = %d\n", omp_get_max_threads()); 
}