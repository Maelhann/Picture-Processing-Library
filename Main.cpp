#include <iostream>
#include <string>
#include <sstream>
#include <iterator>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"

using namespace std;


int main(int argc, char **argv) {
    map<string, int> commands_to_integers;
    commands_to_integers.insert(pair<string, int>("exit", 0));
    commands_to_integers.insert(pair<string, int>("liststore", 1));
    commands_to_integers.insert(pair<string, int>("load", 2));
    commands_to_integers.insert(pair<string, int>("unload", 3));
    commands_to_integers.insert(pair<string, int>("save", 4));
    commands_to_integers.insert(pair<string, int>("display", 5));
    commands_to_integers.insert(pair<string, int>("invert", 6));
    commands_to_integers.insert(pair<string, int>("grayscale", 7));
    commands_to_integers.insert(pair<string, int>("rotate", 8));
    commands_to_integers.insert(pair<string, int>("flip", 9));
    commands_to_integers.insert(pair<string, int>("blur", 10));
    PicLibrary lib;
    Picture picture;
    Mat image = Mat();

    while (true) {
        cout << "\n> ";
        string command;
        getline(cin, command);
        string arg;
        string arg2;
        stringstream arguments(command);
        arguments >> arg;
        auto iter = commands_to_integers.find(arg);
        int command_index = 0;
        if (iter != commands_to_integers.end()) {
            command_index = iter->second;
        } else {
            cout << "Error : input command malformed or inexisting";
        }
        switch (command_index) {
            case 1 :
                lib.print_picturestore();
                break;
            case 2 :
                arguments >> arg;
                arguments >> arg2;
                lib.loadpicture(arg, arg2);
                break;
            case 3 :
                arguments >> arg;
                lib.unloadpicture(arg);
                break;
            case 4 :
                arguments >> arg;
                arguments >> arg2;
                lib.savepicture(arg, arg2);
                break;
            case 5 :
                arguments >> arg;
                lib.display(arg);
                break;
            case 6 :
                arguments >> arg;
                lib.invert(arg);
                break;
            case 7 :
                 arguments >> arg;
                 lib.grayscale(arg);
                break;
            case 8 :;
                break;
            case 9 :;
                break;
            case 10 :;
                break;
            case 0 :
                return 0;


        }
    }


}


