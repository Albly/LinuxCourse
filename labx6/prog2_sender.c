/*
 * Deomonstrates work with procesess. 
 * Sender prog. Creates chared segment, attaches to in and write content 
 *
 * 02.12.21 Alexander Blagodarnyi.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMKEY		1348    // shm id
#define SHMSIZE		1024	// shm size
#define BUFSIZE		 128

main(){
	key_t key;
	size_t  size;
	int     shmid;
	void  * shared_memory;
	char buff[BUFSIZE];

	shmid = shmget((key_t) SHMKEY, SHMSIZE, 0666|IPC_CREAT);  	//Get shared memory segment.
	shared_memory = shmat(shmid, NULL, 0);						// Attach shared memory segment.

	//fgets(buff, BUFSIZE, stdin);
	strcpy(buff,shared_memory); //Copy content to buff.

	printf("Attached to shmem key %d, id: %x output: %s\n",SHMKEY,shmid,buff); 

	return 0;
}
