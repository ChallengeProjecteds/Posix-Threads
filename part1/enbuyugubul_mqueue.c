#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <fcntl.h>
#include <string.h>

#include <mqueue.h>

#define SERVER_QUEUE_NAME   "/ftk-queue1"
#define QUEUE_PERMISSIONS 0666
#define MAX_MESSAGES 50
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int main(int argc, char* argv[]){ //Main.c'den gelen değerlerin alınması.
	int k = atoi(argv[1]); //k sayısının tanımlanması.
	  int sayi[k]; //dizimizin boyutu tanımlanıyor.
	  
	  for(int i=0 ; i<k; i++)
	  {
	  	sayi[i] = 0; //Sıfır dizisi olarak tanımlıyoruz.
	  }
	  
	  int fd, sz, sayac=0, sayi_2, temp_2; // Gerekli değişkenler tanımlanıyor.
	  char *c = (char *) calloc(100, sizeof(char));  //Dosyadan değer okumak için tanımlanacak dizi.
	  char *temp = (char *) calloc(100, sizeof(char)); //Sayıları geçici olarak tutacak dizi.
	  fd = open(argv[2], O_RDONLY); //Dosyamızı sadece okuma olarak açıyoruz
	  
	  if (fd < 0) { perror("r1"); exit(1); } //Dosya başarılı açılmadıysa hata alıyoruz.
	  while(1) //Dosyanın sonuna gelene kadar sürecek bir döngü.
	  {
		sz = read(fd, c, 1); //Tek karakter okuyacak read() komutu.
		if(c[sz-1] == '\0'){ 
		if(sayac != 0)
		{
			sayi_2 = atoi(temp);
			for(int i=0; i<k; i++) //bütün dizi boyunca girilen k. en büyük değeri bulmak için kullanılan döngü.
			{
			    if(sayi[i]<sayi_2)
			    {
				temp_2 = sayi[i];
				sayi[i] = sayi_2;
				sayi_2 = temp_2;
			    }

			}
		}
		break;} //O karakter sonlandırma karakteri ise while döngüsünü kırıyoruz.
		c[sz] = '\0'; 
		if((c[sz-1] == ' ' || c[sz-1] == '\n') && sayac != 0 ) // Karakter boşluk veya satır sonu ise okunan değerleri değişkene atar.
		{
			sayi_2 = atoi(temp);
			for(int i=0; i<k; i++) //bütün dizi boyunca girilen k. en büyük değeri bulmak için kullanılan döngü.
			{
				if(sayi[i]<sayi_2)
				{
					temp_2 = sayi[i];
					sayi[i] = sayi_2;
					sayi_2 = temp_2;	  			
				}

			}

			for(int i=0 ; i < sayac; i++){ //Temp değerimizi sıfırlıyoruz yeni bir sayıya geçince.
				temp[i]='\0';
			}
			sayac=0; //sayacı sıfırlıyoruz.

		}
		else
		{
		temp[sayac]=c[sz-1]; //Eğer iki sayı ard arda geliyorsa o sayıları yan yana ekliyoruz.
		sayac++;
		}
	}
	int pid = getpid();
	
	//printf("\n-%d-\n", sayi[k-1]); //En büyük k.değerini yazıyoruz.
	
	int close(int fd);
	//int dosya;
	
	//char buf[20];
	//char sayiStringi[20];
	
	mqd_t mq_parent;
	
	struct mq_attr attr;

	attr.mq_flags = 0;
	attr.mq_maxmsg = MAX_MESSAGES;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_curmsgs = 0;
	
    	//printf("Parent a gönderilmek üzere mesaj açılıyor\n");
	int sonuc = (mq_parent = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1;
	while(sonuc)
	{
		sonuc = (mq_parent = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1;
	}
	/*
	if ((mq_parent = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1)
	{
		perror ("Client: mq_open (server)");
		exit (1);
    	}*/
    	
    	//printf("Parent a gönderilmek üzere mesaj açıldı\n");

	char in_buffer [MSG_BUFFER_SIZE];
	
	sprintf (in_buffer, "%d", sayi[k-1]);
	
	//printf("Parent e gönderilen mesaj: %s\n",in_buffer);
	// send message to server
	
	//sonuc = mq_send (mq_parent, in_buffer, MSG_BUFFER_SIZE + 1, 0) == -1;
	sonuc = mq_send (mq_parent, in_buffer, strlen(in_buffer) + 1, 0) == -1;
	/*printf("ilk deneme\n");
	while(sonuc)
	{
		//printf("sorunVar");
		sonuc = mq_send (mq_parent, in_buffer, MSG_BUFFER_SIZE + 1, 0) == -1;
	}*/
	/*printf("Parent a mesaj gönderildi\n");
	if(sonuc)
	{
		printf("gönderim başarısız\n");
	}*/
	
	//int son = mq_close (mq_parent) == -1;
	//while(son)
	//{
	//	son = mq_close (mq_parent) == -1;
	//}
	/*
	if (mq_close (mq_parent) == -1) {
		perror ("Client: mq_close");
		exit (1);
	}*/
	//printf("Parent mesaj yolu kapandı\n");
	//return 0;
/*
	if (mq_unlink (client_queue_name) == -1) {
		perror ("Client: mq_unlink");
		exit (1);
	}*/
	/*
        if (mq_send (qd_server, in_buffer, strlen (in_buffer) + 1, 0) == -1) {
            perror ("Client: Not able to send message to server");
            continue;
        }*/
        
	/*
	size_t nbytes;
	
	
	e=0;
	sprintf(sayiStringi,"%d",sayi[k-1]);
	
	dosya = open(buffer,O_CREAT | O_WRONLY,0777);*/
	/*while(1)
	{
		if(sayiStringi[e] == '\0')
		{
			sayiStringi[e] = '\n';
			break;
		}
		else
		{
			e++;
		}
	}*/
	/*
	ssize_t z=1;
	char eleman[2];*/
	/*while(1)
	{
		if(sayiStringi[e] == '\0')
		{
			eleman[0]=sayiStringi[e];
			eleman[1]='\n';
			strcpy(buf, eleman);
			
			nbytes = strlen(buf);
			write(dosya,(char *)sayiStringi[e],z);
			break;
		}
		else
		{
			eleman[0]=sayiStringi[e];
			eleman[1]='\n';
			strcpy(buf, eleman);
			
			//strcpy(buf, sayiStringi[e]);
			nbytes = strlen(buf);
			write(dosya,(char *)sayiStringi[e],z);
			e++;
		}
	}*/
	
	/*
	write(dosya, sayiStringi, strlen(sayiStringi));
	int close(int dosya);*/
	//printf("%d pid li proses kapandı\n",pid);
	exit(0);
	//strcpy(buf, sayiStringi);
	/*
	ssize_t bytes_written;
	
	nbytes = strlen(sayiStringi);
	bytes_written = write(dosya,sayiStringi,nbytes);
	printf("%ld\n",bytes_written);*/
}
