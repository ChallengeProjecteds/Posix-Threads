#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* for calloc */
#include <unistd.h>  /* for read */
#include <fcntl.h> /* for O_RDONLY */
#include <features.h> /*for pthread_mutex_signal*/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int go = 0;

typedef struct thread_data {
   int k;
   char *dosya;
 
} thread_data;

void *dosyaOkuVeKnciEnBuyukDegeriBul(void* arg){

	struct thread_data *tdata;
	tdata = (thread_data *)arg;
	
	int k;
	k = tdata->k;
	char *dosya;
	dosya = tdata->dosya;
	
	int sayi[k]; //dizimizin boyutu tanımlanıyor.

	for(int i=0 ; i<k; i++)
	{
		sayi[i] = 0; //Sıfır dizisi olarak tanımlıyoruz.
	}

	int fd, sz, sayac=0, sayi_2, temp_2; // Gerekli değişkenler tanımlanıyor.
	char *c = (char *) calloc(100, sizeof(char));  //Dosyadan değer okumak için tanımlanacak dizi.
	char *temp = (char *) calloc(100, sizeof(char)); //Sayıları geçici olarak tutacak dizi.
	
	fd = open(dosya, O_RDONLY); //Dosyamızı sadece okuma olarak açıyoruz
	if (fd < 0) { perror("r1"); exit(1); } //Dosya başarılı açılmadıysa hata alıyoruz.
	
	while(1) //Dosyanın sonuna gelene kadar sürecek bir döngü.
	{
		sz = read(fd, c, 1); //Tek karakter okuyacak read() komutu.
		if(c[sz-1] == '\0') //O karakter sonlandırma karakteri ise while döngüsünü kırıyoruz.
		{ 
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
			break;
		}
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
	
			for(int i=0 ; i < sayac; i++) //Temp değerimizi sıfırlıyoruz yeni bir sayıya geçince.
			{
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
	
	
	tdata->k = sayi[k-1];
	
	
	go++;
	pthread_cond_signal(&cond);
	
	pthread_exit(NULL);
}

void main(int argc, char* argv[])
{
	int num; //Process sayısı
	int pid; //Process id
	int k = atoi(argv[1]);//Argümandan alınan K değerinin int'e dönüştürülmesi.
	num = atoi(argv[2]); //Argümandan alınan N değerinin int'e dönüştürülmesi.
	void *thread_result=0; 
	
	pthread_t threadler[num];//thread lerin tutulduğu dizi oluşturuluyor
	thread_data tdata[num];//thread lerden alınan değerler burada saklanacak
	
	pthread_mutex_lock(&mutex);
	
	char *dosyaKonumlari;
    	for(int i=0; i<num; i++)
	{
		tdata[i].k=atoi(argv[1]);//threde verilecek değişkenler atanıyor
		dosyaKonumlari = argv[i+3];
		tdata[i].dosya = dosyaKonumlari;
		
		pthread_create(&threadler[i], NULL, dosyaOkuVeKnciEnBuyukDegeriBul, (void *)&tdata[i] ); //N sayısı kadar thread oluşturulması.
		pthread_join(threadler[i],&thread_result);//threadler başlatılıyor
	}
	
	while(go<num)
	{
        	pthread_cond_wait(&cond, &mutex);
	}
	
	char sayiStringi[20];
	int fd_output = open(argv[argc-1],O_CREAT | O_WRONLY | O_TRUNC , 0777);
	int close(int fd_output);
	fd_output = open(argv[argc-1],O_CREAT | O_WRONLY , 0777);
	for(int i = 0; i < num; i++)
	{
	//sprintf(buffer,"%d",pid);
		sprintf(sayiStringi,"%d\n",tdata[i].k);
		write(fd_output, sayiStringi, strlen(sayiStringi));
	}
	
	int close(int fd_output);
}
