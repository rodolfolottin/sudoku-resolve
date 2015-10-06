#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


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
int _grid[SIZE][SIZE];

int analiseArray (int conjuntoValores[], char area[], int valor, int val_thread);
void *verificadorSudoku(void *arg);
int verificaGrade(int linha, int coluna, int regiao, int val_thread);

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
        pthread_t vetor_threads[numThreads];
	pthread_mutex_init(&mutex_linha, NULL);
	pthread_mutex_init(&mutex_coluna, NULL);
	pthread_mutex_init(&mutex_regiao, NULL);
	pthread_mutex_init(&mutex_qntdErro, NULL);

	/* Le o grid do arquivo, armazena na matriz grid e imprime */
	if(load_grid(_grid, argv[1])) {
		printf("Quebra-cabecas fornecido:\n");
		for(int i = 0; i < 9; i++) {
			for(int j = 0; j < 9; j++){
				printf("%d ", _grid[i][j]);
 		        }
			printf("\n");
		}
		printf("\n");
		for(int i = 1; i <= numThreads; i++){
			int tempIndex = i;
		    pthread_create(&vetor_threads[i], NULL, verificadorSudoku, &tempIndex);	
		}
	    for (int j = 1; j <= numThreads; j++) {
			pthread_join(vetor_threads[j], NULL);
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

int analiseArray (int conjuntoValores[], char area[], int valor, int val_thread) {
	int *temp = (int *) calloc(10, sizeof(int));
	pthread_t tid  = pthread_self();	
	for (int i = 0; i < 9; i++) {
		if (conjuntoValores[i] <= 9) {
			if (temp[conjuntoValores[i] - 1] == 0) {
				temp[conjuntoValores[i] - 1] = 1;
			} else {
				printf("Thread %d: erro %s %d. \n", val_thread, area, valor);
				//sleep(3);
				return 1;
			}
		}else{
		//valor maior que 10
		 	printf("Thread %d: Erro. Valor maior que 10 \n", val_thread);
		 	//sleep(3);
			return 1;
		}
	}	
	return 0;
}

void *verificadorSudoku(void *arg) {
	int val_thread = *((int *)arg);	
	int aux = 0;
	int aux_qntdErro = 0;
	while (1) {
	  	pthread_mutex_lock(&mutex_linha);
		if (_linha <= 0) {
			pthread_mutex_unlock(&mutex_linha);
			break;
  		}
       	aux = _linha;
    	_linha--;
		pthread_mutex_unlock(&mutex_linha);
		aux_qntdErro = analiseArray(_grid[aux-1], "linha", aux, val_thread);
		if (aux_qntdErro > 0) {
	     		pthread_mutex_lock(&mutex_qntdErro);
	     		_qntdErro++;
	     		pthread_mutex_unlock(&mutex_qntdErro);
			}
    	} 
	
	while (1) {
		pthread_mutex_lock(&mutex_coluna);
	    if (_coluna <= 0) {
			pthread_mutex_unlock(&mutex_coluna);
			break;
		}
        aux = _coluna;
    	_coluna--;
    	pthread_mutex_unlock(&mutex_coluna);
		int *temp = (int *) calloc(10, sizeof(int));
		for(int i = 0; i < 9; i++) {        
			temp[i] = _grid[i][aux-1];
		}
		aux_qntdErro = analiseArray(temp, "coluna", aux, val_thread);
    	if (aux_qntdErro > 0) {
    		pthread_mutex_lock(&mutex_qntdErro);
			_qntdErro++;
			pthread_mutex_unlock(&mutex_qntdErro);
		}
	} 

	while (1) {
		pthread_mutex_lock(&mutex_regiao);
		if (_regiao <= 0) {
			pthread_mutex_unlock(&mutex_regiao);
			break;
	  	}
		aux = _regiao;
		_regiao--;
		pthread_mutex_unlock(&mutex_regiao);
		switch (aux) {
		    case 1: aux_qntdErro  = verificaGrade(0, 0, aux, val_thread); break;
	   	    case 2: aux_qntdErro  = verificaGrade(0, 3, aux, val_thread); break;
	     	case 3: aux_qntdErro  = verificaGrade(0, 6, aux, val_thread); break;
	     	case 4: aux_qntdErro  = verificaGrade(3, 0, aux, val_thread); break;
     	    case 5: aux_qntdErro  = verificaGrade(3, 3, aux, val_thread); break;
     	    case 6: aux_qntdErro  = verificaGrade(3, 6, aux, val_thread); break;
     	    case 7: aux_qntdErro  = verificaGrade(6, 0, aux, val_thread); break;
     	    case 8: aux_qntdErro  = verificaGrade(6, 3, aux, val_thread); break;
     	    case 9: aux_qntdErro  = verificaGrade(6, 6, aux, val_thread); 
        }
    	if (aux_qntdErro > 0) {
    		pthread_mutex_lock(&mutex_qntdErro);
    		_qntdErro++;
    		pthread_mutex_unlock(&mutex_qntdErro);
		}
	}
}


int verificaGrade(int linha, int coluna, int regiao, int val_thread) {
	int *temp = (int *) calloc(10, sizeof(int));
	int i = 0;
	int aux_linha  = linha;
	int aux_coluna = coluna;
	for (; linha < (aux_linha + 3); linha++) {
		coluna = aux_coluna;	
        for (; coluna < (aux_coluna + 3); coluna++) {
       		temp[i] = _grid[linha][coluna];
	        i++;
    	}
	}
    return analiseArray(temp, "região", regiao, val_thread);
}