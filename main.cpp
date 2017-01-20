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

void worker(int);
void error(const char *msg)
{
    perror(msg);
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
    if (sockfd < 0){
        error("ERROR opening socket");
        return 0;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 8081;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("ERROR on binding");
        return 0;
    }

    listen(sockfd, 5);

    clilen = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
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

        thread thr(worker, newsockfd);
        thr.detach();
    }

    close(sockfd);
    return 0;
}


void worker (int sock)
{
    int n;

    Mat img(640,480,CV_8U);

    n = recv(sock, reinterpret_cast<void*>(img.data), 480*640, MSG_WAITALL);
    if(n != 480*640){
        error("ERROR reading from socket");
        return;
    }
    Mat img2;

    blur(img, img2, Size(2,2));

    Capture curr(img2);
  //  imshow("test",img);
  //  imwrite( "/home/algys/test.jpg", img2 );
    while(!cat_captures_mutex.try_lock());

    DictCaptures captures = cat.get_captures();
    Recog recog(captures, curr);
    int id = recog.tryrecog();

    cat_captures_mutex.unlock();

    if(id<0){
        n = write(sock,"0",1);
        if (n < 0)
            error("ERROR writing to socket");
    }
    else {
        while(!cat_data_mutex.try_lock());

        DictData data = cat.get_data();
        Data info = data[id];

        cat_data_mutex.unlock();

        char buf[1024];
        bzero(buf, 1024);
        buf[0] = info.author.length()>>24;
        buf[1] = info.author.length()>>16;
        buf[2] = info.author.length()>>8;
        buf[3] = info.author.length();
        strcpy(buf+4, info.author.c_str());
        n = write(sock, buf, 256);
        if (n < 0) error("ERROR writing to socket");

        bzero(buf, 1024);
        buf[0] = info.info.length()>>24;
        buf[1] = info.info.length()>>16;
        buf[2] = info.info.length()>>8;
        buf[3] = info.info.length();
        strcpy(buf+4, info.info.c_str());
        n = write(sock, buf, 1024);
        if (n < 0) error("ERROR writing to socket");

        bzero(buf, 1024);
        buf[0] = info.name.length()>>24;
        buf[1] = info.name.length()>>16;
        buf[2] = info.name.length()>>8;
        buf[3] = info.name.length();
        strcpy(buf+4, info.name.c_str());
        n = write(sock, buf, 256);
        if (n < 0) error("ERROR writing to socket");

        bzero(buf, 1024);
        buf[0] = info.url.length()>>24;
        buf[1] = info.url.length()>>16;
        buf[2] = info.url.length()>>8;
        buf[3] = info.url.length();
        strcpy(buf+4, info.url.c_str());
        n = write(sock, buf, 256);
        if (n < 0) error("ERROR writing to socket");

        bzero(buf, 1024);
        buf[0] = info.year.length()>>24;
        buf[1] = info.year.length()>>16;
        buf[2] = info.year.length()>>8;
        buf[3] = info.year.length();
        strcpy(buf+4, info.year.c_str());
        n = write(sock, buf, 256);
        if (n < 0) error("ERROR writing to socket");
    }
    close(sock);
    count_of_threads_mutex.lock();
    count_of_threads--;
    count_of_threads_mutex.unlock();

    return;
}
