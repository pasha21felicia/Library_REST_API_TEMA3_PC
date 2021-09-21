#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>

#include "requests.hpp"
#include "helpers.hpp"

char *compute_delete_request(const char *host, const char *url, const char *query_params, 
                        bool auth, const char *access_token,
                        char **cookies, int cookies_count)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host

    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);

    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format

    if (auth) {
        sprintf(line, "%s: %s", "Authorization", access_token);
        compute_message(message, line);
    }

    if (cookies != NULL) {
        for (int i = 0; i < cookies_count; ++i) {
            memset(line, 0, LINELEN);
            sprintf(line, "Cookie: %s", cookies[i]);

            compute_message(message, line);
        }
    }
    // Step 4: add final new line
    compute_message(message, "");
    return message;
}

char *compute_get_request(const char *host, const char *url, const char *query_params, 
                        bool auth, const char *access_token,
                        char **cookies, int cookies_count)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host

    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);

    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format

    if (auth) {
        sprintf(line, "%s: %s", "Authorization", access_token);
        compute_message(message, line);
    }

    if (cookies != NULL) {
        for (int i = 0; i < cookies_count; ++i) {
            memset(line, 0, LINELEN);
            sprintf(line, "Cookie: %s", cookies[i]);

            compute_message(message, line);
        }
    }
    // Step 4: add final new line
    compute_message(message, "");
    return message;
}

char *compute_post_request(const char *host, const char *url, 
                        const char* content_type, bool auth, 
                        const char *access_token,
                        const char *content, char **cookies, int cookies_count)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *) calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // check if authentification is nessesary 
    if (auth) {
        sprintf(line, "%s: %s", "Authorization", access_token);
        compute_message(message, line);
    }

    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);
    sprintf(line, "Content-Length: %ld", strlen(content));
    compute_message(message, line);

    // if (headers != NULL) {
    //     for (int i = 0; i < headersCount; i++) {
    //         sprintf(line, "%s: %s", headersNames[i], headers[i]);
    //         compute_message(message, line);
    //     }
    // }

    // Step 4 (optional): add cookies
    if (cookies != NULL) {
       for (int i = 0; i < cookies_count; ++i) {
            memset(line, 0, LINELEN);
            sprintf(line, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }
    }
    // Step 5: add new line at end of header
    compute_message(message, "");
    // Step 6: add the actual payload data
    memset(line, 0, LINELEN);
    compute_message(message, content);

    free(line);
    return message;
}

