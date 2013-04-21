#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>

#include "macros.h"
#include "http.h"
#include "parser.h"
#include "yr.h"



int main(int argc, char **argv)
{

	struct addrinfo hints, *res;
	int sockfd;

	char port[8] = {0};
	char host[256] = {0};
	char path[512] = {0};
	char buff_msg_out[OUT_BUFFSIZE] = {0};
	
	t_Buffer *header = t_Buffer_new(1024);
	t_Buffer *document = t_Buffer_new(1024);	
	

	if ( argc == 1 )
		parseURL( HOST, host, port, path);

	if ( argc > 1 )
	/*  TODO: validar parámetros- evitar buffer overflow  */
		parseURL( argv[1], host, port, path);

	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if ( getaddrinfo(HOST, "80", &hints, &res) != 0) {
		perror("getaddrinfo()");
		exit(EXIT_FAILURE);
	}


	if ( ( sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol) ) < 0 )
	{
		perror("socket()");
		exit(EXIT_FAILURE);
	}


	if ( connect(sockfd, res->ai_addr, res->ai_addrlen) == -1 )
	{
		perror("connect()");
		exit(EXIT_FAILURE);
	}

	printf("Conectado con éxito\n");
	
	
	// Enviar datos
	
	int page_n;

	for ( page_n = 1; page_n <= 50; page_n++ )
	{
		// Cargo página 'page_n' del directorio de resueltas
		
		sprintf(buff_msg_out, "GET %s%d HTTP/%s\r\nHost: %s\r\nUser-Agent: Mozilla Firefox 11.0 \r\n\r\n", DIR, page_n, "1.1", HOST);
	
		// Enviar
		if ( send(sockfd, buff_msg_out, strlen(buff_msg_out), 0) < 0 )
		{
			perror("send()");
			exit(EXIT_FAILURE);
		}
	
		printf("%d\n",page_n);
	    //printf("Mensaje enviado:\n%s\n", buff_msg_out);	

		receiveHeader(sockfd, header);
		receiveContent(sockfd, document, TE_CHUNKED);

		processPage(sockfd, document);
	}
	

	t_Buffer_delete(header);
	t_Buffer_delete(document);
	
	
	freeaddrinfo(res);
	close(sockfd);
	

	exit(EXIT_SUCCESS);
}

