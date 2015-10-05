#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* grid size = 9x9 */
#define SIZE 9

pthread_mutex_t mutex;

int _qntdErro = 0;
int analiseArray (int conjuntoValores[], char area[], int valor, int thread);
int verificadorSudoku(int grid[][SIZE]);
int verificaGrade(int linha, int coluna, int grid[][SIZE], int regiao);

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
	
	int numThreads;
	if (argc == 2) {
		numThreads = 1;
	} else {
		numThreads = atoi(argv[2]);
	}

	pthread_t thread;

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
		for (int i = 0; i < numThreads; numThreads++) {
			pthread_create(&thread, NULL, verificadorSudoku, (void *) grid);
			pthread_join(thread, NULL);
        }
        pthread_exit(NULL);
	}

	if (_qntdErro == 0) {
		printf("Nenhum erro encontrado!\n");
	}

	return 0;
}

int analiseArray (int conjuntoValores[], char area[], int valor, int thread) {
    int *temp = (int *) calloc(10, sizeof(int));
	for (int i = 0; i < 9; i++) {
		if (conjuntoValores[i] <= 9) {
			if (temp[conjuntoValores[i] - 1] == 0) {
				temp[conjuntoValores[i] - 1] = 1;
			} else {
			    printf("Thread %d: Erro %s %d. \n", thread, area, valor + 1);
				return 1;
			}
		} else {
		//valor maior que 10
		    printf("Thread %d: Erro. Valor maior que 10 \n", thread);
			return 1;
		}
	}	

	return 0;
}

int verificadorSudoku(int grid[][SIZE]) {

		for (int i = 0; i < 9; i++) {
	       _qntdErro += analiseArray(grid[i], "Linha", i, 1);
	    }
	    
	    // int temp[9];
	    // for(int j = 0; j < 9; j++) {
	    //     for(int i = 0; i < 9; i++) {
	    //         temp[i] = grid[i][j];
	    //     }
	    //     _qntdErro += analiseArray(temp, "coluna", j, 1);
	    // }
	    
	    // int regiao = 0; 
	    // for(int i = 0; i < 9; i+=3) {
	    //     for(int j = 0; j < 9; j+=3) {
	    //         _qntdErro += verificaGrade(i, j, grid, regiao);           
	    //         regiao++;
	    //     }
	        
	    // }
	// }

    return 1;
}

int verificaGrade(int linha, int coluna, int grid[][SIZE], int regiao) {
    int temp[9];
    int i = 0;

    for (; linha < linha + 3; linha++) {
        for (; coluna < coluna + 3; coluna++) {
            temp[i] = grid[linha][coluna];
            i++;
        }
    }

    return analiseArray(temp, "região", regiao, 1);
}