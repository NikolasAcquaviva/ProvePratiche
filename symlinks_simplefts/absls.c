#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fts.h>
int main(int argc, char* argv[]){
	char* arg[argc];
	arg[0] = argv[1];
	arg[1] = NULL;
	FTS* handle = malloc(256*sizeof(FTS));
	handle = fts_open(arg,FTS_LOGICAL,NULL);
	FTSENT* tmp = malloc(256*sizeof(FTSENT));
	while((tmp=fts_read(handle))!=NULL){
		printf("%s\n",realpath(tmp->fts_name,NULL));		
	}	
	fts_close(handle);
	return 0;
}
