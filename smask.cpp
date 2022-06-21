
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include "aloha.hpp"

using namespace cv;
using namespace std;

double dist(Point p1, Point p2)
{
    double r;
    r = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    if (r > 255)
    {
        return 0;
    }
    else
    {
        return (255 - r) / 255.0;
    }
}

uchar distC(Point p1, Point p2)
{
    double r;
    r = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    if (r > 255)
    {
        return 0;
    }
    else
    {
        return 255 - floor(r);
    }
}

Mat woWfilter_(Mat img)
{

    Mat blured, mask;

    blur(img, blured, Size(27, 27));
    // mask = Mat::zeros(img.size(), img.type());

    // blured = 10 + blured - (10 * blured / blured + blured & blured);

    // return blured;

    // return 11 + blured - (11 * blured / blured + blured & blured);
    // return 11 + blured - (11 * blured / blured + blured ^ blured);
    return 11 + blured - (11 * blured / blured + blured & blured + blured ^ blured);
}

Mat makeAtt(Mat img, Point p)
{
    Mat blured, mask;
    // blured = img.clone();
    blur(img, blured, Size(17, 17));
    mask = Mat::zeros(img.size(), img.type());

    for (int i = 0; i < mask.rows; i++) //бежим по y
    {
        for (int j = 0; j < mask.cols; j++) //по x
        {
            uchar d;
            Vec3b v;
            // v = blured.at<Vec3b>(i, j);
            d = distC(Point(j, i), p);
            // d = 0;
            mask.at<Vec3b>(i, j) = Vec3b(d, d, d);
        }
    }

    // blur(mask, mask, Size(17, 17));
    blured = 255 * mask / img;
    /*
        for (int i = 0; i < blured.rows; i++) //бежим по y
        {
            for (int j = 0; j < blured.cols; j++) //по x
            {
                Vec3b v;
                double d;
                v = blured.at<Vec3b>(i, j);
                d = dist(Point(j, i), p);
                blured.at<Vec3b>(i, j) = Vec3b((v.val[0] * d), (v.val[1] * d), (v.val[2] * d));
                // blured.at<Vec3b>(i, j) = Vec3b(d, d, d);
            }
        }
        */
    return blured;
}

Mat muskStat(Mat img)
{
    Mat ret;
    ret = Mat::zeros(img.size(), img.type());
    for (int i = 0; i < img.cols; i++) //бежим по y
    {
        for (int j = 0; j < img.rows; j++) //по x
        {
            Vec3b v;
            uchar d;
            d = distC(Point(i, j), Point(img.cols / 2, img.rows / 2));
            ret.at<Vec3b>(j, i) = Vec3b(255 - d, 255 - d, 255 - d);
        }
    }
    blur(ret, ret, Size(27, 27));
    return ret;
}

Mat atentionMat(Mat img, Point p)
{
    Mat dst, biga;

    biga = Mat::zeros(img.size() * 2, img.type());
    img.copyTo(biga(cv::Rect(img.cols - p.x, img.rows - p.y, img.cols, img.rows)));
    dst = biga(Rect(img.cols / 2, img.rows / 2, img.cols, img.rows));

    return dst;
}

Point mousePos;
void mouse_callback(int event, int x, int y, int flag, void *param)
{
    if (event == EVENT_MOUSEMOVE)
    {
        // cout << "(" << x << ", " << y << ")" << endl;
        mousePos.x = x;
        mousePos.y = y;
    }
}

int smask()
{
    Mat frame, resframe;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;        // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
         << "Press any key to terminate" << endl;

    namedWindow("Live");
    setMouseCallback("Live", mouse_callback);

    cap.read(frame);
    Mat muskst;
    muskst = muskStat(frame);

    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty())
        {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        // show live and wait for a key with timeout long enough to show images
        imshow("Live", frame);
        // imshow("Musk", makeAtt(frame, mousePos));
        //  imshow("Musk", woWfilter(frame));
        Mat att;
        att = atentionMat(frame, mousePos);
        att = att - muskst + att;
        imshow("Musk", att);

        if (waitKey(5) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

/*
//прикольный эффекто протомалекула

Mat blured, mask;
    // blured = img.clone();
    blur(img, blured, Size(17, 17));
    mask = Mat::zeros(img.size(), img.type());
    for (int i = 0; i < mask.rows; i++) //бежим по y
    {
        for (int j = 0; j < mask.cols; j++) //по x
        {
            uchar d;
            Vec3b v;
            v = blured.at<Vec3b>(i, j);
            d = distC(Point(j, i), p);
            mask.at<Vec3b>(i, j) = Vec3b(d | v.val[0], d | v.val[1], d | v.val[2]);
        }
    }
    //blur(mask, mask, Size(17, 17));
    blured = 10 * mask / img;
    //blured = 10 * mask / img - blured / mask * 10;

    //blured = 10 * mask / img + blured & mask; //трижды вау
*/

/*огонь ваще
        mask.at<Vec3b>(i, j) = Vec3b(d & v.val[0], d & v.val[1], d & v.val[2]);
        }
    }
    // blur(mask, mask, Size(17, 17));
    blured = 255 * mask / img + blured;
*/