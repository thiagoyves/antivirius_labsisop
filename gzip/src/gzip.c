filestoanalyze/*
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

char* isgzip(char* buffer);
char* readfile(char filename[30]);
#define O_BINARY 0

int main(int argc, char* argv[]) {
    char filename[30] = "";
    int numberoffiles = 1, numbertoanalyze = 0;
    char* filestoanalyze[30];

    while (strcmp(filename, "fim") != 0) {
        printf("Insira o caminho para o arquivo número %d a ser lido (digite ''fim'' para parar a leitura): ", numberoffiles);
        scanf("%30[^\n]%*c", &filename);

        char* buffer = isgzip(readfile(filename));

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

    //nesse ponto todos os aruivos já forma lidos e serão madandos para as threads

    printf("wadssadsadsadsadsadsa");

    pthread_t threadh[sizeof(filestoanalyze)];

    for(int i = 0; i < sizeof(filestoanalyze); i++){
        if (pthread_create (&threadh[i], NULL, isgzip, (void *) filestoanalyze[i]) != 0){ //TODO colocar a função para a descompactação dos arquivos em gzip
        	perror("erro na criação da thread %d, do arquivo $c"); //TODO falta colocar os parametros
        }
    }

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
    FILE *fileptr;
    char *buffer;
    long filelen;

    fileptr = fopen(filename, "rb");
    //fileptr = open("yourfile.txt", O_RDONLY | O_BINARY, 0);

    fseek(fileptr, 0, SEEK_END);
    filelen = ftell(fileptr);
    rewind(fileptr);

    buffer = (char *)malloc((filelen+1)*sizeof(char));
    fread(buffer, filelen, 1, fileptr);
    fclose(fileptr);

    return buffer;
}

