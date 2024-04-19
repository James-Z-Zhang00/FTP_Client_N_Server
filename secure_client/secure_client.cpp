//159.334 - Networks
//////////////////////
//	CLIENT GCC
/////////////////////

//Ws2_32.lib
#define _WIN32_WINNT 0x501  //to recognise getaddrinfo()


//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <time.h>

using namespace std;

#define DEFAULT_PORT "1234" 

#define WSVERS MAKEWORD(2,2)

#define USE_IPV6 false //true

WSADATA wsadata;

enum CommandName{USER, PASS, SHUTDOWN};
unsigned long long char2ull(char c) {return (unsigned long long) (c);}

/*                     */

unsigned long long CA_n = 603706469;
unsigned long long CA_e = 30199;


/*                     */


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

 unsigned long long make_nonce() {
    return rand() % 128;
}

unsigned long long repeatSquare(unsigned long long x, unsigned long long e, unsigned long long n) {
    unsigned long long y = 1;
    while (e > 0) {
        if ((e % 2) == 0) {x = (x*x) % n; e = e / 2;}
        else {y = (x*y) % n; e = e - 1;}
    }
    return y;
}

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

unsigned long long nonce = make_nonce();



//*******************************************************************
// Initialization
//*******************************************************************
	
	char portNum[12];
	
   SOCKET s;
   char send_buffer[80000],receive_buffer[80000];
   char input_buffer[800];
   int n,bytes;

//*******************************************************************
//WSASTARTUP 
//*******************************************************************
srand (time(NULL)); // seed the rand()
   if (WSAStartup(WSVERS, &wsadata) != 0) {
      WSACleanup();
      printf("WSAStartup failed\n");
   	exit(1);
   } else {
		printf("\n\n===================<< CLIENT >>==================\n"); 
		printf("\nThe Winsock 2.2 dll was initialised.\n");
	}


//********************************************************************
// set the socket address structure.
//
//********************************************************************
struct addrinfo *result = NULL, hints;
int iResult;


//ZeroMemory(&hints, sizeof (hints)); //alternatively, for Windows only
memset(&hints, 0, sizeof(struct addrinfo));

if(USE_IPV6){
   hints.ai_family = AF_INET6;  
}	 else { //IPV4
   hints.ai_family = AF_INET;
}
	
hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = IPPROTO_TCP;
//hints.ai_flags = AI_PASSIVE;// PASSIVE is only for a SERVER	
	
	
	
//*******************************************************************
//	Dealing with user's arguments
//*******************************************************************
	
	//if there are 3 elements passed to the argv[] array.
   if (argc == 3){ 		
	   sprintf(portNum,"%s", argv[2]);
	   iResult = getaddrinfo(argv[1], portNum, &hints, &result);
	} else {
	   printf("USAGE: ClientWindows IP-address [port]\n"); //missing IP address
		sprintf(portNum,"%s", DEFAULT_PORT);
		printf("Default portNum = %s\n",portNum);
		printf("Using default settings, IP:127.0.0.1, Port:1234\n");
		iResult = getaddrinfo("127.0.0.1", portNum, &hints, &result);
	}
	
	if (iResult != 0) {
		 printf("getaddrinfo failed: %d\n", iResult);
		 WSACleanup();
		 return 1;
   }	 
	
//*******************************************************************
//CREATE CLIENT'S SOCKET 
//*******************************************************************
   s = INVALID_SOCKET; 	
	s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	
   if (s == INVALID_SOCKET) {
      printf("socket failed\n");
		freeaddrinfo(result);
		WSACleanup();
   	exit(1);
   }
  
	
	 if (connect(s, result->ai_addr, result->ai_addrlen) != 0) {
      printf("connect failed\n");
		freeaddrinfo(result);
		WSACleanup();
   	exit(1);
   } else {
		
		char ipver[80];
		
		// Get the pointer to the address itself, different fields in IPv4 and IPv6
		if (result->ai_family == AF_INET)
		{
			strcpy(ipver,"IPv4");
		}
		else if(result->ai_family == AF_INET6)
		{
			strcpy(ipver,"IPv6");
		}
			
		printf("\nConnected to SERVER with IP address: %s, %s at port: %s\n", argv[1], ipver,portNum);
	}






// receive dCA(e)
unsigned long long rsa_e;
unsigned long long rsa_n;
n = 0;
while (1) {
   bytes = recv(s, &receive_buffer[n], 1, 0);
   if ((bytes == SOCKET_ERROR) || (bytes == 0)) { printf("recv failed\n");exit(1);}
   if (receive_buffer[n] == '\n') {  receive_buffer[n] = '\0';break;}
   if (receive_buffer[n] != '\r') n++;   
}
char * ptrRsae;
unsigned long long enc_rsa_e;
enc_rsa_e = strtoull (receive_buffer, &ptrRsae, 10);
rsa_e = repeatSquare(enc_rsa_e,CA_e,CA_n);

// receive dCA(n)

n = 0;
while (1) {
   bytes = recv(s, &receive_buffer[n], 1, 0);
   if ((bytes == SOCKET_ERROR) || (bytes == 0)) { printf("recv failed\n");exit(1);}
   if (receive_buffer[n] == '\n') {  receive_buffer[n] = '\0';break;}
   if (receive_buffer[n] != '\r') n++;   
}
char * ptrRsad;
unsigned long long enc_rsa_n;
enc_rsa_n = strtoull (receive_buffer, &ptrRsad, 10);
rsa_n = repeatSquare(enc_rsa_n,CA_e,CA_n);
printf("Received Server's Certificate: PUBLIC_KEY %llu, %llu, scannedItems = 2\n", enc_rsa_e, enc_rsa_n);
printf("Decrypted Server's Public Key: [e = %llu, n = %llu]\n", rsa_e, rsa_n);

// send ACK

printf("---> Sending reply to SERVER: ACK 226 public key received\n");
sprintf(send_buffer,"ACK 226 public key received\r\n");
bytes = send(s, send_buffer, strlen(send_buffer),0);
memset(send_buffer,0,sizeof(send_buffer));

// send enonce
unsigned long long enonce;
enonce = repeatSquare(nonce,rsa_e,rsa_n);
printf("\n---> Sending Nonce to SERVER: %llu\n",enonce); // use encrypted nonce here
sprintf(send_buffer,"%llu\r\n",enonce);
bytes = send(s, send_buffer, strlen(send_buffer),0);
memset(send_buffer,0,sizeof(send_buffer));

// receive ACK 220 enonce OK 
n = 0;
while (1) {
   bytes = recv(s, &receive_buffer[n], 1, 0);
   if ((bytes == SOCKET_ERROR) || (bytes == 0)) { printf("recv failed\n");exit(1);}
   if (receive_buffer[n] == '\n') {receive_buffer[n] = '\0';break;}
   if (receive_buffer[n] != '\r') n++;  
}
printf("\nReceived packet: %s, scannedItems = 1\n",receive_buffer);
printf("nonce ACKed by Server\n");









	
//*******************************************************************
//Get input while user don't type "."
//*******************************************************************
	printf("\n--------------------------------------------\n");
	printf("you may now start sending commands to the <<<SERVER>>>.\n");
	printf("==============================================\n");
	printf("Enter message to send:\n");
	printf("==============================================\n");
   gets(input_buffer);
   unsigned long long ascii_value;
   unsigned long long position;
   unsigned long long hashed_value;
   unsigned long long rsa_value;
   char temp_buffer[20];
   while (strcmp(input_buffer,".") != 0) {
	   unsigned long long temp_XOR = nonce; // nonce assigned here
	   int buffer_size;
	   buffer_size = strlen(input_buffer);
	   for (int pos = 0; pos < buffer_size; pos++) {
		   ascii_value = char2ull(input_buffer[pos]);
		   position = ascii_value ^ temp_XOR;
		   hashed_value = hashing_map[position];

			rsa_value = repeatSquare(hashed_value,rsa_e,rsa_n);


		sprintf(temp_buffer,"%llu",rsa_value);
		   strcat(send_buffer,temp_buffer);
		   strcat(send_buffer," ");
		   temp_XOR = hashed_value;
	   }

		printf("SENDING TO THE SERVER: %s\n",send_buffer);

      strcat(send_buffer,"\r\n");
//*******************************************************************
//SEND
//*******************************************************************
      bytes = send(s, send_buffer, strlen(send_buffer),0);
		cout << "sent " << bytes << " characters." << endl;
		
      if (bytes == SOCKET_ERROR) {
         printf("send failed\n");
			WSACleanup();
      	exit(1);
      }
	memset(send_buffer,0,sizeof(send_buffer));
		memset(input_buffer,0,sizeof(input_buffer));
      n = 0;
      while (1) {
//*******************************************************************
//RECEIVE
//*******************************************************************
         bytes = recv(s, &receive_buffer[n], 1, 0);
         if ((bytes == SOCKET_ERROR) || (bytes == 0)) {
            printf("recv failed\n");
         	exit(1);
         }
         if (receive_buffer[n] == '\n') {  /*end on a LF*/
            receive_buffer[n] = '\0';
            break;
         }
         if (receive_buffer[n] != '\r') n++;   /*ignore CR's*/
      }
      printf("SERVER's reply:%s\n",receive_buffer);
		
	
		memset(send_buffer,0,sizeof(send_buffer));
		memset(input_buffer,0,sizeof(input_buffer));
      gets(input_buffer);
	
   }
	printf("\n--------------------------------------------\n");
	printf("CLIENT is shutting down...\n");
//*******************************************************************
//CLOSESOCKET   
//*******************************************************************
   closesocket(s);
	WSACleanup();
   return 0;
}

