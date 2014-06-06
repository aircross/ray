#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

static int connfd = -1;
static int debug=0;
static char *data;
int open_tcp_client(char *ip_addr, unsigned short int port)
{
    int sockfd = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        message_print("\n Error : Could not create socket \n");
        return -1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 

    if(inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr)<=0)
    {
        message_print("\n inet_pton error occured\n");
        return -2;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        message_print("\n Error : Connect Failed \n");
        return -3;
    } 
    connfd = sockfd;
    return sockfd;
}

int close_tcp_client(int clifd) {
    if(clifd < 1) {
        message_print("Not a volid connection socket:%d\n", clifd);
        return -1;
    }
    close(clifd);
    connfd = -1;
    return 0;
}

void get_data(char *ap_addr)
{
    FILE * fp;
    char buff[128];
    char mac[24],mac_ap[24],time[32],count[32],rssi[8];

    fp = popen("iwpriv ra0 sensekick","r");
    if( feof( fp ))
    {    
        message_print("File Error!!\n");
        return;
    }

    for(;strlen(fgets(buff, sizeof (buff),fp)) > 1;)
    {
        if (strlen(buff) < 24 )
            continue;
        sscanf(buff,"%s\t%s\t%s\t%s\t%s\n",mac,rssi,time,count,mac_ap);
        sprintf(data+strlen(data),"%s,%s,%s,%s,%s;",mac,time,rssi,count,ap_addr);
    }
    pclose(fp); 
    sprintf(data+strlen(data),"&");
}

void message_print(char *str)
{
    if (str && debug == 1)
        printf("%s\n",str);
}

int main( int argc, char **argv )
{
    int status,time=8;

    if(!argv[1] || !argv[2] || !argv[3])
    {
        printf("ERROR!!\n");
        exit(1);
    }

    if(argv[4] && argv[4] > 2)
        time= atoi(argv[4]) - 2;
    else 
        debug = 1;

    data = (char *)malloc(4096);

    while(1)
    {
        memset(data,0,sizeof(data));
        connfd = open_tcp_client(argv[1], atoi(argv[2]));
        if(connfd > 0)
        {
            struct timeval timeout = {1,0};
            setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

            sprintf(data,"client_id=4765272503474547&client_secret=niUQ2nYjRu8dBVvNENwELqtouWM3eqKB&cmd=am&location=");
            get_data(argv[3]);
            
            status = send(connfd,data,strlen(data),0);
            
            if(status > 0)
                message_print("OK\n");
            else
                message_print("Fail\n");

            sleep(2);
            close_tcp_client(connfd);
        }

        sleep(time);
    }
}
