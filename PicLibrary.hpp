#ifndef PICLIB_H
#define PICLIB_H

#include "Picture.hpp"
#include "Utils.hpp"
#include <mutex>
#include <thread>

class PicLibrary {

    /* THREAD-SAFETY OF THE PICLIBRARY CLASS :
     *
     * My Synchronization strategy was to implement a list of operation threads,
     * each operation thread getting exclusive
     * access to the image it wants to transform via a mutex lock.
     *
     * For each available transformation, the concurrent wrapper works as follow :
     *  - Add a new thread at the back of the list of active threads performing picture transformations
     *  - This thread acquires a locks the picture we want to transform
     *    and executes the transformation before releasing the lock.
     *
     * This strategy ensures that no two transformations happen at the same time on the same picture,
     * making the PicLibrary.
     * Adding lock() and unlock() functions in Pictures gives us a way to independently transform different pictures
     * concurrently. It is also more efficient and fine grained than locking
     * PicLibrary with a general mutex, which would still in effect be sequential, even if it means my implementation
     * is a bit slower.
     *
     * Additionally, since new operations are placed at the back of the thread list, and since they need to acquire
     * the lock of the pictures they want to transform, the operation order is maintained throughout the execution
     * of our program
     *
     * OPTIMIZATION OF THE BLUR FUNCTION :
     *
     * I experimented and pushed multiple implementations for the blur() and getAverageCol() functions,
     * I achieved the best performance when I split the image to blur in four sections, and delegated the blurring
     * of each section to a thread.
     *
     * Initially each of my sub-section thread was computing the pixels concurrently, but the time it took to create
     * the threads was not worth the time they saved. I thus reverted back to pure by-section optimization.
     *
     * I also implemented a check, to make sure we maximize the parallelization of the picture processing,
     * if there are more rows, than we compute concurrently row-by-row, otherwise we compute pixels column-by-column.
     *
     *
     * */


private:

    map<string, Picture> store;
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

