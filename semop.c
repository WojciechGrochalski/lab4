#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#include "err.h"

int main ()
{
  struct sembuf sembuf[10], *sops;
  int retrn, flags, sem_num, i, semid, op;
  unsigned nsops;

  sops = sembuf;

  printf( "\nEnter the semid of the semaphore set = ");
  scanf(  "%d", &semid );
  printf( "\nsemid = %d", semid );

  printf( "\nEnter the number of operations on this set = ");
  scanf(  "%d", &nsops );

  for ( i = 0; i < nsops; i++, sops++) {
    printf( "\nEnter semaphore number = ");
    scanf( "%d", &sem_num );
    sops->sem_num = sem_num;

    printf( "\nEnter the operation = ");
    scanf( "%d", &op );
    sops->sem_op = op;
    printf( "\nsem_num = %d, sem_op = %+d \n", sops->sem_num, sops->sem_op);

    printf( "Flags:\n");
    printf( " 0 ---> No flags \n");
    printf( " 1 ---> IPC_NOWAIT \n");
    printf( " 2 ---> SEM_UNDO \n");
    printf( " 3 ---> IPC_NOWAIT and SEM_UNDO \n");
    printf( "flag = ");
    scanf( "%d", &flags );
    switch ( flags ) {
      case 0:
        sops->sem_flg = 0;
	break;
      case 1:
	sops->sem_flg = IPC_NOWAIT;
	break;
      case 2:
	sops->sem_flg = SEM_UNDO;
	break;
      case 3:
	sops->sem_flg = SEM_UNDO | IPC_NOWAIT;
	break;
    }
    printf( "\nFlags = %o\n", sops->sem_flg );
  } /* for */

  sops = sembuf;

  /*************************************************************/

  retrn = semop( semid, sops, nsops );
  
  /*************************************************************/

  if ( retrn == -1 )
    syserr("semop");
  
  printf( "\nSemop was successful! \n");
  printf( "Semop return ----> %d\n", retrn);
  return 0;
}

