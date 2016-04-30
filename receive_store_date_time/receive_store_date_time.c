#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <time.h>
#include <signal.h>

int ret_loop;

void signal_handler(int signum)
{
	printf("signal handler\n");
	if(SIGINT == signum)
	{
		ret_loop = 0;
	}
}

int main(int argc, char *argv[])
{
		int sockfd = 0, n = 0;
		char recvBuff[1024];
		struct sockaddr_in serv_addr;
		int serPort, sec=0;
		FILE *logFp = NULL;
		time_t ticks;
		serPort = atoi(argv[2]);
		sec = atoi(argv[3]);

		if(argc != 4)
		{
				printf("\n Usage: %s <ip of server> \n",argv[0]);
				return 1;
		} 

		logFp = fopen("./adc_log.txt","a+");
		if(NULL == logFp)
		{
				printf("Failed to open log file\n");
				return 0;
		}
		printf("server addr:%s:%d\n",argv[1],serPort);
		signal(SIGINT,signal_handler);
		
		ret_loop = 1;
		
		while(1 == ret_loop)
		{
				memset(recvBuff, '0',sizeof(recvBuff));
				if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
				{
						printf("\n Error : Could not create socket \n");
						return 1;
				} 

				memset(&serv_addr, '0', sizeof(serv_addr)); 

				serv_addr.sin_family = AF_INET;
				serv_addr.sin_port = htons(serPort); 

				if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
				{
						printf("\n inet_pton error occured\n");
						return 1;
				} 


				if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
				{
						printf("\n Error : Connect Failed %d\n",errno);
						perror("");
						//return 1;
				} 

				if( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
				{
						printf("recv:%s\n",recvBuff);
						recvBuff[n] = 0;
						ticks = time(NULL);
						fprintf(logFp,"%.24s-%s\r\n",ctime(&ticks),recvBuff);
				}

				if(n < 0)
				{
						printf("\n Read error \n");
				} 

				printf("sleeping for %d sec\n",sec);
				sleep(sec);
				close(sockfd);
		}
		fclose(logFp);
		return 0;
}
