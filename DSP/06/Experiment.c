#include <stdio.h>
#include <stdlib.h>
#include "Node.h"
#include "BTree.h"
#include <time.h>
#define BILLION 1000000000
int N = 16777215;
int ho = 177941;
int m = 3;
double y,max=0,min= 100000 ,avg=0;

void process(){
avg = avg+y;
if(y>max)
max = y;
if(y<min)
min=y;
}


int main(int argc, char *argv[]){
         
         
	 struct timespec t1,t0;
         
	 
	 int i,order;
	 
  	 FILE *	myfile;
  	 int element;
	 order = atoi(argv[1]);;
         m = order;
	 
	
         BTree* btt1;
        
         btt1 = createBtree(order);
        
///////////INSERTING IN BST///////////////////////////////////////////////////
        
        myfile=fopen("Input.txt","r");//-----File opened;
    	for (i = 0 ;i < N; i++)
    	{
         
            fscanf(myfile,"%d",&element);
           
		 insertBtree(element,btt1);
               }

  	fclose(myfile); //------------File Closed
        
/////////////INSERTION COMPLETE///////////////////////////////////////
myfile=fopen("SearchInput.txt","r");//-----File opened;
    	for (i = 0 ;i < ho; i++)
    	{
            
            fscanf(myfile,"%d",&element);
                 clock_gettime(CLOCK_REALTIME,&t0);
		 insertBtree(element,btt1);
                 clock_gettime(CLOCK_REALTIME,&t1);
                y = (t1.tv_sec - t0.tv_sec)*BILLION +(t1.tv_nsec - t0.tv_nsec);
                
                 
                 process();
                 
               }

  	fclose(myfile); //------------File Closed
/////////////TIME VALUES SAVED for BST insert////////////////////////

avg = avg/ho;

printf("%d %lf %lf %lf \n", order, min,avg,max);
	return 0;
}



