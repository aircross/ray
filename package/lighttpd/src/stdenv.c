#include "fcgi_stdio.h" /* fcgi library; put it first*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fcgi_common.h"

extern int fcgi_handle_auth_login(void);
extern int util_request(void);
extern int sysinfo_req(void);
static char fcgi_log_buf[256];
extern char **environ;
int count;

typedef enum _QCGI_Query_Cmd_ID {
    E_CMD_AUTHLOGIN,
    E_CMD_UTILREQUEST,
    E_CMD_SYSINFOREQUEST,
    E_CMD_GETTREE,
    E_CMD_GETLIST,
    E_CMD_SENDFILE,
    E_NUM_QFILE_CMDS,
    E_CMD_INVALID = 0xFF
} QCGI_Query_Cmd_ID;

typedef struct _QCGI_Query_Cmd {
    char *key;
    QCGI_Query_Cmd_ID id;
} QCGI_Query_Cmd;

QCGI_Query_Cmd qfile_query_cmds[] = {
    {"authLogin", E_CMD_AUTHLOGIN},
    {"utilRequest", E_CMD_UTILREQUEST},
    {"sysinfoReq", E_CMD_SYSINFOREQUEST},
    {"sendfile", E_CMD_SENDFILE},
    {NULL, E_CMD_INVALID},
};

void sendfile(void) {
    printf("X-Sendfile2: /tmp/mnt/Videos/VIDEO0003.mp4 342920000-343921000\r\n"
            "\r\n"
            "X-Sendfile2: /tmp/mnt/USB/test.html \n");
}

static QCGI_Query_Cmd_ID query_string_mux(void) {
    int i;
    char *query_string = NULL;
    for(i = 0; environ[i] != NULL; i++) {
        if(!strncmp(environ[i], "REQUEST_URI", (sizeof("REQUEST_URI") - 1))) {
            query_string = environ[i];
            break;
        }
    }
    if(!environ[i] || !query_string)
        return E_CMD_INVALID;
    for(i = 0; qfile_query_cmds[i].id != E_CMD_INVALID;i ++) {
        if(strstr(query_string, qfile_query_cmds[i].key)) {
            return qfile_query_cmds[i].id;
        }
    }
    return E_CMD_INVALID;
}

void main(void)
{
    char *envvar = NULL;
    int i;
    QCGI_Query_Cmd_ID qcmd_id;

    /* Response loop. */
    while (FCGI_Accept() >= 0)   {
        qcmd_id = query_string_mux();
        switch(qcmd_id) {
            case E_CMD_AUTHLOGIN:
                fcgi_handle_auth_login();
                break;
            case E_CMD_UTILREQUEST:
                util_request();
                break;
            case E_CMD_SYSINFOREQUEST:
                sysinfo_req();
                break;
            case E_CMD_SENDFILE:
                sendfile();
                break;
            default:
                printf("<i>Under construction</i>\n");
                FCGI_LOG("Unimplemented command\n");
                break;
        }
        /*
        printf("Content-type: text/html\r\n"
                "\r\n"
                "<title>FastCGI Hello! (C, fcgi_stdio library)</title>"
                "<h1>FastCGI Hello! (C, fcgi_stdio library)</h1>"
                "Request number %d running on host <br/>\n",
                ++count);
        for(i = 0; ((envvar = environ[i]) != NULL);i ++) {
            printf("<i>%s</i><br/>\n", envvar);
        }
        */
    }
}
