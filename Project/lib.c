#include "lib.h"

void sig_chld(int signo)
{	pid_t pid;
 	int stat;
 	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		fprintf(stderr,"child %d terminated\n", pid);}

void web(int fd)
{	struct Req req;
	strcpy(req.resource,"");
	strcpy(req.status,"");
	Update_Req(fd,&req);
	
	if(!strncmp(req.status,"200",3))
		Return_res(fd,&req);
	else
		Return_Error_Msg(fd,&req);
	
	
	//printf("%s\n%s\n",req.resource,req.status);
}


void Update_Req(int fd,struct Req * r)
{	int bytes;
	char buf[MAX],*p=NULL;
	getcwd(r->resource,1024);

	if(bytes = read(fd,buf,MAX)<=0)
			{Error_Quit("Error: Reading request");}
	
	if(!strncmp("GET ",buf,4))
		{if(!strncmp("/ H",&buf[4],3))
			strcat(r->resource,"/index.html");
		 else if ((p = strchr(buf,'/'))==NULL)
				Error_Quit("Error: strchr");
			else{
			bytes = strlen(p)- strlen(strchr(p,' '));
			strncat(r->resource,p,bytes);}
		}
	else{ 
		strncpy(r->status,"405 Method Not Allowed",22);}

	if((bytes = open(r->resource,O_RDONLY))<0)
		{strncpy(r->status ,"404 Not Found",13);}
	else {
		strncpy(r->status,"200 OK",6); } close(bytes);

}



void Output_HTTP_Headers(int fd, struct Req * r) {

    char buffer[100];

    sprintf(buffer, "HTTP/1.0 %s\r\n", r->status);
    Writeline(fd, buffer, strlen(buffer));

    Writeline(fd, "Server: http_serv v1\r\n", 24);
    Writeline(fd, "Content-Type: text/html\r\n", 25);
    Writeline(fd, "\r\n", 2);
}



void Return_res(int fd, struct Req * r)
{	Output_HTTP_Headers(fd,r);
	char buf[MAX] = "";
	int f;
	ssize_t i;
	if((f = open(r->resource,O_RDONLY)))
	while ( (i = read(f,buf,MAX))) {
	if ( i < 0 )
	    Error_Quit("Error reading from file.");
	if ( write(fd, buf,MAX)<=0 )
	    Error_Quit("Error sending file.");}
}


ssize_t Readline(int sockd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char    c, *buffer;

    buffer = vptr;

    for ( n = 1; n < maxlen; n++ ) {
	
	if ( (rc = read(sockd, &c, 1)) == 1 ) {
	    *buffer++ = c;
	    if ( c == '\n' )
		break;
	}
	else if ( rc == 0 ) {
	    if ( n == 1 )
		return 0;
	    else
		break;
	}
	else {
	    if ( errno == EINTR )
		continue;
	    Error_Quit("Error in Readline()");
}}
    *buffer = 0;
    return n;}



ssize_t Writeline(int sockd, const void *vptr, size_t n) {
    size_t      nleft;
    ssize_t     nwritten;
    const char *buffer;

    buffer = vptr;
    nleft  = n;

    while ( nleft > 0 ) {
	if ( (nwritten = write(sockd, buffer, nleft)) <= 0 ) {
	    if ( errno == EINTR )
		nwritten = 0;
	    else
		Error_Quit("Error in Writeline()");
	}
	nleft  -= nwritten;
	buffer += nwritten;
    }

    return n;
}



int Return_Error_Msg(int fd, struct Req * r) {
    Output_HTTP_Headers(fd,r);
    char buffer[100];

    sprintf(buffer, "<HTML>\n<HEAD>\n<TITLE>Server Error %s</TITLE>\n"
	            "</HEAD>\n\n", r->status);
    Writeline(fd, buffer, strlen(buffer));

    sprintf(buffer, "<BODY>\n<H1>Server Error %s</H1>\n", r->status);
    Writeline(fd, buffer, strlen(buffer));

    sprintf(buffer, "<P>The request could not be completed.</P>\n"
	            "</BODY>\n</HTML>\n");
    Writeline(fd, buffer, strlen(buffer));

    return 0;

}


void Error_Quit(char const * msg) {
    fprintf(stderr, "SERV: %s\n", msg);
    exit(1);
}
