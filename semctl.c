#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "semun.h"
#include "err.h"

int main()
{
  struct semid_ds semid_ds;
  int c, i, length, uid, gid, mode, retrn, semid, semnum, cmd, choice;
  union semun arg;

  arg.buf = &semid_ds;

  printf( "\nEnter the semid = ");
  scanf("%d", &semid );

  printf( "\nSelect command:");
  printf( "\n  1 ---> GETVAL ");
  printf( "\n  2 ---> SETVAL");
  printf( "\n  3 ---> GETPID ");
  printf( "\n  4 ---> GETNCNT ");
  printf( "\n  5 ---> GETZCNT ");
  printf( "\n  6 ---> GETALL ");
  printf( "\n  7 ---> SETALL ");
  printf( "\n  8 ---> IPC_STAT ");
  printf( "\n  9 ---> IPC_SET ");
  printf( "\n 10 ---> IPC_RMID ");
  printf( "\n cmd = ");
  scanf( "%d", &cmd );

  switch ( cmd ) {
    case 1:
      printf( "\nEnter semnum = ");
      scanf( "%d", &semnum );
      retrn = semctl(semid, semnum, GETVAL, arg);
      printf( "\nThe semval = %d\n", retrn );
      break;
    case 2:
      printf( "\nEnter semnum = ");
      scanf( "%d", &semnum );
      printf( "\nEnter the value = ");
      scanf( "%d", &arg.val );
      retrn = semctl( semid, semnum, SETVAL, arg);
      break;
    case 3:
      printf( "\nEnter semnum = ");
      scanf( "%d", &semnum );
      retrn = semctl( semid, semnum, GETPID, arg);
      printf( "\nThe sempid = %d\n", retrn );
      break;
    case 4:
      printf( "\nEnter semnum = ");
      scanf( "%d", &semnum );
      retrn = semctl( semid, semnum, GETNCNT, arg);
      printf( "\nThe semcnt = %d\n", retrn );
      break;
    case 5:
      printf( "\nEnter semnum = ");
      scanf( "%d", &semnum );
      retrn = semctl( semid, semnum, GETZCNT, arg);
      printf( "\nThe semzcnt = %d\n", retrn );
      break;
    case 6:
      if ((retrn = semctl( semid, 0, IPC_STAT, arg)) == -1)
        break;
      length = arg.buf->sem_nsems;
      arg.array = (ushort *) malloc(length * sizeof(ushort));
      retrn = semctl( semid, 0, GETALL, arg);
      for ( i = 0; i < length; i++ )
	printf( "%d ", arg.array[i] );
      free(arg.array);
      break;
    case 7:
      if ((retrn = semctl(semid, 0, IPC_STAT, arg)) == -1)
        break;
      length = arg.buf->sem_nsems;
      arg.array = (ushort *) malloc(length * sizeof(ushort));
      for (i = 0; i < length; i++) {
	scanf( "%d", &c ); 
	arg.array[i] = c;
      }
      retrn = semctl( semid, 0, SETALL, arg);
      free(arg.array);
      break;
    case 8:
      retrn = semctl( semid, 0, IPC_STAT, arg);
      printf( "\nUser id = %d\n", arg.buf->sem_perm.uid );
      printf( "Group id = %d\n", arg.buf->sem_perm.gid );
      printf( "Operation permissions = 0%o\n", arg.buf->sem_perm.mode );
      printf( "Semaphore number = %ld\n", arg.buf->sem_nsems );
      printf( "Last semop time = %ld\n", arg.buf->sem_otime );
      printf( "Last change time = %ld\n", arg.buf->sem_ctime );
      break;
    case 9:
      printf( "Changing:\n" );
      printf( " sem_perm.uid  ---> 1\n");
      printf( " sem_perm.gid  ---> 2\n");
      printf( " sem_perm.mode ---> 3\n");
      printf( "Entry = ");
      scanf ( "%d", &choice );
      
      switch ( choice )	{
        case 1:
	  printf( "\nEnter user ID = ");
	  scanf( "%d", &uid );
	  arg.buf->sem_perm.uid = uid;
	  break;
	case 2:
	  printf( "\nEnter group ID = ");
	  scanf( "%d", &gid );
	  arg.buf->sem_perm.gid = gid;
	  break;
	case 3:
	  printf( "\nMode = ");
	  scanf( "%o", &mode );
	  arg.buf->sem_perm.mode = mode;
	  break;
      }

      retrn = semctl( semid, 0, IPC_SET, arg);
      break;
    case 10:
      retrn = semctl( semid, 0, IPC_RMID, arg);
      break;
    default:
      printf( "\nWrong command = %d\n", cmd );
      break;
  }

  if ( retrn == -1 )
    syserr("semctl");      

  printf( "\n\nThe system call semctl() was successful! \n");
  printf( "Semctl return ----> %d\n", retrn);
  return 0;
}

