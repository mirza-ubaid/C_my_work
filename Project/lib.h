#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#define PORT 8080
#define	MAX 8192
#define	LISTENQ	1024


struct Req
{
	char resource[1024];
	char status[15];
};
void Return_res(int fd, struct Req * r);
void Update_Req(int fd,struct Req * r);
void Output_HTTP_Headers(int fd, struct Req * r);
int Return_Error_Msg(int fd, struct Req * r);
void Error_Quit(char const * msg);
ssize_t Writeline(int sockd, const void *vptr, size_t n);
ssize_t Readline(int sockd, void *vptr, size_t maxlen);
void sig_chld(int signo);
void web(int fd);
