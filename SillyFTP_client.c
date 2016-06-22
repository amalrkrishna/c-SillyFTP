//TCP Echo Client
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#define buffersize 1024
#define TCPSERVERPORT 6000
#define UDPSERVERPORT 7000
#define SERVERADDRESS "127.0.0.1"
/* reading a file name and estimating the size of the file */
int main()
{
	FILE *fp;
	char x,z[100],a[100];
	int y=0;
	printf("Enter the filename:\n");
	scanf("%s",z);
	fp = fopen(z, "r");
	while( ! feof(fp)){
		x = fgetc(fp);
		y++;
	}
	y--;
	printf("File size: ");
	printf("%d\n",y);

	/* creating a UDP socket */
	struct sockaddr_in s_server, sock_server;
	int sock, si_len = sizeof(s_server), bytes_sent, conn_socket,
	conn_status, sock_len;
	char send_buf[buffersize], send_buffer[100];
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock < 0){
		printf("socket creation failed.\n");
		return 1;
	}
	else{
		printf("socket creation successful at %d\n", sock);
	}
	s_server.sin_family = AF_INET;
	s_server.sin_port = htons(UDPSERVERPORT);
	inet_aton(SERVERADDRESS, &s_server.sin_addr);

	/* sending the file name */
	bytes_sent = sendto(sock, z, strlen(z)+1, 0, (struct
	sockaddr*)&s_server, si_len);

	/* sending the file size */
	sprintf(a, "%d", y);
	bytes_sent = sendto(sock, a, strlen(a)+1, 0, (struct
	sockaddr*)&s_server, si_len);

	/*Create a TCP connection socket for client*/
	conn_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (conn_socket == -1){
		printf("Socket creation failed.\n");
		close(conn_socket);
		return 1;
	}
	else{
		printf("Socket creation successful with descriptor %d.\n", conn_socket);
	}

	/*Requesting for a TCP connection to the server*/
	memset(& sock_server, 0, sizeof(sock_server));
	sock_server.sin_family = AF_INET;
	inet_aton(SERVERADDRESS, &sock_server.sin_addr);
	sock_server.sin_port = htons(TCPSERVERPORT);
	sock_len = sizeof(sock_server);
	conn_status = -1;
	rewind (fp);

	while(conn_status == -1){
		conn_status = connect(conn_socket, (struct
		sockaddr*)&sock_server, sock_len);
	}
	printf("File Transfered Successfully.\n");

	/* reading the content of the file and sending to the server */
	int rd;
	while( ! feof(fp)){
		rd = fread(send_buffer, 1, 10, fp);
		bytes_sent = send(conn_socket, send_buffer, rd, 0);
	}
	fclose(fp);
	close(sock);
}
