#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <assert.h>
#include <iostream>
//
static constexpr const unsigned int N = 10000000U;
static constexpr const float epsilon = 1E-6f;
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
//
int main()
{
	// Tableau CPU
	float *pOut     = NULL;
	float *pIn_1    = NULL;
	float *pIn_2    = NULL;

	//  Allocation de la mémoire
	pOut    = (float*)malloc(sizeof(float) * N);
	pIn_1   = (float*)malloc(sizeof(float) * N);
	pIn_2   = (float*)malloc(sizeof(float) * N);

	// Initialisation des tableaux.
	for(int i = 0; i < N; i++)
	{
		pIn_1[i] = 1.0f; 
		pIn_2[i] = 2.0f;
	}
	// Addition des deux vecteurs.
	vector_add_c(pOut, pIn_1, pIn_2, N);

	// Vérification que le résultat est correct sur le CPU.
	int i;
	for(i = 0; i < N; i++)
	{
		if(std::fabs(pOut[i] - pIn_1[i] - pIn_2[i]) > epsilon)
		{
			printf("Wrong sum computation on CPU!\n");
			break;
		}
	}

	if(i == N)
	{
		printf("Everything's OK!\n");
	}

	// Libération de la mémoire pour le CPU
   free(pOut);      pOut    = NULL;
   free(pIn_1);     pIn_1   = NULL;
   free(pIn_2);     pIn_2   = NULL;
}