#ifndef _SIMPLE_SEM_H_
#define _SIMPLE_SEM_H_

int  sem_init (key_t key);
void sem_done (int sem_id);
void P (int sem_id);
void V (int sem_id);

#endif
