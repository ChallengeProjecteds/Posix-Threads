#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <fcntl.h>
#include <string.h>


#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME   "/ftk-queue1"
#define QUEUE_PERMISSIONS 0666
#define MAX_MESSAGES 50
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10


int main(int argc, char* argv[]){ // komut satırından argüman almak için yapılan tanımlamalar.
	
	//printf("%d\n",MSG_BUFFER_SIZE);
	int num; //Process sayısı
	int pid; //Process id
	int k = atoi(argv[1]);
	
	num = atoi(argv[2]); //Argümandan alınan N değerinin int'e dönüştürülmesi.
	
	int pidler[num];
	int okunanSayilar[num];
	
	mqd_t mq_parent;   // posix queue değişkenleri oluşturuluyor
		
	struct mq_attr attr; //mesaj özelliklerini içeren değişken oluşturuluyor

    	attr.mq_flags = 0;  // değişkene yeni değerler atanıyor
    	attr.mq_maxmsg = MAX_MESSAGES;
    	attr.mq_msgsize = MAX_MSG_SIZE;
    	attr.mq_curmsgs = 0;
	
	if ((mq_parent = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) 
	{
		perror ("Parent: mq_open (server)");
		exit (1);
    	}
    	
    	//printf("parent queue açıldı\n");
    	
	for(int i=0; i<num; i++)
	{
		pid = fork(); //N sayısı kadar child process oluşturulması.
		if(pid == 0)//Eğer child ise exec ile farklı programların yürütülmesi.
		{
			char *args[]={"enbuyugubul_mqueue",argv[1],argv[i+3],NULL}; //Echo dosyasına göndereceğimiz argümanlar
			execv(args[0],args);//Child process'lere başka programların atandığı yer. 
			pidler[i]=pid;
		}
		//printf("%d \n",pid);
	}
	
	//printf("childler bekleniyor\n");
	for(int i=0; i<num; i++){ //Child process'ler bitmeden parent'ın başlamaması için oluşturulan Wait komutu.
	wait(NULL); } 
	//printf("sıra parent'ta\n");
	//printf("child pid=%d\n",pid);
	pid = getpid();
	//printf("parent pid=%d\n",pid);
	if(pid > 0) //Parent ise yapılacak işlem.
	{
		//printf("num %d\n",num);
		char temp3[5][100];
	
		
	    	char in_buffer [MSG_BUFFER_SIZE];
	    
		for(int i=0;i<num;i++)
		{
			//printf("child bekleniyor\n");
			while(1)
			{
				//printf("-");
				if (mq_receive (mq_parent, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) 
				{
				    perror ("Parent: mq_receive");
				    exit (1);
				}
				if(in_buffer!=NULL)
				{
					break;
				}
			}
			//wait(NULL);
			//printf("-");
			/*if (mq_receive (mq_parent, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) 
			{
			    perror ("Parent: mq_receive");
			    exit (1);
			}*/
				
			//printf("mesaj alındı\n");
			//printf("alınan mesaj %s\n",in_buffer);
			//mq_unlink (in_buffer);
			//printf("mesaj silindi\n");
			//mq_unlink (mq_parent);
			/*
			if (mq_receive (mq_parent, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) 
			{
			    perror ("Parent: mq_receive");
			    exit (1);
			}*/
			for(int k=0;k<strlen(in_buffer);k++)
			{
				temp3[i][k] = in_buffer[k];
				temp3[i][k+1]='\n';
				temp3[i][k+2]='\0';
			}
			//printf("temp %s\n",temp3[i]);
			/*while(mq_receive (mq_parent, in_buffer, MSG_BUFFER_SIZE, NULL) != -1)
			{
				in_buffer[0]='\0';
				mq_receive (mq_parent, in_buffer, MSG_BUFFER_SIZE, NULL);
				//printf("mesaj %s\n",in_buffer);
				//mq_unlink (in_buffer);
			}*/
		}
		int fd_output = open(argv[argc-1],O_CREAT | O_WRONLY | O_TRUNC , 0777);
		int close(int fd_output);
		
		fd_output = open(argv[argc-1],O_CREAT | O_WRONLY , 0777);
		for(int i = 0; i < num; i++)
		{
			//printf("temp -- %s\n",temp3[i]);
			write(fd_output, temp3[i], strlen(temp3[i]));
		}
		
		int close(int fd_output);
		
		mq_close (mq_parent);
		return 0;
	}
}

	
