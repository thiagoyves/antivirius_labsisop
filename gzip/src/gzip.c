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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void readFile(int fd);

int main(int argc, char* argv[]) {
	int file_descriptor;

    char *line = NULL;
    size_t len = 1000;
    ssize_t read = 0;
    while (read != -1) {
        puts("Insira o caminho para o arquivo: ");
        read = getline(line, &len, stdin);
        printf("Caminho digitado = %s", line);

        file_descriptor = open(line, O_RDONLY);
        printf("fd content = %i", file_descriptor);

        readFile(file_descriptor);

        //       res = read(file_descriptor, buffer, tam_buffer)

    }
    free(line);
    return 0;

	return EXIT_SUCCESS;
}


void readFile(int fd) {
    char buffer[2];
    int bytes_read;
    int k = 0;
    do {
        char t = 0;
        bytes_read = read(fd, &t, 1);
        buffer[k++] = t;
        printf("%c", t);
        if(t == '\n' && t == '\0') {
            printf("%d", atoi(buffer));
            for(int i=0; i<10; i++)
                buffer[i]='\0';
            k = 0;
        }
    }
    while (bytes_read != 0);
}
