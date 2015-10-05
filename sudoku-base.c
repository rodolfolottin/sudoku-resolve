#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* mutex global*/
pthread_mutex_t mutex;

/* grid size = 9x9 */
#define SIZE 9

/*variável global, quantidade de erros*/
int _numErros = 0;

/*Funcoes*/
int verificaLinhaDeTeste(int grid[][SIZE]);
int funcThreads(int grid[][SIZE]);
int analiseNumAndFuncThreads(int num, int grid[][SIZE]);
int numPrimo(int num);

/* Funcao que le um grid do arquivo "filename" e o armazena em uma matriz */
int load_grid(int grid[][SIZE], char *filename) {
	FILE *input_file = fopen(filename, "r");

	if (input_file != NULL) {
		for(int i = 0; i < SIZE; i++)
			for(int j = 0; j < SIZE; j++)
				fscanf(input_file, "%d", &grid[i][j]);
		fclose(input_file);
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[]) {

	if(argc == 1) {
		printf("Erro: informe o arquivo de entrada!\n Uso: %s <arquivo de entrada>\n\n", argv[2]);
		return 1;
	}

	//checa a quantidade de numero de threads
	int numThreads;
	if (argc == 3) {
		numThreads = atoi(argv[2]);
	} else {
		numThreads = 1;
	}

	/* Le o grid do arquivo, armazena na matriz grid e imprime */
	int grid[9][9];
	if(load_grid(grid, argv[1])) {
		printf("Quebra-cabecas fornecido:\n");
		for(int i = 0; i < 9; i++) {
			for(int j = 0; j < 9; j++)
				printf("%d ", grid[i][j]);
			printf("\n");
		}
		printf("\n");
	}

	pthread_mutex_init(&mutex, NULL);
	analiseNumAndFuncThreads(numThreads, grid);
	printf("%d\n", _numErros);
	pthread_mutex_destroy(&mutex);

	return 0;
}

int analiseNumAndFuncThreads (int num, int grid[][SIZE]) {

	for (int i = 1; i <= num; ++i) {
		if (numPrimo(i) == 1) {
			pthread_t thread;
			pthread_create(&thread, NULL, verificaLinhaDeTeste, grid[][SIZE]);
			//join
			pthread_join(thread, NULL);
		} else {
			if (num % 2 == 0) {
				//
			} else {
				//
			}
		}
	}
	return 0;
}

int numPrimo (int num) {
	if (num == 1) {
		return 0;
	}

	if (num == 2) {
		return 1;
	}

	for(int m = 2; m < num; m++) {
		if (num % m != 0)
			continue;
		else 
			return 0;
	}
	return 1;
}

int verificaLinhaDeTeste (int grid[][SIZE]) {
	//lock
	pthread_mutex_lock(&mutex);

	int *temp = (int *) calloc(10, sizeof(int));

		for (int j = 0; j < 9; j++)	{
			if (temp[grid[0][j] - 1] == 0) {
				temp[grid[0][j] - 1] = 1;
			} else {
			    _numErros++;
			}
		}
	pthread_mutex_unlock(&mutex);

	return 0;
}