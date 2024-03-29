#ifndef PICTURE_H
#define PICTURE_H

#include <mutex>
#include "Colour.hpp"
#include "Utils.hpp"

// The Picture class provided a wrapper for image manipulation via the opencv
// library

class Picture {

private:
    // opencv representation of an image
    Mat img;
    Utils imgio;
    priority_queue<tuple<int, char, int>> queue;
    Mutex lock;

public:
    enum RGB {
        BLUE, GREEN, RED
    };


    void lockpicture();

    void unlockpicture();

    void queuepop();

    tuple<int, char, int> queuegetnext();

    void addcommand(int, char, int);

    // class constructor, loads an image from a provided file
    Picture(string path);

    // class constructor, creates an image provided the size
    Picture(int width, int height);

    // default constructor/deconstructor
    Picture();

    ~Picture() {};

    // determine the dimensions of the underlying image
    int getwidth();

    int getheight();

    // extract a single pixel from the image as a Colour object
    Colour getpixel(int x, int y);

    // set a singel pixel in the  image from a Colour object
    void setpixel(int x, int y, Colour rgb);

    // check if coordinates are within bounds of the stored image
    bool contains(int x, int y);

    // provides the image of the class
    Mat getimage();

    // overwrite Picture image with another image
    void setimage(Mat cpyimg);

};

#endif
