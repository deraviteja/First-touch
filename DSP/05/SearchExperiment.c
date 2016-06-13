#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "BST.h"
#include "AVL.h"
#include <time.h>

int N = 89999;

void fsave(double v[], char s[]){
	int n=N,i;
	FILE *fp;
	//int x;
	fp = fopen(s,"w+");
	for (i=0;i<n;i++){
		fprintf(fp,"%f\n",v[i]);
	}
	fclose(fp);
    }


int main(){

	clock_t start_t, end_t, start, end;

	double y[N],y1[N];
	int i;

	Bst* Lbst = createBST();
        Avl* Mavl = createAVL();

  	FILE *	myfile;
  	int element;

///////////INSERTING IN BST///////////////////////////////////////////////////
        
        myfile=fopen("input.txt","r");//-----File opened

    	for (i = 0 ;i < N; i++)
    	{
      		fscanf(myfile,"%d",&element);
      		//printf("%d ",element);
	        start=clock();
		insertbst(Lbst, element); ////////////////
	        end=clock();
		y[i]=(double)(end - start);
	}

  	fclose(myfile); //------------File Closed

/////////////INSERTION COMPLETE///////////////////////////////////////
	
	fsave(y,"inbst.m");

/////////////TIME VALUES SAVED for BST insert////////////////////////
	
//////////////SEARCHING IN BST////////////////////////////////////////
        
        start = clock();
    	for (i = 0 ;i < N; i++)
    	{
		start_t = clock();
		searchbst(Lbst, i+10000);
		end_t = clock();
		y1[i]=(double)(end_t - start_t);
	}
	end = clock();

//////////////SEARCHING IN BST COMPLETE/////////////////////////////// 
	
	fsave(y1,"shbst.m");

/////////////////Time Values Saved for BST Search

//////////////////////////////////////////////////////////////////////////////////////

///////////INSERTING IN AVL///////////////////////////////////////////////////
        
        myfile=fopen("input.txt","r");//-----File opened

    	for (i = 0 ;i < N; i++)
    	{
      		fscanf(myfile,"%d",&element);
      		//printf("%d ",element);
	        start=clock();
		insertAVL(Mavl, element); ////////////////
	        end=clock();
		y[i]=(double)(end - start);
	}

  	fclose(myfile); //------------File Closed

/////////////INSERTION COMPLETE///////////////////////////////////////
	
	fsave(y,"inavl.m");

/////////////TIME VALUES SAVED for AVL insert////////////////////////
	
//////////////SEARCHING IN AVL////////////////////////////////////////
        
        start = clock();
    	for (i = 0 ;i < N; i++)
    	{
		start_t = clock();
		searchAvl(Mavl, i+10000);
		end_t = clock();
		y1[i]=(double)(end_t - start_t);
	}
	end = clock();

//////////////SEARCHING IN BST COMPLETE/////////////////////////////// 
	
	fsave(y1,"shavl.m");

/////////////////Time Values Saved for BST Search



	return 0;
}
