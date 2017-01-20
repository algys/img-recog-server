#include "catalog.h"
#include <fstream>

Data read(std::ifstream &in){

    char buf[1024];

    in.getline(buf, 256);
    std::string author(buf,strlen(buf));
    in.getline(buf, 256);
    std::string name(buf,strlen(buf));
    in.getline(buf, 256);
    std::string year(buf,strlen(buf));
    in.getline(buf, 1024);
    std::string info(buf,strlen(buf));
    in.getline(buf, 256);
    std::string path(buf,strlen(buf));
    in.getline(buf, 256);
    std::string url(buf,strlen(buf));

    return Data(author, name, year, info, path, url);
}

Catalog::Catalog()
{
    std::ifstream in("/home/algys/tp/c_cpp/server/project-serv/cat.txt");

    assert(in.is_open());

    Data testData1 = read(in);
    Data testData2 = read(in);
    Data testData3 = read(in);
    Data testData4 = read(in);
    Data testData5 = read(in);
    Data testData6 = read(in);
    Data testData7 = read(in);
    Data testData8 = read(in);

    Mat testImg1 = imread(testData1.path, 0);
    imwrite("/home/algys/test1.jpg", testImg1);
    Mat testImg2 = imread(testData2.path, 0);
    imwrite("/home/algys/test2.jpg", testImg2);
    Mat testImg3 = imread(testData3.path, 0);
    imwrite("/home/algys/test3.jpg", testImg3);
    Mat testImg4 = imread(testData4.path, 0);
    imwrite("/home/algys/test4.jpg", testImg4);
    Mat testImg5 = imread(testData5.path, 0);
    imwrite("/home/algys/test5.jpg", testImg5);
    Mat testImg6 = imread(testData6.path, 0);
    imwrite("/home/algys/test6.jpg", testImg6);
    Mat testImg7 = imread(testData7.path, 0);
    imwrite("/home/algys/test7.jpg", testImg7);
    Mat testImg8 = imread(testData8.path, 0);
    imwrite("/home/algys/test8.jpg", testImg8);

    Capture testCapture1(testImg1);
    Capture testCapture2(testImg2);
    Capture testCapture3(testImg3);
    Capture testCapture4(testImg4);
    Capture testCapture5(testImg5);
    Capture testCapture6(testImg6);
    Capture testCapture7(testImg7);
    Capture testCapture8(testImg8);

    captures = new std::map< int, Capture >;
    data = new std::map< int, Data>;

    data->operator [](0) = testData1;
    data->operator [](1) = testData2;
    data->operator [](2) = testData3;
    data->operator [](3) = testData4;
    data->operator [](4) = testData5;
    data->operator [](5) = testData6;
    data->operator [](6) = testData7;
    data->operator [](7) = testData8;

    captures->operator [](0) = testCapture1;
    captures->operator [](1) = testCapture2;
    captures->operator [](2) = testCapture3;
    captures->operator [](3) = testCapture4;
    captures->operator [](4) = testCapture5;
    captures->operator [](5) = testCapture6;
    captures->operator [](6) = testCapture7;
    captures->operator [](7) = testCapture8;
}

bool Catalog::isOpened(){
    return true;
}

const DictCaptures &Catalog::get_captures(){
    return *captures;
}

const DictData &Catalog::get_data(){
    return *data;
}
