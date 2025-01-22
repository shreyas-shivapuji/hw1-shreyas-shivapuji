#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_SIZE 262144 // 512 x 512 = 262144

int read_matrix(double** matrix, int n)
{
	char matrix_filename[50];
	sprintf(matrix_filename, "./inputfiles/mat_%06d.in", n);
	FILE* matrix_file = fopen(matrix_filename, "r");
	if(!matrix_file)
	{
		fprintf(stderr, "Matrix file doesn't exist!\n");
		return 0;
	}

	char line[MAX_LINE_SIZE];
	int i = 0;
	while(i<n && fgets(line, sizeof(line), matrix_file))
	{
		char* token = strtok(line,",");
		int j = 0;
		while(token != NULL)
		{
			matrix[i][j++] = atof(token);
			token = strtok(NULL,",");
		}
		i++;
	}
	fclose(matrix_file);
	
	/*	
	for(int i = 0; i<n; i++)
	{
		for(int j = 0; j<n; j++)
		{
			printf("%0.15e ", matrix[i][j]);
		}
		printf("\n");
	}
	*/

	return 1;
}

int read_vector(double** vct, int n)
{
	for(int i = 1; i<=4; i++)
	{
		char vector_filename[50];
		sprintf(vector_filename, "./inputfiles/vec_%06d_%06d.in", n, i);
		FILE* vector_file = fopen(vector_filename, "r");
		if(!vector_file)
		{
			fprintf(stderr, "Vector file doesn't exist!\n");
			return 0;
		}

		char line[MAX_LINE_SIZE];
		while(i<n && fgets(line, sizeof(line), vector_file))
		{
			char* token = strtok(line, ",");
			int vctcol = 0;
			while(token != NULL)
			{
				vct[i-1][vctcol++] = atof(token);
				token = strtok(NULL, ",");
			}
			
		}
		fclose(vector_file);

		/*
		for(int j = 0; j<4; j++)
		{		
			for(int k = 0; k<n; k++)
			{
				printf("%0.15e ", vct[j][k]);
			}
			printf("\n");
		}
		*/
	}
	return 1;
}

double is_eigen(double** A, double* v, int n)
{
	double* Av = (double*)malloc(n*sizeof(double));
	double lambda = 0;
	const double eps = 1e-6;
	for(int i = 0; i<n; i++)
	{
		Av[i] = 0;
		for(int j = 0; j<n; j++) Av[i] += A[i][j] * v[j];
	}
	
	for(int i = 0; i<n; i++)
	{
		if(fabs(v[i]) > eps)
		{
			lambda = Av[i] / v[i];
			break;
		}
	}

	for(int i = 0; i<n; i++)
	{
		if(fabs(Av[i] - lambda * v[i]) > eps)
		{
			return 0;
		}
	}
	free(Av);
	return lambda;
}

int write_eigen(double** matrix, double** vct, int n, int i)
{
	char vector_filename[50];
	sprintf(vector_filename, "./inputfiles/vec_%06d_%06d.in", n, i+1);
	double eigenvalue = is_eigen(matrix, vct[i], n);
	if(eigenvalue != 0)
	{
		FILE* vector_file = fopen(vector_filename, "a");
		fprintf(vector_file, "\nEigen Value = %0.15e\n", eigenvalue);
		if(!vector_file)
		{
			fprintf(stderr, "Error opening vector file '%s' for writing\n", vector_filename);
			return 0;
		}
		fclose(vector_file);
					
		printf("%s: Yes: %0.15e\n", vector_filename+13, eigenvalue);
	}
	else printf("%s: Not an eigenvector\n", vector_filename+13);
	return 1;
}


int main()
{
	
	FILE* input_file = fopen("input.in", "r");
	int n;
	if(fscanf(input_file, "%d", &n) != 1)
	{
		fprintf(stderr, "Error reading input file!\n");
		return 1;
	}
	fclose(input_file);
	
	double** matrix = (double**)malloc(n * sizeof(double*));
	for(int i = 0; i<n; i++) matrix[i] = (double*)malloc(n * sizeof(double));

	double** vct = (double**)malloc(4 * sizeof(double*));
	for(int i = 0; i<4; i++) vct[i] = (double*)malloc(n * sizeof(double));

	if(!read_matrix(matrix,n))
	{
		printf("Error reading matrix file\n");
		return 1;
	}

	if(!read_vector(vct,n))
	{
		printf("Error reading vector files\n");
		return 1;
	}
	
	for(int i = 0; i<4; i++)
	{
		if(!write_eigen(matrix, vct, n, i))
		{
			printf("Error writing eigen value to vector files\n");
			return 1;
		}
	}

	for(int i = 0; i<n; i++)
	{
		free(matrix[i]);
		if(i<4) free(vct[i]);
	}
	free(matrix);
	free(vct);

	return 0;
}
