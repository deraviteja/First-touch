#include "graph.h"
#include<time.h>


int main(int argc, char**argv)
{
	FILE * fp;
	int i,j,k,total,s,w;
	int *pathLength;
	int V,t;
	int p,q,r;
	char *line = NULL;
	struct timespec start1,start2,end1,end2;
	long double time_taken;
	graph*  newgraph;
	size_t len = 0,sz,read;	
	
	fp = fopen("input.txt", "r");
	read = getline(&line, &len, fp);
	sscanf(line,"%d",&total);
	
	for(i=0;i<total;i++){
		read = getline(&line, &len, fp);
		sscanf(line,"%d",&V);
  		newgraph = createGraph(V);
  		
		for(j=0;j<(V*V);j++){	
  			read = getline(&line, &len, fp);
  			sscanf(line,"%d %d %d",&p,&q,&r);
  			if(r!=0)
  			addEdge(newgraph, p, q, r); 
  		}
  	
       	read = getline(&line, &len, fp);
		sscanf(line,"%d",&t);
	
    	for(k=0;k<t;k++){
			read = getline(&line, &len, fp);
			sscanf(line,"Exp# %d: %d",&w,&s);
			clock_gettime(CLOCK_MONOTONIC,&start1);
			SSShortestPath(newgraph, s, pathLength);
			clock_gettime(CLOCK_MONOTONIC,&end1);
			time_taken = (long double) ((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);
			printf("%d\t%Lf\n",w,time_taken);
		}
	}
	
	fclose(fp);
 
    return 0;
}
