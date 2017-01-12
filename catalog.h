#ifndef CATALOG_H
#define CATALOG_H


#include "core.h"

using namespace cv;

class Catalog{
    private:
        DictCaptures *captures;
        DictData *data;
        //some code
    public:
        Catalog();
        bool isOpened();
        const DictCaptures &get_captures(); //возвращает словарь значений
        const DictData &get_data(); //возвращает словарь информаций
        const std::vector<int> &find_by_author(const std::string &str); //возвращает вектор идентификаторов
        const std::vector<int> &find_by_year(const std::string &str); //возвращает вектор идентификаторов
  //      ~Catalog();
};


#endif // CATALOG_H
