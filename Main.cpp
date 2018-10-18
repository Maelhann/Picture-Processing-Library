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


    for (int i = 1; i < argc; i++) {
        string img_name = ((string) argv[i]).
                substr(((string) argv[i]).find_last_of('/') + 1);
        lib.loadpicture(argv[i], img_name);
    }

    while (true) {
        cout << endl << "> ";
        string command;
        if (!cin.eof()) {
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
                cout << "Invalid or malformed command" << endl;
                continue;
            }
            switch (command_index) {
                case 1 :
                    lib.print_picturestore();
                    break;
                case 2 :
                    arguments >> arg;
                    arguments >> arg2;
                    if (arg.substr(arg.length() - 4) == ".jpg") {
                        if (!lib.isinlibrary(arg2)) {
                            lib.loadpicture(arg, arg2);
                        } else {
                            cout << "Error : file already loaded";
                        }
                    } else {
                        cout << "Error : input is not a .jpg file";
                    }
                    break;
                case 3 :
                    arguments >> arg;
                    if (lib.isinlibrary(arg)) {
                        lib.unloadpicture(arg);
                    } else {
                        cout << "Error : couldn't find any file with a matching name";
                    }
                    break;
                case 4 :
                    arguments >> arg;
                    arguments >> arg2;
                    if (lib.isinlibrary(arg)) {
                        lib.savepicture(arg, arg2);
                    } else {
                        cout << "Error : couldn't find any file with a matching name";
                    }
                    break;
                case 5 :
                    arguments >> arg;
                    if (lib.isinlibrary(arg)) {
                        lib.display(arg);
                    } else {
                        cout << "Error : couldn't find any file with a matching name";
                    }
                    break;
                case 6 :
                    arguments >> arg;
                    if (lib.isinlibrary(arg)) {
                        lib.addtransformation(arg, 0, 'a', command_index);
                        lib.executenexttransformation(arg);
                    } else {
                        cout << "Error : couldn't find any file with a matching name";
                    }
                    break;
                case 7 :
                    arguments >> arg;
                    if (lib.isinlibrary(arg)) {
                        lib.addtransformation(arg, 0, 'a', command_index);
                        lib.executenexttransformation(arg);
                    } else {
                        cout << "Error : couldn't find any file with a matching name";
                    }
                    break;
                case 8 :
                    arguments >> arg;
                    arguments >> arg2;
                    if (lib.isinlibrary(arg2)) {
                        if (arg == "90") {
                            lib.addtransformation(arg2, 90, 'a', command_index);
                        } else if (arg == "180") {
                            lib.addtransformation(arg2, 180, 'a', command_index);
                        } else if (arg == "270") {
                            lib.addtransformation(arg2, 270, 'a', command_index);
                        } else {
                            cout << "Error : Invalid angle of rotation" << endl;
                            break;
                        }
                        lib.executenexttransformation(arg2);
                    } else {
                        cout << "Error : couldn't find any file with a matching name";
                    }
                    break;
                case 9 :
                    arguments >> arg;
                    arguments >> arg2;
                    if (lib.isinlibrary(arg2)) {
                        if (arg == "H") {
                            lib.addtransformation(arg2, 0, 'H', command_index);
                        } else if (arg == "V") {
                            lib.addtransformation(arg2, 0, 'V', command_index);
                        } else {
                            cout << "Error : Invalid plane specified" << endl;
                            break;
                        }
                        lib.executenexttransformation(arg2);
                    } else {
                        cout << "Error : couldn't find any file with a matching name";
                    }
                    break;
                case 10 :
                    arguments >> arg;
                    if (lib.isinlibrary(arg)) {
                        lib.addtransformation(arg, 0, 'a', command_index);
                        lib.executenexttransformation(arg);
                    } else {
                        cout << "Error : couldn't find any file with a matching name";
                    }
                    break;
                case 0 :
                    cout << "Now exiting interpreter" << endl;
                    return 0;
                default:
                    break;
            }
            lib.jointhreads();
        } else {
            return 0;
        }

    }
}


