#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.hpp"
#include "requests.hpp"
#include "commands.hpp"
#include <bits/stdc++.h>
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

int main(int argc, const char** argv) {

    int sockfd;
    string active_cookie;
    string access_token;
    string input;
    
    cout << "Starting client..." << "\n";

    while (true) {
        cout << "-> ";
        cin >> input;

        if (input.compare("exit") == 0) {
            close_connection(sockfd);
            break;
        }
        if (input.compare("register") == 0) {
            if (active_cookie.length() > 0) {
                cout << "Already logged in.\n";
                continue;
            } 
            string username, password;
            cout << "Register\n";
            getline(cin, username, '\n');
            cout << "username=";
            getline(cin, username, '\n');
            cout << "password=";
            getline(cin, password, '\n');
            sockfd = open_connection((char *)HOST, 8080, AF_INET, SOCK_STREAM, 0);
            register_user(username, password, sockfd);
            close_connection(sockfd);
            continue;
        }
        if (input.compare("login") == 0) {
            if (active_cookie.length() > 0) {
                cout << "Already logged in.\n";
                continue;
            } 
            string username, password;
            cout << "Login\n";
            getline(cin, username, '\n');
            cout << "username=";
            getline(cin, username, '\n');
            cout << "password=";
            getline(cin, password, '\n');
            sockfd = open_connection((char *)HOST, 8080, AF_INET, SOCK_STREAM, 0);
            active_cookie = login_user(username, password, sockfd);
            close_connection(sockfd);
            continue;
        }
        if (input.compare("enter_library") == 0) {
            if (active_cookie.empty()) {
                cout << "You should Log In first.\n";
                continue;
            }
            sockfd = open_connection((char *)HOST, 8080, AF_INET, SOCK_STREAM, 0);
            access_token = enter_library(active_cookie, sockfd);
            close_connection(sockfd);
            continue;
        }
        if (input.compare("get_books") == 0)  {
            if (access_token.empty()) {
                cout << "You should use the command 'enter_library' first.\n";
                continue;
            }
            sockfd = open_connection((char *)HOST, 8080, AF_INET, SOCK_STREAM, 0);
            get_all_books(sockfd, access_token);
            close_connection(sockfd);
            continue;
        }
        if (input.compare("get_book") == 0) {
            if (access_token.empty()) {
                 cout << "You should use the command 'enter_library' first.\n";
                continue;
            }
            int id;
            cout << "id=";
            cin >> id;
            sockfd = open_connection((char *)HOST, 8080, AF_INET, SOCK_STREAM, 0);
            get_book(sockfd, access_token, id);
            close_connection(sockfd);
            continue;
        }
        if (input.compare("add_books") == 0) {
            if (access_token.empty()) {
                 cout << "You should use the command 'enter_library' first.\n";
                continue;
            }
            string title, author, genre, publisher, page_count_str;
            int page_count;
            getline(cin, title, '\n'); // Clear buffer
            cout << "Title = "; 
            getline(cin, title, '\n');
            cout << "Author = "; 
            getline(cin, author, '\n');
            cout << "Genre = "; 
            getline(cin, genre, '\n');
            cout << "Publisher = "; 
            getline(cin, publisher, '\n');
            cout << "Page count = "; 
            cin >> page_count_str;
            page_count = atoi(page_count_str.c_str());
            if (page_count <= 0) {
                cout << "Number of Pages should be a positive number\n";
                continue;
            }
           
            sockfd = open_connection((char *)HOST, 8080, AF_INET, SOCK_STREAM, 0);
            add_book(access_token, title, author, genre, publisher, page_count, sockfd);
            close_connection(sockfd);
            continue;
        }
        if (input.compare("delete_book") == 0) {
            if (access_token.empty()) {
                 cout << "You should use the command 'enter_library' first.\n";
                continue;
            }
            int id;
            cout << "id=";
            cin >> id;
            sockfd = open_connection((char *)HOST, 8080, AF_INET, SOCK_STREAM, 0);
            delete_book(sockfd, access_token, id);
            close_connection(sockfd);
            continue;
        }
        if (input.compare("logout") == 0) {
            if (active_cookie.empty()) {
                cout << "You are not logged in.\n";
                continue;
            } 
            sockfd = open_connection((char *)HOST, 8080, AF_INET, SOCK_STREAM, 0);
            logout(sockfd, active_cookie);
            active_cookie = "";
            access_token = "";
            close_connection(sockfd);
            continue;
        }
        cout << "Command doesn't exist.\n";
    }
    cout << "Connection with client closed\n";

    return 0;
}