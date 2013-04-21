#include "http.h"
#include "macros.h"


t_Buffer* t_Buffer_new(int size)
{
	t_Buffer *result = (t_Buffer*) malloc(sizeof(t_Buffer));
	
	result->ptr = (char*) calloc(size, 1);
	result->size = size;
	
	return result;
} 

inline void t_Buffer_delete(t_Buffer* buffer)
{
	if (buffer->ptr)
		free(buffer->ptr);
		
	free(buffer);
} 



int receiveHeader(int sockfd, t_Buffer* dst)
{
	assert( dst->ptr );
	
	int i = 0;
	int n_crlf = 0;
	char recv_buff[2];
	char *header_buffer = dst->ptr;
	int size_header_buffer= dst->size;
	
	header_buffer[0] = '\0';		
	//alternativa barata a: memset(body_buffer,0,size_header_buffer) ?		
 
	for(;;)
	{
		if ( n_crlf == 2 )
			break;
			
		INCREASE_IFNEEDED(header_buffer, i, 2, size_header_buffer);
		
		recv(sockfd, recv_buff, 2, 0);
		header_buffer[i++] = recv_buff[0];
		header_buffer[i++] = recv_buff[1];
		
					
		if ( UNLIKELY((recv_buff[0] == '\r') && (recv_buff[1] == '\n')) )
			n_crlf++;
		else
		if ( UNLIKELY(recv_buff[1] == '\r') )
		{
			if ( UNLIKELY(recv_buff[0] == '\n') )
				n_crlf++;
			else
				n_crlf = 1;
			
			if (LIKELY(n_crlf == 2))
			{
				
				INCREASE_IFNEEDED(header_buffer, i, 2, size_header_buffer);
				
				// Lee el caracter que falta para completar \r\n\r\n
				recv(sockfd, recv_buff, 1, 0);
				header_buffer[i++] = recv_buff[0];
				header_buffer[i++] = '\0';
			}
		}
		else 
			if ( UNLIKELY(n_crlf) )
				n_crlf = 0;
			
	}
	
	dst->ptr = header_buffer;
	dst->size = size_header_buffer;
	
	return i; 
		
}

inline int hex2int(char c)
{
	if (c & 0x40)
		return ((int)c - 87);
	else
		return ((int)c - 48);
}

int readChunkSize(int sockfd)
{
	int chunk_size = 0;
	char recv_buff[1];
	
	for(;;)
	{
		recv(sockfd, recv_buff, 1, 0);

		if ( UNLIKELY(recv_buff[0] == '\r') )
		{
			// Lee el caracter '\n' que falta
			recv(sockfd, recv_buff, 1, 0);
			break;
		}
		else {
			chunk_size = (chunk_size << 4);
			chunk_size += hex2int(recv_buff[0]);
		}
	}

	return chunk_size;
	
};


int receiveBody_TE_Identity(int sockfd, t_Buffer* dst)
{
	assert( dst->ptr );
		
	int bytes_recv = 0;
	int total = 0;
	int size_body_buffer = dst->size;
	char recv_buff[1024];
	char *body_buffer = dst->ptr;
	
	body_buffer[0] = '\0';
	//alternativa barata a: memset(body_buffer,0,size_body_buffer) ?

	while ( ( bytes_recv = recv(sockfd, recv_buff, 1023, 0)) > 0 )
	{
		recv_buff[bytes_recv] = '\0';
		
		INCREASE_IFNEEDED(body_buffer, total, bytes_recv, size_body_buffer);	
	
		strcat(body_buffer, recv_buff);
		total += bytes_recv;
	}
	
	dst->ptr = body_buffer;  	
  	dst->size = size_body_buffer;
  	
	return total;	
}


int receiveBody_TE_Chunked(int sockfd, t_Buffer* dst)
{
	assert( dst->ptr );
	
	int bytes_recv = 0;
	int total = 0;
	int size_body_buffer = dst->size;
	int remaining = 0;
	
	char recv_buff[IN_BUFFSIZE];
	int chunk_size = 0;
	
	char *body_buffer = dst->ptr;
	
	body_buffer[0] = '\0';		
	//alternativa barata a: memset(body_buffer,0,size_body_buffer) ?

	
	for(;;)
	{
		chunk_size = readChunkSize(sockfd);
				
		if (chunk_size == 0)
			break;
			
		remaining = chunk_size;	
		
		
		while (remaining > 0)
		{
			bytes_recv = recv(sockfd, recv_buff, MIN(IN_BUFFSIZE-1, remaining), 0);
			remaining -= bytes_recv;
			
			recv_buff[bytes_recv] = '\0';

			INCREASE_IFNEEDED(body_buffer, total, bytes_recv, size_body_buffer);			

#ifdef DEBUG			
			assert( strlen(body_buffer) + strlen(recv_buff) < size_body_buffer );
#endif
			
			strcat(body_buffer, recv_buff);
			total += bytes_recv;
		}
		
		// Lee \r\n
		recv(sockfd, recv_buff, 2, 0);
		total += 2;		
	}

	dst->ptr = body_buffer;  	
  	dst->size = size_body_buffer;
  	
	return total;	
}


int receiveContent( int sockfd, t_Buffer* dst, t_TransferEncoding te )
{
	int size = 0;
	int mode = (int) te;
	
	switch(mode)
	{
		case TE_IDENTITY:
			size = receiveBody_TE_Identity(sockfd, dst);
			break;
		case TE_CHUNKED:
			size = receiveBody_TE_Chunked(sockfd, dst);
			break;
		case TE_DEFLATE:
		case TE_GZIP:
		case TE_COMPRESS:
			size = 0;
			printf("Not supported yet\n");
			break;
		default:
			size = -1;
			printf("Unknown mode\n");
	}	
	
	return size;
}
