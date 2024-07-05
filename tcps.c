
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main()
{
	int server_fd , new_socket;
	struct sockaddr_in address;
	char buffer[1024] = {0};
	int addrlen = sizeof(address);
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(int){1}, sizeof(int));
	
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr =INADDR_ANY;
	address.sin_port = htons(PORT);
	
	bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	listen(server_fd, 3);
	
	while(1)
	{
		char hello[100] = {0};
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		read(new_socket, buffer, 1024);
		printf("%s msg got\n",buffer);
		
		printf("msg to send ");
		fgets(hello, sizeof(hello), stdin);
		hello[strcspn(hello,"\n")] =0;
		
		send(new_socket, hello, strlen(hello), 0);
		
		close(new_socket);
		
	}
	close(server_fd);
	return 0;
}
