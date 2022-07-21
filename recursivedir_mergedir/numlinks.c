#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int max = 0;

void maxlinks(const char *name)
{
        DIR *dir = opendir(name);
        struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            maxlinks(path);
        } else {

                struct stat sb;
                stat(entry->d_name,&sb);
                if(S_ISREG(sb.st_mode) && sb.st_nlink > max) 
			max = sb.st_nlink; 
        }
    }
    closedir(dir);
}

void listdir(const char *name, int nlinks)
{
	DIR *dir = opendir(name);    
	struct dirent *entry;
	
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            listdir(path, nlinks);
        } else {
		
		struct stat sb;
		stat(entry->d_name,&sb);
		if(S_ISREG(sb.st_mode) && sb.st_nlink==nlinks)
		       printf("%s\n",entry->d_name);	
        }
    }
    closedir(dir);
}

int main(int argc,char* argv[]){
	if(argc==1) {
		maxlinks(".");
		for(int i = 1; i <= max; i++){
                	printf("stampo file con %d links:\n",i);
                	listdir(".",i);
                	printf("\n");
        	}
	}
	else {
		maxlinks(argv[1]);
		for(int i = 1; i <= max; i++){
			printf("stampo file con %d links:\n",i);
			listdir(argv[1],i);
			printf("\n");
		}
	}
	return 0;
}
