#define _GNU_SOURCE /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "cnet.h"

client *init_client(const char *ip, int port, client *in_client)
{
    in_client->port = port;
    in_client->err = 0;

    in_client->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (in_client->socket == -1)
    {
        in_client->err = errno;
        return in_client;
    }

    memset(&in_client->dest, 0, sizeof(in_client->dest)); /* zero the struct */
    in_client->dest.sin_family = AF_INET;
    in_client->dest.sin_addr.s_addr = inet_addr(ip); /* set destination IP number - localhost, 127.0.0.1*/
    in_client->dest.sin_port = htons(port);          /* set destination port number */

    if (connect(in_client->socket, (struct sockaddr *)&in_client->dest, sizeof(struct sockaddr_in)) == -1)
    {
        in_client->err = errno;
        return in_client;
    }
    return in_client;
}

client *close_client(client *in_client)
{
    in_client->err = 0;
    if (close(in_client->socket) == -1)
    {
        in_client->err = errno;
    }
    return in_client;
}

server *init_server(int port, unsigned int max_conn, server *in_server)
{
    // Set values
    in_server->port = port;
    in_server->max_conn = max_conn;
    in_server->addrlen = sizeof(in_server->dest);
    in_server->err = 0;

    // Create socket ifle descriptor
    if ((in_server->socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        in_server->err = errno;
        return in_server;
    }
    // Attach socket
    if (setsockopt(in_server->socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(in_server->sockopt), sizeof(in_server->sockopt)))
    {
        in_server->err = errno;
        return in_server;
    }

    // Zero settings
    memset(&(in_server->dest), 0, sizeof(in_server->dest));

    // Settings
    in_server->dest.sin_family = AF_INET;
    in_server->dest.sin_addr.s_addr = INADDR_ANY;
    in_server->dest.sin_port = htons(port);

    // Bind to the port
    if (bind(in_server->socket, (struct sockaddr *)&(in_server->dest), sizeof(in_server->dest)) < 0)
    {
        in_server->err = errno;
        return in_server;
    }

    // Listen to the port
    if (listen(in_server->socket, max_conn) < 0)
    {
        in_server->err = errno;
        return in_server;
    }

    return in_server;
}
connection *connection_server(connection *in_conn, server *in_server)
{
    in_conn->err = 0;
    in_conn->fdesc = accept(in_server->socket, (struct sockaddr *)&(in_server->dest), (socklen_t *)&(in_server->addrlen));
    if (in_conn->fdesc <= 0)
    {
        in_conn->err = errno;
    }
    return in_conn;
}

connection *connection_client(connection *in_conn, client *in_client)
{
    in_conn->err = 0;
    in_conn->fdesc = in_client->socket;
    return in_conn;
}

connection *send_connection(connection *in_conn, const char *data, size_t m_len)
{
    in_conn->err = 0;
    size_t sent = send(in_conn->fdesc, data, m_len, 0);
    if (sent != m_len)
    {
        in_conn->err = errno;
    }
    return in_conn;
}

connection *read_connection(connection *in_conn, char *to_write, size_t w_len)
{
    in_conn->err = 0;
    size_t received = read(in_conn->fdesc, to_write, w_len);
    if (received <= 0)
    {
        in_conn->err = errno;
    }
    return in_conn;
}