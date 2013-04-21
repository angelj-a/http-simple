#include "parser.h"


void parseURL( const char *url, char *host, char* port, char *resource)
{	
// Asume que url es de la forma: http://(host):(port)(resource)

// e.g.  www.foo.org/about.html
// Salida esperada:
// host = www.foo.org
// resource = /about.html
	
	const char *p = url;
	int i = 0;
	
	
	/*  Omite http: */
	if ( strstr(url, "http://") == url )
		p += 7;		
	
	/* Lee host */
	while ( (*p != '/') && (*p != 0) && (*p != ':') )
	{
		host[i++] = *p;
		p++;
	}
	host[i] = '\0';

	
	/* Lee puerto */
	/* default: 80 */
	i = 0;
	if ( *p == ':' )
	{
		p++;
		while ( (*p != '/') && (*p != 0) )
		{
			port[i++] = *p;
			p++;
		} 
	}
	else
	{
		port[i++] = '8';
		port[i++] = '0';
	}
	port[i] = '\0';
	
	
	// Lee URI
	/* default: / */
	i = 0;
	if ( *p == 0 )
		resource[i++] = '/';
	else	
		while ( *p != 0 )
		{
			resource[i++] = *p;
			p++;
		}
	
	resource[i] = '\0';
	
}



/*
static void constructRequest(char *request, char *host, char *rs, char *http_ver) {

	sprintf(request, "GET %s HTTP/%s\r\nHost: %s\r\n\r\n", rs, http_ver, host);
	
}



int main(int argc, char **argv) {
	
	if (argc > 1) {
		perror("Cantidad de argumentos incorrecta\n");
		return 1;
	}
	
	char host[256];
	char rs[256];
	char request[1024];
	
	parseURL("http://www.abcdef.net", host, rs);
	constructRequest(request, host, rs, "1.0");	
	printf("%s\n", request);

	parseURL("http://www.abcdef.net/", host, rs);
	constructRequest(request, host, rs, "1.0");	
	printf("%s\n", request);	
	
	parseURL("www.abcdef.net", host, rs);
	constructRequest(request, host, rs, "1.0");	
	printf("%s\n", request);		
	
	parseURL("http://www.abcdef.net/index.html", host, rs);
	constructRequest(request, host, rs, "1.0");	
	printf("%s\n", request);			
	
	return 0;
}
*/
