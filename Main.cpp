#include <iostream>
#include <string>
#include <sstream>
#include <iterator>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"

using namespace std;

// TODO: Implement the picture library command-line interpreter

int main(int argc, char ** argv)
{
  while(true){
  string command;
  cin >> command; 
  istringstream iss(command);  
  vector<string> arguments(istream_iterator<string>{iss},istream_iterator<string>());
   
   switch(arguments[0]){
   case "liststore" : ; break;
   case "load" : ; break ; 
   case "unload" : ; break ; 
   case "save" : ; break ;
   case "display" : ; break ; 
   case "invert" : ; break ;
   case "grayscale" : ; break ;
   case "rotate" :  ; break;
   case "flip" : ; break ;   
   case "blur" : ; break ; 
   case "exit" : return ;


   } 
  }
  return 0;

}


