#include "core.h"

Capture::Capture(Mat &_img){
    int minHessian = 400;

    img = _img;

    SurfFeatureDetector detector( minHessian );
    detector.detect( img, keypoints );

    SurfDescriptorExtractor extractor;
    extractor.compute(img, keypoints, descriptors);
}

void Object::set_points(const vector<Point2f> &_points){
    points = _points;
}

void Object::set_background(const Mat &_background){
    background = _background;
}

void Object::draw(Mat &src, int mode){

}
