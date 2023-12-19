// http.c

#include "http.h"
#include "stdlib.h"
#include "stdio.h"

#define TEMP 1024

char* read_file(char* html_path) {
    FILE* fp = fopen(html_path, "r");
    if (fp == NULL) {
        printf("Error: cannot open file %s\n", html_path);
        return read_file("htdocs/404.html");
    }
    static char* file_content = NULL;
    if (file_content == NULL) {
        file_content = malloc(TEMP);
    }
    memset(file_content, 0, TEMP);
    char* buffer_content = malloc(TEMP);
    while(fgets(buffer_content, TEMP, fp) != NULL) {
        strcat(file_content, buffer_content);
    }
    free(buffer_content);
    fclose(fp);
    printf("%d\n", strlen(file_content));
    printf("file_content: %s\n", file_content);
    return file_content;
}

// 请完成这个函数
// 这个函数的参数是一个字符串，表示原始的请求，你可以用printf打印看看是啥
// 这个函数的返回值是response，表示输出的响应，你需要按照格式来自行构造
// 理论上，你写完这个函数，编译运行，能通过访问 http://127.0.0.1 来交互
// 你可以尝试着让你写的函数去访问htdocs里面的网页，方法是通过读取文件
// 如果你不会写，可以试试最暴力地方法，先自己构造一个静态的response在浏览器里看看效果
// 如果你实在不会，可以在answer文件夹里参考答案，然后自己独立实现出来
char *http_process(char *request) {
    static char response[1024]; // 如果1024不够用你还可以自己添加
    // *response = '\0';
    if (strncmp(request, "GET ", 4) != 0) {
        snprintf(response, sizeof(response), "HTTP/1.1 400 Bad Request\r\n\r\n");
        printf("400 because this isn't a GEt request\n");
        return response;
    }
    char* path_start = request + 4;
    char* path_end = strchr(path_start, ' ');
    if (path_end == NULL) {
        printf("400 because path_end is NULL\n");
        snprintf(response, sizeof(response), "HTTP/1.1 400 Bad Request\r\n\r\n");
        return response;
    }

    size_t len = path_end - path_start;
    char path[len + 1];
    strncpy(path, path_start, len);
    path[len] = '\0';
    if (strcmp(path, "/") == 0 || strcmp(path, "/index") == 0 || strcmp(path, "/index.html") == 0) {
        char* index_html = read_file("htdocs/index.html");
        snprintf(response, sizeof(response), 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type:text/html\r\n"
        "\r\n"
        "%s\r\n", index_html);
        // printf("200 OK but Browser shows 400\n");
        return response;
    } else {
        char* not_found_html = read_file("htdocs/404.html");
        snprintf(response, sizeof(response), 
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "%s\r\n", not_found_html);
        return response;
    }
}
