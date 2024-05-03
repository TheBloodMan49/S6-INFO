#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <iostream>
#include <time.h>
//
static void HandleError(    cudaError_t err,
							const char *file,
							int line )
{
	if (err != cudaSuccess)
	{
		printf( "%s in %s at line %d\n", cudaGetErrorString( err ),
		file, line );
		exit( EXIT_FAILURE );
	}
}
#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))
//
#define N 10000000U
#define epsilon 1E-6f
//
void vector_add_c(  float*  pOut,
					float*  pIn_1,
					float*  pIn_2,
					int     n)
{
	for(int i = 0; i < n; i++)
	{
		pOut[i] = pIn_1[i] + pIn_2[i];
	}
}
// Le mot clé '__global__' indique que cette méthode est appelée par le CPU ('host') et exécutée sur le GPU ('device').
__global__ void vector_add_basic_cuda(  float*  dOut,
										float*  dIn_1,
										float*  dIn_2,
										int     n)
{
	for(int i = 0; i < n; i++)
	{
		dOut[i] = dIn_1[i] + dIn_2[i];
	}
}

__global__ void vector_add_thread_cuda(	float*  dOut,
										float*  dIn_1,
										float*  dIn_2,
										int	 n)
{
  for (int i = threadIdx.x; i < n; i += blockDim.x)
  {
    dOut[i] = dIn_1[i] + dIn_2[i];
  }
}

__global__ void vector_add_grid_cuda(	float*  dOut,
	float*  dIn_1,
	float*  dIn_2,
	int	 n)
{
	  for (int i = blockIdx.x * blockDim.x + threadIdx.x; i < n; i += blockDim.x * gridDim.x)
  {
    dOut[i] = dIn_1[i] + dIn_2[i];
  }
}

//
int main()
{
  try{
	// Tableau CPU
	float *pOut     = NULL;
	float *pIn_1    = NULL;
	float *pIn_2    = NULL;
	
	// Tableau GPU : à ce stade, une adresse mémoire
	float *dOut     = NULL;
	float *dIn_1    = NULL;
	float *dIn_2    = NULL;

	//  Allocation de la mémoire
	pOut    = (float*)malloc(sizeof(float) * N);
	pIn_1   = (float*)malloc(sizeof(float) * N);
	pIn_2   = (float*)malloc(sizeof(float) * N);

	// Allocation de la mémoire sur le GPU
	// cudaMalloc(void **devPtr, size_t count) : Permet d'allouer 'count' bits sur le GPU.
	HANDLE_ERROR(cudaMalloc((void**)&dOut,  sizeof(float) * N));
	HANDLE_ERROR(cudaMalloc((void**)&dIn_1, sizeof(float) * N));
	HANDLE_ERROR(cudaMalloc((void**)&dIn_2, sizeof(float) * N));

	// Initialisation des tableaux.
	for(int i = 0; i < N; i++)
	{
		pIn_1[i] = 1.0f; 
		pIn_2[i] = 2.0f;
		// Les lignes suivants compileraient mais provoqueraient un crash à l'exécution :
		// dIn_1[i] = 1.0f; 
		// dIn_2[i] = 2.0f;
		// Il n'est en effet pas possible d'accéder à de la mémoire dite 'device', c'est à dire allouée
		// via 'cudaMalloc' directement depuis le CPU...
		// Il faut faire des copies !
	}

	// On va maintenant copier ces tableaux sur le GPU, plus précisément :
	// copier le contenu de 'pIn_1' dans 'dIn_1'
	// copier le contenu de 'pIn_2' dans 'dIn_2'
	// cudaMemcpy(void *dst, void *src, size_t count, cudaMemcpyKind kind) : copie les 'count' bits de 'src' vers 'dst'
	HANDLE_ERROR(cudaMemcpy(dIn_1, pIn_1, sizeof(float) * N, cudaMemcpyHostToDevice));
	// Si l'ordre de transfert des données n'est pas correct, une erreur est remontée :
	HANDLE_ERROR(cudaMemcpy(dIn_2, pIn_2, sizeof(float) * N, cudaMemcpyHostToDevice));

	// Addition des deux vecteurs.
	vector_add_c(pOut, pIn_1, pIn_2, N);

  // Time
  int start = clock();
  vector_add_thread_cuda<<<1, 256>>>(dOut, dIn_1, dIn_2, N);
  cudaDeviceSynchronize();
  int end = clock();
  std::cout << "Time : " << (end - start) / (double) CLOCKS_PER_SEC << std::endl;

	// La ligne suivante compile mais l'exécution crash.
	// En effet de la même façon qu'il n'est pas possible d'accéder directement à de la mémoire GPU ('device') depuis le CPU ('host'),
	// l'inverse n'est pas non plus possible.
	//vector_add_basic_cuda<<<1, 1>>>(dOut, pIn_1, pIn_2, N);

    // On copie cette fois-ci les données de 'dOut' sur le CPU dans 'pOut_fromGPU'.
    float *pOut_fromGPU     = NULL;
    pOut_fromGPU            = (float*)malloc(sizeof(float) * N);
	// Cette fois-ci, il faut transférer des données depuis le GPU ('device') vers le CPU ('host').
    HANDLE_ERROR(cudaMemcpy(pOut_fromGPU, dOut, sizeof(float) * N, cudaMemcpyDeviceToHost));
	// On affiche la première sortie du résultat :
	std::cout << "First value in CPU buffer : " << pOut[0] << std::endl;

	float first_value_on_gpu = pOut_fromGPU[0];
	std::cout << "First value in GPU buffer : " << first_value_on_gpu << std::endl;
	// Comment copier la première valeur contenu dans le buffer 'dOut' dans un float ?

	// Vérification que le résultat est correct sur le CPU.
	int i;
	for(i = 0; i < N; i++)
	{
		if(std::abs(pOut[i] - pIn_1[i] - pIn_2[i]) > epsilon)
		{
			printf("Wrong sum computation on CPU!\n");
			break;
		}
		if(std::abs(pOut_fromGPU[i] - pIn_1[i] - pIn_2[i]) > epsilon)
		{
			printf("Wrong sum computation on GPU!\n");
			break;
		}
	}

	if(i != N)
	{
		printf("Something's wrong... try again\n");	
	}
	else
	{
		printf("Success! Everythings ok\n");
	}

	// Libération de la mémoire à la fois pour le CPU et le GPU.
   free(pOut);      pOut    = NULL;
   free(pIn_1);     pIn_1   = NULL;
   free(pIn_2);     pIn_2   = NULL;
   free(pOut_fromGPU);     pOut_fromGPU   = NULL;
   HANDLE_ERROR(cudaFree(dIn_1)); dIn_1   = NULL;
   HANDLE_ERROR(cudaFree(dIn_2)); dIn_2   = NULL;
   HANDLE_ERROR(cudaFree(dOut)); dOut   = NULL;
  }
  catch(...)
  {
	printf("Something's wrong... try again\n");	 
  }
}