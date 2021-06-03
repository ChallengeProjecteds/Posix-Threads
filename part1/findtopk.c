#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]){ // komut satırından argüman almak için yapılan tanımlamalar.
	
	int num; //Process sayısı
	int pid; //Process id
	int k = atoi(argv[1]);
	
	num = atoi(argv[2]); //Argümandan alınan N değerinin int'e dönüştürülmesi.
	
	int pidler[num];
	int okunanSayilar[num];
	
	for(int i=0; i<num; i++)
	{
		pid = fork(); //N sayısı kadar child process oluşturulması.
		if(pid == 0)//Eğer child ise exec ile farklı programların yürütülmesi.
		{
			char *args[]={"enbuyugubul",argv[1],argv[i+3],NULL}; //Echo dosyasına göndereceğimiz argümanlar
			execv(args[0],args); //Child process'lere başka programların atandığı yer. 
			  //child lar başka bir değişkende toplanıyor
		}
		pidler[i]=pid;
	}
	 char temp3[5][100];
	 for(int i=0; i<num; i++){ //Child process'ler bitmeden parent'ın başlamaması için oluşturulan Wait komutu.
	 wait(NULL); }
	 
	 int fd_t,size;
	 char ch[100];
	 char *s;
 	 int okunanChar;
 	 char buffer[20];
	 if(pid > 0) //Parent ise yapılacak işlem.
	 {
		 for(int i=0;i<num;i++)
		 {
		 	
			sprintf(buffer,"%d",pidler[i]);
			//printf("okunan dosya %s\n",buffer);
			int e=0;
			while(1)
			{
				if(buffer[e] == '\0')
				{
					break;
				}
				else
				{
					e++;
				}
			}
			buffer[e] = '.';
			e++;
			buffer[e] = 't';
			e++;
			buffer[e] = 'x';
			e++;
			buffer[e] = 't';
			e++;
			buffer[e] = '\0';
			
			
			
			
			fd_t = open(buffer, O_RDONLY);
			if (fd_t <0) {perror("r1"); exit(1);}
			char temp2[100];
			okunanChar=0;
			
			while(1)	
			{ 
			   size = read(fd_t, ch, 1);
			  	
			   if(size == 0){
			   	if(okunanChar>0)
			   	{
			   	   temp2[okunanChar] = '\n';
				   temp2[okunanChar+1] = '\0';
			   	}
			   	okunanChar = 0;
			   	break;
			   }
			   else if(ch[0]==' ' || ch[0]=='\n')
			   {
			   	if(okunanChar > 0)
			   	{
			   	   	temp2[okunanChar] = '\n';
				   	temp2[okunanChar+1] = '\0';
			   		okunanChar = 0;
			   	}
			   }
			   else{
			   	temp2[okunanChar] = ch[0];
			   	okunanChar++;
			   	temp2[okunanChar] = '\n';
				temp2[okunanChar+1] = '\0';
			   }
			  
			}
			int close(int fd_t);
			remove(buffer);
			for(int k=0; k < strlen(temp2); k++){
			//printf("%d %d temp-- a \n",temp3[i][k],temp2[k]);
			temp3[i][k] = temp2[k];
			temp3[i][k+1]='\0';
			//temp3[i][k+2]='\n';
			}
			
		 }
		 remove(buffer);
		 /*printf("Sonuçlar birleştirildi\n");
		 for(int i=0;i<num;i++)
		 {
		 	printf("%d\n",okunanSayilar[i]);
		 }*/
	 
		int fd_output = open(argv[argc-1],O_CREAT | O_WRONLY | O_TRUNC , 0777);
		char *temp4;
		int close(int fd_output);
		fd_output = open(argv[argc-1],O_CREAT | O_WRONLY , 0777);
		for(int i = 0; i < num; i++)
		{
			//printf("temp -- %s",temp3[i]);
			write(fd_output, temp3[i], strlen(temp3[i]));
		}
		
		int close(int fd_output);
	}
	return 0;
}

	
