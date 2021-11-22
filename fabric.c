#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "err.h"
#include <unistd.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semun.h"

bool is_even(int object)
{
    if (object % 2 == 0)
    {
        return true;
    }
    return false;
}
int work_on(int object)
{
    object <= 0 ? object = 1 : object;
    if (object == 1)
    {
        return 1;
    }
    if (is_even(object))
    {
        return object / 2;
    }
    else
    {
        return (object * 3 + 1);
    }
}

int calculate_result_wihout_semafors(int object, int workers, int cycles)
{
    for (int i = 0; i < cycles; i++)
    {
        for (int j = 1; j < workers - 1; j++)
        {
            object = work_on(object);
        }
    }
    return object;
}

int start_work(int semafor, int object)
{
    union semun arg;
    arg.val = object;
    semctl(semafor, 0, SETVAL, arg);
}
int work_for_proces(int semafor)
{
    union semun arg;

    arg.val = work_on(semctl(semafor, 0, GETVAL));
    semctl(semafor, 0, SETVAL, arg);
}
int end_work(int semafor)
{
    return (semctl(semafor, 0, GETVAL));
}

int create_production_lines()
{
    return semget(0, 1, 0666 | IPC_CREAT);
}

int delete_prodution_lines()
{
    union semun arg;
    semctl(0, 0, IPC_RMID, arg);
}

int create_workers(int number_of_workers, int semafor, int object, int cycles_of_work)
{
    pid_t pid;
    start_work(semafor, object);
    for (int j = 0; j < cycles_of_work; j++)
    {
        for (int i = 1; i < number_of_workers; i++)
        {

            pid = fork();

            if (pid == -1)
            {
                printf("Error in fork\n");
            }
            if (pid == 0)
            {
                if (i == number_of_workers - 1)
                {

                    return 0;
                }
                else
                {
                    work_for_proces(semafor);
                    return 0;
                }
            }
        }
    }
    for (int j = 0; j < cycles_of_work; j++)
    {
        for (int i = 1; i < number_of_workers; i++)
        {
            if (wait(0) == -1)
            {
                printf("Error in wait\n");
            }
        }
    }
    printf("Result of work is %d\n", end_work(semafor));
    printf("Result of work wihout semaphors is %d\n", calculate_result_wihout_semafors(object, number_of_workers, cycles_of_work));
}

int main(int argc, char *argv[])
{
    int size_of_production = 0;
    int number_of_workers = 0;
    int cycles_of_work = 0;

    size_of_production = atoi(argv[1]);

    number_of_workers = atoi(argv[2]);

    cycles_of_work = atoi(argv[3]);

    int semafor = 0;

    printf("Size of production = %d\nNumber of workers = %d\nCycles of work = %d\n\n", size_of_production,
           number_of_workers, cycles_of_work);

    semafor = create_production_lines();

    create_workers(number_of_workers, semafor, size_of_production, cycles_of_work);

    delete_prodution_lines();
    exit(0);
}
