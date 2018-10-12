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
  map<string,int> commands_to_integers; 
  commands_to_integers.insert (pair<string,int>("exit",0));
  commands_to_integers.insert (pair<string,int>("liststore",1));
  commands_to_integers.insert (pair<string,int>("load",2));
  commands_to_integers.insert (pair<string,int>("unload",3));
  commands_to_integers.insert (pair<string,int>("save",4));
  commands_to_integers.insert (pair<string,int>("display",5));
  commands_to_integers.insert (pair<string,int>("invert",6));
  commands_to_integers.insert (pair<string,int>("grayscale",7));
  commands_to_integers.insert (pair<string,int>("rotate",8));
  commands_to_integers.insert (pair<string,int>("flip",9));
  commands_to_integers.insert (pair<string,int>("blur",10));
  	
  
  while(true){
  string command;
  cin >> command; 
  istringstream iss(command);  
  vector<string> arguments(istream_iterator<string>{iss},istream_iterator<string>());
  auto iter = commands_to_integers.find(arguments[0]); 
  int command_index =  0;
   if(iter != commands_to_integers.end()){
     command_index = iter->second; 
   }else{
    cout << "Input command malformed or inexisting";  
   }
   switch(command_index){
   case 1 : ; break;
   case 2 : ; break ; 
   case 3 : ; break ; 
   case 4 : ; break ;
   case 5 : ; break ; 
   case 6 : ; break ;
   case 7 : ; break ;
   case 8 :  ; break;
   case 9 : ; break ;   
   case 10 : ; break ; 
   case 0 : return 0 ;


   } 
  }
  return 0;

}


