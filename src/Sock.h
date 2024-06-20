#pragma once

#if defined(_WIN32) || defined(_WIN64)

#define __SYS_WINDOWS

#include <winsock2.h>
#include <ws2tcpip.h>

typedef SOCKET socket_t;
typedef int msg_size_t;

#define WSA_LAST_ERR WSAGetLastError()
#define SOCKET_SEND SD_SEND
#define SOCKET_RECEIVE SD_RECEIVE

#pragma comment (lib, "Ws2_32.lib")

#elif defined(__unix__) || defined(__linux__)

#define __SYS_LINUX

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef int socket_t;
typedef ssize_t msg_size_t;

#define INVALID_SOCKET (socket_t)(-1)
#define SOCKET_ERROR (-1)
#define SOCKET_SEND SHUT_RDWR
#define SOCKET_RECEIVE SHUT_RDWR
#define WSA_LAST_ERR errno

#else

#error Library is not supported on this platform

#endif