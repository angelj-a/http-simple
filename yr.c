#include "yr.h"

void processPage(int sockfd, t_Buffer* document) {
	
	char *pos = document->ptr;
	char query_get[1024] = {0};
	char qid[22] = {0};
	
	t_Buffer *header_q = t_Buffer_new(1024);
	t_Buffer *document_q = t_Buffer_new(1024);
	
	
	srand(1200);
	
	// Busca URL de una pregunta de la página
	while ( (pos = strstr(pos, SEARCH_QUESTION_TOKEN )) )
	{		
		// Identifica el qid
		pos = strstr(pos, SEARCH_QID_TOKEN );
		
		memcpy(qid, pos+5, 21);
		
		sprintf( query_get,\
				"GET %s%s%s HTTP/%s\r\nHost: %s\r\nUser-Agent: Mozilla Firefox 11.0 \r\n\r\n",\
				SEARCH_QUESTION_TOKEN, SEARCH_QID_TOKEN, qid,\
				"1.1", HOST);
	
		// Enviar
		if ( send(sockfd, query_get, strlen(query_get), 0) < 0 )
		{
			perror("send()");
			exit(EXIT_FAILURE);
		}
	
		receiveHeader(sockfd, header_q);
		receiveContent(sockfd, document_q, TE_CHUNKED);
		
		if ( strstr(document_q->ptr, SEARCH_USER) )
			printf("http://%s%s%s%s\n", HOST,SEARCH_QUESTION_TOKEN, SEARCH_QID_TOKEN,qid);
	
	    sleep(rand() % 3);
		
	}
	
	printf("\n");
	
	
	// Liberar recursos	
	t_Buffer_delete(header_q);
	t_Buffer_delete(document_q);

	
}
