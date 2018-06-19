#include <stdio.h>
#include <string.h>

#include <nerror1/nerror.h>
#include <cnet/cnet.h>

#define PORT 3333
#define MAX_CONN 20
#define BUFF_SIZE 1024
#define HELLO_SERVER "Hello from the server"

#define CLOSE_UP(x)                 \
	if (x == 0)                     \
	{                               \
		PNERROR(low, "Exiting..."); \
	}                               \
	return x

#define CHECK_ERR(x, ...)                  \
	if (x != 0)                            \
	{                                      \
		NERROR(stderr, high, __VA_ARGS__); \
		NERROR(stderr, high, strerror(x)); \
		CLOSE_UP(1);                       \
	}

int main(int argc, char **argv)
{
	// Variables
	server my_server;					// Server accepts clients
	connection my_conn;					// Connection to client
	const char *to_send = HELLO_SERVER; // Data to send
	size_t ts_len = strlen(to_send);	// Length
	char buff[BUFF_SIZE];				// Data buffer

	// Initialize server
	memset(&my_server, 0, sizeof(my_server)); // Zero server
	init_server(PORT, MAX_CONN, &my_server);
	CHECK_ERR(my_server.err, "Failed to init server");
	PNERROR(low, "Hosting on port %d...", PORT);

	// Listen
	while (1)
	{
		memset(&my_conn, 0, sizeof(my_conn)); // Zero connection
		connection_server(&my_conn, &my_server);
		CHECK_ERR(my_conn.err, "Failed to connect to client");
		PNERROR(low, "Responding to client with %s...", to_send);

		send_connection(&my_conn, to_send, ts_len);
		CHECK_ERR(my_conn.err, "Failed to send data");

		read_connection(&my_conn, buff, BUFF_SIZE);
		CHECK_ERR(my_conn.err, "Failed to read from client");
		PNERROR(low, "Received %s from client", buff);
	}

	// Exit
	CLOSE_UP(0);
}