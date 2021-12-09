
#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<fcntl.h>
#include<cJSON/cJSON.h>
#include<utils_cpp/utils.h>
#include<math.h>
#include<string.h>


const int batch_size = 512;

int main(){

	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch = 'A';

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("192.168.10.100");
	address.sin_port = htons(1025);
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);

	int numRead = 0;
	int fd;
	// const char *filename = "/home/dev/vim.20190506-093345.tar.xz";
	const char *filename = "/home/dev/code";

	unsigned long filesize = getFileSizeU(filename);
	unsigned long epoch = 0;
	unsigned long epochs = ceil( filesize * 1.0 / batch_size  );

	fd = open(filename, O_RDONLY);
	char buf[BUF_SIZE];
	while((numRead = read(fd, buf, batch_size)) > 0){
		epoch ++;
		// printf("buf = %s\n", buf);

		cJSON* data = cJSON_CreateObject();
		cJSON_AddStringToObject(data, "name", "voice");
		cJSON_AddStringToObject(data, "raw", buf);
		cJSON_AddNumberToObject(data, "epoch", epoch);
		cJSON_AddNumberToObject(data, "epochs", epochs);

		char *out = cJSON_Print(data);

		printf("out = %s\n size = %lu\n", out, strlen(out));
		// write(sockfd, buf, BUF_SIZE);
		write(sockfd, out, strlen(out));
		sleep(10);
	}
	close(fd);
	close(sockfd);

}
