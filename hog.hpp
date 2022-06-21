#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;
class App
{
public:
    App(CommandLineParser &cmd);
    void run();
    void handleKey(char key);
    void hogWorkBegin();
    void hogWorkEnd();
    string hogWorkFps() const;
    void workBegin();
    void workEnd();
    string workFps() const;

private:
    App operator=(App &);
    // Args args;
    bool running;
    bool make_gray;
    double scale;
    double resize_scale;
    int win_width;
    int win_stride_width, win_stride_height;
    int gr_threshold;
    int nlevels;
    double hit_threshold;
    bool gamma_corr;
    int64 hog_work_begin;
    double hog_work_fps;
    int64 work_begin;
    double work_fps;
    string img_source;
    string vdo_source;
    string output;
    int camera_id;
    bool write_once;
};

int hog(int argc, char **argv);