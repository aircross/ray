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
int open_tcp_client(char *ip_addr, unsigned short int port)
{
    int sockfd = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 
    struct hostent * remoteHost;

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return -1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 

    if( (remoteHost = gethostbyname(ip_addr)) == 0 )
    {
        printf("Error resolving host\n");
        return -2;
    }

    serv_addr.sin_addr.s_addr = ( (struct in_addr *)(remoteHost->h_addr) )->s_addr;
    //if(inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr)<=0)
    //{
    //    printf("\n inet_pton error occured\n");
    //    return -2;
    //} 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return -3;
    } 
    connfd = sockfd;
    return sockfd;
}

int close_tcp_client(int clifd) {
    if(clifd < 1) {
        printf("Not a volid connection socket:%d\n", clifd);
        return -1;
    }
    close(clifd);
    connfd = -1;
    return 0;
}

char* str_change(char str[])
{
    int i;

    for(i=0;i < strlen(str);i++)
    {
        if(str[i] >= 'A' && str[i] <= 'Z')
            str[i]+=32;
    }
    return str;
}    

void main( void )
{
    char* host = "standards.ieee.org";
    char *name = "/tmp/mac_access.h";
    int port = 80,sockfd;
    char buff[2048],message[2048],mac[16];
    char ch_1[8],ch_2[8],ch_3[8];
    char *str,*delime="<hr><p><pre>";
    FILE *fp,*file;

    if ((sockfd = open_tcp_client(host,port)) < 0 )
    {
        printf("Error opening socket!\n");
        exit(1);
    }

    memset(message,0,sizeof(message));
    memset(buff,0,sizeof(buff));

    sprintf(message,"GET /cgi-bin/ouisearch?1%20Infinite%20Loop HTTP/1.1\r\n");
    strcat(message,"Host:standards.ieee.org\r\n");
    strcat(message,"User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:29.0) Gecko/20100101 Firefox/29.0\r\n");
    strcat(message,"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
    strcat(message,"Accept-Language: zh-tw,zh;q=0.8,en-us;q=0.5,en;q=0.3\r\n");
    strcat(message,"Connection: keep-alive\r\n");
    strcat(message,"Cache-Control: max-age=0\r\n");
    strcat(message, "\r\n\r\n");


    if( send(sockfd, message, strlen(message), 0) == -1)
    {
        printf("Error in send\n");
        exit(1);
    }
    
    struct timeval timeout = {1,0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

    fp=fopen("/tmp/123.tmp","w");
    for(;recv(sockfd, buff, sizeof(buff), 0);)
    {
        fprintf(fp,"%s",buff);
        memset(buff,0,sizeof(buff));
    }
    fclose(fp);
    close_tcp_client(sockfd);

    fp=fopen("/tmp/123.tmp","r");
    file=fopen(name,"w");

    fprintf(file,"char *mac_access[] = {\n");
    while (fgets(buff,sizeof(buff),fp) != NULL)
    {
      str = strtok(buff,delime);
      while(str != NULL)
      {
          if (strstr(str,"-") - str == 4 )
          {    
              sscanf(str,"  %[^-]-%[^-]-%s",ch_1,ch_2,ch_3); 

              if((strlen(ch_1)+strlen(ch_2)+strlen(ch_3)) == 6)
                fprintf(file,"\t\"%s:%s:%s\",\n",str_change(ch_1),str_change(ch_2),str_change(ch_3));
          }
          str = strtok(NULL,delime);
      }
    }
    fprintf(file,"};");
    fclose(file);
    fclose(fp);
}
