#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(){
	pid_t	pid1 = 0;
	int		status, foo = 0;

	//pid1 = fork(); // processo 1: Oi processo2 : Tchau 

	if ( pid1 == 0 ){
		printf("Child process 1 (PID %d)\n", getpid());
		foo = 1;
		printf("Fodasse %d\n", foo); // 1
		
		char *const arg[] = { "teste", NULL };
		printf("Passei por aqui1 %s - %d\n", arg[0], strlen(arg[0]));

		char *path = "src/ubin/teste/teste2.c\0";
		strcpy(path, arg[0]);
		//printf("Passei por aqui2 %s - %s\n", path, path[strlen(path)]);
		//printf("PORRAAA\n");

		/*if(path[strlen(path)] == '\0') { 
			int size = strlen(path);
			printf("Vasquinho legalzinho fofinho\n");
			for(int i = 0; i < size; i++) {
				path[i] = path[i];
				printf("%c", path[i]);
			}
			printf("Terminado");
		} else {
			printf("Como amo minha vida hahahahahaha\n");
		}*/

		execvp(path, arg);
		printf("porraaa\n");

		printf("ArrozDeu erro nessa caceta cara\n");		
		exit(1);

		exit(0);
	}

	wait(&status);
	printf("Father process done %d\n", foo); // 0


	return 0;

}
