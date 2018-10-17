#ifndef PICLIB_H
#define PICLIB_H

#include "Picture.hpp"
#include "Utils.hpp"
#include <mutex>
#include <thread>

class PicLibrary {

 /* THREAD-SAFETY OF THE PICLIBRARY CLASS :
  *
  * My Synchronization strategy was to implement a list of operation threads, each operation thread getting exclusive
  * access to the image it wants to transform
  *
  * For each available picture transformation, I wrote a concurrent wrapper which adds a mutex lock
  * to get exclusive access to the picture we want to transform,
  *
  * */


private:

    map<string, Picture> store;
    mutex lock;
    vector<thread> active_threads;


public:
    PicLibrary() {};
    ~PicLibrary() {};

    // command-line interpreter routines
    void print_picturestore();

    void loadpicture(string path, string filename);

    void unloadpicture(string filename);

    void savepicture(string filename, string path);

    void display(string filename);

    Picture getpicture(string filename);

    void setpicture(string filename, Picture picture);

    // picture transformation routines

    void invert(string filename);

    void grayscale(string filename);

    void rotate(int angle, string filename);

    void rotateonce(string filename);

    void flipVH(char plane, string filename);

    void blur(string filename);

    Colour getaveragecol(Picture pic, int x, int y);

    // functions handling concurrent transformations.

    void concurrentinvert(string filename);

    void concurrentgrayscale(string filename);

    void concurrentrotate(int angle, string filename);

    void concurrentflip(char direction, string filename);

    void concurrentblur(string filename);

    void jointhreads();


};

#endif

