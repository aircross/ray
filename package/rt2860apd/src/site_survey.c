#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

static const char base64_str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char base64_pad = '=';

static const signed char base64_table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

static int base64decode_block(unsigned char *target, const char *data, size_t data_size)
{
    int w1,w2,w3,w4;
    int i;
    size_t n;

    if (!data || (data_size <= 0)) {
        return 0;
    }

    n = 0;
    i = 0;
    while (n < data_size-3) {
        w1 = base64_table[(int)data[n]];
        w2 = base64_table[(int)data[n+1]];
        w3 = base64_table[(int)data[n+2]];
        w4 = base64_table[(int)data[n+3]];

        if (w2 >= 0) {
            target[i++] = (char)((w1*4 + (w2 >> 4)) & 255);
        }
        if (w3 >= 0) {
            target[i++] = (char)((w2*16 + (w3 >> 2)) & 255);
        }
        if (w4 >= 0) {
            target[i++] = (char)((w3*64 + w4) & 255);
        }
        n+=4;
    }
    return i;
}

unsigned char *base64decode(const char *buf, size_t *size)
{
    if (!buf) return NULL;
    size_t len = strlen(buf);
    if (len <= 0) return NULL;
    unsigned char *outbuf = (unsigned char*)malloc((len/4)*3+3);
    const char *ptr = buf;
    int p = 0;

    do {
        ptr += strspn(ptr, "\r\n\t ");
        if (*ptr == '\0') {
            break;
        }
        len = strcspn(ptr, "\r\n\t ");
        if (len > 0) {
            p+=base64decode_block(outbuf+p, ptr, len);
            ptr += len;
        } else {
            break;
        }
    } while (1);

    outbuf[p] = 0;
    *size = p;
    return outbuf;
}


int main()
{
    char *decode_ssid;
    char cur_ssid[256]={0};
    char cur_bssid[256]={0};
    char buf[512]={0}, wifi_buf[512]={0}, pre_ssid[256]={0};
    char *delim = "$";
    char *tmp = NULL;
    int i, time_interval, pre_sig, pre_wave;

    FILE *p, *time, *wifi_tmp;
    time = fopen("/tmp/wifi_site_time", "r");
    ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", cur_ssid, sizeof(cur_ssid), 0);
    ezplib_get_attr_val("wl0_apcli_rule", 0, "bssid", cur_bssid, sizeof(cur_bssid), 0);
    size_t len = 0;
    if(cur_ssid[0] != '\0') {
        decode_ssid = base64decode(cur_ssid, &len);
    }

    struct timespec cur_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cur_time);

    if(time!=NULL) {
        fgets(buf, sizeof(buf), time);
        double previous_time = atof(buf);
        time_interval = cur_time.tv_sec - previous_time;
        fclose(time);
    }
    sprintf(buf, "echo \'%ld\' > /tmp/wifi_site_time", cur_time.tv_sec);
    system(buf);

    if(time_interval>300) {
        unlink("/tmp/wifi_site_not_encode");
    }

    if((p = popen("/sbin/getsysinfo site_survey", "r")) == NULL)
    {
        printf("getsysinfo execute error!\n");
        return 0;
    }

    while(fgets(buf, sizeof(buf), p))
    {
        char line[256]={0};
        sprintf(line, "echo \'%s\' >> /tmp/wifi_site_not_encode", buf);
        system(line);
    }
    pclose(p);
    system("sort -f -t$ -k2 /tmp/wifi_site_not_encode -o /tmp/wifi_site_tmp");
    unlink("/tmp/wifi_site_not_encode");
    wifi_tmp = fopen("/tmp/wifi_site_tmp", "r");

    while(fgets(wifi_buf, sizeof(wifi_buf), wifi_tmp)!=NULL)
    {
        char ch[256]={0}, ssid[256]={0}, bssid[256]={0}, security[256]={0}, wmode[256]={0}, extch[256]={0}, nt[256]={0}; 
        int sig = 0; 
        tmp = strtok(wifi_buf, delim);
        int counter = 0;
        while(tmp!=NULL) {
            switch(counter) {
                case 0:
                    strcpy(ch, tmp);
                    break;
                case 1:
                    strcpy(ssid, tmp);
                    break;
                case 2:
                    strcpy(bssid, tmp);
                    break;
                case 3:
                    strcpy(security, tmp);
                    break;
                case 4:
                    sig = atoi(tmp);
                    break;
                case 5:
                    strcpy(wmode, tmp);
                    break;
                case 6:
                    strcpy(extch, tmp);
                    break;
                case 7:
                    strcpy(nt, tmp);
                    break;
            }
            tmp = strtok(NULL, delim);
            counter++;
        }
        if(!strcmp(ssid, pre_ssid)) {
            if(sig>pre_sig) {
                if(pre_wave!=0) {
                    sprintf(buf, "sed '$d' -i /tmp/wave%d", pre_wave);
                    system(buf);
                }
            }
            else
                continue;
        }

        strcpy(pre_ssid, ssid);
        pre_sig = sig;

        if((decode_ssid!=NULL) && !strcmp(ssid, decode_ssid)) {
            sprintf(buf, "echo \'%s$%s$%s$%s$%d$%s$%s$%s$\' > /tmp/wifi_site_not_encode", ch, ssid, bssid, security, sig, wmode, extch, nt);
            pre_wave = 0;
        }
        else if(sig>=76 && sig<=100){
            sprintf(buf, "echo \'%s$%s$%s$%s$%d$%s$%s$%s$\' >> /tmp/wave4", ch, ssid, bssid, security, sig, wmode, extch, nt);
            pre_wave = 4;
        }
        else if(sig>=51 && sig<=75) {
            sprintf(buf, "echo \'%s$%s$%s$%s$%d$%s$%s$%s$\' >> /tmp/wave3", ch, ssid, bssid, security, sig, wmode, extch, nt);
            pre_wave = 3;
        }
        else if(sig>=26 && sig<=50) {
            sprintf(buf, "echo \'%s$%s$%s$%s$%d$%s$%s$%s$\' >> /tmp/wave2", ch, ssid, bssid, security, sig, wmode, extch, nt);
            pre_wave = 2;
        }
        else if(sig>0 && sig<=25) {
            sprintf(buf, "echo \'%s$%s$%s$%s$%d$%s$%s$%s$\' >> /tmp/wave1", ch, ssid, bssid, security, sig, wmode, extch, nt);
            pre_wave = 1;
        }
        system(buf);
    }
    fclose(wifi_tmp);

    for(i=4;i>=1;i--) {
        char wave[256]={0};
        sprintf(wave, "cat /tmp/wave%d >> /tmp/wifi_site_not_encode", i);
        system(wave);
        sprintf(wave, "rm /tmp/wave%d", i);
        system(wave);
    }

    return 0;
}
