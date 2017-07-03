#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <utime.h>

#define UNKNOWN 2
#define FILE 1
#define FOLDER 0

void doTouch(char* file) {
	struct utimbuf *times;
	times = (struct utimbuf*) malloc(sizeof(struct utimbuf));
	// get current time
	time_t atime = time();
	time_t mtime = time();
	// set up timestamps
	times -> actime = atime;
	times -> modtime = mtime;
	// modify time
	if(utime(file, times) == -1) {
		perror("utime");
		exit(EXIT_FAILURE);
	}
}


int checkType(struct dirent *dp) {
	struct stat file_stat;
	if(stat(dp -> d_name, &file_stat) == -1) {
		perror("read");
		return UNKNOWN;
	}

	// check is ordinary file
	if(S_ISREG(file_stat.st_mode)) {
		return FILE;
	}
	// check is folder
	if(S_ISDIR(file_stat.st_mode)) {
		return FOLDER;
	}

	return UNKNOWN;
}

void dfs(char *path) {
	DIR *dirp;
	struct dirent *dp;
	char curDir[1024];
	getcwd(curDir, sizeof(curDir));
	printf("Current Dir: %s\n", curDir);

	if(!(dirp = opendir("."))) {
		perror("open");
		return;
	}

	while(dp = readdir(dirp)) {
		// prevent infinite loop
		if(!strcmp(dp->d_name,".") || !strcmp(dp->d_name,".."))	continue;
		
		// check folder
		if(checkType(dp) == FOLDER) {
			char cwd[1024];
			getcwd(cwd, sizeof(cwd));

			if(chdir(dp->d_name) == -1) {
				perror("chdir");
				exit(EXIT_FAILURE);
			}
			
			// processing on child directories
			dfs(cwd);
			
			if(chdir("..") == -1) {
				perror("chdir");
				exit(EXIT_FAILURE);
			}
		}

		if(checkType(dp) == FILE || checkType(dp) == UNKNOWN) 
			doTouch(dp -> d_name);
	}
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Usage: experiment [/path-to-beginning-folder]");
		exit(EXIT_FAILURE);
	}

	// redirect to specified path
	char *path = argv[1];
	if(chdir(path) == -1) {
		perror("chdir");
		exit(EXIT_FAILURE);
	}

	// get cwd
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	
	// touching
	dfs(cwd);

	//resume
	chdir(cwd);
}

