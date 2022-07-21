#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char* argv[]){
	char* buffer = malloc(64*sizeof(char));
	char* absolute = malloc(64*sizeof(char));
	readlink(argv[1],buffer,64);
	absolute = realpath(buffer,NULL);
	remove(argv[1]);
	symlink(absolute,argv[1]);
	return 0;
}
