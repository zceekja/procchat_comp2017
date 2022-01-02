#include "server.h"
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>


int main(void){
    char buffer[2049]= {0};
    char* identifier_ptr = &buffer[2];
    char* domain_ptr = &buffer[258];
    char input_buffer[3000];
    char* command;
    char* identifier;
    char* domain;

    buffer[0]= 0;
    buffer[1] = 0;
    char read_pipe[2000];
    char* read_pipe_ptr = & read_pipe[0];
    char write_pipe[2000];
    char* write_pipe_ptr =&write_pipe[0];
    FILE * fa;



    while (1){
        for(int i =0; i <2048;i++){
            buffer[i]=0;
        }
        fgets(input_buffer,3000,stdin);
        command = strtok(input_buffer," ");
        if (strcmp(command,"CONNECT")== 0){
            identifier = strtok(NULL," ");
            domain = strtok(NULL, "\n");
            buffer[0] =0;
            buffer[1] =0;
            memcpy(identifier_ptr,identifier,strlen(identifier));
            memcpy(domain_ptr,domain,strlen(domain));
            memcpy(&buffer[2],identifier,strlen(identifier));
            memcpy(&buffer[258],domain,strlen(domain));
            int gevent = open("gevent", O_WRONLY);
            memcpy(&read_pipe[0],domain,strlen(domain));
            memcpy(&read_pipe[strlen(domain)],"/",1);
            memcpy(&read_pipe[strlen(domain)+1],identifier,strlen(identifier));
            memcpy(&read_pipe[strlen(domain)+1+strlen(identifier)],"_RD\0",4);

            memcpy(&write_pipe[0],domain,strlen(domain));
            memcpy(&write_pipe[strlen(domain)],"/",1);
            memcpy(&write_pipe[strlen(domain)+1],identifier,strlen(identifier));
            memcpy(&write_pipe[strlen(domain)+1+strlen(identifier)],"_WR\0",4);

            write(gevent,buffer,2048);
            break;
        }
    }

    char read_pipe_path[2000] = {0};

    read_pipe_path[0] = '.';
    read_pipe_path[1] = '/';
    memcpy(&read_pipe_path[2],read_pipe_ptr,strlen(read_pipe_ptr));
    int write_Pipe; 
    int size;
    sleep(1);
    char* msg =&buffer[258];
    int read_Pipe;
    while(1){
        read_Pipe = open(read_pipe_ptr, O_RDONLY);
        size = read(read_Pipe, &buffer, 2048);
        //printf("Size is:%d\n",(int)size);
        if (size <0){
            ;
        }
        else if (size == 2048){
            //SAY <message>

            if((int)*buffer == 1 && (int)*(buffer+1)==0 ){

                write_Pipe = open(write_pipe_ptr, O_WRONLY);
                write(write_Pipe,buffer,2048);
            }
            else if((int)*buffer == 2 && (int)*(buffer+1)==0 ){
  
                write_Pipe = open(write_pipe_ptr, O_WRONLY);
                write(write_Pipe,buffer,2048);
            }
            else if ((int)*buffer == 3 && (int)*(buffer+1)== 0 ){
                fa = fopen("outA.txt","w");
                fprintf(fa,"Protocol: RECIEVE\n");
                fprintf(fa,"Message from: %s\n",identifier_ptr);
                fprintf(fa,"Message is: %s\n",msg);
                fclose(fa);
            }  
            else if ((int)*buffer == 4 && (int)*(buffer+1)== 0 ){
                fa = fopen("outA.txt","w");
                fprintf(fa,"Protocol: RECVCONT\n");
                fprintf(fa,"Message from: %s\n",identifier_ptr);
                fprintf(fa,"Message is: %s\n",msg);
                fclose(fa);
            } 
            else if ((int)*buffer == 5 && (int)*(buffer+1)== 0 ){
                fa = fopen("outA.txt","w");
                fprintf(fa,"Protocol: PING\n");
                fclose(fa);
                write_Pipe = open(write_pipe_ptr, O_WRONLY);
                buffer[0] = 6;
                write(write_Pipe,buffer,2048);
            } 
            else if ((int)*buffer == 7 && (int)*(buffer+1)== 0 ){
                write_Pipe = open(write_pipe_ptr, O_WRONLY);
                buffer[0] = 7;
                write(write_Pipe,buffer,2048);
       
            } 
            else if ((int)*buffer == 9 && (int)*(buffer+1)== 0 ){
           
                return 0;
            } 
        }
        close(read_Pipe);
    }
}
