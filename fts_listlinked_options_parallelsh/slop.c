#include <wait.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fts.h>
int main(int argc, char* argv[]){
	char* arg[argc];
	char *abs = malloc(256*sizeof(char));
	abs = realpath(argv[1],NULL);
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
			   st->st_ino == input_inode){

				if(strcmp(argv[3],"-c")==0){
					pid_t pid = fork();
					if(pid==0){
					remove(tmp->fts_name);
					execl("/bin/cp","/bin/cp",abs,tmp->fts_name,NULL);
					}
					else waitpid(-1,NULL,0);
				}
				else if(strcmp(argv[3],"-s")==0){
					pid_t pid = fork();
					if(pid==0){
					remove(tmp->fts_name);
					execl("/bin/ln","/bin/ln", "-s", abs, tmp->fts_name,NULL);
					}
					else waitpid(-1,NULL,0);
				}

			
				else
					printf("%s è hard link\n",tmp->fts_name);
			}	
		}
		else if(S_ISLNK(st->st_mode)){
			int l = strlen(argv[1]);
			char buffer[64];
	        	readlink(tmp->fts_name,buffer,64);	
	        	if(strstr(buffer,argv[1])){
				if(strcmp(argv[3],"-c")==0){
					pid_t pid = fork();
					if(pid==0){	
                                	remove(tmp->fts_name);
                                	execl("/bin/cp","/bin/cp",abs,tmp->fts_name,NULL);
					}
					else waitpid(-1,NULL,0);
				}
                        	else if(strcmp(argv[3],"-l")==0){
					pid_t pid = fork();
					if(pid==0){
                                	remove(tmp->fts_name);
                                	execl("/bin/ln","/bin/ln",abs,tmp->fts_name,NULL);
					}
					else waitpid(-1,NULL,0);
				}
				else printf("%s is symbolic\n", tmp->fts_name);
			} 
			
		}		
	}	
	fts_close(handle);
	return 0;
}
