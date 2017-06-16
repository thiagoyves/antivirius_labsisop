/*
 ============================================================================
 Name        : gzip.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int filedescriptor;
int numberoffilesvirus;
char* buffer_file;
void* thread(char* file, char* argv);
char* isgzip(char* buffer);
char* readfile(char filename[30]);
void* containvirus(char* gzipfile ,char* virus_signature);
#define O_BINARY 0

sem_t thread_lock;

typedef struct infoThread {
	pthread_t thread;
	char file_name;

} infoThread_t;

int main(int argc, char* argv[]) {
    char filename[30] = "";
    int numberoffiles = 1, numbertoanalyze = 0;
    char* filestoanalyze[30];
	sem_init(&thread_lock, 0, 1);

    if(argv[1] == NULL){
    	perror("parametro com assinatura do virus esta vazio");
    }

    while (strcmp(filename, "fim") != 0) {
        printf("Insira o caminho para o arquivo número %d a ser lido (digite ''fim'' para parar a leitura): ", numberoffiles);
        scanf("%30[^\n]%*c", &filename);

        char* buffer = isgzip(readfile(filename));
        printf("file descriptior: %d", filedescriptor);

        if(buffer != NULL){
        	filestoanalyze[numbertoanalyze] = buffer;
        	printf("\n\né gzip\n\n");
        	numbertoanalyze++;
       }
        else {
        	printf("\n\narquivo não éno formato gzip\n\n");
        }

        numberoffiles++;
    }

    //nesse ponto todos os aruivos já forma lidos e aserão madandos para as threads

    printf("antes da criacao do array thread");

//    pthread_t threadh[sizeof(filestoanalyze)];

    infoThread_t threadh[sizeof(filestoanalyze)];

    printf("depois da criacao do array thread");

    for(int i = 0; i < sizeof(filestoanalyze); i++){
        if (pthread_create (&threadh[i]->thread, NULL, isgzip, (void *) filestoanalyze[i]) != 0){
        	perror("erro na criação da thread %d, do arquivo $c"); //TODO falta colocar os parametros
        }
        else{
        	&threadh[i]->file_name = ;
        	printf("entrou para criar thread");
        	thread(filestoanalyze[i], argv[1]);
        }
    }

	// aguarda finalização das threads para poder liberar o semáforo.
	for (int i=0; i<sizeof(filestoanalyze); i++) pthread_join(threadh[i], NULL);
	sem_destroy(&thread_lock);
	pthread_exit(NULL);

	return EXIT_SUCCESS;
}

char* isgzip(char* buffer){
    const unsigned char header[] = {0x1F, 0x8B};

    if(memcmp(buffer, header, sizeof header) == 0){
    	return *buffer;
    }
    else{
    	return NULL;
    }
}

char* readfile(char filename[30]){
	char *buffer;

	int fd = open(filename, O_RDONLY);
	filedescriptor = fd; //variavel global que as thread acessarão com o valor do descritor

	lseek(fd, 0, SEEK_CUR);

	buffer = (char *)malloc((fd)*sizeof(char));
	read(fd, buffer, 1000);
	close(fd);

	return buffer;
}

void* containvirus(char* gzipfile ,char* virus_signature){


	printf("entrou no metodo containvirus");
	if(memmem(gzipfile, sizeof gzipfile, virus_signature, sizeof virus_signature) != NULL){
		printf("assinatura do viruus encontrada");
	}
	else{
		printf("virus nao encontrado");
	}
}

void* thread(char* file, char* argv) {

	//se for um arquivo gzip e precisar forkar
	int pipe_thread[2];
	int fd;

	if (pipe(pipe_thread) == -1); //handle_error();
	switch(fork()) {
		case -1: perror("fork");
		case 0:

			printf("entrou no metodo thread case 0");

			close(pipe_thread[0]);//fecha leitura npo filho
			dup2(pipe_thread[1], STDOUT_FILENO); //seta o pipe de escrita para a saida padrao do processo filho, para não perder o tracking dps do exec
			pipe_thread[1] = file;
			execlp(pipe_thread[1], pipe_thread[1], "-l", NULL);
			buffer_file = pipe_thread[0];
			containvirus(buffer_file, argv[1]);
			break;
		default:
			close(pipe_thread[1]);// fecha escrita no pai
			dup2(pipe_thread[0], filedescriptor);//seta a leitura do pipe no pai para algum descritor
	}


	//tratar o que for recebido no descritor selecionado
	//while(condition){
	//    incrementar o contador de virus
	//}

	// fecha semaforo
	if (sem_wait(&thread_lock) != 0) exit(85);
		//contador_global += contador_local}
	// libera semaforo
	if (sem_post(&thread_lock) != 0) exit(85);

	return NULL;
}
