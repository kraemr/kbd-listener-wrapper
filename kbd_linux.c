#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include<sys/time.h>
#include <vector>

#define KEY_NOTPRESSED 0
#define KEY_PRESSED 1
#define KEY_HELD 2

struct key{
    __uint16_t key;
    __uint16_t state;
};
int shift_active(){

}

int strg_active(){
    
}
int timeout_ms = 5000;
struct key keys[256];
struct pollfd fds[1];
const int input_size = 4096;
unsigned char input_data[input_size];

int zero_keys(){
    for(int i = 0; i< 256; i++){
        keys[i].key = 0;
        keys[i].state = 0;
    }
}


void readkeys(){
    int ret=0;
    unsigned short curr_key= 0;
    unsigned short state = 0;
    while(1)
    {
        ret = poll(fds, 1, timeout_ms);
        if(ret>0)
        {
            if(fds[0].revents)
            {
                ssize_t r = read(fds[0].fd,input_data,input_size);
                if(r<0)
                {
                    printf("error %d\n",(int)r);
                    break;
                }
                else
                {
                    curr_key = input_data[20];
                    state = input_data[44];  //position 44 contains the state of the key
                    keys[curr_key].key = curr_key;
                    keys[curr_key].state = state;
                    

                    for(int i = 0; i < 256; i++){
                        if(keys[i].state == 0) continue;
                        else if(keys[i].state == KEY_PRESSED) printf("%u key is pressed ",keys[i].key);
                        else printf("%u key is held ",keys[i].key);
                    }
                    
                    printf("%u",state);
                    printf("\n");
                    memset(input_data,0,input_size);
                }
            }
            else
            {
                printf("error\n");
            }
        }
        else
        {
            printf("timeout\n");
        }
    }

}


int main(){
    char input_dev[] = "/dev/input/event3";
    int st;
    fds[0].fd = open(input_dev, O_RDONLY | O_NONBLOCK);
    zero_keys();
    if(fds[0].fd<0)
    {
        printf("error unable open for reading '%s'\n",input_dev);
        return(0);
    }


    memset(input_data,0,input_size);

    fds[0].events = POLLIN;
    readkeys();
    int exit_on_key_press_count = __INT_MAX__;
    close(fds[0].fd);
    return 0;
}
