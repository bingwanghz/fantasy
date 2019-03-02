#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define MB_1 (1024*1024)
#define MB_8 (8*MB_1)

char *a;
int shmid;

void init_hugepage_seg()
{
	shmid = shmget(1, MB_8 , SHM_HUGETLB | IPC_CREAT | SHM_R | SHM_W);

	if (shmid < 0) {
		perror("shmget error!");
		exit(1);
	}

	printf("init_hugepage_seg shmid: 0x%x.\n", shmid);

	a = shmat(shmid , 0, 0);

	if(a == (char *)(-1)) {
		perror("Shared memory attached failure!\n");
		shmctl(shmid, IPC_RMID, NULL);
		exit(2);
	}
}

void wr_to_array()
{
	int i;

	for( i = 0 ; i < MB_8; i++) {

		a[i] = 'A';
	}
}

void rd_from_array()
{
	int i, count = 0;

	for (i = 0; i < MB_8; i++) {

		if ('A' == a[i]) {
			count++;
		}
	}

	if (count == i) {
		printf("HugeTLB read success\n");
	} else {
		printf("HugeTLB read failed\n");
	}
}

int main()
{
	init_hugepage_seg();
	printf("Hugepage memory segmet initialized\n");
	printf("Press any key to write/read memory!\n");
	getchar();
	wr_to_array();
	rd_from_array();
	printf("Press any key to free hugepages and exit!\n");
	getchar();
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
