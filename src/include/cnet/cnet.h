#ifndef H_CNET
#define H_CNET

#include <netinet/in.h>

typedef struct client
{
    int socket;
    struct sockaddr_in dest;
    int err;
    int port;
} client;

typedef struct server
{
    int socket;
    struct sockaddr_in dest;
    size_t addrlen;
    int sockopt;
    int max_conn;
    int err;
    int port;
} server;

typedef struct connection
{
    int err;
    int fdesc;
} connection;

client *init_client(const char *ip, int port, client *in_client);

client *close_client(client *in_client);

server *init_server(int port, unsigned int max_conn, server *in_server);

connection *connection_server(connection *in_conn, server *in_server);

connection *connection_client(connection *in_conn, client *in_client);

connection *send_connection(connection *in_conn, const char *data, size_t m_len);

connection *read_connection(connection *in_conn, char *to_write, size_t w_len);

#endif // H_CNET