#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


typedef struct rowN{
int *row_get;
int startN;
int endN;
int sizeofrow;
int Qno;
struct rowN* next; 
}rowQ; 

typedef struct hey2{
rowQ* top;
rowQ* bottom;
} computeQ;
computeQ* createQ();
rowQ* deQ(computeQ* theq);
int enQ(computeQ* theq,rowQ* recvrow);

int main(int argc,char ** argv){

// ------------------------------- PARALLEL IMPLEMENTATION VARIABLES ---------------------

                            int i,j,k = 0; 
                            int p_cs = atoi(argv[1]);
                            int p_rank = 0;
                            int steps = atoi(argv[2]);

                            int exec[p_cs][steps];
                            for(i= 0; i<p_cs; i++)
                                for(j=0; j<steps;j++)
                                    exec[i][j] = -1;
                                    
                            int send_p[p_cs][steps-1];
                            int rec_p[p_cs][steps-1];
                            for(i= 0; i<p_cs; i++){
                                for(j=0; j<steps-1;j++){
                                    send_p[i][j] = -1;
                                    rec_p[i][j] = -1;
                                }
                            }

                            int end_p[p_cs];
                            for(i= 0; i<p_cs; i++)
                                end_p[i] = 0;

                            exec[0][0] = 1;
                            end_p[0] = p_cs-1;
                            
//-------------------------------NORMAL VARIABLES ----------------------------------

FILE *fp;
int i1,i2,i3,j1,j2,j3,k1,k2,k3=0;
int *row;
int *col;
int rs,cs;
int r_all;

int sps;
int *A,*B,*B2;
int *Da,*Db;
char *lA,*lB;
int *Fa,*Fb,*FG;
int *rowG;
char *abG; 
int G1,G2,G3 = 1;
int Gm1=-20;
int Gm2=1,Gm3=1;
int sA,sB,ma,mb,mk=-1,mk2=0,w;
int t_old,t_new,t_s;
int p_start,p_end;

        rowQ* r_set;
        rowQ* r_get;
        computeQ* procQ;
        int Qid = 0;
        procQ = createQ();
        
/////-------------------------------------------------------------------------------------------------------\\\\\\\\
////////////----------------------       FILE HANDLING        ------------------------------------------\\\\\\\\\\\\\    


                            fp = fopen("row.dat","r"); // --------------------------> NEEDS EDIT
                            rs = 1;
                            row = malloc(sizeof(int)*4001);
                            rowG = malloc(sizeof(int)*4000);
                            *row = 0;
                            *rowG = 0;
                             while(fgetc(fp) != EOF ){            
                                    fscanf(fp,"%d",row + rs);
                                    *(rowG + rs) = rs;
                                    rs = rs+1;
                                    fgetc(fp);
                                }
                            fclose(fp);
                            
                            fp = fopen("col.dat","r");
                            cs = 0;
                            col = malloc(sizeof(int)*15560);//----------------------> NEEDS EDIT
                            
                             while(fgetc(fp) != EOF ){            
                                    fscanf(fp,"%d",col + cs);
                                    *(col +cs) = *(col +cs)-1;
                                    cs = cs+1;            
                                    fgetc(fp);
                                }
                            fclose(fp);
                            
                            rs--;
                            printf("rs:%d cs:%d \n",rs,cs);
                            r_all = rs;
                            //rs = rs/4;
    
/////---------------------------------  End of file handling  ----------------------------------------------\\\\\\\\
////////////--------------------------------------------------------------------------------------------\\\\\\\\\\\\\    

///------------------------------------------ ALL PREPROCESSING from 02_ND_ file
for(i = 0;i<steps-1;i++){
    for (j=0;j<p_cs;j++){
        if(exec[j][i] == 1){
            k = (j+end_p[j])/2;
            exec[j][i+1] = 1;
                //if(k!=0){
                exec[k+1][i+1] = 1;
                end_p[k+1] = end_p[j];
                if(j != end_p[j] ){
                    send_p[j][i] = k+1;
                    rec_p[k+1][i] = j; 
                }
                else{
                send_p[j][i] = k;
                rec_p[k][i] = j;
                }
               // }
            end_p[j] = k;                    
        }       
    }
}

//-------------------------------------------------------------
    
    Da = malloc(sizeof(int)*r_all);
    Db = malloc(sizeof(int)*r_all);
    abG = malloc(sizeof(char)*r_all);
    
    // -----------------------------------------------------------
    for(i=0;i<r_all;i++){
        *(abG +i) = 'p';
    }

    //printf("check %d\n",p_rank);
///////-----------------------------------------End of PREPROCESSING

MPI_Status stat;
MPI_Comm comm = MPI_COMM_WORLD;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&p_rank);

    fflush(stdout);
    int exec_ps[steps-1];
    int send_ps[steps-1];
    int rec_ps[steps-1];
    int s,r=0;
    int *ps = malloc(sizeof(int)*1);
    int *pr = malloc(sizeof(int)*1);
    *ps = 1;
    *pr = 1;
    int *sendbuff;
    int *recvbuff;

// ----------------- PRE COMPUTATION PICKUPS -------------------------------------------
                for(i=0;i<steps-1;i++)
                    exec_ps[i] = exec[p_rank][i];
                for(i=0;i<steps-1;i++){
                    send_ps[i] = send_p[p_rank][i];
                    rec_ps[i] = rec_p[p_rank][i];
                }
//-------------------------------------------------------------------------------------

        if(p_rank==0){
            r_set = malloc(sizeof(rowQ));
            r_set->row_get = rowG;
            r_set->sizeofrow = rs;
            r_set->startN = 0;
            r_set->endN = rs-1;
            r_set->Qno = Qid;
            Qid = Qid+1;
            r_set->next = NULL;            
            rowG = NULL;
            enQ(procQ,r_set);
        }
        

for(sps=0;sps<steps-1;sps++){


    if(exec_ps[sps] == 1){
     //--------------------------------------COMPUTATION STARTS ----------------------------
            t_new = 0;
            t_old = 0;
            t_s = 0;    
            Gm2=1;
            mk2 = 0;
            
    // ---------------------- UNPACK FROM QUEUE ------------------------ 
    r_get = deQ(procQ);
    rowG = NULL;
    rowG = r_get->row_get;
    rs = r_get->sizeofrow;
    p_start = r_get->startN;
    p_end = r_get->endN;
    
    
    printf("ps:%d ce:%d rows: %d qid: %d at: %d step:%d \n",p_start,p_end,rs, r_get->Qno,p_rank,sps);
    
    t_old =rs;
    
    sA = rs/2;
    sB = rs-rs/2;    
    
    A = malloc(sizeof(int)*sA);
    B = malloc(sizeof(int)*sB);

    lA = malloc(sizeof(char)*sA);
    lB = malloc(sizeof(char)*sB);

    Fa = malloc(sizeof(int)*sA);
    Fb = malloc(sizeof(int)*sA);
    FG = malloc(sizeof(int)*sA);
    
    for(i = 0;i<sA;i++) *(A + i) = *(rowG + i);    
    for(i = 0;i<sB;i++) *(B + i) = *(rowG + i+sA);


/*                            fp = fopen("abG1.dat","w");*/
/*                            for(i=0;i<200;i++){*/
/*                                for(j=0;j<20;j++)*/
/*                                    fprintf(fp,"%c ",abG[i*20+j]);*/
/*                                    fprintf(fp,"\n");*/
/*                                }*/
/*                            fclose(fp);*/
   
    Gm3 = 0;
    mk2 = 0;
    
    while(Gm2>0){
      
      Gm2 = 0;
      t_new = 0;
      
            for(i = 0;i<sA;i++){
            *(lA+i) = 'N';
            *(abG+ *(A + i)) = 'a';  
            *(Da + *(A + i)) = 0;
            }
        
            for(i = 0;i<sB;i++){
                *(lB + i) = 'N';
                *(abG + *(B + i)) = 'b';
                *(Db + *(B + i)) = 0;
            }
        
            for(i=0;i<sA;i++){
                k1 = *(A + i);
                for(j=0;j<sB;j++){
                    k2 = *(B+j);
                    for(i1 = row[k1];i1<row[k1+1];i1++){
                        if(col[i1]==k2){
                            Da[k1]= Da[k1]+1;
                            Db[k2] = Db[k2]+1;
                            t_new = t_new+1;
                        }
                    }
                }
                for(j=0;j<sA;j++){
                    k2 = *(A+j);
                    for(i1 = row[k1];i1<row[k1+1];i1++)
                        if(col[i1]==k2)
                            Da[k1]= Da[k1]-1;
                }
            }
            
            for(i=0;i<sB;i++){
                k1 = *(B + i);
                for(j=0;j<sB;j++){
                    k2 = *(B+j);
                    for(i1 = row[k1];i1<row[k1+1];i1++)
                        if(col[i1]==k2)
                            Db[k1]= Db[k1]-1;

                }
            }
   

   ////#########################################################
   ///###################################################
    
        k=0;
        
//------------   Run the algirithm until A is fully labelled (A is shorter than B)  ---------------//
        while(k<sA){ 
        
      // Finding max gain pair--------------------------------------  
      
          for(i=0;i<sA;i++){
            if(*(lA+i)=='N'){
              k1 = *(A + i);
              for(j=0;j<sB;j++){
                if(*(lB+j)=='N'){
                  k2 = *(B+j);         
                  for(i1 = row[k1];i1<row[k1+1];i1++){
                    if(col[i1]==k2)
                      w = 1;
                    else
                      w=0;
                  }
                  G1 = Da[*(A+i)] + Db[*(B+j)] -2*w;
                  if(G1>Gm1){
                    Gm1= G1;            
                    ma = i;
                    mb = j;
                  }
                }
              }
            }
          }
    // End of Finding max gain pair ---------------------------------------

    // Storing cumulative gain ----------------------    
          if(k==0){
              *(FG +k) = Gm1;
          }
          else{
              *(FG +k) = *(FG+k-1) + Gm1;
          }       
   // End of storing cumulative gain-----------------
   
   // Storing the to-be swapped pairs ------------
          
          *(Fa +k) = ma;
          *(Fb +k) = mb;
          
   // Enf of Storing the to-be swapped pairs ------------

   //  Re-compute waits for neighbours of swapped pair ---------
            
            k1 = *(A+ma);
            for(i=row[k1];i<row[k1+1];i++){
                if(*(abG + col[i]) =='b')
                    Db[col[i]] = Db[col[i]]-2;
                if(*(abG + col[i]) =='a')
                    Da[col[i]] = Da[col[i]]+2;
            }
            
            k1 = *(B+mb);
            for(i=row[k1];i<row[k1+1];i++){
                if(*(abG + col[i]) =='b')
                    Db[col[i]] = Db[col[i]]+2;
                if(*(abG + col[i]) =='a')
                    Da[col[i]] = Da[col[i]]-2;
            }
    //  End of Re-compute waits for neighbours of swapped pair ---------
            
    // Label swapped pair-----------         
            *(lA + ma) = 'Y';
            *(lB + mb) = 'Y';
    // End of Label swapped pair-----------
             
            if(*(FG + k) > Gm2 ){
                Gm2 = *(FG + k);
                mk = k;
            }

            k=k+1;
            Gm1 = -10000;
        }
                    
            if(Gm2>0){
            for(i=0;i<=mk;i++){
                k1 = *(Fa+i);
                k2 = *(Fb + i);
                j = *(A + k1);
                *(abG + *(A+k1)) = 'b';
                *(abG + *(B+k2)) = 'a';
                *(A + k1) = *(B + k2);
                *(B + k2) = j; 
            }
            }
        
        
                if(t_old <= t_new )
                    Gm2 = -1;
        
        t_old = t_new;
        Gm3 = Gm2;
        mk2 = mk;
        mk = -1;
    }
        
    
    
//------------------------A and B has been genrated--------------------//

    w=0;
    for(i=0;i<sA;i++){
        k1 = *(A + i);
        for(j=0;j<sB;j++){
            k2 = *(B+j);
            for(i1 = row[k1];i1<row[k1+1];i1++){
                if(col[i1]==k2){
                    if(*(abG + k2) != 'V'){
                    *(abG + k2) = 'V';
                    w = w+1;
                    }
                }
            }
        }
    }
    printf("w: %d\n",w);

// -------------------------- NUMBER V here -------------------------

// -------------------- PUSH A TO SELF -----------------------
        r_set = NULL;
        r_set = malloc(sizeof(rowQ));
        r_set->row_get= A;
        r_set->sizeofrow = sA;
        r_set->startN = p_start;
        r_set->endN = p_start + sA-1;
        r_set->Qno = Qid;
        r_set->next = NULL;
        Qid = Qid+1;
        enQ(procQ,r_set);
        
// ---------------------REGENRATE B -------------------
        B2 = malloc(sizeof(int)*(sB-w));
        j=0;
        for(i=0;i<sB;i++){
            k2 = *(B+i);
                if(*(abG+k2)=='b'){
                    *(B2+j) = k2; 
                    j++;
                  }
        }
        sB = sB-w;
        free(B);
        B = B2;
        
    // PREPARE SEND BUFER IF NECESSARY -------------------------------------------------
 }


///-----------------------------------COMMUNICATION LAYER----------------------------------------
    s = send_ps[sps];
    r = rec_ps[sps];

    if(r==p_rank || r==-1)
        r = MPI_PROC_NULL;
    if(s==p_rank || s==-1)
        s = MPI_PROC_NULL;

        
    if(s != MPI_PROC_NULL){
        sendbuff = malloc(sizeof(int)*(sB + 3));
        *ps = sB+3;
        *(sendbuff) = sB;
        *(sendbuff + 1) = p_start + sA;
        *(sendbuff + 2) = p_end -w;
        
        for(i=0;i<sB;i++){
            *(sendbuff+3+i) = *(B+i);        
        } 
        free(B);
        B= NULL;
    }
    
    else{
        sendbuff = malloc(sizeof(int));
               if(exec_ps[sps] ==1){        
        
        r_set = NULL;
        r_set = malloc(sizeof(rowQ));
        r_set->row_get= B;
        r_set->sizeofrow = sB;
        r_set->startN = p_start + sA;
        r_set->endN = p_end -w;
        r_set->Qno = Qid;
        r_set->next = NULL;
        Qid = Qid+1;

        enQ(procQ,r_set);
        
            A = NULL;
            B = NULL;
            
            free(rowG);    
            rowG = NULL;
            free(lA);
            lA= NULL;
            free(lB);
            lB = NULL;
            free(Fa);
            Fa = NULL;
            free(Fb);
            Fb = NULL;
            free(FG);
            FG = NULL;
        
        }
       
       
        }
        
    
    if((s != MPI_PROC_NULL) || (r != MPI_PROC_NULL)){
        MPI_Sendrecv(ps,1,MPI_INT,s,sps,pr,1,MPI_INT,r,sps,comm,&stat ); 
        if(r != MPI_PROC_NULL){    
        recvbuff=NULL;
        recvbuff = malloc(sizeof(int)*(*pr));
        }
    //    printf("start of 1 comm %d %d\n",(*ps),p_rank);
        MPI_Sendrecv(sendbuff,*ps,MPI_INT,s,sps,recvbuff,*pr,MPI_INT,r,sps,comm,&stat );   
  //      printf("End of 1 comm %d %d\n",(*pr),p_rank);

    }
 //   MPI_Barrier(comm);
            // UNPACK BUFFER IF NECESSARY -------------------------
    if(r != MPI_PROC_NULL){
        sB = *(recvbuff);           
        B = malloc(sizeof(int)*(sB));
        
        for(i=0;i<sB;i++){
         *(B+i) = *(recvbuff+3+i);
        } 
        r_set = NULL;
        r_set = malloc(sizeof(rowQ));
        r_set->row_get= B;
        r_set->sizeofrow = sB;
        r_set->startN = *(recvbuff+1);
        r_set->endN = *(recvbuff+2);
        r_set->Qno = Qid;
        r_set->next = NULL;
        Qid = Qid+1;

        enQ(procQ,r_set);
        B = NULL;
    }

    if(s != MPI_PROC_NULL)
        free(sendbuff);
    if(r != MPI_PROC_NULL)
        free(recvbuff); 
  }

MPI_Finalize();
return 0;
}



                                ////////////////////////////////////
                              ////////   Queue Implementation   /////
                               /////////////////////////////////////
               
computeQ* createQ(){
    computeQ* new = malloc(sizeof(computeQ));
    new->top = NULL;
    new->bottom = NULL;
    return new;    
}

rowQ* deQ(computeQ* theq){
    rowQ* sendrow = theq->top;
    if(sendrow == NULL)
        return sendrow;
    if(sendrow->Qno == theq->bottom->Qno){        
        theq->bottom = NULL;
        }
    theq->top = sendrow->next;
    return sendrow;
}

int enQ(computeQ* theq,rowQ* recvrow){
    if(theq->top == NULL){
        theq->top = recvrow;
        theq->bottom = recvrow;
        return 0;
    }
    else{
    theq->bottom->next = recvrow;
    theq->bottom = recvrow;
    return 0;
    }
}



