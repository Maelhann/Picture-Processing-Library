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
    queue<tuple<int, char, int >> commands; // this models an operation.
    Mutex lock;

    // code functions to add tuples to the queue and pop.
    //
    // don't create a thread every time --- try this.
    // on each load, create a thread t
    // t {

    //   while(picture list of commands not empty)
    //    lock.lock ;
    //    run command at the head of the queue;
    //    pop queue;
    //    lock.unlock ;
    //   }

    // !! the locks are so the thread waits and also to avoir race conditions with the queue !!!!
    // ----> need to add commands to picture's queue
    // ----> need to add sa

public:
    bool hasnext;

    enum RGB {
        BLUE, GREEN, RED
    };

    void addtocommandlist(int i, char plane, int op);

    void popcommand();

    void lockpicture();

    void unlockpicture();

    tuple<int, char, int> getnexttransformation();

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
