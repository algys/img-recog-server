#include "core.h"

#ifndef RECOG_H
#define RECOG_H

using namespace cv;
using namespace std;

class Recog{
    private:
        DictCaptures captures;
        Capture frame;
        Object *object;
        int recognized_id;
        //some code
    public:
        Recog(const DictCaptures &_captures, const Capture &_frame):captures(_captures), frame(_frame) { recognized_id = -1; object = NULL; };
        int tryrecog(); //return ID of recognized capture or -1 if cant recognize
        const Object &get_object();
        int get_id(){ return recognized_id; }
        ~Recog();
        //some code
};

#endif // RECOG_H
