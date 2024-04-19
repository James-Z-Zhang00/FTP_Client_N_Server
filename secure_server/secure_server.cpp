//////////////////////////////////////////////////////////////
// TCP SERVER GCC (towards IPV6 ready)
//
//
// References: https://msdn.microsoft.com/en-us/library/windows/desktop/ms738520(v=vs.85).aspx
//             http://long.ccaba.upc.edu/long/045Guidelines/eva/ipv6.html#daytimeServer6
//
//////////////////////////////////////////////////////////////
//Ws2_32.lib
#define _WIN32_WINNT 0x501  //to recognise getaddrinfo()

//"For historical reasons, the Windows.h header defaults to including the Winsock.h header file for Windows Sockets 1.1. The declarations in the Winsock.h header file will conflict with the declarations in the Winsock2.h header file required by Windows Sockets 2.0. The WIN32_LEAN_AND_MEAN macro prevents the Winsock.h from being included by the Windows.h header"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


//159.334 - Networks
//single threaded server
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <sstream>

//#pragma directives offer a way for each compiler to offer machine- and operating system-specific features
// Need to link with Ws2_32.lib
//#pragma comment (lib, "Ws2_32.lib") 
#define SECRET_PASSWORD "334"

#define WSVERS MAKEWORD(2,2) /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
                             //The high-order byte specifies the minor version number; 
									  //the low-order byte specifies the major version number.

#define USE_IPV6 false //true

#define DEFAULT_PORT "1234" 
unsigned long long char2ull(char c) {return (unsigned long long) (c);}
char ull2char(unsigned long long ull) {return (char) (ull);}
WSADATA wsadata; //Create a WSADATA object called wsadata. 
using namespace std;

unsigned long long END[4];


unsigned long long hashing_map[] = {
   	110,77,85,93,101,118,89,126,111,115,92,							// 0 - 10
 	116,127,114,82,96,108,75,121,81,117,					// 11 - 20
 	78,73,94,104,95,102,125,71,80,100,					// 21 - 30
 	76,112,124,109,90,68,123,113,86,105,					// 31 - 40
 	83,91,122,103,72,106,87,70,120,69,					// 41 - 50
 	119,107,99,67,79,98,88,74,84,97,					// 51 - 60
 	66,65,64,63,62,
	 
	 
	61,54,36,50,48,					// 61 - 70
 	28,45,22,58,17,31,1,21,55,29,					// 71 - 80
 	19,14,41,59,2,39,47,57,6,35,					// 81 - 90
 	42,10,3,23,25,15,60,56,53,30,					// 91 - 100
 	4,26,44,24,40,46,52,16,34,0,		// 101 - 110
 	8,32,38,13,9,11,20,5,51,49,		// 111 - 120
 	18,43,37,33,27,7,12						// 121 - 127
 };

bool not_prime(unsigned long long num) {
    unsigned long long temp;
    temp = num / 2;
    for (int i = 2; i <= temp; i++) { if (num % i == 0) {return true;}}
    return false;
}

unsigned long long make_prime() {
    unsigned long long num = rand();
    while (not_prime(num)) { num += 1; }
    return num;
}

unsigned long long _gcd_(unsigned long long a, unsigned long long b) {
    unsigned long long temp;
    while ((a % b) > 0) { temp = a % b; a = b;b = temp;}
    return b;
}

unsigned long long make_e(unsigned long long n,unsigned long long z) {
    unsigned long long num = rand();
    while (num >= n) { num--; }
    while (_gcd_(num,z) != 1) { num--; }
    return num;
}

unsigned long long make_d(unsigned long long e,unsigned long long z) {
    unsigned long long num = 1;
    while ((e * num) % z != 1) {num++;}
    return num;
}

unsigned long long repeatSquare(unsigned long long x, unsigned long long e, unsigned long long n) {
    unsigned long long y = 1;
    while (e > 0) {
        if ((e % 2) == 0) {x = (x*x) % n; e = e / 2;}
        else {y = (x*y) % n; e = e - 1;}
    }
    return y;
}

void make_END() {
    unsigned long long p,q,n,z,e,d,nonce;
    p = make_prime();
    q = make_prime();
    n = p * q;
    z = (p - 1) * (q - 1);
    e = make_e(n,z);
    d = make_d(e,z);
    END[0] = e; END[1] = n; END[2] = nonce; END[3] = d; 
}

//*******************************************************************
//MAIN
//*******************************************************************
int main(int argc, char *argv[]) {
	
//********************************************************************
// INITIALIZATION of the SOCKET library
//********************************************************************
   //struct sockaddr_in clientAddress;  //IPV4
	struct sockaddr_storage clientAddress; //IPV6
	
	char clientHost[NI_MAXHOST]; 
	char clientService[NI_MAXSERV];
	
   SOCKET s,ns;
   char send_buffer[80000],receive_buffer[80000];
   int n,bytes,addrlen;
	char portNum[NI_MAXSERV];
	char username[80];
	char passwd[80];
		
   //memset(&localaddr,0,sizeof(localaddr));

//********************************************************************
// WSSTARTUP
/*	All processes (applications or DLLs) that call Winsock functions must 
	initialize the use of the Windows Sockets DLL before making other Winsock 
	functions calls. 
	This also makes certain that Winsock is supported on the system.
*/
//********************************************************************
	int err;
	
	err = WSAStartup(WSVERS, &wsadata);
   if (err != 0) {
      WSACleanup();
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
      printf("WSAStartup failed with error: %d\n", err);
		exit(1);
   }
	
//********************************************************************
/* Confirm that the WinSock DLL supports 2.2.        */
/* Note that if the DLL supports versions greater    */
/* than 2.2 in addition to 2.2, it will still return */
/* 2.2 in wVersion since that is the version we      */
/* requested.                                        */
//********************************************************************

    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        exit(1);
    }
    else{
		  printf("\n\n<<<TCP SERVER>>>\n"); 
		  printf("\nThe Winsock 2.2 dll was initialised.\n");
	 }

/*     */

make_END();
unsigned long long rsa_e = END[0];
unsigned long long rsa_d = END[3];
unsigned long long rsa_n = END[1];

unsigned long long CA_d = 553802299;
unsigned long long CA_n = 603706469;

/*     */

	 

//********************************************************************
// set the socket address structure.
//
//********************************************************************
struct addrinfo *result = NULL;
struct addrinfo hints;
int iResult;


//********************************************************************
// STEP#0 - Specify server address information and socket properties
//********************************************************************

	 
//ZeroMemory(&hints, sizeof (hints)); //alternatively, for Windows only
memset(&hints, 0, sizeof(struct addrinfo));

if(USE_IPV6){
   hints.ai_family = AF_INET6;  
}	 else { //IPV4
   hints.ai_family = AF_INET;
}	 

hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = IPPROTO_TCP;
hints.ai_flags = AI_PASSIVE; // For wildcard IP address 
                             //setting the AI_PASSIVE flag indicates the caller intends to use 
									  //the returned socket address structure in a call to the bind function. 

// Resolve the local address and port to be used by the server
if(argc==2){	 
	 iResult = getaddrinfo(NULL, argv[1], &hints, &result); //converts human-readable text strings representing hostnames or IP addresses 
	                                                        //into a dynamically allocated linked list of struct addrinfo structures
																			  //IPV4 & IPV6-compliant
	 sprintf(portNum,"%s", argv[1]);
	 printf("\nargv[1] = %s\n", argv[1]); 	
} else {
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result); //converts human-readable text strings representing hostnames or IP addresses 
	                                                             //into a dynamically allocated linked list of struct addrinfo structures
																				    //IPV4 & IPV6-compliant
	 sprintf(portNum,"%s", DEFAULT_PORT);
	 printf("\nUsing DEFAULT_PORT = %s\n", portNum); 
}

if (iResult != 0) {
    printf("getaddrinfo failed: %d\n", iResult);
    WSACleanup();
    return 1;
}	 

//********************************************************************
// STEP#1 - Create welcome SOCKET
//********************************************************************

s = INVALID_SOCKET; //socket for listening
// Create a SOCKET for the server to listen for client connections

s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

//check for errors in socket allocation
if (s == INVALID_SOCKET) {
    printf("Error at socket(): %d\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    exit(1);//return 1;
}
//********************************************************************

	
//********************************************************************
//STEP#2 - BIND the welcome socket
//********************************************************************

// bind the TCP welcome socket to the local address of the machine and port number
    iResult = bind( s, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
		 
        closesocket(s);
        WSACleanup();
        return 1;
    }
	 
	 freeaddrinfo(result); //free the memory allocated by the getaddrinfo 
	                       //function for the server's address, as it is 
	                       //no longer needed
//********************************************************************
	 
/*
   if (bind(s,(struct sockaddr *)(&localaddr),sizeof(localaddr)) == SOCKET_ERROR) {
      printf("Bind failed!\n");
   }
*/
	
//********************************************************************
//STEP#3 - LISTEN on welcome socket for any incoming connection
//********************************************************************
	if (listen( s, SOMAXCONN ) == SOCKET_ERROR ) {
     printf( "Listen failed with error: %d\n", WSAGetLastError() );
     closesocket(s);
     WSACleanup();
     exit(1);
   } else {
		printf("\n<<<SERVER>>> is listening at PORT: %s\n", portNum);
	}
	
//*******************************************************************
//INFINITE LOOP
//********************************************************************
while (1) {  //main loop
      addrlen = sizeof(clientAddress); //IPv4 & IPv6-compliant
		
//********************************************************************
//NEW SOCKET newsocket = accept
//********************************************************************
      
	   ns = INVALID_SOCKET;

		//Accept a client socket
		//ns = accept(s, NULL, NULL);

//********************************************************************	
// STEP#4 - Accept a client connection.  
//	accept() blocks the iteration, and causes the program to wait.  
//	Once an incoming client is detected, it returns a new socket ns
// exclusively for the client.  
// It also extracts the client's IP address and Port number and stores
// it in a structure.
//********************************************************************
	
	ns = accept(s,(struct sockaddr *)(&clientAddress),&addrlen); //IPV4 & IPV6-compliant
	
	if (ns == INVALID_SOCKET) {
		 printf("accept failed: %d\n", WSAGetLastError());
		 closesocket(s);
		 WSACleanup();
		 return 1;
	} else {
		printf("\nA <<<CLIENT>>> has been accepted.\n");
		
		//strcpy(clientHost,inet_ntoa(clientAddress.sin_addr)); //IPV4
		//sprintf(clientService,"%d",ntohs(clientAddress.sin_port)); //IPV4
		
		memset(clientHost, 0, sizeof(clientHost));
		memset(clientService, 0, sizeof(clientService));

      getnameinfo((struct sockaddr *)&clientAddress, addrlen,
                    clientHost, sizeof(clientHost),
                    clientService, sizeof(clientService),
                    NI_NUMERICHOST);
		
      printf("\nConnected to <<<Client>>> with IP address:%s, at Port:%s\n",clientHost, clientService);
	}	

// send dCA(e)

unsigned long long enc_rsa_e = repeatSquare(rsa_e,CA_d,CA_n);
sprintf(send_buffer,"%llu\r\n",enc_rsa_e);
bytes = send(ns, send_buffer, strlen(send_buffer), 0);
if (bytes == SOCKET_ERROR) { printf("send failed\n");WSACleanup();exit(1);}
memset(send_buffer,0,sizeof(send_buffer));

// send dCA(n)

unsigned long long enc_rsa_n = repeatSquare(rsa_n,CA_d,CA_n);
sprintf(send_buffer,"%llu\r\n",enc_rsa_n);
bytes = send(ns, send_buffer, strlen(send_buffer), 0);
if (bytes == SOCKET_ERROR) {printf("send failed\n");WSACleanup();exit(1);}
memset(send_buffer,0,sizeof(send_buffer));
printf("%llu %llu\n",rsa_e, rsa_n);
printf("Sending packet: PUBLIC_KEY %llu, %llu\n\n",enc_rsa_e,enc_rsa_n);


// receive ACK

n = 0;
while (1) {
   bytes = recv(ns, &receive_buffer[n], 1, 0);
   if ((bytes == SOCKET_ERROR) || (bytes == 0)) break;
   if (receive_buffer[n] == '\n') { receive_buffer[n] = '\0';break;}
   if (receive_buffer[n] != '\r') n++;
}
printf("Received packet: %s, scannedItems = 1\n",receive_buffer);
   
// receive enonce
n = 0;
while (1) {
   bytes = recv(ns, &receive_buffer[n], 1, 0);
   if ((bytes == SOCKET_ERROR) || (bytes == 0)) break;	 
   if (receive_buffer[n] == '\n') {receive_buffer[n] = '\0';break;}
   if (receive_buffer[n] != '\r') n++; /*ignore CRs*/
}
printf("Received packet: NONCE %s, scannedItems = 1\n",receive_buffer);
char * ptrNonce;
unsigned long long enonce;
enonce = strtoull (receive_buffer, &ptrNonce, 10);
unsigned long long nonce;
nonce = repeatSquare(enonce,rsa_d,rsa_n);
printf("After decryption, received nonce = %llu\n\n",nonce); 

		

// send enonce OK
printf("NONCE = %llu\n",nonce);
printf("Sending packet: ACK 220 nonce OK\n\n");
sprintf(send_buffer,"ACK 220 nonce OK");
strcat(send_buffer,"\r\n");
bytes = send(ns, send_buffer, strlen(send_buffer), 0);
if (bytes == SOCKET_ERROR) {printf("send failed\n");WSACleanup();exit(1);}
memset(send_buffer,0,sizeof(send_buffer));
		
//********************************************************************		
//Communicate with the Client
//********************************************************************
		printf("\n--------------------------------------------\n");
	   printf("the <<<SERVER>>> is waiting to receive commands.\n");
		//Clear user details
		memset(username,0,80);
		memset(passwd,0,80);

      while (1) {
         n = 0;
//********************************************************************
//RECEIVE one command (delimited by \r\n)
//********************************************************************
         while (1) {
            bytes = recv(ns, &receive_buffer[n], 1, 0);

            if ((bytes == SOCKET_ERROR) || (bytes == 0)) break;
					 
            if (receive_buffer[n] == '\n') { /*end on a LF, Note: LF is equal to one character*/  
               receive_buffer[n] = '\0';
               break;
            }
            if (receive_buffer[n] != '\r') n++; /*ignore CRs*/
         }
			
         if ((bytes == SOCKET_ERROR) || (bytes == 0)) break;
         //sprintf(send_buffer, "The <<<Client>>> typed '%s' - There are %d bytes of information\r\n", receive_buffer, n);
//********************************************************************
//PROCESS REQUEST
//********************************************************************			
         printf("The <<<Client>>> sent: %s\n",receive_buffer);


char * pEnd;
unsigned long long ulli1;
unsigned long long receive_ascii[800];
unsigned long long XOR_value;
unsigned long long de_hashed_value;
unsigned long long rsa_value;
char convert_buffer[800];
unsigned long long temp_XOR = nonce;
int i = 0;
ulli1 = strtoull (receive_buffer, &pEnd, 10);
receive_ascii[i] = ulli1;
i++;
while(ulli1 != 0) {ulli1 = strtoull (pEnd, &pEnd, 10);receive_ascii[i] = ulli1;i++;}
int k = 0;
while (receive_ascii[k] != 0) { 
   rsa_value = repeatSquare(receive_ascii[k],rsa_d,rsa_n);
   de_hashed_value = hashing_map[rsa_value];
   XOR_value = de_hashed_value ^ temp_XOR;
   convert_buffer[k] = ull2char(XOR_value);
   temp_XOR = rsa_value;
   k++;
}

printf("%s\n",convert_buffer);

sprintf(send_buffer, "The <<<Client>>> typed '%s' - There are %d bytes of information\r\n", convert_buffer, n);
























//********************************************************************
//SEND
//********************************************************************         
			bytes = send(ns, send_buffer, strlen(send_buffer), 0);
         if (bytes == SOCKET_ERROR) break;
         memset(send_buffer,0,sizeof(send_buffer));
		memset(receive_buffer,0,sizeof(receive_buffer));
      memset(receive_ascii,0,sizeof(receive_ascii));
			memset(convert_buffer,0,sizeof(convert_buffer));
      }
//********************************************************************
//CLOSE SOCKET
//********************************************************************
		int iResult = shutdown(ns, SD_SEND);
      if (iResult == SOCKET_ERROR) {
         printf("shutdown failed with error: %d\n", WSAGetLastError());
         closesocket(ns);
         WSACleanup();
         exit(1);
      }	
//***********************************************************************
      closesocket(ns);
				
		//~ strcpy(clientHost,inet_ntoa(clientAddress.sin_addr));
		//~ sprintf(clientService,"%d",ntohs(clientAddress.sin_port));
		
      printf("\ndisconnected from <<<Client>>> with IP address:%s, Port:%s\n",clientHost, clientService);
		printf("=============================================");
		
} //main loop
//***********************************************************************

	closesocket(s);
	WSACleanup(); /* call WSACleanup when done using the Winsock dll */
   
   return 0;
}


