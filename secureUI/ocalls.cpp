#include "DVSE_u.h"
#include <stdio.h>
#include <time.h>

#pragma comment(lib,"Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>



void* ocall_file_open(/*[in, out, string] */char* file_name,
	/*[in, out, string] */char* format)
{

	return (void*)fopen(file_name, format);
}

int ocall_file_close(/*[user_check]*/void* handle)
{
	return fclose((FILE*)handle);
}

int ocall_file_read(void *handle, size_t offset, size_t size, /*[in, out]*/unsigned char *data)
{
	size_t pos = ftell((FILE*)handle);
	if (pos != offset)
	{
		fseek((FILE*)handle, offset, SEEK_SET);
	}
	return fread(data, 1, size, (FILE*) handle);
}

int ocall_file_write(/*[user_check]*/void *handle, size_t size, /*[in, out]*/unsigned char *data)
{
	return fwrite(data, 1, size, (FILE*)handle);
}

SOCKET s = INVALID_SOCKET;

int ocall_socket_connect(/*[in, string]*/char *url, unsigned int port)
{
	struct sockaddr_in server;
	s = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_addr.s_addr = inet_addr(url);
	server.sin_family = AF_INET;
	server.sin_port = port;
	if (connect(s,(struct sockaddr*) &server, sizeof(server)) < 0)
	{
		return 0;
	}
	return 1;
}
int ocall_socket_send(/*[in, out, size = data_size]*/ void* data, size_t data_size)
{
	return send(s,(const char*)data, data_size, 0);
}
int ocall_socket_receive(/*[in, out, size = data_size]*/ void* data, size_t data_size)
{
	return recv(s, (char*) data,data_size, 0 );
}
int ocall_socket_shutdown()
{
	return shutdown(s, SD_BOTH);
}
int ocall_get_the_current_time(unsigned char time_holder[16])
{
	static_assert(sizeof (time_t) < 16, "NEED mode buffer for time repr"); 
	time((time_t*)time_holder);
	return 0;
}
size_t ocall_file_size(void* handle)
{
	size_t pos = ftell((FILE*)handle);
	fseek((FILE*)handle, 0, SEEK_END);
	size_t sz = ftell((FILE*)handle);
	fseek((FILE*)handle, pos, SEEK_SET); // getting back to thne position
	return sz;
}