//black magic incantation.
#define _XOPEN_SOURCE 500

#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/hmac.h>

//TODO eventually anonymizer 
//TODO eventually have this buffered and periodically dump out to a csv

void print_stat(const struct stat *sb);
int info_dump(const char *fpath, const struct stat *sb, int typeflag, struct FTW * ftwbuf);
char * make_csv(const struct stat *sb, const char *fpath);

FILE *ofp;

void print_stat(const struct stat *sb){
	
	
	printf("Device ID: %d\n",sb->st_dev);
	printf("Inode: %d\n", sb->st_ino);
	printf("HardLink Count: %d\n", sb->st_nlink);
	printf("User ID: %d\n", sb->st_uid);
	printf("Group ID: %d\n", sb->st_gid);
	printf("ATime: %d\n", sb->st_atime);
	printf("Mtime: %d\n", sb->st_mtime);
	printf("CTime: %d\n", sb->st_ctime);
	printf("Size: %d\n", sb->st_size);
	printf("Block Size: %d\n", sb->st_blksize );
	printf("Number of Blocks: %d\n", sb->st_blocks);
}

char * make_csv(const struct stat *sb, const char *fpath){

	//use the malloc hammer, allocate more than we need
	char *csv_string = NULL;
	csv_string = malloc(sizeof(char)*4096);


	//use a gigantic sprintf call to make our string
	sprintf(csv_string, "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
		fpath,
		sb->st_dev, sb->st_ino, sb->st_nlink, sb->st_uid,
		sb->st_gid, sb->st_atime, sb->st_mtime,  sb->st_ctime,
		sb->st_size,  sb->st_blksize, sb->st_blocks);

	return csv_string;
}


// in theory, this will hash things by path component
// and return a string TODO, kinda borked right now
// tokenizing 
char * hash_fpath(const char * fpath, char *hmac_key){

	char *hashed_path_ptr = NULL; 
	char *cur_token_ptr = NULL;
	char split_on[2] = "/";
	char *fpath_copy = NULL;
	unsigned char * digest;
	int i = 0;
	char mdString[20];
	
	
	//make a copy of fpath
	printf("-----\nPathsize:%d\n------\n", strlen(fpath));
	fpath_copy = malloc(sizeof(char)*(strlen(fpath))+1);
	strcpy(fpath_copy, fpath);	

	//tokenize
	//get first token
	cur_token_ptr = strtok(fpath_copy, split_on);
	printf("Beging Printing Tokens!\n");
	while( cur_token_ptr != NULL){

		printf("%s\n", cur_token_ptr);
		//digest = HMAC(EVP_sha1(), hmac_key, strlen(hmac_key), (unsigned char*)cur_token_ptr, strlen(cur_token_ptr), NULL, NULL);
		//for(i=0;i<20;i++){
		//	sprintf(&mdString[i*2],"%02x", (unsigned int)digest[i]);
		//}
		cur_token_ptr = strtok(NULL, split_on);
		//printf("%s\n",mdString);
	}
	
	free(fpath_copy);
}

int info_dump(const char *fpath, const struct stat *sb, int typeflag, struct FTW * ftwbuf){

	char * csv_string = NULL;

	// Checking out 
	printf("--------\n%s\n-----------\n", fpath);
	
	char *stat_info = "  ";

	//first, tells us about the type flag
	if(typeflag == FTW_F){
		stat_info = "fl";
	}else if(typeflag == FTW_D){
		stat_info = "dr";
	}else if(typeflag == FTW_DNR){
		stat_info = "ff";
	}else if(typeflag == FTW_DP){
		stat_info = "dp";
	}else if(typeflag == FTW_NS){
		stat_info = "sf";
	}else if(typeflag == FTW_SL){
		stat_info = "sl";
	}else if(typeflag == FTW_SLN){
		stat_info = "mf";
	}

	//now try to get out the stat info
	//print_stat(sb);
	csv_string = make_csv(sb, fpath);
	fprintf(ofp, csv_string);
	//printf("%s", csv_string);
	//hash_fpath(fpath, "foo");
	//free(csv_string);

	return 0;
}



int main(int argc, char *argv){

	int flags = 0;
	char *outpath = "testout.dat";
	char *mode = "w+";
	//open file handle for output
	ofp = fopen(outpath, mode);	

	nftw("/", info_dump, 20, flags);
	fclose(ofp);	

	return 0;

}
