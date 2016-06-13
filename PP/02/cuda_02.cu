// Meesala Raviteja
// 06-02-01-10-51-14-1-11051
#include <stdio.h>
#include <assert.h>

#define N_t 64
#define N_b 100
#define rows 6400
#define columns 6400

 __global__ void mult_mat_vectdiag(float *dev_A,float *dev_b, float *dev_c, int *row_k, int *col_k, int *N_bls,int *N_tds){

		int bid = blockIdx.x;
		int tid = threadIdx.x;
        int i=0;
        int c_k = bid*(*N_tds) + tid;
        
        for(i=0;i< *(row_k);i++)
            *(dev_c + c_k) = *(dev_c + c_k) +  *(dev_A + i*(*row_k) + c_k)*(*(dev_b+(c_k+i)%rows));
}

 __global__ void mult_mat_vectrow(float *dev_A,float *dev_b, float *dev_c, int *row_k, int *col_k, int *N_bls,int *N_tds){

		int bid = blockIdx.x;
		int tid = threadIdx.x;
        int i=0;
        int c_k = bid*(*N_tds) + tid;
        
        for(i=0;i< *(row_k);i++)
            *(dev_c + i) = *(dev_c + i) +  *(dev_A + i*(*row_k) + c_k)*(*(dev_b+c_k));
}

 __global__ void mult_mat_vectcol(float *dev_A,float *dev_b, float *dev_c, int *row_k, int *col_k, int *N_bls,int *N_tds){

		int bid = blockIdx.x;
		int tid = threadIdx.x;
        int i=0;
        int c_k = bid*(*N_tds) + tid;
        
        for(i=0;i< *(row_k);i++)
            *(dev_c + i) = *(dev_c + i) +  *(dev_A + i+ c_k*(*row_k) )*(*(dev_b+c_k));
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

    float *A, *b, *c;
    int i,j; 
    int N_ts, N_bs;
    int r_s=rows;
    int c_s = columns;
    N_ts = N_t;
	N_bs = N_b;

			//////////////////////////////////////////////////////////////////////////////////
				////////////////	DEVICE VARIABLES 	//////////////////
			//////////////////////////////////////////////////////////////////////////////////

   	float *dev_A, *dev_b,*dev_c;
    int *dev_rs,*dev_cs;
	int *N_tds,*N_bls; // No. of threads and blocks deployed
	    
					//////////////////////////////////////////////////////
			/////////////////////////// HOST VARIABLE ALLOCATIONS	//////////////////////////
					//////////////////////////////////////////////////

	A = (float *)malloc(sizeof(float)*rows*columns);
	b = (float *)malloc(sizeof(float)*columns);
	c = (float *)malloc(sizeof(float)*rows);
	
					///////////////////////////////////////////////////////
		///////////////////////////	HOST VARIABLE ASSIGNMENT 	////////////////////////////////
					//////////////////////////////////////////////////////

        for(i=0;i<rows;i++){
            c[i] = 0.0;   
            for(j=0;j<columns;j++)
	            *(A + i*rows+j) = 0.00232;
	}
	    for(j=0;j<columns;j++)
	        *(b +j) = 1.21;
	        
					/////////////////////////////////////////////////////	
			///////////////////////////	DEVICE ALLOCATIONS 	//////////////////////////
					//////////////////////////////////////////////////

	cudaMalloc((void**)&dev_A, sizeof(float)*rows*columns );
	cudaMalloc((void**)&dev_b, sizeof(float)*columns);
	cudaMalloc((void**)&dev_c, sizeof(float)*rows);
    cudaMalloc((void**)&N_tds, sizeof(int));
    cudaMalloc((void**)&N_bls, sizeof(int));
    cudaMalloc((void**)&dev_cs, sizeof(int));
    cudaMalloc((void**)&dev_rs, sizeof(int));

					/////////////////////////////////////////////////////
			/////////////////////	 COPY FROM HOST TO DEVICE   ///////////////////////////
					////////////////////////////////////////////////////////

	cudaMemcpy(dev_A, A, sizeof(float)*rows*columns , cudaMemcpyHostToDevice);
	cudaMemcpy(dev_b, b, sizeof(float)*columns, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_c, c, sizeof(float)*rows, cudaMemcpyHostToDevice);
	cudaMemcpy(N_tds, &N_ts, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(N_bls, &N_bs, sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_cs, &r_s, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_rs, &c_s, sizeof(int), cudaMemcpyHostToDevice);
	cudaThreadSynchronize();

    
	
				//////////////////////////////////////////////////////////////
			//////////////////#########	CALL KERNEL non coalesced##########///////////////////////////
				/////////////////////////////////////////////////////////////

    clock_gettime(CLOCK_MONOTONIC,&start1);
	mult_mat_vectdiag<<<N_b,N_t>>>( dev_A,dev_b,dev_c, dev_rs,dev_cs,N_bls, N_tds);
	cudaThreadSynchronize();
	clock_gettime(CLOCK_MONOTONIC, &end1);
    time_taken = (double)((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);
	printf("Non sequential timediag1: %lf \n", time_taken);
    clock_gettime(CLOCK_MONOTONIC,&start1);
    
    cudaMemcpy(c, dev_c, sizeof(float)*rows, cudaMemcpyDeviceToHost);    
    cudaThreadSynchronize();
    
	mult_mat_vectcol<<<N_b,N_t>>>( dev_A,dev_b,dev_c, dev_rs,dev_cs,N_bls, N_tds);
	cudaThreadSynchronize();    
	clock_gettime(CLOCK_MONOTONIC, &end1);
    time_taken = (double)((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);
	printf("Non sequential timecol1: %lf \n", time_taken);
    clock_gettime(CLOCK_MONOTONIC,&start1);
    
	mult_mat_vectrow<<<N_b,N_t>>>( dev_A,dev_b,dev_c, dev_rs,dev_cs,N_bls, N_tds);
    cudaThreadSynchronize();    
    clock_gettime(CLOCK_MONOTONIC, &end1);
    time_taken = (double)((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);
	printf("Non sequential timerow1: %lf \n", time_taken);
    clock_gettime(CLOCK_MONOTONIC,&start1);
       
	mult_mat_vectdiag<<<N_b,N_t>>>( dev_A,dev_b,dev_c, dev_rs,dev_cs,N_bls, N_tds);
	cudaThreadSynchronize();
	clock_gettime(CLOCK_MONOTONIC, &end1);
    time_taken = (double)((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);
	printf("Non sequential timediag2: %lf \n", time_taken);
    clock_gettime(CLOCK_MONOTONIC,&start1);

  	mult_mat_vectcol<<<N_b,N_t>>>( dev_A,dev_b,dev_c, dev_rs,dev_cs,N_bls, N_tds);
	cudaThreadSynchronize();
  	clock_gettime(CLOCK_MONOTONIC, &end1);
    time_taken = (double)((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);
	printf("Non sequential timecol2: %lf \n", time_taken);
    clock_gettime(CLOCK_MONOTONIC,&start1);
    
	mult_mat_vectrow<<<N_b,N_t>>>( dev_A,dev_b,dev_c, dev_rs,dev_cs,N_bls, N_tds);
	cudaThreadSynchronize();
   	clock_gettime(CLOCK_MONOTONIC, &end1);
    time_taken = (double)((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);
	printf("Non sequential timerow2: %lf \n", time_taken);
    clock_gettime(CLOCK_MONOTONIC,&start1);
    
	mult_mat_vectdiag<<<N_b,N_t>>>( dev_A,dev_b,dev_c, dev_rs,dev_cs,N_bls, N_tds);
	cudaThreadSynchronize();
	clock_gettime(CLOCK_MONOTONIC, &end1);    
    time_taken = (double)((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);
	printf("Non sequential timediag3: %lf \n", time_taken);
    clock_gettime(CLOCK_MONOTONIC,&start1);
    
	mult_mat_vectcol<<<N_b,N_t>>>( dev_A,dev_b,dev_c, dev_rs,dev_cs,N_bls, N_tds);
	cudaThreadSynchronize();
	clock_gettime(CLOCK_MONOTONIC, &end1);
    time_taken = (double)((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);
	printf("Non sequential timecol3: %lf \n", time_taken);
    clock_gettime(CLOCK_MONOTONIC,&start1);
    
	mult_mat_vectrow<<<N_b,N_t>>>( dev_A,dev_b,dev_c, dev_rs,dev_cs,N_bls, N_tds);
	cudaThreadSynchronize();
	clock_gettime(CLOCK_MONOTONIC, &end1);
    time_taken = (double)((end1.tv_sec-start1.tv_sec)*1e9 + end1.tv_nsec - start1.tv_nsec);
	printf("Non sequential timerow3: %lf \n", time_taken);
    clock_gettime(CLOCK_MONOTONIC,&start1);

				/////////////////////////////////////////////////////////////
			/////////////////////		COPY RESULTS 	////////////////////////////////////////////
				//////////////////////////////////////////////////////////

    
	cudaFree(dev_A);
	cudaFree(dev_b);
	cudaFree(dev_c);
    
    for(i=0;i<columns;i+=2000)
    printf("%d %lf \n",i+1,c[i]);
 
    return 0;

}
