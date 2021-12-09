#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdint.h>
#include<fcntl.h>
#include<memory.h>
#include<cJSON/cJSON.h>


const int batch_size = 1024;

static char *filename = NULL;

void accept_request(void *);


void accept_request(void *arg){
	printf("accept request ...\n");
	int client = (intptr_t)arg;
	int n;

	char buf[batch_size];
	int fd = -1;

	// fd = open("2.txt", O_CREAT | O_WRONLY);
	fd = open(filename, O_CREAT | O_WRONLY);
	if(fd ==-1){
		printf("file open failed..\n");
		return;
	}
	
	memset(buf, 0, sizeof(0));

	printf("recv data : \n");

	// 这个地方有坑, 注意n=的外侧需要括号, 不然n接受的字节数就会一直是1
	while((n = recv(client, buf, batch_size, 0)) > 0)
	{
		cJSON *data = cJSON_Parse(buf);

		char *name = cJSON_GetStringValue(cJSON_GetObjectItem(data, "name"));
		printf("name = %s\n", name);
		
		char *raw = cJSON_GetStringValue(cJSON_GetObjectItem(data, "raw"));
		printf("raw = %s\n", raw);

		printf("bytes: %d\n", n);
		printf("raw bytes: %d\n", n);

		write(fd, raw, batch_size);
		n = 10;
	}
	close(client);
	close(fd);
	printf("recv finished..\n");

}


int main(int argc, char* argv[]){
	
	filename = argv[1];
	int httpd;
	int port = 1025;
	httpd = socket(AF_INET, SOCK_STREAM, 0);
	if(httpd == -1){
		exit(-1);
	}

	struct sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	
	in_addr_t ip = inet_addr("192.168.10.100");
	if(ip ==-1){
		exit(-1);
	}

	name.sin_addr.s_addr = ip;

	printf("%s\n", inet_ntoa(name.sin_addr));

	int on = 1;

	setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(bind(httpd, (struct sockaddr *)&name, sizeof(name))){
		exit(-1);
	}

	if(listen(httpd, 5) < 0){
		exit(-1);
	}

	struct sockaddr_in client_name;
	socklen_t client_name_len = sizeof(client_name);
	int client_sock = -1;
	pthread_t newthread;

	while(1){
		client_sock = accept(httpd, (struct sockaddr *)&client_name, &client_name_len );

		pthread_create(&newthread, NULL, (void *)accept_request, (void*)(intptr_t)client_sock);
	}
	close(httpd);


}
