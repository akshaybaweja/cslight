// -lws2_32 rs232.c
#include <windows.h>
#include <winsock2.h>
#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


// for arduino
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include "rs232.h"

#define FILENAME "cslight.conf"
#define MAXBUF 1024 
#define DELIM "="


/* Config-Management */

struct config
{
   char ip[MAXBUF];
   char port[MAXBUF];
   char comport[MAXBUF];
   char baudrate[MAXBUF];
   char num_leds[MAXBUF];
   char static_rgb[MAXBUF];
   char	
	crit_3[12],
	crit_2[12], 
	crit_1[12], 
	crit0[12], 
	crit1[12], 
	crit2[12], 
	crit3[12], 
	crit4[12], 
	crit5[12], 
	crit6[12], 
	crit7[12], 
	crit8[12], 
	crit9[12];
};

struct config get_config(char *filename) 
{
        struct config conf;
        FILE *file = fopen (filename, "r");
        if (file != NULL)
        { 
                char line[MAXBUF];
                int i = 0;

                while(fgets(line, sizeof(line), file) != NULL)
                {
                        char *cfline;
                        cfline = strstr((char *)line,DELIM);
                        cfline = cfline + strlen(DELIM);
    
                        if (i == 0){
                                memcpy(conf.ip,cfline,strlen(cfline));
                                printf("%s",conf.ip);
                        } else if (i == 1){
                                memcpy(conf.port,cfline,strlen(cfline));
                                printf("%s",conf.port);
                        } else if (i == 2){
                                memcpy(conf.comport,cfline,strlen(cfline));
                                printf("%s",conf.comport);
                        } else if (i == 3){
                                memcpy(conf.baudrate,cfline,strlen(cfline));
                                printf("%s",conf.baudrate);
                        } else if (i == 4){
                                memcpy(conf.num_leds,cfline,strlen(cfline));
                                printf("%s",conf.num_leds);
                        } else if (i == 5){
                                memcpy(conf.static_rgb,cfline,strlen(cfline)-1);
                                printf("%s",conf.static_rgb);
						}
						// Colorsettings
						switch (i) {
							case 6 : memcpy(conf.crit_3,cfline,strlen(cfline)-1); break;
							case 7 : memcpy(conf.crit_2,cfline,strlen(cfline)-1); break;
							case 8 : memcpy(conf.crit_1,cfline,strlen(cfline)-1); break;
							case 9 : memcpy(conf.crit0,cfline,strlen(cfline)-1); break;
							case 10 : memcpy(conf.crit1,cfline,strlen(cfline)-1); break;
							case 11 : memcpy(conf.crit2,cfline,strlen(cfline)-1); break;
							case 12 : memcpy(conf.crit3,cfline,strlen(cfline)-1); break;
							case 13 : memcpy(conf.crit4,cfline,strlen(cfline)-1); break;
							case 14 : memcpy(conf.crit5,cfline,strlen(cfline)-1); break;
							case 15 : memcpy(conf.crit6,cfline,strlen(cfline)-1); break;
							case 16 : memcpy(conf.crit7,cfline,strlen(cfline)-1); break;
							case 17 : memcpy(conf.crit8,cfline,strlen(cfline)-1); break;
							case 18 : memcpy(conf.crit9,cfline,strlen(cfline)-1); break;
						}
                        i++;
                } // End while
                fclose(file);
        } else { 
			printf("Error opening File \n");
		} 
        return conf;
}


int updateArduino(int port, char buf[])
{
	//RS232_SendByte(port, startbyte);
	RS232_SendBuf(port, buf, strlen(buf) );
	printf("sent: %s\n", buf);
	printf("in length: %d\n", strlen(buf));
}

int startWinsock()
{
  WSADATA wsa;
  return WSAStartup(MAKEWORD(2,0),&wsa);
}


    
int main()
{
	 struct config conf;
     conf = get_config(FILENAME);
	
	/* --- Arduino --- */
	
	/* Arduiono Serial Connector */
	int cport_nr=atoi(conf.comport),  	// /dev/ttyS0 (COM3 on windows) 
		bdrate=atoi(conf.baudrate);       // 57600 baud */	
	char mode[]={'8','N','1',0}; 			//8N1 means eight databits, no parity, one stopbit
	
	// open COM-Port to Arduino
	if(RS232_OpenComport(cport_nr, bdrate, mode))
	{
		printf("Can not open comport\n");
		return(0);
	}
	Sleep(2000);
	/* Check for Static-RGB-Light Settings */
	if (conf.static_rgb[0] != 0) {
		printf("Static RGB-Light %i\n", conf.static_rgb[0]);
		updateArduino(cport_nr, conf.static_rgb);
	}
	
	/* --- Socket --- */
	long rc;
	SOCKET s;
	SOCKADDR_IN addr;
	uint8_t sendbuf[256], recvbuf[256];
	  
	/* init Winsock and check for error*/
	rc=startWinsock();
	if(rc!=0)
	{
		printf("Fehler: startWinsock, fehler code: %d\n",rc);
		return 1;
	}
	else
	{
		printf("\nWinsock gestartet!\n");
	}
 
 
	/* create socket */
	s=socket(AF_INET,SOCK_STREAM,0);
	if(s==INVALID_SOCKET)
	{
		printf("Fehler: Der Socket konnte nicht erstellt werden, fehler code: %d\n",WSAGetLastError());
		return 1;
	}
	else
	{
		printf("Socket erstellt!\n");
	}
	
	
	/* connect to server */
	
	memset(&addr,0,sizeof(SOCKADDR_IN)); // clear all
	addr.sin_family=AF_INET;	//AF_INET for TCP/IP
	addr.sin_port=htons(atoi(conf.port)); // port of server, turned into Host to Network Short
	addr.sin_addr.s_addr=inet_addr(conf.ip); // IP of endpoint-server
	//addr.sin_addr.s_addr=inet_addr("127.0.0.1");

	rc=connect(s,(SOCKADDR*)&addr,sizeof(SOCKADDR));
	if(rc==SOCKET_ERROR)
	{
		printf("Fehler: connect gescheitert, fehler code: %d\n",WSAGetLastError());
		return 1;
	}
	else
	{
		printf("Verbunden mit 127.0.0.1..\n");
	}

	
	/* transfer data */
	
	strcpy(sendbuf,"obsti Connected");
	send(s,sendbuf,strlen(sendbuf),0);
	while(rc!=SOCKET_ERROR)
	{	
		rc=recv(s,recvbuf,256,0);
		if(rc==0)
		{
			printf("Server hat die Verbindung getrennt..\n");
			
		}
		if(rc==SOCKET_ERROR)
		{
			printf("Fehler: recv, fehler code: %d\n",WSAGetLastError());
			
		}
		
		// recieved data - call arduino
		recvbuf[rc]='\0'; //ToDo .. maybe delete for arduino		
		uint8_t state = 0;
		
		if (recvbuf[0] == 'S') {
			if (recvbuf[1] == '-') {
				state = recvbuf[2];
				printf("\nIsolated state: -%c\n", state);
				switch(state) {
					case '1': updateArduino(cport_nr, conf.crit_1); break;
					case '2': updateArduino(cport_nr, conf.crit_2); break;
					case '3': updateArduino(cport_nr, conf.crit_3); break;				
					default: printf("negative State is not known"); break;
				}
			} else {
				state = recvbuf[1];
				printf("\nIsolated state: %c\n", state);
				
				switch(state) {
					case '0': updateArduino(cport_nr, conf.crit0); break;
					case '1': updateArduino(cport_nr, conf.crit1); break;
					case '2': updateArduino(cport_nr, conf.crit2); break;
					case '3': updateArduino(cport_nr, conf.crit3); break;
					case '4': updateArduino(cport_nr, conf.crit4); break;
					case '5': updateArduino(cport_nr, conf.crit5); break;
					case '6': updateArduino(cport_nr, conf.crit6); break;
					case '7': updateArduino(cport_nr, conf.crit7); break;
					case '8': updateArduino(cport_nr, conf.crit8); break;
					case '9': updateArduino(cport_nr, conf.crit9); break;				
					default: printf("positive State not known"); break;
				}
			}
		} else { printf("Keyword not known"); }
				
	}
	
	closesocket(s);
	WSACleanup();	
		
	
	return 0;
}


