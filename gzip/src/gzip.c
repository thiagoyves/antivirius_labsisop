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
void* thread(char* file);
char* isgzip(char* buffer);
char* readfile(char filename[30]);
void* containvirus(char* gzipfile ,char* virus_signature);
#define O_BINARY 0

sem_t thread_lock;

int main(int argc, char* argv[]) {
    char filename[30] = "";
    int numberoffiles = 1, numbertoanalyze = 0;
    char* filestoanalyze[30];
    int pipe_thread[2];
	sem_init(&thread_lock, 0, 1);

    while (strcmp(filename, "fim") != 0) {
        printf("Insira o caminho para o arquivo número %d a ser lido (digite ''fim'' para parar a leitura): ", numberoffiles);
        scanf("%30[^\n]%*c", &filename);

        char* buffer = isgzip(readfile(filename));
        printf("file descriptior: %d", filedescriptor);

        if(buffer != NULL){
        	filestoanalyze[numbertoanalyze] = buffer;
        	printf("\n\né gzip\n\n");

        	//Call method which verifies if the gzip file contains virus signature
//			if(argv[1] != NULL){
//				containvirus(argv[1],filestoanalyze[numbertoanalyze]);
//			}
//

        	numbertoanalyze++;
       }
        else {
        	printf("\n\narquivo não éno formato gzip\n\n");
        }

        numberoffiles++;
    }

    //nesse ponto todos os aruivos já forma lidos e serão madandos para as threads

    pthread_t threadh[sizeof(filestoanalyze)];

    for(int i = 0; i < sizeof(filestoanalyze); i++){
        if (pthread_create (&threadh[i], NULL, isgzip, (void *) filestoanalyze[i]) != 0){
        	perror("erro na criação da thread %d, do arquivo $c"); //TODO falta colocar os parametros
        }
        else{


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
	if(memmem(gzipfile, sizeof gzipfile, virus_signature, sizeof virus_signature) != NULL){
		numberoffilesvirus++;
	}
}

void* thread(char* file) {

	//se for um arquivo gzip e precisar forkar
	int pipe_thread[2];
	int fd;

	if (pipe(pipe_thread) == -1) //handle_error();
	switch(fork()) {
		case -1: perror("fork");
		case 0:
			close(pipe_thread[0]);//fecha leitura npo filho
			dup2(pipe_thread[1], STDOUT_FILENO); //seta o pipe de escrita para a saida padrao do processo filho, para não perder o tracking dps do exec
			execlp(file, file, "-l", NULL);
			break;
		default:
			close(pipe_thread[1]);// fecha escrita no pai
			dup2(pipe_thread[0], filedescriptor);//seta a leitura do pipe no pai para algum descritor
	}
	else{
		perror("pipe");
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
