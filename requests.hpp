#ifndef _REQUESTS_
#define _REQUESTS_

char *compute_delete_request(const char *host, const char *url, const char *query_params, 
                        bool auth, const char *access_token,
                        char **cookies, int cookies_count);


// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(const char *host, const char *url, const char *query_params,
                            bool auth, const char *access_token,
							char **cookies, int cookies_count);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *compute_post_request(const char *host, const char *url, 
                        const char* content_type, bool auth,
                        const char *access_token, const char *content, char **cookies,
                        int cookies_count);

#endif
