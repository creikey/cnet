#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <nerror1/nerror.h>
#include <cnet/cnet.h>

#define IP "127.0.0.1"
#define PORT 3333
#define BUFF_SIZE 1028

#define CLOSE_UP(x)                 \
    close_client(&my_client);       \
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
    client my_client;                          // Main client
    char buff[BUFF_SIZE];                      // Receive buffer
    connection my_conn;                        // Connection to server
    const char *to_send = "Testing.. Client!"; // Data to send
    size_t ts_len = strlen(to_send);           // Length of data to send

    // Initialize client
    memset(&my_client, 0, sizeof(my_client)); // Zero client
    init_client(IP, PORT, &my_client);        // Init and connect
    CHECK_ERR(my_client.err, "Failed to init client");
    PNERROR(low, "Successfully connected to %s:%d!", IP, PORT);

    // Make new connection
    memset(&my_conn, 0, sizeof(my_conn));    // Zero connection
    connection_client(&my_conn, &my_client); // Make connection
    CHECK_ERR(my_conn.err, "Failed to connect client");

    // Wait for data
    read_connection(&my_conn, buff, BUFF_SIZE);
    CHECK_ERR(my_conn.err, "Failed to read from connection");
    PNERROR(low, "Received %s", buff);

    // Send data
    send_connection(&my_conn, to_send, ts_len + 1);
    CHECK_ERR(my_conn.err, "Failed to send data");
    PNERROR(low, "Successfully send `%s`", to_send);

    // Exit
    CLOSE_UP(0);
}