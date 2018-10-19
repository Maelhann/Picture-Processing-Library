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
     *I chose my synchronization strategy after having implemented and carefully thought about other strategies,
     *
     * Using a global mutex lock for the library and locking at each transformation
     * was a lot less efficient despite being much easier to implement.
     * The cost of locking was too high and mobilized the whole library, making the program in effect sequential.
     * It also didn't ensure that operations were executed in order.
     *
     * Picture locking and creating a thread for each function transformation fixed the concurrency problem,
     * but still wasn't enough to ensure operations were executed in the right order. Following this observation I came
     * up with my own synchronization system.
     *
     * MY FINAL SYNCHRONIZATION STRATEGY WORKS AS FOLLOW :
     *
     * Each Picture holds a queue of commands, which I modeled as a priority queue of <int,char,int> tuples.
     * for each tuple the first int represents an optional angle, the char represents an optional plane, and the last
     * int represents an opcode.
     *
     * When the interpreter receives a picture transformation command for picture P
     * it pushes a tuple representing this command at the back of P's priorityqueue, and then calls our
     * library to execute the next transformation on P (that is, to process the tuple at the top of P's priorityqueue)
     *
     * The library then retrieves the tuple at the top of P's priority queue and applies the right transformation to it
     * using the information stored in the tuple. It then finishes by popping this command off the top of the queue.
     * This strategy ensures that all transformations made to a picture are applied in the order in which they were
     * given
     *
     * Additionally, since my picture transformation functions operate concurrently
     * with their own threads referenced in the active_threads vector declared in PicLibrary
     * , I have equipped each picture with a Mutex lock to ensure exclusive access to the picture for each of
     * my transformation functions and avoid potential race conditions.
     *
     * The interpreter routine functions save, load, unload and display are all called atomically and rely on utils,
     * as such, they can't really be atomically optimized, and are very light operations. I found that adding
     * concurrency for these routines is slower than just letting them be executed sequentially, as that would
     * imply having to lock our entire library each time to avoid race conditions on our image store.
     *
     *
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

    void jointhreads();

    // mandatory alternative "blur" implementations

    void sequentialblur(string filename);

    void pixelbypixelblur(string filename);




};

#endif

