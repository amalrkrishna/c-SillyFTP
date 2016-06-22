//TCP Echo Server
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#define buffersize 1024
#define UDPSERVERPORT 7000
#define TCPSERVERPORT 6000
int main()
{
	int sock,req_socket, conn_socket, sock_len, connect_status, size=0;
	int bind_status, bytes_received, recv_bytes, b;
	char recv_buf[buffersize],recv_buffer[100], z[100], a[100];
	struct sockaddr_in s_server, s_client, sock_server, sock_client;
	int si_len = sizeof(s_client);

	/*creating the UDP socket */
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock < 0){
		printf("socket creation failed.\n");
		return 1;
	}
	else{
		printf("socket creation successful at %d\n", sock);
	}

	/*initilizing and binding the socket */
	s_server.sin_family = AF_INET;
	s_server.sin_port = htons(UDPSERVERPORT);
	s_server.sin_addr.s_addr = htonl(INADDR_ANY);
	bind_status = bind(sock, (struct sockaddr*)&s_server,
	sizeof(s_server));
	if(bind_status < 0){
		printf("binding socket failed.\n");
	}
	else{
		printf("binding socket successful.\n");
	}

	/*server wait for a filename */
	bytes_received = recvfrom(sock, z, sizeof(recv_buf), 0, (struct
	sockaddr*)&s_client, &si_len);
	printf("File name received: %s\n", z);

	/* wait for the file size */
	bytes_received = recvfrom(sock, a, sizeof(recv_buf), 0, (struct
	sockaddr*)&s_client, &si_len);
	b = atoi (a);
	printf("Size of the file is: %d\n", b);

	/* opening the file */
	FILE *fp;
	fp = fopen(z, "w");
	/*create TCP server socket for request*/
	req_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(req_socket == -1){
		printf("socket creation failed \n");
		close(req_socket);
		return 1;
	}
	else{
		printf("socket creation successful with descriptor %d \n",
		req_socket);
	}

	/*binding the socket*/
	sock_server.sin_family = AF_INET;
	sock_server.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_server.sin_port = htons(TCPSERVERPORT);
	bind_status = bind(req_socket, (struct sockaddr*)&sock_server,
	sizeof(sock_server));
	if(bind_status == 1){
		printf("socket binding failed \n");
		close(req_socket);
		return 1;
	}
	else{
		printf("socket binding successful \n");
	}

	/* listening for tcp connection requests*/
	printf("listening for conection request \n");
	listen(req_socket, 5);
	sock_len = sizeof(sock_client);
	conn_socket = accept(req_socket, (struct sockaddr*)&sock_client,
	&sock_len);
	if(conn_socket == -1){
		printf("socket creation for client failed \n");
		close(conn_socket);
		close(req_socket);
		return 1;
	}
	else{
		printf("socket created for client with descriptor %d \n",
		conn_socket);
	}

	/* receiving the content of the file */
	while (size < b){
		recv_bytes = recv(conn_socket, recv_buffer, buffersize, 0);
		fwrite(recv_buffer, 1, recv_bytes, fp);
		size = size + recv_bytes;
	}
	printf("File received successfully. \n");
	fclose(fp);
	close(sock);
}
