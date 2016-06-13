// raviteja meesala ## 06-02-01-10-51-14-1-11051
#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>

int main(int argc, char** argv){

 int rank, size, len;
 char procName[100];
 int rank2;

 int siz_m = atoi(argv[1]);
 int pros = atoi(argv[2]);
 int iter = 0;

	double t1=0,t2= 0;
 //printf("The size of the problem is: %d \n",siz_m);
 
 MPI_Comm comm = MPI_COMM_WORLD;
 MPI_Init(&argc, &argv);

	t1 = MPI_Wtime();

 MPI_Status stat;

// 
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  MPI_Barrier(comm);
 
 	 printf("Hello from Process %d of %d\n", rank, size);

	/////////////////////////////////////////////
	////////Variables for creating cartesian coords/////////////////////////
	////////////////////////////////////////////////
	int ndims = 2;
	int dims[ndims];
	int period[ndims];
	int reorder = 0;
	int coords[2];
	int i,j=0;

	int vert = 0, hor = 1;
	int lef = -1,rit = 1;
	int bot = 1, t_op = -1;

	////////////////////////////////
	//// Junk variables	///////////////
	//////////////////////////////////
	
	int chk[2];
	int ch=0;
	int r_s,r_d;


        dims[0] = 4;
        dims[1] = pros/dims[0];

	if(pros == 1){
		dims[0] = 1;
		dims[1] = 1;
}


	///////////////////////////////////////////
	//////	COMPUTAITONAL VARIABLES		/////////
	////////////////////////////////////////////
	
	int prob1 = siz_m/dims[0];
	int prob2 = siz_m/dims[1];

	double* mat_old = (double*)malloc(sizeof(double)*prob1*prob2);
	double* mat_new = (double*)malloc(sizeof(double)*prob1*prob2);

	double* left_send = ( double*)malloc(sizeof(double)*prob1);
	double* left_rec = ( double*)malloc(sizeof(double)*prob1);

	double* right_send = (double*)malloc(sizeof(double)*prob1);
	double* right_rec = (double*)malloc(sizeof(double)*prob1);

	double* top_send = (double*)malloc(sizeof(double)*prob2);
	double* top_rec = (double*)malloc(sizeof(double)*prob2);

	double* bottom_send = (double*)malloc(sizeof(double)*prob2);
	double* bottom_rec = ( double*)malloc(sizeof(double)*prob2);

	double l1,r1,u1,b1, c1;

	//////////////////////////////////////////////
	///	Cartesian Preliminaries		//////////
	///////////////////////////////////////////////
	
//	dims[0] = 4;
//	dims[1] = pros/dims[0];


	period[0] = 0;
	period[1] = 0;

	MPI_Comm new;

    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, period, reorder, &new);
    MPI_Cart_coords(new, rank, 2,coords);
 

     printf(" Ranks is %d and the coordinates are %d and %d \n", rank, coords[0],coords[1]);

    // MPI_Cart_rank(new,chk,&ch);
  //   printf("Process %d says (%d,%d) is occupied by %d",rank,chk[0],chk[1],ch);

	//////////////////////////////////////#######///////////////////////////////
	/////////////////////////	INITIALIZE	/////////////////////////////
	////////////////////////////////////#############//////////////////////////
//
	for(i = 0;i<prob1;i++){
	for(j=0;j<prob2;j++){
               *((mat_old + prob2*i) + j ) = 2.923423;
               // *((mat_new + prob2*i) + j ) = 2.90;
               }
        } 
 

			/////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////             ///////////////////////////////////////////
	///////////////////////		START OF JACOBI ITERATION PROGRAM 	/////////////////////////////////////////
		//////////////////////////////////////////              /////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////



for(iter = 0; iter < 1000; iter++){


	/////////////////////////////////////////////////
       //////	INITIALIZE THE SENDING GHOST ROWS 	////
	////////////////////////////////////////////////
 	for(i = 0;i< prob1;i++) {
   		*(left_send+ i) = *((mat_old + i*prob2));
		*(right_send + i) = *((mat_old + i*prob2 + prob1 -1));
	}
	
	for(i = 0;i<prob2;i++){
		*(top_send + i) = *(mat_old + i);
		*(bottom_send + i) = *(mat_old + (prob1*prob2) + i - prob2);
}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////		SEND RECIEVE OPERTAION IN 4 BATCHES 	////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ///
// OPERATION : SENDING TO TOP, RECIEVING FROM BOTTOM 
// /////////////////
// arrays used: top_send, bottom_rec, prob2
// 
	MPI_Cart_shift(new,vert,t_op,&r_s,&r_d);

	MPI_Sendrecv(top_send, prob2, MPI_DOUBLE, r_d,0, bottom_rec, prob2, MPI_DOUBLE, r_s, 0, new, &stat );

// ///
// // OPERATION : SENDING TO BOTTOM, RECIEVING FROM TOP 
// // /////////////////
// // arrays used: top_rec, bottom_send, prob2
// // 
         MPI_Cart_shift(new,vert,bot,&r_s,&r_d);

         MPI_Sendrecv(bottom_send, prob2, MPI_DOUBLE, r_d,0, top_rec, prob2, MPI_DOUBLE, r_s, 0, new, &stat );
//
//

// ///
// // OPERATION : SENDING TO LEFT, RECIEVING FROM RIGHT [0,0 IS LEFT TO 0,1 AND 0,2 IS RIGHT TO 0,1] 
// // /////////////////
// // arrays used: left_send, right_rec,prob1
// 
         MPI_Cart_shift(new,hor,lef,&r_s,&r_d);

         MPI_Sendrecv(left_send, prob1, MPI_DOUBLE, r_d,0, right_rec, prob1, MPI_DOUBLE, r_s, 0, new, &stat );
//
//

// ///
// // // OPERATION : SENDING TO RIGHT, RECIEVING FROM LEFT [0,0 IS LEFT TO 0,1 AND 0,2 IS RIGHT TO 0,1] 
// // // /////////////////
// // // arrays used: left_rec, right_send, prob1, 
// // 
          MPI_Cart_shift(new,hor,rit,&r_s,&r_d);
//                     
          MPI_Sendrecv(right_send, prob1, MPI_DOUBLE, r_d,0, left_rec, prob1, MPI_DOUBLE, r_s, 0, new, &stat );
//                              
//

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     //////////////////////////		END OF SEND RECIEVE OPERTAION IN 4 BATCHES	//////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

		///////////////////////////////////////////////////////////////////////
			////####	CAlculation  COMPUTATION STARTS 	#///////////
		//////////////////////////////////////////////////////////////////////


 
		/////////##########################################################################33
	///////////////////////		GHOST ROWS ADDITONS 		//////////////////////////////////////////
		////////////########################################################################


/////+++++++++++++++++++++++++++++++++++
	if(coords[1] != 0 ){

///////////////
	if(coords[0] != 0){

	*(mat_new) = (*(mat_old) + *(left_rec) + *(top_rec) + *(mat_old + 1) + *(mat_old + prob2))/5 ;
}

///////////////
        if(coords[0] != dims[0]){

        *(mat_new + prob2*(prob1-1)) = (*(mat_old + prob2*(prob1-1)) + *(left_rec + prob1 -1) + *(bottom_rec ) + *(mat_old + prob2*(prob1-1) +1) + *(mat_old + prob2*(prob1-2)))/5 ;
}
////////////////

	for(i = 1;i<prob1-1; i++){
	*(mat_new + prob2*i) = (*(mat_old + (prob2)*(i-1)) + *(mat_old + prob2*(i+1)) + *(mat_old + prob2*i + 1) + *(mat_old + prob2*i) + *(left_rec + i))/5 ;
	}
}


					////////////////+++++++++++++++++++
					/////////////////////////
						////////////////////////////
  if(coords[1] != dims[1] ){
/////
        if(coords[0] != 0){

        *(mat_new + prob2 - 1 ) = (*(mat_old + prob2 - 1) + *(right_rec) + *(top_rec + prob2 -1) + *(mat_old + prob2 - 2) + *(mat_old + 2*prob2 -1))/5 ;
}

        if(coords[0] != dims[0]){

        *(mat_new + prob2*(prob1) -1) = (*(mat_old + prob2*(prob1) -1) + *(right_rec + prob1 -1) + *(bottom_rec + prob2 -1) + *(mat_old + prob2*(prob1) -2) + *(mat_old + prob2*(prob1-1)-1 ))/5 ;
}


///////
        for(i = 1;i<prob1-1 ;i++){
        *(mat_new + prob2*i + prob1-1) = (*(mat_old + (prob2)*(i-1)+ prob1-1) + *(mat_old + prob2*(i+1)+prob1-1) + *(mat_old + prob2*i - 1 + prob1-1) + *(mat_old + prob2*i +  prob1-1) + *(right_rec + i))/5 ;
        }
}

                   	//////////////////////////////////////////////////////////////////////////////

	if(coords[0] != 0){
        for(i = 1; i <prob2-1 ; i++){
        *(mat_new + i) = (*(mat_old + (i-1)) + *(mat_old +(i+1)) + *(mat_old + i ) + *(mat_old + i + prob2) + *(top_rec + i)) /5;
        }
}

	
		//////////////////////////////////////////////////////////////////////////////////////////////
	

        if(coords[0] != dims[0]){
        for(i = 1; i < prob2-1 ; i++){
        *(mat_new + i + prob2*(prob1-1)) = (*(mat_old + (i-1) + prob2*(prob1-1)) + *(mat_old +(i+1) +  prob2*(prob1-1)) + *(mat_old + i +  prob2*(prob1-1)) + *(mat_old + i + prob2*(prob1-2) ) + *(bottom_rec + i)) /5;
        }
}

		/////////////////////################################////////////////////////
	////////////////////// 		GHOST ROW - COLUMNS END       ////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
//



		/////////////////////////////////################////////////////////////////////////
			//////////////		 Jacobi operations ON REST , l1,r1,b1,u1,c1	/////////////
		///////////////////////########################/////////////////////////////////////


	for(i =1;i < prob1-1;i++ ){
	   
	    for(j = 1;j<prob2-1;j++){
  		 
		l1 = (*((mat_old + prob2*i) + (j-1)));
                r1 = (*((mat_old + (i)*prob2)+(j+1)));
		c1 = (*((mat_old + (i)*prob2)+(j)));
		u1 = (*((mat_old + (i-1)*prob2)+(j)));
		b1 = (*((mat_old + (i+1)*prob2)+(j)));
  
		*((mat_new + i*prob2) + j) = (l1+ r1 + b1 + u1 + c1)/5;

		}
	}  

			///////////////////////////////////////////////////////////////
			////#########	COPY BACK TO OLD MATRIX		############
			//////////////////////////////////////////////////////////////


     for(i=0;i<prob1; i++){
	for(j=0;j<prob2; j++){
	*(mat_old + i*prob2 + j) = *(mat_new + i*prob2 + j) ;
 }
}

 printf("iteration number: %d reporting by: %d \n",iter,rank);
 
////////////////////////////////

MPI_Barrier(new);
}

           /////////////////////////////////////////////////////////////////////////////////////                       //               
           		//////////////////////////////////////             //////////////////////
           	////////////////////////////        END OF JACOBI ITERATION PROGRAM       ///////////////////
         //////////////////////////////////////////              /////////////////////////////////////////////////
             ////////////////////////////////////////////////////////////////////////////////////////



t2 = MPI_Wtime();
t2 = t2 - t1;

	printf(" Proc:%d  Size: %d time: %lf \n", pros, siz_m, t2);

	MPI_Finalize();
}
