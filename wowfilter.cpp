#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/core/utils/logger.hpp"

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

using namespace cv;
using namespace std;

Mat woWfilter(Mat &img, int bl, int opt)
{
    // return img;

    Mat blured;

    // medianBlur(img, blured, bl);

    blur(img, blured, Size(bl, bl));

    switch (opt)
    {
    case 1:
        blured = 11 + blured - (11 * blured / blured + blured & blured);
    case 2:
        blured = 11 + blured - (11 * blured / blured + blured ^ blured);
    default:
        blured = 11 + blured - (11 * blured / blured + blured & blured + blured ^ blured);
    }

    return blured;
}

Mat woWfilter2(Mat &img, int bl, int opt)
{
    // return img;

    Mat caned, blured;

    // medianBlur(img, blured, bl);

    // blured = 11 + blured - (11 * blured / blured + blured & blured);
    cvtColor(img, blured, COLOR_BGRA2GRAY);
    switch (opt)
    {
    case 0:
        blured = (blured & blured);
        break;
    case 1:
        blur(blured, blured, Size(bl, bl));
        blured = blured & (3.141592654 * blured);
        break;
    default:
        blur(blured, blured, Size(bl, bl));
        blured = (11 * blured / blured + blured & blured);
    }
    Canny(blured, caned, 37, 39);
    cvtColor(caned, caned, COLOR_GRAY2RGB);
    return caned;
}

//событие трекбара, перемещение по фреймам
static void on_trackbar(int, void *)
{
}

const char *keys =
    {
        "{help h usage ?    |   | print this message}"
        "{@video            |   | Video file, if not defined try to use webcamera}"
        "{b blur            |17 | blur for input}"
        "{from              |0  | start from frame # 0=from start}"
        "{to                |0  | stop when frame # 0=to end}"
        "{o option          |3  | option=1,2,3 for input}"
        "{w wow             |1  | wowfilter=1,2,3 type filter}"
        "{look              |2  | show video = 2 - both raw and filtered, 0 - not showing, 1 just filtered}"
        "{s save            |0  | save: 0-not save, 1-save as DIVX, 2-MJPG, 3-MPEG, another=codecTag (like -s=1145656920 is XVID, see fourcc tag, but use digital, not hex!)}"};

int slider = 0;
int slider_max = 0;
int wowfilter(int argc, char *argv[])
{
    std::srand((uint)std::time(nullptr));
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    CommandLineParser parser(argc, argv, keys);
    parser.about("Convert to Tea v1.0.0");
    // If requires help show
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    String videoFile = parser.get<String>(0);

    int bluro, optiono, looko, fromo, too, wowo, saveo;

    bluro = parser.get<int>("b");
    cout << "Blur using: " << bluro << "\n";

    optiono = parser.get<int>("o");
    cout << "Option using: " << optiono << "\n";

    looko = parser.get<int>("look");
    cout << "Look using: " << looko << "\n";

    fromo = parser.get<int>("from");
    cout << "from using: " << fromo << "\n";

    too = parser.get<int>("to");
    cout << "to using: " << too << "\n";

    wowo = parser.get<int>("w");
    cout << "wow using: " << wowo << "\n";

    saveo = parser.get<int>("s");
    cout << "Save option: " << saveo << "\n\t0-not save, 1-save as DIVX, 2-MJPG, 3-MPEG, another=codecTag"
         << "\n";

    // Check if params are correctly parsed in his variables
    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }

    VideoCapture cap;
    if (videoFile != "")
    {
        cap.open(videoFile);
        slider_max = (int)cap.get(CAP_PROP_FRAME_COUNT);
        if (too == 0)
        {
            too = slider_max;
        }
        if (too > slider_max)
        {
            cout << "<to> parameter more than max possible frame\n Set it to 0 or not use for end of file";
            return -1;
        }
        if (looko)
        {
            namedWindow("Output", 1); //нам нужно именованое окно, чтобы добавить трекбар

            createTrackbar("Frame #", "Output", &slider, slider_max, on_trackbar);
            // on_trackbar( slider, 0 );
        }
    }
    else
    { //нет файла - открываем камеру
        cap.open(0);
        fromo = 0; //у камеры нет начала
        too = 0;   //и конца
    }
    if (!cap.isOpened()) // check if we succeeded
        return -1;

    Size S = Size((int)cap.get(CAP_PROP_FRAME_WIDTH), // Acquire input size
                  (int)cap.get(CAP_PROP_FRAME_HEIGHT));
    VideoWriter outputVideo;

    double fps;
    fps = cap.get(CAP_PROP_FPS);
    cout << "\n"
         << "fps: " << fps << "\n";
    if (fps < 1)
    {
        fps = 16;
        cout << "fps<1, so set fps to 16"
             << "\n";
    }
    if (saveo)
    {
        String filename;

        filename = "b" + to_string(bluro) + "o" + to_string(optiono) + "f" + to_string(fromo) + "t" + to_string(too) + "s" + to_string(saveo) + "_" + to_string(std::hash<double>{}(std::rand())) + ".avi";
        switch (saveo)
        {
        case 1:

            // int codec = 0x58564944; // select desired codec (must be available at runtime)
            //  outputVideo.open(filename, codec, fps, S, true);
            outputVideo.open(filename, VideoWriter::fourcc('D', 'I', 'V', 'X'), fps, S, true);
            break;
        case 2:

            // int ex = static_cast<int>(cap.get(CAP_PROP_FOURCC)); // Get Codec Type- Int form
            outputVideo.open(filename, VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, S, true);
            break;
        case 3:
            outputVideo.open(filename, VideoWriter::fourcc('M', 'P', 'E', 'G'), fps, S, true);
            break;
        default:
            outputVideo.open(filename, saveo, fps, S, true);
        }

        if (!outputVideo.isOpened())
        {
            cout << "Could not open the output video for write: "
                 << filename << endl;
            return -1;
        }
        else
        {
            cout << "\nStarting save to " << filename << "\n";
        }
    }

    int num = 0;
    if (fromo)
    {
        if (fromo >= slider_max)
        {
            cout << "<from> parameter more than max possible frame\n";
            return -1;
        }
        cap.set(CAP_PROP_POS_FRAMES, fromo);
        num = fromo;
        slider = num;
    }
    cout << "Frame: " << num;
    for (;;)
    {
        if (num != slider && looko)
        { //наверное крутили слайдер
            cap.set(CAP_PROP_POS_FRAMES, slider);
            num = slider;
        }

        cout << "\r"
             << "Frame: " << ++num;

        if (looko)
        {
            slider = num;
            setTrackbarPos("Frame #", "Output", slider);
        }

        Mat frame, out;
        cap >> frame; // get a new frame from camera
        // imshow("Video", frame);
        switch (wowo)
        {
        case 2:
            out = woWfilter2(frame, bluro, optiono);
            break;

        default:
            out = woWfilter(frame, bluro, optiono);
            break;
        }

        if (saveo)
        {
            outputVideo << out;
        }

        if (looko)
        {
            imshow("Output", out);
            if (looko == 2)
                imshow("Movie", frame);

            if (waitKey(1) == 27) // ESC - break
                break;
        }
        if (too)
        {
            if (num >= too)
            {
                cout << "\nThat all folks!\n";
                break;
            }
        }
    }
    // Release the camera or video cap
    cap.release();
    outputVideo.release();
    return 0;
}