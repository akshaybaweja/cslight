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
	crit_3[13],
	crit_2[13],
	crit_1[13],
	crit0[13],
	crit1[13],
	crit2[13],
	crit3[13],
	crit4[13],
	crit5[13],
	crit6[13],
	crit7[13],
	crit8[13],
	crit9[13];
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
                        char *cfline, *dot;
                        cfline = strstr((char *)line,DELIM);
                        cfline = cfline + strlen(DELIM);
                        dot = strstr(cfline,".");

                        if (i == 0){
                                memcpy(conf.ip,cfline,strlen(cfline)-1);
                                conf.ip[strlen(cfline)-1] = '\0';
                                //printf("laenge csfline %d, laenge ip %d\n",strlen(cfline),strlen(conf.ip));
                                printf("conf.ip = %s\n",conf.ip);
                        } else if (i == 1){
                                memcpy(conf.port,cfline,strlen(cfline)-1);
                                conf.port[strlen(cfline)-1] = '\0';
                                printf("conf.port = %s\n",conf.port);
                        } else if (i == 2){
                                memcpy(conf.comport,cfline,strlen(cfline)-1);
                                conf.comport[strlen(cfline)-1] = '\0';
                                printf("conf.comport = %s\n",conf.comport);
                        } else if (i == 3){
                                memcpy(conf.baudrate,cfline,strlen(cfline)-1);
                                conf.baudrate[strlen(cfline)-1] = '\0';
                                printf("conf.baudrate = %s\n",conf.baudrate);
                        } else if (i == 4){
                                memcpy(conf.num_leds,cfline,strlen(cfline)-1);
                                conf.num_leds[strlen(cfline)-1] = '\0';
                                printf("conf.numleds = %s\n",conf.num_leds);
                        } else if (i == 5){
                                memcpy(conf.static_rgb,cfline,strlen(cfline)-1);
                                conf.static_rgb[strlen(cfline)-1] = '\0';
                                printf("conf.static_rgb = %s|\n",conf.static_rgb);
						}
						// Colorsettings
						switch (i) {
							case 6 : memcpy(conf.crit_3,cfline,dot-cfline+1); conf.crit_3[dot-cfline+1] = '\0'; printf("color-3 = %s| in length %d\n",conf.crit_3,strlen(conf.crit_3)); break;
							case 7 : memcpy(conf.crit_2,cfline,dot-cfline+1); conf.crit_2[dot-cfline+1] = '\0'; printf("color-2 = %s| in length %d\n",conf.crit_2,strlen(conf.crit_2)); break;
							case 8 : memcpy(conf.crit_1,cfline,dot-cfline+1); conf.crit_1[dot-cfline+1] = '\0'; printf("color-1 = %s| in length %d\n",conf.crit_1,strlen(conf.crit_1)); break;
							case 9 : memcpy(conf.crit0,cfline,dot-cfline+1); conf.crit0[dot-cfline+1] = '\0'; printf("color 0 = %s| in length %d\n",conf.crit0,strlen(conf.crit0)); break;
							case 10 : memcpy(conf.crit1,cfline,dot-cfline+1); conf.crit1[dot-cfline+1] = '\0'; printf("color 1 = %s| in length %d\n",conf.crit1,strlen(conf.crit1));break;
							case 11 : memcpy(conf.crit2,cfline,dot-cfline+1); conf.crit2[dot-cfline+1] = '\0'; printf("color 2 = %s| in length %d\n",conf.crit2,strlen(conf.crit2));break;
							case 12 : memcpy(conf.crit3,cfline,dot-cfline+1); conf.crit3[dot-cfline+1] = '\0'; printf("color 3 = %s| in length %d\n",conf.crit3,strlen(conf.crit3));break;
							case 13 : memcpy(conf.crit4,cfline,dot-cfline+1); conf.crit4[dot-cfline+1] = '\0'; printf("color 4 = %s| in length %d\n",conf.crit4,strlen(conf.crit4));break;
							case 14 : memcpy(conf.crit5,cfline,dot-cfline+1); conf.crit5[dot-cfline+1] = '\0'; printf("color 5 = %s| in length %d\n",conf.crit5,strlen(conf.crit5));break;
							case 15 : memcpy(conf.crit6,cfline,dot-cfline+1); conf.crit6[dot-cfline+1] = '\0'; printf("color 6 = %s| in length %d\n",conf.crit6,strlen(conf.crit6));break;
							case 16 : memcpy(conf.crit7,cfline,dot-cfline+1); conf.crit7[dot-cfline+1] = '\0'; printf("color 7 = %s| in length %d\n",conf.crit7,strlen(conf.crit7));break;
							case 17 : memcpy(conf.crit8,cfline,dot-cfline+1); conf.crit8[dot-cfline+1] = '\0'; printf("color 8 = %s| in length %d\n",conf.crit8,strlen(conf.crit8));break;
							case 18 : memcpy(conf.crit9,cfline,dot-cfline+1); conf.crit9[dot-cfline+1] = '\0'; printf("color 9 = %s| in length %d\n",conf.crit9,strlen(conf.crit9));break;
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
		printf("Cannot open comport\n");
		return(0);
	}
	Sleep(2000);
	/* Check for Static-RGB-Light Settings */
	if (conf.static_rgb[0] != 0) {
        updateArduino(cport_nr, conf.crit_3);
        Sleep(300);
        updateArduino(cport_nr, conf.crit_2);
        Sleep(300);
        updateArduino(cport_nr, conf.crit_1);
        Sleep(300);
        updateArduino(cport_nr, conf.crit0);
        Sleep(300);
        updateArduino(cport_nr, conf.crit1);
        Sleep(300);
        updateArduino(cport_nr, conf.crit2);
        Sleep(300);
        updateArduino(cport_nr, conf.crit3);
        Sleep(300);
        updateArduino(cport_nr, conf.crit4);
        Sleep(300);
        updateArduino(cport_nr, conf.crit5);
        Sleep(300);
        updateArduino(cport_nr, conf.crit6);
        Sleep(300);
        updateArduino(cport_nr, conf.crit7);
        Sleep(300);
        updateArduino(cport_nr, conf.crit8);
        Sleep(300);
        updateArduino(cport_nr, conf.crit9);
        Sleep(300);
        printf("Static RGB-Light %s\n", conf.static_rgb);
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
		printf("Error: startWinsock, error code: %d\n",rc);
		return 1;
	}
	else
	{
		printf("\nWinsock started!\n");
	}


	/* create socket */
	s=socket(AF_INET,SOCK_STREAM,0);
	if(s==INVALID_SOCKET)
	{
		printf("Error: Socket could not be opened, error code: %d\n",WSAGetLastError());
		return 1;
	}
	else
	{
		printf("Socket opened!\n");
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
		printf("Error: connect failed, error code: %d\n",WSAGetLastError());
		return 1;
	}
	else
	{
		printf("Connected to %s:%s.\n",conf.ip,conf.port);
	}


	/* transfer data */

	strcpy(sendbuf,"pheenyx Connected");
	send(s,sendbuf,strlen(sendbuf),0);
	while(rc!=SOCKET_ERROR)
	{
		rc=recv(s,recvbuf,256,0);
		if(rc==0)
		{
			printf("Connection closed by server.\n");

		}
		if(rc==SOCKET_ERROR)
		{
			printf("Error: recv, error code: %d\n",WSAGetLastError());

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
					default: printf("State < -3 is unknown"); break;
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
					default: printf("State > 9 is unknown"); break;
				}
			}
		} else { printf("Keyword unknown"); }

	}

	closesocket(s);
	WSACleanup();


	return 0;
}


