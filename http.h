#ifndef __HTTP_H__
#define __HTTP_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#undef  DEBUG

#define IN_BUFFSIZE 1024
#define OUT_BUFFSIZE 1024

typedef struct _buffer {
	char *ptr;
	int size;
} t_Buffer;


typedef enum  {
	TE_CHUNKED,
	TE_COMPRESS,
	TE_DEFLATE,
	TE_GZIP,
	TE_IDENTITY
} t_TransferEncoding;

int receiveHeader(int sockfd, t_Buffer* dst);
int receiveBody_TE_Identity(int sockfd, t_Buffer* dst);
int receiveBody_TE_Chunked(int sockfd, t_Buffer* dst);
int receiveContent( int sockfd, t_Buffer* dst, t_TransferEncoding te );
/* Asume que n es el tamaño del bloque de memoria al que apunta dst 
 * ó que se desea reservar (inicialmente) */


t_Buffer* t_Buffer_new(int size);
inline void t_Buffer_delete(t_Buffer* buffer);

#endif
