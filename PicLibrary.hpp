#ifndef PICLIB_H
#define PICLIB_H

#include "Picture.hpp"
#include "Utils.hpp"

class PicLibrary {
 
  
  private:
  
  map<string,Picture> store;
   

  public:
   PicLibrary(){};
  ~PicLibrary(){};

  // command-line interpreter routines
  void print_picturestore();
  void loadpicture(string path, string filename);
  void unloadpicture(string filename);
  void savepicture(string filename, string path);
  void display(string filename);
  Picture getpicture(string filename);
  void setpicture(string filename,Picture picture);
  // picture transformation routines
   
  void invert(string filename);
  void grayscale(string filename);
  void rotate(int angle, string filename);
  void rotateonce(string filename);
  void flipVH(char plane, string filename);
  void blur(string filename);
  Colour getaveragecol(Picture pic, int x , int y);
};

#endif

