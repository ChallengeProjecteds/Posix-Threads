#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<errno.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define SHM_KEY 0x5196
sem_t mutex;

struct node *root = NULL;

typedef struct thread_data {
   int k;
   char *dosya;
 
} thread_data;

int go = 0;

void pointeraDegerAta(int *p, int *size, int k, int data)
{
	if(*size == 0)
	{
		p[(*size)] = data;
		*size += 1;
	}
	else
	{
		int temp = 0;
		if(*size < k)
		{
			for (int i = 0; i<(*size)+1; i++)
			{
				if(p[i] < data)
				{
					temp = p[i];
					p[i] = data;
					data = temp;
				}
			}
			(*size) += 1;
		}
		else
		{
			for (int i = 0; i<k; i++)
			{
				if(p[i] < data)
				{
					temp = p[i];
					p[i] = data;
					data = temp;
				}
			}
		}
	}
}

void findtop_k(struct thread_data *tdata, int *p,int *size){


	FILE *fp;

	
	int k, i;
	k = tdata->k;
	char *fileLocs;
	fileLocs = tdata->dosya;
	int sayi[k];
	int sz; 

	fp = fopen(fileLocs,"r");
	
	
	if (fp < 0) { perror("r1"); exit(1); }

	while(fscanf(fp,"%d \n",&sz)!=EOF){
  				
		sem_wait(&mutex);

		pointeraDegerAta(p, size, k, sz);

		sem_post(&mutex);
				
	}
	fclose(fp);
	//printf("işim bitti\n");
	go+=1;
}

int main(int argc, char* argv[]){ // komut satırından argüman almak için yapılan tanımlamalar.

	sem_init(&mutex, 1, 1);
	
	int k = atoi(argv[1]);
	int num = atoi(argv[2]); //Process sayısı
	char *fileLocs[num];
	int pid;
	int agacBoyu = 0;
	

	struct thread_data tdata[num];
	
	for(int i = 0; i<num; i++){
		fileLocs[i] = argv[i+3];
	}

	
	FILE *fp;

	int *p = mmap(NULL,k*sizeof(int),
		PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS,
		0,0);

	if(p == MAP_FAILED){
		printf("Mapping Failed\n");
		return 1;
	}


	int *size = mmap(NULL,sizeof(int),
		PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS,
		0,0);

	if(size == MAP_FAILED){
		printf("Mapping Failed\n");
		return 1;
	}


	int main = 1;
	for(int i=0; i<num; i++)
	{	
		tdata[i].k=atoi(argv[1]);
		fileLocs[num] = argv[i+3];
		tdata[i].dosya = fileLocs[num];
		
		pid = fork();
		
		if(pid == 0)//Eğer child ise exec ile farklı programların yürütülmesi.
		{		
			main = 0;
			findtop_k(&tdata[i], p, size);
			break;
		}
		else
		{
			wait(NULL);
		}
	 }
	 
	if(main == 1)
	{/*
		while(go < num)
		{
			wait(NULL);
		}*/
	 	fp = fopen(argv[argc-1],"w+");//out dosyası adında bir dosya aç

		//printf("%s dosyası açıldı\n",argv[argc-1]);

		for(int i = 0; i<k ; i++)
		{
			char sayiStringi[30];
			sprintf(sayiStringi,"%d\n",p[i]);
			//printf("%s ==> String \n",sayiStringi);
			fwrite(sayiStringi, strlen(sayiStringi), 1, fp);
		}

		fclose(fp);

		//printf("Program tamamlandı.\n");
		 
		 
		
		sem_destroy(&mutex);

		int err = munmap(p, k*sizeof(int));

		if(err != 0){
			printf("UnMapping Failed\n");
			return 1;
		}

		sem_destroy(&mutex);

		int err2 = munmap(size, sizeof(int));

		if(err2 != 0){
			printf("UnMapping Failed\n");
			return 1;
		}
	}
}
