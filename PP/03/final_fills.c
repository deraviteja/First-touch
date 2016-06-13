#include <stdio.h>
#include <stdlib.h>

typedef struct lis{
    int id;
    struct lis* next;
} list;

typedef struct n_o_d_e{
    int no_id;
    int neighbcount;
    list* neighbours;
} node;

int assign_Node_id(int n_id,node* new);
int addedge(node* A, node* B,node** all_N);
int deleteedge(node*A,node*B,node** all_N);
int deletenode(node* A,node** all_N);
int checkedge(node*A,node*B,node** all_N);
int printneighbours(node* A);
                            ///////////////////////////////////
            ////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////
        
int main(){

    int to_si = 4000;
    int i =0,j=0,rs=0,cs=0;
    int *rowG,*col;
    FILE* fp;
    int p1,p2=0;
    int at_min,min,wr;
    node** aln;
    list* get;
    int fill;
    aln = (node**)(malloc(sizeof(node*)*to_si));
    
    for(i=0;i<to_si;i++)
        assign_Node_id(i,(node*)aln + i);
    

                        fp = fopen("rowfrommo.dat","r");
                        rowG = malloc(sizeof(int)*4000);
                        rs=1;
                        *rowG = 0;
                         while(fgetc(fp) != EOF ){            
                                fscanf(fp,"%d",rowG + rs);
                                rs = rs+1;
                                fgetc(fp);
                            }
                        fclose(fp);
                        
                        fp = fopen("colfrommo.dat","r");
                        cs = 0;
                        col = malloc(sizeof(int)*15560);
                        
                         while(fgetc(fp) != EOF ){            
                                fscanf(fp,"%d",col + cs);
                                *(col +cs) = *(col +cs)-1;
                                cs = cs+1;            
                                fgetc(fp);
                            }
                        fclose(fp);
        rs--;
                        
        for(i=0;i<to_si;i++)
            for(j = rowG[i];j<rowG[i+1];j++)
                addedge((node*)aln + i,(node*)aln+ col[j],aln);

                    fill=0;
        for(p1=0;p1<to_si;p1++){

            for(p2=0;p2<to_si;p2++){
                get = ((node*)aln+p2)->neighbours;
                while(get!=NULL){
                fill = fill- ((node*)aln+ get->id)->neighbcount;
                get=get->next;
                }
                get = ((node*)aln+p2)->neighbours;
                deletenode((node*)aln+p2,aln);
                while(get!=NULL){
                fill = fill+ ((node*)aln+ get->id)->neighbcount;
                get=get->next;
                }
              }
        }
        printf("The fills are %d\n",fill);
   
return 0;
}

                    ///////////////////////////////////////////////////////////////
                        ///////////////////////
                    ///////////////////////////////////////////////////////////////

int addedge(node* A, node* B,node** all_N){
list* n1 = NULL;
list* n2 = NULL;

    if((checkedge(A,B,all_N)==0)&& A!= NULL && B != NULL){
        A->neighbcount = A->neighbcount+1;
        B->neighbcount = B->neighbcount+1;
        n1 = (list*)malloc(sizeof(list));
        n2 = (list*)malloc(sizeof(list));

        n2->id = A->no_id;
        //n1->next = NULL;    
        n2->next = B->neighbours;
        B->neighbours = n2;
        
        n1->id = B->no_id;
        //n1->next = NULL;
        n1->next = A->neighbours;
        A->neighbours = n1;


    }
    return 0;
}

//////////////////////////////////////////////

int assign_Node_id(int n_id,node* new){
    if(new != NULL){
    new->no_id = n_id;
    new->neighbcount = 0;
    new->neighbours = NULL;
    }
    return 0;
}

//////////////////////////////////////////////

int deleteedge(node*A,node*B,node** all_N){
    
    int k,a1;
    int b1;
    node* AB;
    list* nebr_A;
    list* nebr_A2;
    if(checkedge(A,B,all_N)){

        A->neighbcount = A->neighbcount-1;
        B->neighbcount = B->neighbcount-1;
     //   printf("changein: %d to %d\n",A->no_id,A->neighbcount);
       // printf("changein: %d to %d\n",B->no_id,B->neighbcount);
        
        b1 = B->no_id;
        nebr_A = A->neighbours;
        nebr_A2 = A->neighbours;
        a1 = A->neighbours->id;
        if(a1==b1){
            A->neighbours = nebr_A->next;
            nebr_A = NULL;
            }
        while(nebr_A != NULL){
            k = nebr_A->id;
            AB = (node*)all_N + k;
            if(AB->no_id == b1){
            nebr_A2->next = nebr_A->next;
            
            free(nebr_A);
            nebr_A = NULL;
            }
            else{
            nebr_A2 = nebr_A;
            nebr_A = nebr_A->next;
            
            }
        }
        
        b1 = A->no_id;
        nebr_A = B->neighbours;
        nebr_A2 = B->neighbours;
        a1 = B->neighbours->id;
        
        if(a1==b1){
            B->neighbours = nebr_A->next;
            nebr_A = NULL;
        }
        
        while(nebr_A != NULL){
            k = nebr_A->id;
            AB = (node*)all_N + k;
            if(AB->no_id == b1){
            nebr_A2->next = nebr_A->next;
            free(nebr_A);
            nebr_A = NULL;
            }
            else{
            nebr_A2 = nebr_A;
            nebr_A = nebr_A->next;
            }
        }
    }
    return 0;
}

int deletenode(node* A,node** all_N){

// Add neighbour to neighbour edges
    node* A1;
    node* A2;
    list* nofA1;
    list* nofA2;
    
    if(A!=NULL){      
        nofA1 = A->neighbours;
        while(nofA1!= NULL){
         A1 = (node*)all_N + nofA1->id;
         nofA2 = nofA1->next;
            while(nofA2!= NULL){
            A2 = (node*)all_N + nofA2->id;
            addedge(A1,A2,all_N);
            nofA2 = nofA2->next;
            }
         nofA1 = nofA1->next;
        }
    }
// Delete neighbour to node edges

        if(A!=NULL){ 
            nofA1 = A->neighbours;
        while(nofA1 != NULL){
            A1 = (node*)all_N + nofA1->id;   
            deleteedge(A,A1,all_N);
            nofA1 = A->neighbours;
            }
        }

// deletenode
        free(A->neighbours);
        A->neighbcount = -1;
        return 0;

}

int checkedge(node*A,node*B,node** all_N){
        
    int k;
    int b1;
    node* AB=NULL;
        b1 = B->no_id;
        list* nebr_A = A->neighbours;
        while(nebr_A != NULL){
            k = nebr_A->id;
            AB = (node*)all_N + k;
            if(AB->no_id == b1){
            return 1;
            }
            nebr_A = nebr_A->next;
        }

return 0;
}
int printneighbours(node* A){
int i,cnt;
list* e1;
    
    if(A==NULL){
        printf("No node in the first place\n");
        return 0;
        }
    e1 = A->neighbours;
    if(A->neighbcount<=0){
        printf("No neighbours: %d\n",A->neighbcount);
        if(e1==NULL)
        return 0;
        }
    cnt = A->neighbcount;    

    //for(i=0;i<cnt;i++){
    i=0;
    printf("node: %d neighbours:",A->no_id);
    while(e1 != NULL){
    printf(" %d",e1->id);
    i++;
    e1 = e1->next;
    }
    printf("\n");
    return 0;
       
}
