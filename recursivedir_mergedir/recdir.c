#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int listdir(const char *name, int indent)
{
	int count = 0;
	DIR *dir = opendir(name);    
	struct dirent *entry;
	
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
	    count++;
            listdir(path, indent + 2);
        } else {
		
		struct stat sb;
		stat(entry->d_name,&sb);
		if(S_ISREG(sb.st_mode)) count++;
        }
    }
    closedir(dir);
    return count;
}

int main(int argc,char* argv[]){
	int ok;
	if(argc==1) ok = listdir(".",0);
	else ok = listdir(argv[1],0);
	printf("numero file regolari/directory: %d\n", ok);
	return 0;
}
