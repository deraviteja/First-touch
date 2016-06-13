#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#define edg 200000
#define vert 50000
int main(int argc, char** argv){

int num_p = atoi(argv[1]);

int i = 0;
int rank;
char c = 'a';
MPI_Status stat;
int row[vert+1];
int col[edg+1];


    for(i=0;i<50001;i++)
    row[i] = 0;
    
        //////////////////////////////////////////////////////////////////////////
    ///////////////////                                 ////////////////////////////
        /////////////////                           /////////////////////////

FILE *fp;
char ch;
char **inf;
inf = malloc(sizeof(char)*3);
char *get_it;
int st = 0;
int gt = 0;
int r_o,r_o_2, c_o;
r_o = 1;

    fp = fopen("graph.dat","r");
   
   while((ch=fgetc(fp)) != EOF ){ // Read the first character of the line
      if(ch == 'a'){            
       gt = 0;
        ch = fgetc(fp); // Space has been provided after a, hence move ahead
        while( ch != '\n'){ // Move until end is reached            
            ch = fgetc(fp);
            get_it = malloc(sizeof(char)*5);
            st = 0;            
            while(ch != ' ' && ch != '\n'){               
                get_it[st++] = ch;
                ch = fgetc(fp);             
            }
            get_it[st] = '\0';
            inf[gt++] = get_it;
        }
        //////////////////////////////////////////
     /////////////////////
   ////////////////////  STORING THE GRAPH IN A csr FORMAT
        
        r_o_2 = atoi(inf[0]);
        c_o = atoi(inf[1]);
      
        if(r_o == r_o_2)
          row[r_o] = row[r_o]+ 1;   
        
        else{
            if((r_o_2-1) > r_o){
                while(r_o != r_o_2){
                    row[r_o+1] = row[r_o];
                    r_o++;
                    }
              }
        r_o = r_o_2;
        row[r_o] = row[r_o -1] +1 ;
        }
        col[row[r_o]] = c_o;
                                         ////////////////////////
                      //END OF CSR  //////////////////////////
               /////////////////////////////////////////////

      for(i = 0;i<3;i++)
      free(inf[i]);
      }
      else
        while(ch != '\n')
            ch = fgetc(fp);
    }
    fclose(fp);
    
    for(i = r_o_2 ; i<50002; i++)
        row[i] = row[r_o];
            
    ///////////////////                                 ////////////////////////////
        /////////////////                           /////////////////////////
            //////////////////////////////////////////////////////////////////////////
 
MPI_Comm w_com = MPI_COMM_WORLD; // Global communicator

///////////

MPI_Init(&argc,&argv);
MPI_Comm_rank(w_com,&rank);

double *pick = malloc(sizeof(double)*50001);
double *drop = malloc(sizeof(double)*50001);
double *ver_r = malloc(sizeof(double)*50001);
int start_r, end_r = 0;
int s_r = (edg*rank)/num_p;
int e_r = (edg*(rank+1))/num_p;
int r_f =0;
int temp = vert;
int j=0;
// search for row[start] >= edg*(rank/num_p) to row[end] < edg*(rank/num_p)


        while(row[start_r] < s_r || temp-1 > start_r ){
        
            if(temp-1 == start_r)
                start_r = temp;
            if( row[(start_r+ temp)/2] < s_r)
                start_r = (start_r+ temp)/2;
            else
                temp = (start_r+ temp)/2;
          
        }
        
temp = vert;
        while(row[end_r] < e_r && temp > end_r ){
        
            if(temp-1 == end_r)
                end_r = temp;
            if( row[(end_r+ temp)/2] < e_r)
                end_r = (end_r+ temp)/2;
            else
                temp = (end_r+ temp+1)/2;
           
        }
end_r = end_r-1; 
if(e_r == edg)
       end_r = vert;
if(s_r == 0)
    start_r = 1;
       
//printf("For rank:%d , row: %d,  end:%d , s_entry: %d, e_entry: %d || limit: %d, id: %d, re: %d \n", rank, start_r, end_r, row[start_r-1] +1, row[end_r], s_r, edg/num_p, row[end_r]-(row[start_r-1]+1));
       
       printf("%d %d\n",start_r,end_r);
       
for(i = 0;i<50001;i++){
    pick[i] = (1.0)/vert ; 
    ver_r[i] = (1.0)/vert ; 
    }
    
    for(i = 1;i<31;i++){

        for(j =0;j<50001;j++)
            drop[j] = 0.0;
            
        for(r_f = start_r;r_f<=end_r;r_f++){    
            ver_r[r_f] = ((0.15)/(vert)) + ((0.85)*(pick[r_f]));
            if(r_f != 0)
            for(j = row[r_f-1] +1 ; j<=row[r_f]; j++ )
                drop[col[j]] = drop[col[j]] + (ver_r[r_f]/(row[r_f]-row[r_f-1]));
            }

        
        for(j =0;j<50001;j++)
            pick[j] = 0.0;
           
            MPI_Barrier(w_com);
    
      MPI_Allreduce(drop,pick,50001,MPI_DOUBLE, MPI_SUM,w_com);

    }
    

/*for(j = 0;j<num_p;j++){*/
/*if(j==rank)   */
/* for(r_f = start_r;r_f<=end_r;r_f++)*/
/*           //printf("final rank:%lf at %d \n",ver_r[r_f],r_f);*/
/*           printf("%d %lf\n",r_f,ver_r[r_f]);*/
/*fflush(stdout);*/
/*MPI_Barrier(w_com);*/
/*}           */
MPI_Barrier(w_com);
MPI_Finalize();



}
