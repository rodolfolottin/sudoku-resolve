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
				printf("%d ", _grid[i][j]);
			printf("\n");
		}
		printf("\n");
		for (int i = 0; i < numThreads; i++) {
			pthread_create(&thread, NULL, verificadorSudoku, NULL);
		}
        for (int j = 0; j < numThreads; j++) {
			pthread_join(thread, NULL);
		}
         
       	pthread_mutex_destroy(&mutex_linha);
       	pthread_mutex_destroy(&mutex_coluna);
       	pthread_mutex_destroy(&mutex_regiao);
       	pthread_mutex_destroy(&mutex_qntdErro);
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

	pthread_mutex_init(&mutex_linha, NULL);
	pthread_mutex_init(&mutex_coluna, NULL);
	pthread_mutex_init(&mutex_regiao, NULL);
	pthread_mutex_init(&mutex_qntdErro, NULL);

    int aux = 0;
    int aux_qntdErro = 0;

    //while o que true?
    while (true) {
    	pthread_mutex_lock(&mutex_linha);
		if (_linha <= 0) {
			pthread_mutex_unlock(&mutex_linha);
			break;
  		}
        aux = _linha;
    	_linha--;
		pthread_mutex_unlock(&mutex_linha);
		aux_qntdErro = analiseArray(_grid[_linha], "Linha", aux, 1);
		if (aux_qntdErro > 0) {
			pthread_mutex_lock(&mutex_qntdErro);
			_qntdErro++;
			pthread_mutex_unlock(&mutex_qntdErro);
		}
	} 
	
	//while o que true?    	       
	while (true) {
		pthread_mutex_lock(&mutex_coluna);
    	if (_coluna <= 0) {
			pthread_mutex_unlock(&mutex_coluna);
			break;
		}
        aux = _coluna;
    	_coluna--;
    	pthread_mutex_unlock(&mutex_coluna);

    	//inicializar temp
        for(int i = 0; i < 9; i++) {
            int *temp = (int *) calloc(10, sizeof(int));
	    	temp[i] = _grid[i][aux];
        }
        aux_qntdErro = analiseArray(temp, "Coluna", _coluna, 1);
        if (aux_qntdErro > 0) {
	    	pthread_mutex_lock(&mutex_qntdErro);
			_qntdErro++;
			pthread_mutex_unlock(&mutex_qntdErro);
		}
	} 
	
    //while o que true?  
    while (true) {
		pthread_mutex_lock(&mutex_regiao);
    	if (_regiao <= 0) {
			pthread_mutex_unlock(&mutex_regiao);
			break;
  		}
        aux = _regiao;
    	_regiao--;
    	pthread_mutex_unlock(&mutex_regiao);
    	int linha_aux;
    	int coluna_aux;
		
		switch (aux) {
			case 1: aux_qntdErro  += verificaGrade(0, 0, _regiao); 
			case 2: aux_qntdErro  += verificaGrade(0, 3, _regiao); 
     	    case 3: aux_qntdErro  += verificaGrade(0, 6, _regiao); 
     	    case 4: aux_qntdErro  += verificaGrade(3, 0, _regiao); 
     	    case 5: aux_qntdErro  += verificaGrade(3, 3, _regiao); 
     	    case 6: aux_qntdErro  += verificaGrade(3, 6, _regiao); 
     	    case 7: aux_qntdErro  += verificaGrade(6, 0, _regiao); 
     	    case 8: aux_qntdErro  += verificaGrade(6, 3, _regiao); 
     	    case 9: aux_qntdErro  += verificaGrade(6, 6, _regiao); 
        }
        if (aux_qntdErro > 0) {
			pthread_mutex_lock(&mutex_qntdErro);
			_qntdErro++;
			pthread_mutex_unlock(&mutex_qntdErro);
		}
	}
    return 1;
}


int verificaGrade(int linha, int coluna, int regiao) {
    int temp[9];
    int i = 0;

    for (; linha < linha + 3; linha++) {
        for (; coluna < coluna + 3; coluna++) {
            temp[i] = _grid[linha][coluna];
            i++;
        }
    }

    return analiseArray(temp, "Região", regiao, 1);
}