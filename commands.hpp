#ifndef _COMMANDS_
#define _COMMANDS_
#include "helpers.hpp"
#include "requests.hpp"
#include "buffer.hpp"
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
#define CONN_HOST "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com"
#define MAX_USER_DATA 16

string get_Cookies(char *response);

void register_user(string username, string password, int sockfd);

string login_user(string username, string password, int sockfd);

string enter_library(string sessionCookie, int sockfd);

void add_book(string access_token, string title, string author, string genre, string publisher, int page_count, int sockfd);

void get_all_books(int sockfd, string access_token);

void get_book(int sockfd, string access_token, int id);

void delete_book(int sockfd, string accessToken, int id);

void logout(int sockfd, string cookie);
#endif
