#ifndef SERVER_H
#define SERVER_H

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
void sent_message(unsigned char* message, char* identifier,int type)
    relay message to clients within the domain of sender.
*/
void sent_message( unsigned char* message, char* identifier,int type);

#endif
