//AKRAM ABID
//BELKACEM ROUIBI

#include<unistd.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<semaphore.h>

sem_t full;
sem_t empty;
sem_t mute;
int LA,CA,LB,CB;
typedef struct tomp{
    int state;
    int value;
}tomp;

int taille=3;
int a[10][10];
int b[10][10];
int c[10][10];
tomp tampon[10];

void* add(void* arg){
    int i = (int)arg;
    int sm=0;
    for(int j=0; j<CB; j++){
             
		for(int k=0; k<LB; k++){
                  sm+=a[i][k]*b[k][j];
              }
              
              sem_wait(&empty);
              sem_wait(&mute);
              if(tampon[j].state == 0){
                  //c[i][j]=sm;
                  tampon[j].value = sm;
                  tampon[j].state = 1;
              }
              
              sem_post(&mute);
              sem_post(&full);
              sm=0;
    }
}


void* consumer(void* arg){
    int j = (int)arg;

    for(int k=0; k<CB; k++){
             sem_wait(&full);
             sem_wait(&mute);	
	     if(tampon[k].state == 1){	
                  c[j][k]=tampon[k].value;
		  tampon[k].state = 0;
		}
             sem_post(&mute);
             sem_post(&empty);
	}
}

int main(){
	
   int x,y,w,z;
  
   printf("enter the size please ligne de A");
   scanf("%d",&x);
   LA=x;
   
   printf("enter the size please columne de A");
   scanf("%d",&y);
   CB=y;
   
   printf("enter the size please ligne de B");
   scanf("%d",&w);
   LB=w;
   
   printf("enter the size please columne de B");
   scanf("%d",&z);
   CB=z;
   
   pthread_t t[LA];
   pthread_t cons[LA];   
   
   sem_init(&full,0,0);
   sem_init(&empty,0,3);
   sem_init(&mute,0,1);
   
   int i,j;
   if(y==w){ 
   
   printf("Fill The First Matrix(A)\n");    
   for( i=0; i<x ;i++){
       for( j=0; j<y; j++){
           printf("enter a[%d][%d]=",i,j);
           scanf("%d",&a[i][j]);
       }
   }
   printf("Fill The Second Matrix(b)\n");
   for( i=0; i<w ;i++){
       for( j=0; j<z; j++){
           printf("enter b[%d][%d]=",i,j);
           scanf("%d",&b[i][j]);
       }
   }

     
   for(i=0; i<LA; i++){
       pthread_create(&t[i],NULL, &add, &i);
       pthread_create(&cons[i],NULL, &consumer, &i);
       pthread_join(t[i], NULL);
       pthread_join(cons[i], NULL); 
   }
   
  
   for(i=0; i<LA; i++){
       for(j=0; j<CB; j++){
           printf("C[%d][%d]=%d\t",i,j,c[i][j]);
          
       }
       printf("\n");
       }
       
  }else{
      printf("\nTHE LINE NUMBER OF THE FIRST MATRIX IS NOT EQUAL TO THE SECOND MATRIX'S COLUMN NUMBER\nYOU HAVE TO ENTER IT RIGHT!!!..\n");
  }
   
}
