#include <stdio.h>
#include <stdlib.h>
#include<cuda.h>

#define n 5

void main() {

	void MatrixMultiplication(float *, float *, float *);

	float A[n*n], B[n*n], C[n*n];

	for(int i = 0; i < (n*n) ; i++) {
        A[i] = 5;
        B[i] = 5;
        C[i] = 0;
    }

	MatrixMultiplication(A, B, C);

	for(int i = 0; i < n; i++){
		for(int i = 0; i < n; i++){
			printf("%f  ", C[i]);
		}
		printf("\n")
    }

}


__global__ void MatrixMul(float *pA, float *pB, float *pC) {

    int i = threadIdx.i;
    int j = threadIdx.j;

    float Cvalue = 0;

    for(int k = 0; k < n ; k++) {
        float Mdelement = Md[j*n + k];
        float Ndelement = Nd[k*n + j];
        Cvalue = Cvalue + (Mdelement*Ndelement);
    }

    pC[j*n + i] = Cvalue;
}

void MatrixMultiplication(float *A, float *B, float *C) {
    int size = n*n*sizeof(float);
    float *pA, *pB, *pC;

    cudaMalloc((void**)&pA, size);
    cudaMemcpy(pA,A,size,cudaMemcpyHostToDevice);
    cudaMalloc((void**)&pB, size);
    cudaMemcpy(pB,B,size,cudaMemcpyHostToDevice);

    cudaMalloc((void**)&pC,size);

    dim3 dimBlock(n,n);
    dim3 dimGrid(1,1);

    MatrixMul<<<dimGrid,dimBlock>>>(pA,pB,pC);

    cudaMemcpy(C,pC,size,cudaMemcpyDeviceToHost);

    cudaFree(pA);
    cudaFree(pB);
    cudaFree(pC);
}
