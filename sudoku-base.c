#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* grid size = 9x9 */
#define SIZE 9

pthread_mutex_t mutex_linha;
pthread_mutex_t mutex_coluna;
pthread_mutex_t mutex_regiao;
pthread_mutex_t mutex_qntdErro;

int _qntdErro = 0;
int _coluna = 9;
int _linha = 9;
int _regiao = 9;
int _grid[][SIZE];

int analiseArray (int conjuntoValores[], char area[], int valor, int thread);
int verificadorSudoku();
int verificaGrade(int linha, int coluna, int regiao);

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
	if(load_grid(_grid, argv[1])) {
		printf("Quebra-cabecas fornecido:\n");
		for(int i = 0; i < 9; i++) {
			for(int j = 0; j < 9; j++)
				printf("%d ", grid[i][j]);
			printf("\n");
		}
		printf("\n");
		for (int i = 0; i < numThreads; i++) {
			pthread_create(&thread, NULL, verificadorSudoku, NULL);
		}
        for (int j = 0; j < numThreads; j++) {
			pthread_join(thread, NULL);
		}         
         
	    printf("Erros encontrados: %d\n", _qntdErro);
        pthread_exit(NULL);
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

int verificadorSudoku() {

    int aux = 0;
    int aux_qntdErro = 0;

    //while o que true?
    while (true) {
    	mutex_linha_lock();
		if (_linha <= 0) {
			mutex_linha_unlock();
			break;
  		}
        aux = _linha;
    	_linha--;
		mutex_linha_unlock();
		aux_qntdErro = analiseArray(grid[i], "Linha", aux, 1);
		if (aux_qntdErro > 0) {
			mutex_qntdErro_lock();
			_qntdErro++;
			mutex_qntdErro_unlock();
		}
	} 
	
	//while o que true?    	       
	while (true) {
		mutex_coluna_lock();
    	if (_coluna <= 0) {
			mutex_coluna_unlock();
			break;
		}
        aux = _coluna;
    	_coluna --;
    	mutex_coluna_unlock();
        for(int i = 0; i < 9; i++) {
            int *temp = (int *) calloc(10, sizeof(int));
	    	temp[i] = grid[i][aux];
        }
        aux_qntdErro = analiseArray(temp, "coluna", j, 1);
        if (aux_qntdErro > 0) {
	    	mutex_qntdErro_lock();
	    	_qntdErro++;
	    	mutex_qntdErro_unlock();
		}
	} 
	
    //while o que true?  
    while (true) {
		mutex_regiao_lock()
    	if (_regiao <= 0) {
			mutex_regiao_unlock();
			break;
  		}
        aux = _regiao;
    	_regiao--;
    	mutex_regiao_unlock()
    	int linha_aux;
    	int coluna_aux;
		//NAO CONSEGUI PENSAR NUMA LOGICA PRA REGIAO, FAZER "JEITO FEIO" SE FUNCIONAR PENSAMOS NA OTIMIZACAO
		switch (aux) {
			CASE: 1: aux_qntdErro  += verificaGrade(0, 0, regiao); 
			CASE: 2: aux_qntdErro  += verificaGrade(0, 3, regiao); 
     	    //ATE 9
      	   
        }
        if (aux_qntdErro > 0) {
			mutex_qntdErro_lock();
		    _qntdErro++;
		    mutex_qntdErro_unlock();
		}
	}
    return 1;
}


int verificaGrade(int linha, int coluna, int regiao) {
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