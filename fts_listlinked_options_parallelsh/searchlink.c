#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fts.h>
int main(int argc, char* argv[]){
	char* arg[argc];
	struct stat input;
	int input_inode;
	stat(argv[1],&input);
	input_inode = input.st_ino;
	arg[0] = argv[2];
	arg[1] = NULL;
	FTS* handle = malloc(256*sizeof(FTS));
	handle = fts_open(arg,FTS_PHYSICAL,NULL);
	FTSENT* tmp = malloc(256*sizeof(FTSENT));
	while((tmp=fts_read(handle))!=NULL){
		struct stat *st = tmp->fts_statp;
		if(S_ISREG(st->st_mode)){
			if(strcmp(tmp->fts_name,argv[1]) && 
			   st->st_ino == input_inode)
				printf("%s è hard link\n",tmp->fts_name);
		}
		else if(S_ISLNK(st->st_mode)){
		int l = strlen(argv[1]);
		char buffer[64];
	        readlink(tmp->fts_name,buffer,64);	
	        if(strstr(buffer,argv[1])) printf("%s is symbolic\n", tmp->fts_name);	
		}		
	}	
	fts_close(handle);
	return 0;
}
