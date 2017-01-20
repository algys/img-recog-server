#include <vector>
#include <map>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>


#ifndef CORE_H_
#define CORE_H_

using namespace cv;

class Capture{
    private:
        Mat img;
        Mat descriptors;
        std::vector<KeyPoint> keypoints;
        //some code
    public:
        Capture(Mat &_img);
        Capture(Mat &_img, Mat &_descriptors, std::vector<KeyPoint> &_keypoints):
            img(_img),descriptors(_descriptors),keypoints(_keypoints) {};
        Capture(){};
        const Mat &get_descriptors() { return descriptors; };
        const std::vector<KeyPoint> &get_keypoints() { return keypoints; };
        const Mat &get_img() { return img; };
};

class Data{
    public:
        std::string name;
        std::string author;
        std::string year;
        std::string info;
        std::string path;
        std::string url;
        //some code
    public:
        Data(const std::string &_name = "", const std::string &_author = "", const std::string &_year = "", const std::string &_info = "", const std::string &_path = "", const std::string &_url = ""):
            name(_name), author(_author), year(_year), info(_info), path(_path), url(_url) {};
};

class Object{
    private:
        std::vector<Point2f> points;
        Mat background;
    public:
        Object(std::vector<Point2f> &_points, Mat &_background):
            points(_points),background(_background) {};
        Object(std::vector<Point2f> &_points):
            points(_points) {};
        Object(Mat &_background):
            background(_background) {};
        Object(){};

        void set_points(const std::vector<Point2f> &_points);
        void set_background(const Mat &_background);
        const std::vector<Point2f> &get_points(){ return points; };
        const Mat &get_background(){ return background; };
        void draw(Mat &src, int mode = 0); //mode: 0 - draw points and background, 1 - draw points, 2 - draw background

};

typedef std::map<int, Capture> DictCaptures;
typedef std::map<int, Data> DictData;


#endif // CORE_H
