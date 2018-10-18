#ifndef PICTURE_H
#define PICTURE_H

#include "Colour.hpp"
#include "Utils.hpp"

// The Picture class provided a wrapper for image manipulation via the opencv
// library

class Picture {

  private:
    // opencv representation of an image
    Mat img;
    Utils imgio;
    Mutex lock;
    queue<tuple<int,string,string,int,char > tuple>; // this models an operation.

    // code functions to add tuples to the queue and pop.
    //
    // don't create a thread every time --- try this.
    // on each load, create a thread t
    // t {
    //   while filename you just loaded is in library
    //    run command at the head of the queue
    //    pop queue.
    //   }

  public:


  enum RGB {BLUE, GREEN, RED};
  void lockpicture();
  void unlockpicture();
  // class constructor, loads an image from a provided file
  Picture(string path);

  // class constructor, creates an image provided the size 
  Picture(int width, int height);  

  // default constructor/deconstructor
  Picture();
  ~Picture(){};

  void upcount();
  void downcount();

    int getWaitingThreads() const;

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
