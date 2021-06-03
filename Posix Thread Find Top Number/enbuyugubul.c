#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]){ //Main.c'den gelen değerlerin alınması.
	int k = atoi(argv[1]); //k sayısının tanımlanması.
	  int sayi[k]; //dizimizin boyutu tanımlanıyor.
	  for(int i=0 ; i<k; i++){
	  	sayi[i] = 0; //Sıfır dizisi olarak tanımlıyoruz.
	  }
	  int fd, sz, sayac=0, sayi_2, temp_2; // Gerekli değişkenler tanımlanıyor.
	  char *c = (char *) calloc(100, sizeof(char));  //Dosyadan değer okumak için tanımlanacak dizi.
	  char *temp = (char *) calloc(100, sizeof(char)); //Sayıları geçici olarak tutacak dizi.
	  fd = open(argv[2], O_RDONLY); //Dosyamızı sadece okuma olarak açıyoruz
	  if (fd < 0) { perror("r1"); exit(1); } //Dosya başarılı açılmadıysa hata alıyoruz.
	  while(1){ //Dosyanın sonuna gelene kadar sürecek bir döngü.
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
	  if((c[sz-1] == ' ' || c[sz-1] == '\n') && sayac != 0 ){ // Karakter boşluk veya satır sonu ise okunan değerleri değişkene atar.
	  	sayi_2 = atoi(temp);
	  	for(int i=0; i<k; i++){ //bütün dizi boyunca girilen k. en büyük değeri bulmak için kullanılan döngü.
	  		if(sayi[i]<sayi_2){
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
	  else{
	  	temp[sayac]=c[sz-1]; //Eğer iki sayı ard arda geliyorsa o sayıları yan yana ekliyoruz.
	  	sayac++;
	  }
	}
	int pid = getpid();
	char buffer[20];
	sprintf(buffer,"%d",pid);
	//printf("%s",buffer);
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
	//printf("\n-%d-\n", sayi[k-1]); //En büyük k.değerini yazıyoruz.
	
	int close(int fd);
	int dosya;
	
	char buf[20];
	char sayiStringi[20];
	size_t nbytes;
	
	
	e=0;
	sprintf(sayiStringi,"%d",sayi[k-1]);
	
	dosya = open(buffer,O_CREAT | O_WRONLY,0777);
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
	ssize_t z=1;
	char eleman[2];
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
	
	write(dosya, sayiStringi, strlen(sayiStringi));
	int close(int dosya);
	exit(0);
	//strcpy(buf, sayiStringi);
	/*
	ssize_t bytes_written;
	
	nbytes = strlen(sayiStringi);
	bytes_written = write(dosya,sayiStringi,nbytes);
	printf("%ld\n",bytes_written);*/
}
