#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char* argv[]){
	char* commands[argc];
	for(int i = 0; i < argc; i++) {
		commands[i] = malloc(32*sizeof(char));
	}
	
	int i,j;
       	i = 1; j = 0;
	
	while(i < argc){
		int equal = strcmp(argv[i],"//");
		if(equal != 0){
			strcat(commands[j],argv[i]);
			strcat(commands[j]," ");
			i++;
		}
		else{
			i++;
			j++;
		}
	}
	for(int i = 0; i < argc, strcmp(commands[i],""); i++){
		char* tmp = malloc(64*sizeof(char));
		strcpy(tmp,commands[i]);
		char* args[64];
		for(int i = 0; i < 64; i++) args[i] = malloc(64*sizeof(char));
		int j = 0;
		char* maincmd = strtok(tmp," ");
		char* bincmd = malloc(sizeof(char)*64);
		strcpy(bincmd,"/bin/");
		strcat(bincmd,maincmd);
		while(maincmd!=NULL){
			strcpy(args[j],maincmd);
			maincmd = strtok(NULL," ");
			j++;
		}
		args[j] = (char*) NULL;
		pid_t pid = fork();
		if(pid==0) {execv(bincmd,args); exit(0);}
	
	}
        for(int i = 0; i < argc/2; i++) wait(NULL); 
	return 0;
}
