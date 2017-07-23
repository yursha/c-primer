#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 8096
#define ERROR 42
#define SORRY 43
#define LOG   44

// LIMITS
#define LOG_FILENAME_MAX_LENGTH 100

struct {
	char* ext;
	char* filetype;
} extensions [] = {
	{"jpg", "image/jpeg"}, 
	{"jpeg","image/jpeg"},
	{"png", "image/png" },  
	{"js","text/js"     },
	{"css","test/css"   }, 
	{0,0} };

int log_fileno = 0; // must be initialized from args to main

// f[ile] log
//
// Logs a message to a file
//
// @param fileno - open file descriptor to write into.
// @param message - message to write as is.
void flog(int fileno, char* message) {
	int message_length = strlen(message);
  write(fileno, message, message_length); 
}

// f[ile] log n[ewline]
//
// Logs a message to a file, appending newline to the message
//
// @param fileno - open file descriptor to write into.
// @param message - message to write as is.
void flogn(int fileno, char* message) {
	flog(fileno, logbuffer,strlen(logbuffer)); 
	write(fileno, "\n" , 1);      
}

// die
//
// print error message and exit the process.
// Relies on OS to release all acquired resources (including memory and open file descriptors).
//
// @param fileno - open file descriptor to write into.
// @param message - message to write as is.
void die(int fileno, char* message) {
	flog(fileno, "ERROR: ");
	flogn(fileno, message);
	exit(1);
}

// TODO: We need a logging library
// It should expose vararg APIs.

// TODO: implement generic templating system
const char * const template = "<html><body><h1> %S </h1></body></html>\r\n" - 2;

// User is responsible to free memory
char* web_message(char* message) {
	int web_message_length = sizeof(template) + strlen(message);
	char* web_message = malloc(web_message_length);
	sprintf(web_message, template, message);
	return web_message;
}

void system_error(int fileno, char* message) {
	flog(fileno, message);
	exit(1);
}

void log(int type, char *message, char *s2, int num, int log_fileno)
{
	char logbuffer[BUFSIZE * 2];

	switch (type) {
	case ERROR:
		sprintf(logbuffer,"ERROR: %s:%s Errno=%d exiting pid=%d",message, s2, errno, getpid());
		system_error(log_fileno, logbuffer);
		break;
	case SORRY: 
		sprintf(logbuffer,"SORRY: %s:%s",message, s2); 
		flog(log_fileno, logbuffer);
		break;
	case LOG:
		sprintf(logbuffer," INFO: %s:%s:%d",message, s2,num);
		flog(log_fileno, logbuffer);
		break;
	}	
}

void web(int fd/*socket_fd*/, int hit, int log_fileno)
{
	int j, file_fd, buflen, len;
	long i, ret;
	char * fstr;
	static char buffer[BUFSIZE+1];

	ret =read(fd,buffer,BUFSIZE); 
	if(ret == 0 || ret == -1) {
		log(SORRY,"failed to read browser request","",fd);
		log(SORRY,"failed to read browser request","",log_fileno);
	}
	if(ret > 0 && ret < BUFSIZE)	
		buffer[ret]=0;	
	else buffer[0]=0;

	for(i=0;i<ret;i++)	
		if(buffer[i] == '\r' || buffer[i] == '\n')
			buffer[i]='*';
	log(LOG,"request",buffer,hit, log_fileno, fd);

	if( strncmp(buffer,"GET ",4) && strncmp(buffer,"get ",4) )
		log(SORRY,"Only simple GET operation supported",buffer, log_fileno, fd);
		log(SORRY,"Only simple GET operation supported",buffer, log_fileno, fd);

	for(i=4;i<BUFSIZE;i++) { 
		if(buffer[i] == ' ') { 
			buffer[i] = 0;
			break;
		}
	}

	for(j=0;j<i-1;j++) 	
		if(buffer[j] == '.' && buffer[j+1] == '.')
			log(SORRY,"Parent directory (..) path names not supported",buffer, log_fileno, fd);
			log(SORRY,"Parent directory (..) path names not supported",buffer, log_fileno, fd);

	if( !strncmp(&buffer[0],"GET /\0",6) || !strncmp(&buffer[0],"get /\0",6) ) 
		(void)strcpy(buffer,"GET /index.html");

	buflen=strlen(buffer);
	fstr = (char *)0;
	for(i=0;extensions[i].ext != 0;i++) {
		len = strlen(extensions[i].ext);
		if( !strncmp(&buffer[buflen-len], extensions[i].ext, len)) {
			fstr =extensions[i].filetype;
			break;
		}
	}
	if(fstr == 0) log(SORRY,"file extension type not supported",buffer, log_fileno, fd);

	if(( file_fd = open(&buffer[5],O_RDONLY)) == -1) 
		log(SORRY, "failed to open file",&buffer[5],log_fileno, fd);

	log(LOG,"SEND",&buffer[5],hit);

	(void)sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: %s\r\n\r\n", fstr);
	(void)write(fd,buffer,strlen(buffer));

	while (	(ret = read(file_fd, buffer, BUFSIZE)) > 0 ) {
		(void)write(fd,buffer,ret);
	}
#ifdef LINUX
	sleep(1);
#endif
	exit(1);
}


int main(int argc, char **argv)
{
	int i, port, pid, listenfd, socketfd, hit;
	size_t length;
	static struct sockaddr_in cli_addr; 
	static struct sockaddr_in serv_addr;

	// TODO: parse args.
	// if arg_log_filename > LOG_FILENAME_MAX_LENGTH -> error & exit
	// assign to log_fileno
	if((log_fileno = open("server.log", O_CREAT| O_WRONLY | O_APPEND,0644)) >= 0) {
		flogn(log_fileno, "listening...");
	}
	if(type == ERROR || type == SORRY) exit(3);
	

	if( argc < 3  || argc > 3 || !strcmp(argv[1], "-?") ) {
		(void)printf("usage: server [port] [server directory] &"
	"\tExample: server 80 ./ &\n\n"
	"\tOnly Supports:");
		for(i=0;extensions[i].ext != 0;i++)
			(void)printf(" %s",extensions[i].ext);

		(void)printf("\n\tNot Supported: directories / /etc /bin /lib /tmp /usr /dev /sbin \n"
		    );
		exit(0);
	}
	if( !strncmp(argv[2],"/"   ,2 ) || !strncmp(argv[2],"/etc", 5 ) ||
	    !strncmp(argv[2],"/bin",5 ) || !strncmp(argv[2],"/lib", 5 ) ||
	    !strncmp(argv[2],"/tmp",5 ) || !strncmp(argv[2],"/usr", 5 ) ||
	    !strncmp(argv[2],"/dev",5 ) || !strncmp(argv[2],"/sbin",6) ){
		(void)printf("ERROR: Bad top directory %s, see server -?\n",argv[2]);
		exit(3);
	}
	if(chdir(argv[2]) == -1){ 
		(void)printf("ERROR: Can't Change to directory %s\n",argv[2]);
		exit(4);
	}

	if(fork() != 0)
		return 0; 
	(void)signal(SIGCLD, SIG_IGN); 
	(void)signal(SIGHUP, SIG_IGN); 
	for(i=0;i<32;i++)
		(void)close(i);	
	(void)setpgrp();	

	log(LOG,"http server starting",argv[1],getpid());

	if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0)
		log(ERROR, "system call","socket",0);
	port = atoi(argv[1]);
	if(port < 0 || port >60000)
		log(ERROR,"Invalid port number try [1,60000]",argv[1],0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0)
		log(ERROR,"system call","bind",0);
	if( listen(listenfd,64) <0)
		log(ERROR,"system call","listen",0);

	for(hit=1; ;hit++) {
		length = sizeof(cli_addr);
		if((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0)
			log(ERROR,"system call","accept",0);

		if((pid = fork()) < 0) {
			log(ERROR,"system call","fork",0);
		}
		else {
			if(pid == 0) {
				(void)close(listenfd);
				web(socketfd,hit);
			} else {
				(void)close(socketfd);
			}
		}
	}

	// Commented out because we don't need to do it. OS we take care of it.
	// close(log_fileno);
}
