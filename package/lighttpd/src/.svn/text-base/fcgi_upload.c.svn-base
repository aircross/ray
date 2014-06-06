#include "fcgi_stdio.h" /* fcgi library; put it first*/
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "fcgi_nas_cgiutil.h"
#include "fcgi_upload.h"
#include "fcgi_memdbg.h"

//extern char remote_user[MAX_USER_LEN];
#define MAX_USER_LEN 256

extern char remote_user[MAX_USER_LEN];
extern char user_sid[32];

/*******************************************************
                    Upload
 ******************************************************/
static ssize_t savefile(const char *filepath, const void *buf, size_t size)
{
    int fd = open(filepath, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (fd < 0) return -1;
    ssize_t count = write(fd, buf, size);
    close(fd);
    return count;
}

int op_get_chunked_upload_id(INPUT *input) 
{
    INPUT *tmp;
    char id[32] = "---";
    int euid, egid, ret = 0;
    int exist;
    char *full_path, path[1024];
    
    char lastpath[1024] = "", *filename = NULL;
    int length = 0;
    int fd = -1;
    if( (tmp = CGI_Find_Parameter(input, "upload_root_dir")) ) {
       snprintf(path, sizeof(path), "%s/%s", tmp->val, UPLOAD_CACHE_DIR);
    }

    full_path = (char *)__ax_calloc(1, strlen(path) + MAX_FILE_LENGTH + 15);
    exist = check_exist(path, full_path);

    if (exist < 0) {
       if(mkdir(full_path, S_IRUSR | S_IWUSR | S_IXUSR |S_IRGRP | S_IWGRP | S_IXGRP |S_IROTH | S_IWOTH | S_IXOTH)< 0){
                ret = WFM2_OPEN_FILE_FAIL;
                       goto end;
       }
       chmod(full_path, S_IRUSR | S_IWUSR | S_IXUSR |S_IRGRP | S_IWGRP | S_IXGRP |S_IROTH | S_IWOTH | S_IXOTH);
    }

    // Create unique file
    strcpy(lastpath, full_path);
    strcat(lastpath, "/");
    length = strlen(lastpath);
    strcat(lastpath, "XXXXXXX");
    mktemp(lastpath);    

    fd = open(lastpath, O_CREAT | O_RDWR);
    if (fd == -1) {
		ret = WFM2_OPEN_FILE_FAIL;
		goto end;
    }
    filename = __ax_strndup(lastpath + length, 7);
             
end:
    printf("{\"status\": %d, \"upload_id\":\"%s\"}\n", ret, filename);
    if (filename) __ax_free(filename);
    if (fd != -1) close(fd);
    if (full_path) __ax_free(full_path);
    return 0;
}

int op_chunked_upload(INPUT *input)
{
    INPUT    *tmp; 
    int     exist = 0, overwrite = 0, ret = 0;
    char    *dest_path = "", src_name[256], *upload_id = "", *upload_dir = NULL, size_str[64] = "0";
    char    *path_tmp = NULL, *full_path, *full_upload_path, *full_src_path, *type, *progress;
    char    *rename_string, *full_rename_path, *upload_name;
    int     euid, egid, pid, multipart = 0;
    long long offset = 0, filesize = 0, cache_filesize = 0;
    FILE    *fp = NULL;
    struct  stat     st_buf;
    time_t  mtime = 0, actime = 0;

    if( (tmp=CGI_Find_Parameter(input, "dest_path")) ) 
         dest_path = tmp->val;

    if( (tmp=CGI_Find_Parameter(input, "rename_dest")) ) {
        rename_string = tmp->val;
        if(strlen(rename_string) == 0)
            rename_string = NULL;
    } 

    if( (tmp=CGI_Find_Parameter(input, "upload_name")) ) {
        upload_name = tmp->val;
        if(strlen(upload_name) == 0)
           upload_name = NULL;
    }
    
    //path_tmp = (char *)__ax_calloc(1, strlen(dest_path) + 1);

    //strcpy(path_tmp, dest_path);
/*
    if(check_user_read_write_privilege(remote_user, path_tmp) < 0){
       if(path_tmp) __ax_free(path_tmp);
       //Status_With_Size_To_JSON(WFM2_PERMISSION_DENY, size_str);
       return -1;
    }
*/
    if( (tmp=CGI_Find_Parameter(input, "overwrite")) ) {
         overwrite = atoi(tmp->val);
    }

    if( (tmp=CGI_Find_Parameter(input, "type")) ) {
         type = tmp->val;
    }

    if( (tmp=CGI_Find_Parameter(input, "offset")) ) {
         offset = atoll(tmp->val);
    }

    if( (tmp=CGI_Find_Parameter(input, "filesize")) ) {
        filesize = atoll(tmp->val);
    }

    if( (tmp=CGI_Find_Parameter(input, "multipart")) ) {
         multipart = atoll(tmp->val);
    }

    if( (tmp=CGI_Find_Parameter(input, "upload_id")) ) {
         upload_id = tmp->val;
    }

    //if(path_tmp) __ax_free(path_tmp);

    if( (tmp=CGI_Find_Parameter(input, "progress")) ) {
        progress = (char *)__ax_calloc(1, strlen(tmp->val) + 6);
        if(strlen(tmp->val) > MAX_FILE_NAME_LENGTH) {
            // if progress name too long.
            //_get_progress_time_string(progress);
        }
        else {
            sprintf(progress, "/tmp/%s", tmp->val);
        }
        //sprintf(progress, "/tmp/%s", tmp->val);

        fp=fopen(progress, "w+");
        if (fp == NULL) {
            if (progress) __ax_free(progress);
            // Status_With_Size_To_JSON(WFM2_FAIL, size_str);
            return -1;
        }
        fclose(fp);

//         Set_Private_Profile_Integer("", UPLOAD_PID, pid, progress);
        if (progress) __ax_free(progress);
    }
   
    // Become_ID(remote_user, &euid, &egid);

    if((tmp=CGI_Find_Parameter(input, "upload_root_dir")) ) {
        char uppath[1024];
        snprintf(uppath, sizeof(uppath), "%s/%s", tmp->val, UPLOAD_CACHE_DIR);
        upload_dir = (char *)__ax_calloc(1, strlen(uppath) + MAX_FILE_LENGTH + 15);
        exist = check_exist(uppath, upload_dir);
        if (exist < 0) {
            //Status_With_Size_To_JSON(WFM2_FILE_NO_EXIST, size_str);
            if (upload_dir) __ax_free(upload_dir);
            return -1;
        }
    }

    // Finish uploading, now rename (move) it to real file and name
    if((tmp=CGI_Find_Parameter(input, "Filename")) && (upload_id != NULL) ) {

        char final_dest_path[2048], tmpfile[2048];

        // Destination folder path
        strcpy(final_dest_path, "/tmp/mnt");
        strcat(final_dest_path, dest_path);
        strcat(final_dest_path, "/");
		char *file_name = tmp->val;
        if (file_name[0] == '\\') {
            file_name++;
            strcat(final_dest_path, file_name);
        }
        else
            strcat(final_dest_path, file_name);

        // Original temp file path
        strcpy(tmpfile, upload_dir);
        strcat(tmpfile, "/");
        strcat(tmpfile, upload_id);


        ret = rename(tmpfile, final_dest_path) ;
        if(ret) FCGI_LOG("Fail to move from tmp file %s to %s\n", tmpfile, final_dest_path);
        

    }
    if (upload_dir) __ax_free(upload_dir);


    return ret;
}

int op_delete_chunked_upload_file(INPUT *input)
{
	INPUT   *tmp = NULL;
	int ret = 0, exist = 0;
	char *upload_id = NULL, *full_path = NULL, *upload_dir = NULL;
	struct stat statbuf;

	if( (tmp=CGI_Find_Parameter(input, "upload_id")) ) {
		upload_id = tmp->val;
	}

	if( (tmp=CGI_Find_Parameter(input, "upload_root_dir")) ) {
		char uppath[1024];
		snprintf(uppath, sizeof(uppath), "%s/%s", tmp->val, UPLOAD_CACHE_DIR);
		upload_dir = (char *)calloc(1, strlen(uppath) + MAX_FILE_LENGTH + 15);
		exist = check_exist(uppath, upload_dir);
		if (exist < 0) {
			ret = WFM2_FILE_NO_EXIST;
			goto end;
		}
		full_path = (char *)calloc(1, strlen(upload_dir) + strlen(upload_id) + 2);
		sprintf(full_path, "%s/%s", upload_dir, upload_id);
	}
	if(!stat(full_path, &statbuf)){
		if(S_ISDIR(statbuf.st_mode))
			ret = WFM2_OPEN_FILE_FAIL;
		else
#ifdef RECYCLE_EX
			qnap_unlink(full_path);
#else
		unlink(full_path);
                 
#endif
	}else
		ret = WFM2_FILE_NO_EXIST;

end:
	if(full_path)
		free(full_path);
         if(upload_dir)
                free(upload_dir);

	Status_To_JSON(ret);
	return ret;

}

int op_get_chunked_upload_status(INPUT *input)
{
    INPUT	*tmp = NULL;
    int ret = 0, exist = 0, count = 0;
    char *upload_id = NULL, *full_path = NULL, size_str[64] = "0", size_str2[64] = "0", *upload_dir = NULL;
    struct stat statbuf;
    unsigned long long size = 0, total_len = 0;
    char id[32] = "---";
    //char buf[BLK_SIZE], md5_str[128] = "---";
    char buf[BLK_SIZE], md5_str[128] = {0};

    if( (tmp=CGI_Find_Parameter(input, "upload_id")) ) {
        upload_id = tmp->val;
    }

    if( (tmp=CGI_Find_Parameter(input, "upload_root_dir")) ) {
        char uppath[1024];
        snprintf(uppath, sizeof(uppath), "%s/%s", tmp->val, UPLOAD_CACHE_DIR);
        upload_dir = (char *)calloc(1, strlen(uppath) + MAX_FILE_LENGTH + 15);
        exist = check_exist(uppath, upload_dir);
        if (exist < 0) {
            ret = WFM2_FILE_NO_EXIST;
            goto end;
        }
        full_path = (char *)calloc(1, strlen(upload_dir) + strlen(upload_id) + 2);
        sprintf(full_path, "%s/%s", upload_dir, upload_id);
    }

    if(!stat(full_path, &statbuf)){
        if(S_ISDIR(statbuf.st_mode)){
            ret = WFM2_FILE_NO_EXIST;
        }else{
            size = statbuf.st_size;
            sprintf(size_str, "%lld", statbuf.st_size);
        }
    }else
        ret = WFM2_FILE_NO_EXIST;

end:
    if(full_path)
        free(full_path);
    if(upload_dir) 
        free(upload_dir);
    Status_With_Size_And_MD5_To_JSON(ret, size_str, md5_str, size_str2);
    return ret;
}
