// raviteja meesala ## 06-02-01-10-51-14-1-11051
#include <stdio.h>
#include <assert.h>

#define N_t 1000
#define N_b 20
#define dimension 10000000
#define entries 30000000

 __global__ void mult_mat_vect(int *dev_row,int *dev_col,float *dev_ents,float *dev_vect,float *dev_res, int *dimer, int *N_tds, int *N_bls){

		int bid = blockIdx.x;
		int tid = threadIdx.x;

	int indx = bid*(*(N_tds)) + tid ; // Starts from zero and goes till as many threads deployed
	int dims = dimer[0]; // get the dimensions
	int i =0;
		if(indx < dims)
			dev_res[indx]= dev_vect[dev_col[dev_row[indx]]];

				while(indx < dims){
					dev_res[indx] = 0.0;
				for(i = dev_row[indx]; i< dev_row[indx+1];i++){
                                       dev_res[indx] = dev_res[indx] + dev_ents[i]*(dev_vect[dev_col[i]]);
				}

			indx = indx + (*(N_tds))*(*(N_bls)); // JUMP TO NEXT AVAILABLE ROW TO PICK;

			}	

}



				///////////////////////////////////////////////////////////////
					////////////	 Program main   ////////////
				//////////////////////////////////////////////////////////////


int main( int argc, char** argv)
{
	

					//////////////////////////////////////////
			////////////////////    HOST VARIABLES		 //////////////////////////////////
				/////////////////////////////////////////////////

	struct timespec start1, end1;
	double time_taken;

  int *row, *col;
  float *ents;
  float *vect;
  float *res;
	float* rest;
  int N_ts, N_bs;

	N_ts = N_t;
	N_bs = N_b; 

	int iter =0;

  int entry;
  int dim; 

  entry = entries;
  dim = dimension;


			//////////////////////////////////////////////////////////////////////////////////
				////////////////	DEVICE VARIABLES 	//////////////////
			//////////////////////////////////////////////////////////////////////////////////


   	int *dev_row, *dev_col;
	float *dev_ents;
	float *dev_vect;
	float *dev_res;
	int *N_tds,*N_bls; // No. of threads and blocks deployed
	int *dimer;
	int tp =0;
	int r_v = dim*(sizeof(int));
	int e_v = entry*(sizeof(int));

	int rd_v = dim*(sizeof(float));
	int ed_v = entry*(sizeof(float));
    // allocate device copies of a, b, c
    
					//////////////////////////////////////////////////////
			/////////////////////////// HOST VARIABLE ALLOCATIONS	//////////////////////////
					//////////////////////////////////////////////////

	row = (int *)malloc(r_v + sizeof(int));
	col = (int *)malloc(e_v);
	ents = (float *)malloc(ed_v);
	vect = (float *)malloc(rd_v);
	res = (float *)malloc(rd_v);
	rest= (float*)malloc(rd_v);

					///////////////////////////////////////////////////////
		///////////////////////////	HOST VARIABLE ASSIGNMENT 	////////////////////////////////
					//////////////////////////////////////////////////////


	row[0] = 0;

		for( tp = 1; tp<=dimension; tp++){
			row[tp] = row[tp-1] + 3;
	}

	col[0] = 0;
	col[1] = 1;
	col[2] = 2;

	ents[0] = 1;
	ents[1] = 2;
	ents[2] = 1;
	iter = 3;
	
		for( tp = 1; tp< dimension-1; tp++){

			col[tp*3] =tp -1;
			col[tp*3 + 1] = tp;  
			col[tp*3 + 2] = tp+1;
			ents[tp*3] = 1;
			ents[tp*3 + 1] = 2;
			ents[tp*3 + 2] = 1;
		//	iter = tp
		}

	
		col[tp*3] = tp-2;
		col[tp*3 + 1] = tp-1;
		col[tp*3 +2] = tp;
		ents[tp*3] = 1;
		ents[tp*3 +1] = 2;
		ents[tp*3 +2] = 1;

//				printf("%d heybro! %d", tp*3 +3, tp*3 +3);
	
		for( tp = 0; tp<dimension; tp++){
			vect[tp] = tp/1000;
			}


/////////////////////////////////////////////////////					///////////////////////


//	for(tp = 0; tp<=dimension; tp=tp+(entries/100))	
//		printf("row: %d, vect: %f \n", row[tp],vect[tp]);

//	for(tp=0; tp<entries;tp= tp+(entries/100))
//		printf("col:%d val:%f \n", col[tp],ents[tp]);


//////////////////////////////////////////	//////////////////////////////////////	/////////////////////////////////////
					/////////////////////////////////////////////////////	
			///////////////////////////	DEVICE ALLOCATIONS 	//////////////////////////
					//////////////////////////////////////////////////


	cudaMalloc((void**)&dev_row,r_v + sizeof(int) );
	cudaMalloc((void**)&dev_col, e_v);
	cudaMalloc((void**)&dev_ents,ed_v);
	cudaMalloc((void**)&dev_vect, rd_v);
	cudaMalloc((void**)&dev_res, rd_v);

	cudaMalloc((void**)&N_tds, sizeof(int));	
	cudaMalloc((void**)&N_bls, sizeof(int));
	cudaMalloc((void**)&dimer, sizeof(int));

cudaThreadSynchronize();
					/////////////////////////////////////////////////////
			/////////////////////	 COPY FROM HOST TO DEVICE   ///////////////////////////
					////////////////////////////////////////////////////////

	cudaMemcpy(dev_row, row, r_v + sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_col, col, e_v, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_ents, ents, ed_v, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_vect, vect, rd_v, cudaMemcpyHostToDevice);
	
	cudaMemcpy(N_tds,&N_ts, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(N_bls,&N_bs, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dimer,&dim, sizeof(int), cudaMemcpyHostToDevice);

cudaThreadSynchronize();

clock_gettime(CLOCK_MONOTONIC,&start1);
				//////////////////////////////////////////////////////////////
			//////////////////#########	CALL KERNEL 	##########///////////////////////////
				/////////////////////////////////////////////////////////////


	mult_mat_vect<<<N_b,N_t>>>( dev_row, dev_col, dev_ents, dev_vect, dev_res,dimer, N_tds, N_bls);

	cudaThreadSynchronize();

clock_gettime(CLOCK_MONOTONIC, &end1);

time_taken = (double)((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);

	printf("Non sequential time: %lf \n", time_taken);

				/////////////////////////////////////////////////////////////
			/////////////////////		COPY RESULTS 	////////////////////////////////////////////
				//////////////////////////////////////////////////////////

	cudaMemcpy(res, dev_res, rd_v, cudaMemcpyDeviceToHost);

cudaThreadSynchronize();

//	for(tp =0; tp<dimension;tp+= (dimension/100))
//	printf("value: %d, %f \n",tp,res[tp]);


	cudaFree(dev_row);
	cudaFree(dev_col);
	cudaFree(dev_ents);
	cudaFree(dev_vect);
	cudaFree(dev_res);

clock_gettime(CLOCK_MONOTONIC,&start1);

				////////////////////////////////////////////////////////////////////////////	
		//////////////////////////////		SEQUENTIAL 	CODE		////////////////////////////////////	
				///////////////////////////////////////////////////////////////////////////

	for(tp = 0; tp<dimension;tp++){
			rest[tp] = 0;
		for(iter = row[tp]; iter<row[tp+1]; iter++){
			rest[tp] = rest[tp] + (ents[iter]*(vect[col[iter]]));					
			

		}	
	}


clock_gettime(CLOCK_MONOTONIC, &end1);

time_taken = (double)((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);

        printf("Sequential time: %lf \n", time_taken);
//        for(tp =0; tp<dimension;tp+= (dimension/100))
  //      printf("value difference: %d, %f \n",tp,rest[tp] - res[tp]);



    return 0;
}
