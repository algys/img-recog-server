
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>
#include <chrono>

#include "core.h"
#include "catalog.h"
#include "recog.h"

using namespace std;

void dostuff(int);
void error(const char *msg)
{
    perror(msg);
//    exit(1);
}

int count_of_threads;
mutex count_of_threads_mutex;

Catalog cat;
mutex cat_captures_mutex;
mutex cat_data_mutex;

int main()
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 8081;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);



    while (1) {
        newsockfd = accept(sockfd,
                           (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        while(1){
            count_of_threads_mutex.lock();
            if(count_of_threads < 5){
                count_of_threads_mutex.unlock();
                break;
            }
            count_of_threads_mutex.unlock();
        }

        thread thr(dostuff,newsockfd);

        thr.detach();

    }
    close(sockfd);
    return 0;
}


void dostuff (int sock)
{
    int n;

    Mat img(480,640,CV_8U);
    this_thread::sleep_for(chrono::milliseconds(20));

    n = recv(sock,reinterpret_cast<void*>(img.data),480*640,MSG_WAITALL);
    printf("%d\n", n);
    if (n != 480*640) error("ERROR reading from socket");

    Capture curr(img);
    while(!cat_captures_mutex.try_lock());
    DictCaptures captures = cat.get_captures();
    Recog recog(captures, curr);
    int id = recog.tryrecog();
    cat_captures_mutex.unlock();
    if(id==-1){
        n = write(sock,"0",1);
        if (n < 0) error("ERROR writing to socket");
    }
    else {
        while(!cat_data_mutex.try_lock());
        DictData data = cat.get_data();
        Data info = data[id];
        cat_data_mutex.unlock();
        n = write(sock,info.author.c_str(),256);
        if (n < 0) error("ERROR writing to socket");

        n = write(sock,info.info.c_str(),256);
        if (n < 0) error("ERROR writing to socket");

        n = write(sock,info.name.c_str(),256);
        if (n < 0) error("ERROR writing to socket");

        n = write(sock,info.path.c_str(),256);
        if (n < 0) error("ERROR writing to socket");

        n = write(sock,info.year.c_str(),256);
        if (n < 0) error("ERROR writing to socket");
    }
    close(sock);
    count_of_threads_mutex.lock();
    count_of_threads--;
    count_of_threads_mutex.unlock();
}
