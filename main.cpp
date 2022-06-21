#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

#include "vidcap.hpp"
#include "polar.hpp"
#include "hog.hpp"
#include "aloha.hpp"

int main(int a, char **c)
{
    // return blured();
    //  return polar(a, c);
    //  return hog(a, c);

    // return frameDifferencing(a, c);
    //   return bgSub(a, c);
    // return smask();
    // return conv(a, c);
    return wowfilter(a, c);
}