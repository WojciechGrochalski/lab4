#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "simple_sem.h"

#include "semun.h"
#include "err.h"

static void sem_call (int sem_id, int op)
{
  struct sembuf sb;

  sb.sem_num = 0;
  sb.sem_op = op;
  sb.sem_flg = 0;
  if (semop (sem_id, &sb, 1) == -1)
    syserr("semop; op = %d", op);
}

int sem_init (key_t key)
{
  int sem_id;

  if ((sem_id = semget (key, 1, 0666 | IPC_CREAT)) == -1)
    syserr("semget");
  return (sem_id);
}

void sem_done (int sem_id)
{
  union semun su;

  if (semctl (sem_id, 0, IPC_RMID, su) == -1)
    syserr("semctl");
}

void P (int sem_id)
{
  sem_call(sem_id, -1);
}

void V (int sem_id)
{
  sem_call(sem_id, 1);
}

