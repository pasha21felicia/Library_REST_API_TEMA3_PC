#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "commands.hpp"
#include <bits/stdc++.h>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

#define HOST "34.118.48.238"
#define PORT 8080
#define REGISTER "/api/v1/tema/auth/register"
#define AUTH "/api/v1/tema/auth/login"
#define REQUEST_ACCESS "/api/v1/tema/library/access"
#define VIEW_ALL_INFO "/api/v1/tema/library/books"
#define VIEW_ONE_INFO "/api/v1/tema/library/books/"
#define ADD_BOOK "/api/v1/tema/library/books"
#define DELETE_BOOK "/api/v1/tema/library/books/"
#define LOGOUT "/api/v1/tema/auth/logout"

#define MAX_USER_DATA 16


string get_Cookies(char *response) {   
    char *p = strstr(response, "Set-Cookie: ");
    char *token = strtok(p, " ;:");
    while (token != NULL) {
        token = strtok(NULL, " ");
        break; 
    }
    string cookie(token);
    cookie.erase(cookie.length()-1);
    return cookie;
}

int get_StatusCode(char *response) {
    char *token = strtok(response, " ");
    int statusCode;
    while (token != NULL) {
        token = strtok(NULL, " ");
        if (token[0] > '1' && token[0] < '9' ) {
            statusCode = atoi(token);
            break;
        }    
    }
    return statusCode;
}

void register_user(string username, string password, int sockfd) {
    bool auth = false;
    char *response;
    char *message;
    json register_data = {
        { "username", username },
        { "password", password }
    };
    string content = register_data.dump(); //transform to string the json data

    message = compute_post_request(HOST, REGISTER, "application/json", auth, NULL, content.c_str() , NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    
    // puts(response);
    int statusCode = get_StatusCode(response);

    switch (statusCode) {
    case 200:
        cout << "Registered successfully. Welcome, " << username << "!\n";
        break;
    case 201:
        cout << "Registered successfully. Welcome, " << username << "!\n";
        break;
    case 400:
        cout << "An error was encountered while registering this user!\n";
        break;
    case 429:
        cout << "Too many requests! Take it slower.\n";
        break;
    default:
        cout << "An error was encountered while registering this user!\n";
        break;
    }
}
string login_user(string username, string password, int sockfd) {
    bool auth = false;
    char *response;
    char *response_copy = (char *) calloc(BUFLEN, sizeof(char));
    char *message;
    string cookie;
    json login_data = {
        { "username", username },
        { "password", password }
    };
    string content = login_data.dump(); //transform to string the json data

    message = compute_post_request(HOST, AUTH, "application/json", auth, NULL, content.c_str(), NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    strcpy(response_copy, response);

    //puts(response_copy);
    int statusCode = get_StatusCode(response);

    switch (statusCode) {
        case 200:
            cookie = get_Cookies(response_copy);
            cout << "Successfully logged in. Hi, " << username << "!\n";
            break;
        case 201:
            cout << "intra aici 0 \n";
            cookie = get_Cookies(response_copy);
            cout << "Successfully logged in. Hi, " << username << "!\n";
            break;
        case 400:
            cout << "Bad Request!\n";
            break;
        case 429:
            cout << "Too many requests! Take it slower.\n";
            break;
        default:
            cout << "An error was encountered while registering this user!\n";
            break;
    }
    return cookie;
}
string enter_library(string cookie, int sockfd) {
    bool auth = false;
    char *message;
    char *response;
    char *response_copy = (char *) calloc(BUFLEN, sizeof(char));
    string accessToken;

    char **cookies = (char**)malloc(1 * sizeof(char*));
    int len = cookie.length() + 1;
    cookies[0] = new char[len];
    strcpy(cookies[0], cookie.c_str());
    
    message = compute_get_request(HOST, REQUEST_ACCESS, NULL, auth, NULL,  cookies, 1);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    strcpy(response_copy, response);

    int statusCode = get_StatusCode(response);
   
    //puts(response_copy);
    string jsonStr;
    json jsonVal;
    jsonStr = basic_extract_json_response(response_copy);
    jsonVal = json::parse(jsonStr); 

    switch (statusCode) {
        case 200:
            accessToken = "Bearer " + jsonVal["token"].get<string>();
            cout << "Access opened!\n";
            break;
        default:
            cout << "An error occured while performing this procedure!: " << "\n";
            break;
    }

    free(cookies[0]);
    free(cookies);
    return accessToken;
}

void add_book(string access_token, string title, string author, string genre, string publisher, int page_count, int sockfd) {
    bool auth = true;
    char *message;
    char *response;
    char *response_copy = (char *) calloc(BUFLEN, sizeof(char));

    json book_data = {
        { "title", title },
        { "author", author },
        { "genre", genre },
        { "publisher", publisher },
        { "page_count", page_count }
    };

    string payload = book_data.dump();
    
    message = compute_post_request(HOST, ADD_BOOK, "application/json", auth, access_token.c_str(), payload.c_str(), NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    //puts(response);

    strcpy(response_copy, response);

    int statusCode = get_StatusCode(response);

    switch (statusCode) {
        case 200:
            cout << "Book added successfully" << "\n";
            break;
        default:
             cout << "An error occured while performing this procedure!: " << "\n";
            break;
    }
}

void get_all_books(int sockfd, string access_token) {
    bool auth = true;
    char *message;
    char *response;
    char *response_copy = (char *) calloc(BUFLEN, sizeof(char));

    message = compute_get_request(HOST, VIEW_ALL_INFO, NULL, auth, access_token.c_str(), NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    // puts(response);
    strcpy(response_copy, response);

    int statusCode = get_StatusCode(response);
    char *booksList;

    switch (statusCode) {
        case 200:
            booksList = basic_extract_json_response(response_copy);
            if (booksList == NULL) {
                cout << "We couldn't find any books in your library.\n";
                break;
            }
            cout << "[" + string(booksList) << "\n";
            break;
        default:
            cout << "An error occured while performing this procedure!: " << "\n";
            break;
    }
}


void get_book(int sockfd, string access_token, int id) {
    bool auth = true;
    char *message;
    char *response;
    char *response_copy = (char *) calloc(BUFLEN, sizeof(char));

    string book_route = VIEW_ONE_INFO + to_string(id);

    message = compute_get_request(HOST, book_route.c_str(), NULL, auth, access_token.c_str(), NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    //puts(response);
    strcpy(response_copy, response);

    int statusCode = get_StatusCode(response);

    string bookDetails;

    switch (statusCode) {
        case 200:
            bookDetails = basic_extract_json_response(response_copy);
            cout << bookDetails << "\n";
            break;
        default:
            cout << "An error occured while performing this procedure!: " << "\n";
            break;
    }
}

void delete_book(int sockfd, string access_token, int id) {
    bool auth = true;
    char *message;
    char *response;
    char *response_copy = (char *) calloc(BUFLEN, sizeof(char));
    string book_route = VIEW_ONE_INFO + to_string(id);

    message = compute_delete_request(HOST, book_route.c_str(), NULL, auth, access_token.c_str(), NULL, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    //puts(response);
    strcpy(response_copy, response);

    int statusCode = get_StatusCode(response);

    string bookDetails;

    switch (statusCode) {
        case 200:
            cout << "Book deleted successfully." << "\n";
            break;
        default:
            cout << "An error occured while performing this procedure!: " << "\n";
            break;
    }

}

void logout(int sockfd, string cookie) {
    bool auth = false;
    char *message;
    char *response;
    char *response_copy = (char *) calloc(BUFLEN, sizeof(char));

    char **cookies = (char**)malloc(1 * sizeof(char*));
    int len = cookie.length() + 1;
    cookies[0] = new char[len];
    strcpy(cookies[0], cookie.c_str());

    message = compute_get_request(HOST, LOGOUT, NULL, auth, NULL, cookies, 1);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    //puts(response);
    strcpy(response_copy, response);

    int statusCode = get_StatusCode(response);
    switch (statusCode) {
        case 200:
            cout << "Logged out!" << "\n";
            break;
        default:
            cout << "An error occured while performing this procedure!: " << "\n";
            break;
    }

    free(cookies[0]);
    free(cookies);
}
