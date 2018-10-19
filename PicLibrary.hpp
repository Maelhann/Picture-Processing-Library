#ifndef PICLIB_H
#define PICLIB_H

#include "Picture.hpp"
#include "Utils.hpp"
#include <mutex>
#include <thread>
#include <map>
#include <queue>
#include <condition_variable>

class PicLibrary {

    /* THREAD-SAFETY OF THE PICLIBRARY CLASS :
     *
     * My synchronization strategy works as follow, each Picture holds a queue of commands, which I modeled with
     * tuples
     *
     *
     *
     * OPTIMIZATION OF THE BLUR FUNCTION :
     *
     * I experimented and pushed multiple implementations for the blur() and getAverageCol() functions,
     * I achieved the best performance when I split the image to blur in four squares
     * 2d sections, and delegated the blurring
     * of each section to a thread.
     *
     * I have experimented with multiple optimization techniques including :
     *  - Row by Row
     *  - Column by Column
     *  - Cutting up the Image in rows and blurring these row-sections concurrently
     *  - Concurrently blurring half of each pictures
     *  - Concurrently blurring quarters of each pictures
     *  - Mixing Halving and Line by Line optimization.
     *
     *  I also created, for each of these versions of blur, an alternative one with an if-check, that switches
     *  the type of optimization (for line by line) depending on wether or not we have more rows or columns.
     *  (in case of very disproportionate pictures)
     *
     *  Here are a few of my observations regarding the speed of my program with these different methods :
     *
     * Concurrently blurring line by line significantly boosts the speed of the program, with a speed of 1700 to
     * execute 10 concurrent blurs, if we add an if check to choose the best option between row-by-row and col-by-col
     * for each picture, then the speed goes up to 1400/1500. This is a good option, but has many pitfalls regardless
     * especially if we take into account how slow and expansive creating a new thread for each line is when we have
     * huge pictures.
     *
     * Splitting the image into subsections, which we concurrently blur achieves approximately the same result.
     * It requires less thread creation but more operations on each thread, which evens things out, still, trying
     * to apply line-by-line optimization for the threads on each half may still be worth it.
     *
     * The best result I achieved when I split the image into 4 subsections, boosting the blurring speed for 10
     * blurs to around 800, which is a signifcant improvement. Surprisingly, when I tried to apply line by line
     * optimization, the time it took to create threads in each section slowed me down to about 1400, Hence my choice
     * to keep the final version with the 4 subsections, but without line by line optimization for each of them. 
     *
     * MORE INPUT AND DATA IN REPORT.
     *
     */


private:

    map<string, Picture> store;
    vector<thread> active_threads;
    Utils utils;

public:
    PicLibrary() {};

    ~PicLibrary() {};


    // command-line interpreter routines
    void addtransformation(string filename, int angle, char plane, int opcode);

    void executenexttransformation(string filename);


    void print_picturestore();

    bool isinlibrary(string filename);

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

    void sequentialblur(string filename);

    void pixelbypixelblur(string filename);


    void jointhreads();


};

#endif

