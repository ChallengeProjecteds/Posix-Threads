#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <features.h> 

struct node  
{ 
    int data; 
    struct node *left; 
    struct node *right;
} node;

void insertNode();

struct node* newNode(int data)
{ 
    // Allocate memory for new node
    struct node* node = (struct node*)malloc(sizeof(struct node)); 
  
    // Assign data to this node
    node->data = data; 

    // Initialize left and right children as NULL 
    node->left = NULL; 
    node->right = NULL; 
    return(node); 
}

void checkTheSmallest();

void insert(struct node **root, int data, int k, int *size)
{
    struct node *temp = newNode(data);

    if (!*root)
    {
        *root = temp;
        *size += 1;
        //printf("%d\n",*size);
    }
    else
    {
        //insertNode(*root, temp, k, size);

        if(*size < k)
        {
            insertNode(*root, temp, size);
        }
        else
        {
            checkTheSmallest(root, temp, size);
        }
    }
}

struct node* findSmallestNode(struct node *targetNode)
{
    if(targetNode->left == NULL)
        return(targetNode);
    else
        findSmallestNode(targetNode->left);
}

struct node* findSecondSmallestNode(struct node *targetNode, struct node *secondSmallestNode)
{
    if(targetNode == secondSmallestNode || secondSmallestNode->left == targetNode)
        return(secondSmallestNode);
    else
        findSecondSmallestNode(targetNode, secondSmallestNode->left);
}

void checkTheSmallest(struct node **currentNode, struct node *addedNode, int *size)
{
    struct node *smallestOne = findSmallestNode(*currentNode);
    struct node *secondSmallestOne = NULL;
    struct node *temp = NULL;
    if(smallestOne->data < addedNode->data)
    {
        //printf("siliniyor\n");
        *size -=1;
        //printf("%d = size\n",*size);
        secondSmallestOne = findSecondSmallestNode(smallestOne,*currentNode);
        if(smallestOne == secondSmallestOne)
        {
            //printf("parent %d degeri\n",secondSmallestOne->data);
            //printf("parent %d degerini goruyor\n",secondSmallestOne->right->data);
            temp = smallestOne->right;
            //printf("child %d degeri\n",temp->data);
            //printf("child %d solu\n",temp->left->data);
            //printf("child %d sagi\n",temp->right->data);
            free(smallestOne);
            *currentNode = temp;

            //printf("current %d degeri\n",**currentNode->data);
            insertNode(*currentNode, addedNode, size);
        }
        else if(smallestOne->right == NULL)
        {
            //printf("parent %d degeri\n",secondSmallestOne->data);
            //printf("parent %d degerini goruyor\n",secondSmallestOne->left->data);
            //printf("smalest de %d degeri\n",smallestOne->data);
            free(smallestOne);
            secondSmallestOne->left = NULL;
            insertNode(*currentNode, addedNode, size);
        }
        else
        {
            temp = smallestOne->right;
            free(smallestOne);
            secondSmallestOne->left = temp;
            insertNode(*currentNode, addedNode, size);
        }
    }
}

void insertNode(struct node *currentNode, struct node *addedNode, int *size)
{
    if(addedNode->data < currentNode->data)
    {
        if(currentNode->left == NULL)
        {
            currentNode->left = addedNode;
            *size += 1;
        }
        else
        {
            insertNode(currentNode->left, addedNode, size);
        }
    }
    else
    {
        if(currentNode->right == NULL)
        {
            currentNode->right = addedNode;
            *size += 1;
        }
        else
        {
            insertNode(currentNode->right, addedNode, size);
        }
    }
}

////////ağaç sonu\\\\\\\\

pthread_mutex_t mutext = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condt = PTHREAD_COND_INITIALIZER;
int go = 0;

struct node *root = NULL;

typedef struct thread_data {
   int k;
   struct node **root;
   int *agacBoyu;
   char *dosya;
 
} thread_data;

pthread_mutex_t lock;

void *findtop_k(void *args){


	FILE *fp;
	struct thread_data *tdata;
	tdata = (thread_data *)args;

	struct node **root = tdata->root;  
	
	int k, i;
	k = tdata->k;
	char *dosya;
	dosya = tdata->dosya;
	int sayi[k];
	fp = fopen(dosya,"r");
	
	int sz, biggest_num; 
	
	
	if (fp < 0) { perror("r1"); exit(1); } 	
	biggest_num = 0;
		
	while(fscanf(fp,"%d \n",&sz)!=EOF){
		/*if (sz > biggest_num){
		  	biggest_num = sz;
		
		}*/
		pthread_mutex_lock(&lock);
		//printf("ağacı çağır %d degerini yaz\n",sz);
		//int pointer = *tdata->agacBoyu;
		int *p = tdata->agacBoyu;
		//printf("ağaç boyu %d degerini yaz\n",pointer);
		insert(root,sz,k,p);

		pthread_mutex_unlock(&lock);
	}
	
	
	
	fclose(fp);

	go++;
	pthread_cond_signal(&condt);
	printf("işim bitti\n");
	pthread_exit(NULL);
}

void dosyayaYaz(struct node *root, FILE *fp)
{
	//printf("ağaç oku\n");
	if(root!=NULL)
	{		//visit the root
		dosyayaYaz(root->right, fp);

		//printf("%d\n",root->data);
		
		char sayiStringi[30];
		sprintf(sayiStringi,"%d\n",root->data);/*
		if (fwrite(sayiStringi, strlen(sayiStringi), 1, fp) != 1) {
			printf("Dosyaya yazma hatası!\n");
			exit(1);
		}*/
		fwrite(sayiStringi, strlen(sayiStringi), 1, fp);
		dosyayaYaz(root->left, fp);
	}
}


int main(int argc, char* argv[]){

	int k = atoi(argv[1]);
	int num = atoi(argv[2]);
	int agacBoyu = 0;

	int rc,t;
	void *status;
	
	pthread_t pthread_arr[num];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	struct thread_data tdata[num];
	char *fileLocs[num];
	
	if(pthread_mutex_init(&lock, NULL) != 0){
		printf("\n Mutex oluşturulamadı");
		return 1;
	}

	for(t=0; t<num; t++){
		
		tdata[t].k=atoi(argv[1]);
		tdata[t].root = &root;
		tdata[t].agacBoyu = &agacBoyu;
		fileLocs[num] = argv[t+3];
		tdata[t].dosya = fileLocs[num];
		printf("Ana Thread e %s dosyası veriliyor\n", fileLocs[num]);
		printf("Ana Thread oluşturuluyor %d\n", t);
		rc = pthread_create(&pthread_arr[t], &attr, findtop_k, (void *)&tdata[t]);
		if (rc) {
			printf("Hata kodu: %d\n", rc);
			exit(-1);
		} 
	}
		
	for(t=0; t<num; t++){
		rc = pthread_join(pthread_arr[t], &status);
		if (rc){
			printf("Hata kodu: %d\n", rc);
			exit(-1);
		
		}

		printf("Main: completed join with thread %d having a status of %ld\n",t,(long)status);
	} 
		pthread_attr_destroy(&attr);

	pthread_mutex_lock(&mutext);
	while(go<num)
	{
        	pthread_cond_wait(&condt, &mutext);
	}

	printf("sıra bende\n");
	FILE *fp;
	fp = fopen(argv[argc-1],"w+");//out dosyası adında bir dosya aç
	printf("%s dosyası açıldı\n",argv[argc-1]);

	dosyayaYaz(root,fp);

	fclose(fp);

	printf("Program tamamlandı.\n");
	pthread_mutex_destroy(&lock);
	pthread_exit(NULL);
	return 0;
}
