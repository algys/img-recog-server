#include "recog.h"

using namespace std;
using namespace cv;

double dist(const Point2f &p1, const Point2f &p2){
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

bool isValid(const vector<Point2f> &corners){
    double eps = 0.3;
    double p1, p2;

    p1 = fabs(1 - dist(corners[0],corners[1])/dist(corners[2],corners[3]));
    p2 = fabs(1 - dist(corners[1],corners[2])/dist(corners[3],corners[0]));

    if(p1<eps && p2<eps && dist(corners[0], corners[1])>50)
        return true;
    return false;
}

int Recog::tryrecog(){
    int minHessian = 400;
    Mat img_scene = frame.get_img();

    vector<KeyPoint> keypoints_scene = frame.get_keypoints();

    Mat descriptors_scene;
    descriptors_scene = frame.get_descriptors();

    int max_matches = 1000000;
    int max_id = -1;
    double max_ratio = -1;
    double max_distance = 0.2;
    DictCaptures::iterator ii;
    vector <vector<DMatch> > f_matches;
    vector <Mat> tr_descriptors;
    for(ii = captures.begin(); ii != captures.end(); ii++){
        vector<KeyPoint> keypoints_temple;
        keypoints_temple = (ii->second).get_keypoints();
        Mat descriptors_temple = (ii->second).get_descriptors();
        tr_descriptors.push_back(descriptors_temple);
    }

    FlannBasedMatcher fmatcher;
//	cout<<tr_descriptors.size()<<endl;
    fmatcher.add(tr_descriptors);
    fmatcher.knnMatch(descriptors_scene, f_matches, 1);

    max_id = -1;
    if(f_matches.size()>0)
        max_id = f_matches[0][0].imgIdx;

    if(max_id!=-1){
        FlannBasedMatcher matcher;
        Mat descriptors_temple = captures[max_id].get_descriptors();
        vector <DMatch> matches;
        vector <DMatch> good_matches;

        matcher.match(descriptors_temple, descriptors_scene, matches);

        double max_dist = 0; double min_dist = 100;
        for( int i = 0; i < matches.size(); i++ ){
            double dist = matches[i].distance;
            if( dist < min_dist ) min_dist = dist;
            if( dist > max_dist ) max_dist = dist;
        }

        for( int i = 0; i < matches.size(); i++ ){
            if( matches[i].distance < 3 * min_dist ){
                good_matches.push_back( matches[i]);
            }
        }

        vector<Point2f> temple;
        vector<Point2f> scene;

        vector<KeyPoint> keypoints_temple;
        keypoints_temple = captures[max_id].get_keypoints();
        Mat img_temple = captures[max_id].get_img();
//		cout<<keypoints_temple.size()<<endl;
//		cout<<good_matches.size()<<endl;
//		cout<<keypoints_temple[good_matches[0].queryIdx].pt<<endl;

        for(int i = 0; i < good_matches.size(); i++ ){
            temple.push_back( keypoints_temple[ good_matches[i].queryIdx ].pt );
            scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
        }

//		cout<<keypoints_temple.size()<<endl;
//		cout<<good_matches.size()<<endl;
//		cout<<(double)good_matches.size()/(double)keypoints_temple.size()<<endl;

        if(good_matches.size()>=100){

            Mat H = findHomography( temple, scene, CV_RANSAC );
            std::vector<Point2f> temple_corners(4);
            temple_corners[0] = cvPoint(0,0);
            temple_corners[1] = cvPoint( img_temple.cols, 0 );
            temple_corners[2] = cvPoint( img_temple.cols, img_temple.rows );
            temple_corners[3] = cvPoint( 0, img_temple.rows );
            std::vector<Point2f>   scene_corners(4);

            perspectiveTransform( temple_corners, scene_corners, H);

            if(isValid(scene_corners)){
                recognized_id = max_id;
                object = new Object(scene_corners);
            }
        }

    }
    cout<<recognized_id<<endl;
    return recognized_id;
}

const Object &Recog::get_object(){
    return *object;
}

Recog::~Recog(){
    if(object != NULL)
        delete object;
}

