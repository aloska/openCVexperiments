// FRAME DIFFERENCING

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

Mat frameDiff(Mat prevFrame, Mat curFrame, Mat nextFrame)
{
    Mat diffFrames1, diffFrames2, diffFrames3, output;

    // Compute absolute difference between current frame and the next frame
    absdiff(nextFrame, curFrame, diffFrames1);

    // Compute absolute difference between current frame and the previous frame
    absdiff(curFrame, prevFrame, diffFrames2);

    // Bitwise "AND" operation between the above two diff images
    bitwise_and(diffFrames1, diffFrames2, output);

    return output;
}

Mat getFrame(VideoCapture cap, float scalingFactor)
{
    // float scalingFactor = 0.5;
    Mat frame, output;

    // Capture the current frame
    cap >> frame;

    // Resize the frame
    resize(frame, frame, Size(), scalingFactor, scalingFactor, INTER_AREA);

    // Convert to grayscale
    cvtColor(frame, output, COLOR_BGRA2GRAY);

    return output;
}

int frameDifferencing(int argc, char *argv[])
{
    Mat frame, prevFrame, curFrame, nextFrame;
    char ch;

    // Create the capture object
    // 0 -> input arg that specifies it should take the input from the webcam
    VideoCapture cap(0);

    // If you cannot open the webcam, stop the execution!
    if (!cap.isOpened())
        return -1;

    // Scaling factor to resize the input frames from the webcam
    float scalingFactor = 0.25;

    prevFrame = getFrame(cap, scalingFactor);
    curFrame = getFrame(cap, scalingFactor);
    nextFrame = getFrame(cap, scalingFactor);

    // Iterate until the user presses the Esc key
    while (true)
    {
        // Show the object movement
        Mat dif;
        dif = frameDiff(prevFrame, curFrame, nextFrame);
        imshow("Object Movement", dif);

        // Update the variables and grab the next frame
        prevFrame = curFrame;
        curFrame = nextFrame;
        nextFrame = getFrame(cap, scalingFactor);
        Mat contours;
        Canny(curFrame, // gray-level image
              contours, // output contours
              37,       // low threshold
              39);      // high threshold
        imshow("Canny", contours);
        // bitwise_or(contours, dif, dif);
        //   GaussianBlur(dif, dif, Size(11, 11), 3.5, 3.5);
        // imshow("sum", dif);
        //   Get the keyboard input and check if it's 'Esc'
        //   27 -> ASCII value of 'Esc' key
        ch = waitKey(10);
        if (ch == 27)
        {
            break;
        }
    }

    // Release the video capture object
    cap.release();

    // Close all windows
    destroyAllWindows();

    return 1;
}