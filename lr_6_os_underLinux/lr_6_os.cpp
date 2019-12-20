int main()
{
	int sem_id, shm_id, process_producer, process_customer;
	key_t key;
	char *seg_ptr;
	struct sembuf lock_producer = {0, -1, 0}, unlock_producer = {0, 1, 0},
		          lock_customer = {1, -1, 0}, unlock_customer = {1, 1, 0};

	key = ftok(".", 'S');
	sem_id = semget(key, 2, 0660 | IPC_CREAT | IPC_EXCL);
	if (sem_id < 0)
	{
		perror("semget");
		exit(1);	
	}
	if ((shm_id = shmget(key, 0, 0660|IPC_CREAT)) == -1)
	{
		perror("shmget");
		exit(1);
	}
	if ((seg_ptr = shmat(shm_id, 0, 0)) == -1)
	{
		perror("shmat");
		exit(1);	
	}
	semctl(sem_id, 0, SETVAL, (union semun) 1);
	semctl(sem_id, 1, SETVAL, (union semun) 0);
	if ((process_producer = fork()) == 0)
	{
		while(1)
			{
				semop(sem_id, &lock_producer, 1);
				writeshm(shm_id, seg_ptr, "text for consumer");
				semop(sem_id, &unlock_customer, 1);
			}
	}
	else
	{
		if ((process_customer = fork()) == 0)
			{
				while(1)
					{
						semop(sem_id, &lock_customer, 1);
						readshm(shm_id, seg_ptr);
						semop(sem_id, &unlock_producer, 1);	
					}
			}	
	}

	sleep(10);
	kill(process_producer, SIGINT);
	kill(process_customer, SIGINT);
	semctl(sem_id, 0, IPC_RMID, 0);
	shmctl(shm_id, IPC_RMID, 0);
	
}

writeshm(int shm_id, char *seg_ptr, char *text)
{
	strcpy(seg_ptr, text);
	printf("Done...\n");
}

readshm(int shm_id, char *seg_ptr)
{
	printf("seg_ptr: %\n", seg_ptr);
}
