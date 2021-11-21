#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#include "err.h"

int main ()
{
  key_t key;
  int opperm, flags, nsems, semid, opperm_flag;

  printf( "\nEnter the desired key in hex = ");
  scanf(  "%x", &key );

  printf( "\nEnter the operation \npermissions in octal = ");
  scanf(  "%o", &opperm );

  printf( "\nEnter corresponding number to \nset desired flags:\n");
  printf( "0 --> No flags \n");
  printf( "1 --> IPC_CREAT \n");
  printf( "2 --> IPC_EXCL \n");
  printf( "3 --> IPC_CREAT and IPC_EXCL \n");
  printf( "Flags = ");
  scanf(  "%d", &flags );
  
  printf( "\nkey = 0x%x, opperm = 0%o, flags = 0%o\n", key, opperm, flags );

  switch ( flags ) {
    case 0:
      opperm_flag = opperm | 0;
      break;
    case 1:
      opperm_flag = opperm | IPC_CREAT;
      break;
    case 2:
      opperm_flag = opperm | IPC_EXCL;
      break;
    case 3:
      opperm_flag = opperm | IPC_EXCL | IPC_CREAT;
      break;
  }
  
  printf( "\nEnter the number of desired semaphores for this set (25 max) = ");
  scanf(  "%d", &nsems );
  printf( "\nNsems = %d\n", nsems );

  /*********************************************************/

  semid = semget( key, nsems, opperm_flag );

  /*********************************************************/

  if ( semid == -1 )
    syserr("semget");
  
  printf( "\nThe semid = %d\n", semid );
  return 0;
}
