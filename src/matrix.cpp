#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <chrono>
#include <omp.h>
using namespace std::chrono;


float** serialInitializeMatrix(int dimension){

    float** matrix = (float**)malloc(dimension * sizeof(float*));

	for(int i=0; i < dimension; i++){
		matrix[i] = (float*)malloc(dimension * sizeof(float));
	}

	srand((unsigned)time(NULL));

	for(int i=0; i < dimension; i++){
		for(int j=0; j < dimension; j++){
			matrix[i][j] = static_cast <float> (rand()) / static_cast <float>(RAND_MAX);
		}
	}
	return matrix;
}

float** initializeSolutionMatrix(int dimension){

    float** matrix = (float**)malloc(dimension * sizeof(float*));

	for(int i=0; i < dimension; i++){
		matrix[i] = (float*)malloc(dimension * sizeof(float));
	}

	srand((unsigned)time(NULL));

	for(int i=0; i < dimension; i++){
		for(int j=0; j < dimension; j++){
			matrix[i][j] = 0.0f;
		}
	}
	return matrix;
}

double serialMultiply(float** const matrixA, float** const matrixB, float** const sol, int dimension){
	high_resolution_clock::time_point begin = high_resolution_clock::now();
	for(int i=0; i < dimension; i++){
		for(int j=0; j < dimension; j++){
			for(int k=0; k < dimension; k++){
				sol[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
	}
	high_resolution_clock::time_point end = high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms = end - begin;
	std::cout << "Time in milliseconds for multiplication: " << ms.count() << std::endl;

	return ms.count();
}

void serialTest(int dimension){
	high_resolution_clock::time_point begin = high_resolution_clock::now();
    float** a = serialInitializeMatrix(dimension);
	float** b = serialInitializeMatrix(dimension);
	high_resolution_clock::time_point end = high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms = end - begin;
	std::cout << "Time in milliseconds: " << ms.count() << std::endl;

	FILE * pFile;
	pFile = fopen("../results/initializationTimes.txt", "a+");
	fprintf(pFile, "----------------------------------\n");
	fprintf(pFile, "Test : Serial Initialization      \n");
	fprintf(pFile, "----------------------------------\n");
	fprintf(pFile, "Dimension : %d\n", dimension);
	fprintf(pFile, "Time in ms : %f\n", ms.count());
	fprintf(pFile, "..................................\n");
	fclose(pFile);


	float** sol = initializeSolutionMatrix(dimension);

	double totalTime = serialMultiply(a, b, sol, dimension);

	FILE * pFile1;
	pFile1 = fopen("../results/multiplicationTimes.txt", "a+");
	fprintf(pFile1, "----------------------------------\n");
	fprintf(pFile1, "Test : Serial Multiplication      \n");
	fprintf(pFile1, "----------------------------------\n");
	fprintf(pFile1, "Dimension : %d\n", dimension);
	fprintf(pFile1, "Time in ms : %f\n", totalTime);
	fprintf(pFile1, "..................................\n");
	fclose(pFile1);

    free(a);
    free(b);
    free(sol);
}

float** parallelInitializeMatrix(int dimension){

    float** matrix = (float**)malloc(dimension * sizeof(float*));

	for(int i=0; i < dimension; i++){
		matrix[i] = (float*)malloc(dimension * sizeof(float));
	}

	srand((unsigned)time(NULL));

	#pragma omp parallel for
	for(int i=0; i < dimension; i++){
		for(int j=0; j < dimension; j++){
			matrix[i][j] = static_cast <float> (rand()) / static_cast <float>(RAND_MAX);
		}
	}
	return matrix;
}

double parallelMultiply(float** const matrixA, float** const matrixB, float** const sol, int dimension){
	high_resolution_clock::time_point begin = high_resolution_clock::now();
	#pragma omp parallel for
	for(int i=0; i < dimension; i++){
		for(int j=0; j < dimension; j++){
			for(int k=0; k < dimension; k++){
				sol[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
	}
	high_resolution_clock::time_point end = high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms = end - begin;
	std::cout << "Time in milliseconds for multiplication: " << ms.count() << std::endl;

	return ms.count();
}

void parallelTest(int dimension){
	high_resolution_clock::time_point begin = high_resolution_clock::now();
    float** a = parallelInitializeMatrix(dimension);
	float** b = parallelInitializeMatrix(dimension);
	high_resolution_clock::time_point end = high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms = end - begin;
	std::cout << "Time in milliseconds: " << ms.count() << std::endl;

	FILE * pFile;
	pFile = fopen("../results/initializationTimes.txt", "a+");
	fprintf(pFile, "----------------------------------\n");
	fprintf(pFile, "Test : Parallel Initialization      \n");
	fprintf(pFile, "----------------------------------\n");
	fprintf(pFile, "Dimension : %d\n", dimension);
	fprintf(pFile, "Time in ms : %f\n", ms.count());
	fprintf(pFile, "..................................\n");
	fclose(pFile);


	float** sol = initializeSolutionMatrix(dimension);

	double totalTime = parallelMultiply(a, b, sol, dimension);

	FILE * pFile1;
	pFile1 = fopen("../results/multiplicationTimes.txt", "a+");
	fprintf(pFile1, "----------------------------------\n");
	fprintf(pFile1, "Test : Parallel Multiplication      \n");
	fprintf(pFile1, "----------------------------------\n");
	fprintf(pFile1, "Dimension : %d\n", dimension);
	fprintf(pFile1, "Time in ms : %f\n", totalTime);
	fprintf(pFile1, "..................................\n");
	fclose(pFile1);

    free(a);
    free(b);
    free(sol);
}

int main()
{	
    int dimension = 3;

	serialTest(dimension);

	parallelTest(dimension);

    return 0;
}