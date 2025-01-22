#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_to_file(double** matrix, int n, int format_flag)
{
	char filename[50];
	snprintf(filename, sizeof(filename), "array_%06d_%s.out", n, format_flag == 0 ? "asc" : "bin");
	
	FILE *file = fopen(filename, format_flag == 0 ? "w" : "wb");
	if(!file)
	{
		fprintf(stderr, "FILE OPEN ERROR!\n");
		return;
	}

	if(format_flag == 0)
	{
		for(int i = 0; i<n; i++)
		{
			for(int j = 0; j<n; j++)
			{
				fprintf(file, "%.15f", matrix[i][j]);
			}
			fprintf(file, "\n");
		}
	} else
	{
		for(int i = 0; i<n; i++)
		{
			fwrite(matrix[i], sizeof(double), n, file);
		}
	}
	fclose(file);
	printf("WRITE SUCCESSFUL TO FILE: %s\n", filename);
}

int main(int argc, char* argv[])
{
	int format_flag = atoi(argv[1]);
	
	FILE *input_file = fopen("input.in", "r");
	if(!input_file)
	{
		fprintf(stderr, "Input file doesn't exist!\n");
		return 1;
	}

	int n;
	fscanf(input_file, "%d", &n);
	fclose(input_file);

	double** matrix = (double**)malloc(n * sizeof(double*));
	for(int i = 0; i<n; i++)
		matrix[i] = (double*)malloc(n * sizeof(double));

	for(int i = 0; i<n; i++)
	{
		for(int j = 0; j<n; j++)
			matrix[i][j] = (double)(i+j);
	}
	
	print_to_file(matrix, n, format_flag);

	for(int i = 0; i<n; i++)
		free(matrix[i]);
	free(matrix);

	return 0;
}

