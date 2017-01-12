#include "catalog.h"

Catalog::Catalog()
{
    Mat testImg1 = imread("/home/algys/tp/c_cpp/project/testImg/1.jpeg", 0);
    Mat testImg2 = imread("/home/algys/tp/c_cpp/project/testImg/2.jpeg",0);
    Mat testImg3 = imread("/home/algys/tp/c_cpp/project/testImg/3.png",0);

    Capture testCapture1(testImg1);
    Capture testCapture2(testImg2);
    Capture testCapture3(testImg3);

    Data testData1("AlpenGold", "Kraft Foods", "1994 год", "Альпен Гольд (англ. Alpen Gold) — бренд кондитерских изделий, ранее принадлежавший компании Kraft Foods, а теперь Mondelēz International. Под данной торговой маркой представлены такие кондитерские изделия, как шоколад, мороженое, печенье, конфеты. Продукция компании представлена только в некоторых странах СНГ (России, Украине, Казахстане и Белоруссии) и в Польше. Название торговой марки переводится как — Альпийское золото, однако продукция не имеет никакого отношения к Альпам. Все заводы расположены лишь на территории стран восточной Европы.","/home/algys/tp/c_cpp/project/testImg/1.jpeg");
    Data testData2("Monolisa", "Леонардо да Винчи", "1503—1519", "«Мо́на Ли́за», она же «Джоко́нда»; (итал. Mona Lisa, La Gioconda, фр. La Joconde), полное название — Портре́т госпожи́ Ли́зы дель Джоко́ндо, итал. Ritratto di Monna Lisa del Giocondo) — картина Леонардо да Винчи, находящаяся в Лувре (Париж, Франция), одно из самых известных произведений живописи в мире[1][2], которое, как считается, является портретом Лизы Герардини, супруги торговца шёлком из Флоренции Франческо дель Джокондо","/home/algys/tp/c_cpp/project/testImg/2.jpeg");
    Data testData3("Test","test","test","test","/home/algys/tp/c_cpp/project/testImg/3.png");

    captures = new std::map< int, Capture >;
    data = new std::map< int, Data>;

    data->operator [](0) = testData1;
    data->operator [](1) = testData2;
    data->operator [](2) = testData3;

    captures->operator [](0) = testCapture1;
    captures->operator [](1) = testCapture2;
    captures->operator [](2) = testCapture3;
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
