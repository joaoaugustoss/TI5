#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100


int main(int argc, char *argv[]){
	printf("Vai se fuder paporeto");
	int	i, j;
	int 	foo = argc;
	printf("cachorro\n");
	printf("%d", foo);

	for( i = 0; i < MAX; i++ ){
        for( j = 0; j < MAX; j++ ){
            if ( j > MAX / 2 ) {
                printf("At child %s\t", argv[1]);
            }
       	}
	}

	printf("\n*Ending execution of child %s\n", argv[1]);

	return 0;
}
