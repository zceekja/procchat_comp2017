#include "server.h"

static int  client_handler_pid = -1;
static int  client_pong_pid = -1;
static char client_RD[2048] ={0};
static char client_WR[2048] = {0};
static char domain[1790] = {0};
struct stat st = {0};

void sig_alrm_handler(int signum){
    remove(&client_RD[0]);
    remove(&client_WR[0]);
    kill(client_pong_pid, SIGKILL);
    rmdir(&domain[0]);
    exit(0);
}

void sig_chid_handler(int signum)
{
    wait(NULL);
}

int main(int argc, char** argv) {

    signal(SIGCHLD, sig_chid_handler);  // To prevent zomebie process
    signal(SIGALRM,sig_alrm_handler);   
    mkfifo("gevent", 0777);
    unsigned char buffer[2048] = {0};
    char identifier[256] = {0};
    unsigned char message[1790] = {0};
    int size;
    int pid;
    int gevent =  open("gevent", O_RDONLY);
    while(1){
        size = read(gevent,buffer,2048);
        if(size < 0){
            fprintf(stderr, "Error in Global\n"); 
            exit(1);
        }
        else if ( size == 2048){
            // CONNECT <indentifier> <domain> 
            if((int)*buffer == 0 && (int)*(buffer+1)==0){
                memcpy(&domain,&buffer[258],1790);
                memcpy(&identifier,&buffer[2],256);
                if(stat(domain, &st) == -1){
                    mkdir(domain,0777);
                }
                strcat(&client_RD[0] ,&domain[0] );
                strcat(&client_RD[0], "/" );
                strcat(&client_RD[0], &identifier[0] );
                strcat(&client_RD[0], "_RD\0" );
                strcat(&client_WR[0],&domain[0] );
                strcat(&client_WR[0], "/" );
                strcat(&client_WR[0], &identifier[0] );
                strcat(&client_WR[0], "_WR\0" );
                mkfifo(&client_RD[0], 0777);
                mkfifo(&client_WR[0], 0777);
                pid = fork();  
                close(gevent);
                //Child process
                if( pid == 0){
                    break;
                }
            }
            gevent = open("gevent", O_RDONLY);
            memset(identifier,0,sizeof(identifier));
            memset(domain, 0, sizeof(domain));
            memset(client_RD, 0, sizeof(client_RD));
            memset(client_WR, 0, sizeof(client_WR));
        }
    }
    //Client handler
    int pid2 = fork();
    int pipe_RD;
    if (pid2 != 0){
        client_pong_pid = pid2;
        client_handler_pid = getpid();
        alarm(16);
    }

    while(1){
        if (pid2 == 0){
            break;
        }
        pipe_RD = open(client_WR, O_RDONLY);
        memset(buffer,0,sizeof(buffer));
        size = read(pipe_RD, buffer, 2048);
        if (size == 2048){
            // SAY
            if ((int)*buffer == 1 &&(int)*(buffer+1) ==0){
                memcpy(&message,&buffer[2],1790);
                sent_message(&message[0],&identifier[0],1);
            }
            // SAYCONT
            else if ((int)*buffer == 2 &&(int)*(buffer+1) ==0){
                memcpy(&message,&buffer[2],1789);
                message[1789] = (unsigned char)*(buffer+2047);
                sent_message(&message[0],&identifier[0],2);
            }
            // PONG
            else if ((int)*buffer == 6 &&(int)*(buffer+1) ==0){

                alarm(16); 
            }
            // DISCONNECT and CLIENT FAILURE.
            else {
                close(pipe_RD);
                alarm(1);
                sleep(2);
            }  
        }
        close(pipe_RD);    
    }
    //Child of Client Handler (process that sent PING to client every 15 seconds)
    int pipe_WR;
    memset(buffer,0,sizeof(buffer));
    buffer[0] = 5;
    while(1){
        sleep(15);
        pipe_WR = open(client_RD,O_WRONLY );
        write(pipe_WR,buffer,2048);
        close(pipe_WR);
    }
}
/*
void sent_message(unsigned char* message, char* identifier,int type)
    relay message to clients within the domain of sender
*/
void sent_message(unsigned char* message, char* identifier,int type){
    DIR* d;
    unsigned char buffer[2049]= {0};
    char* rd_name = client_RD + strlen(domain) + 1;
    if(type == 1){
        buffer[0] = 3;
    }
    else if(type == 2){
        buffer[0] = 4;
    }
    memcpy(&buffer[2],identifier,256);
    memcpy(&buffer[258],message,1790);
    char current_RD[2048]= {0};
    char domain_path[1792] = {0};
    domain_path[0] = '.';
    domain_path[1] = '/';
    struct dirent *dir;
    memcpy(&domain_path[2],domain,strlen(domain));
    int pipe_WR;
    d = opendir(&domain_path[0]);
    char* from_last_underscore;
    if(d){
        while((dir = readdir(d)) != NULL){
            if(strcmp(dir->d_name,rd_name) == 0){
                ; // Do nothing, client do not send message to itself/
            }
            else{
                memset(current_RD,0,sizeof(current_RD));
                memcpy(&current_RD[0],domain,strlen(domain));
                memcpy(&current_RD[strlen(domain)],"/",1);
                memcpy(&current_RD[strlen(domain)+1],dir->d_name,strlen(dir->d_name));
                from_last_underscore = strrchr(dir->d_name,'_');
                if(from_last_underscore && !strcmp(from_last_underscore,"_RD")){
                    pipe_WR = open(current_RD, O_WRONLY);
                    write(pipe_WR,buffer,2048);
                    close(pipe_WR);
                }
            }
        }
        closedir(d);
    }
    return;
}