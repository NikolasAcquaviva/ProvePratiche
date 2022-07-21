#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char*argv[]){
	if(access(argv[1], F_OK) != 0) {
		printf("Il file %s non esiste!\n", argv[1]);
		exit(-1);
	}
	char* arguments[argc];
	char* absolute = malloc(20*sizeof(char));
	absolute = realpath(argv[1], NULL);
	for(int i = 0; i < argc; i++) arguments[i] = malloc(sizeof(char)*10);
	for(int i = 1; i < argc; i++){
		strcpy(arguments[i-1],argv[i]);
	}
	void *handle = dlopen(absolute,RTLD_LAZY);
	if (!handle) {
		strcat(arguments[0],".so");
		arguments[argc-1] = NULL;
		execv(absolute,arguments);  
	}
	dlerror(); 
	int (*hwmain)(int,char*[]);
	hwmain = dlsym(handle, "main");  
	hwmain(argc-1,arguments);
	return 0;
}
