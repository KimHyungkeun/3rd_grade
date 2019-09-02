#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void search_directory(char *dir) {

	struct dirent *dentry;
	struct stat statbuf;
	DIR *dirp;
	char cwd[1024];

	getcwd(cwd, 1024);

	if(lstat(dir, &statbuf) < 0) {
		fprintf(stderr, "lstat error\n");
		exit(1);
	}

	if(!S_ISDIR(statbuf.st_mode)) {
		printf("%s/%s\n", cwd, dir);
		return;
	}

	if((dirp = opendir(dir)) == NULL) {
		fprintf(stderr, "opendir error\n");
		exit(1);
	}

	chdir(dir);
	while((dentry = readdir(dirp)) != NULL) {
		if(strcmp(dentry -> d_name, ".") == 0 || strcmp(dentry -> d_name, "..") == 0) {
			continue;
		}

		search_directory(dentry -> d_name);
	}

	if(strcmp(".", dir) != 0)
		printf("%s/%s\n", cwd, dir);

	chdir(cwd);

	closedir(dirp);
}

int main(int argc, char *argv[]) {
	
	if(argc < 2) {
		fprintf(stderr, "Usage : %s directoryname\n", argv[0]);
		exit(1);
	}

	search_directory(argv[1]);
}
