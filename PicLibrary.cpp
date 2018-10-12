#include "PicLibrary.hpp"
#include "Colour.hpp"
#include "Utils.hpp"

using namespace std;

void PicLibrary::add_to_store(Picture image, string filename) {
    PicLibrary::store.insert(pair<string, Picture>(filename, image));
}

void PicLibrary::remove_from_store(string filename) {
    PicLibrary::store.erase(filename);
}

void PicLibrary::print_picturestore() {
    for (auto cursor = PicLibrary::store.begin(); cursor != PicLibrary::store.end(); ++cursor) {
        cout << cursor->first;
    }
}

void PicLibrary::savepicture(string filename, string path) {
    auto picture_iter = store.find(filename);
    Utils utils;
    if(picture_iter == store.end()){
        cout << "\n Error : Couldn't find picture with matching filename" << endl ;
    }else{
        utils.saveimage(picture_iter->second.getimage(),path);
    }

}

void PicLibrary::display(string filename) {
    auto picture_iter = store.find(filename);
    Utils utils;
    if(picture_iter == store.end()){
        cout << "\n Error : Couldn't find picture with matching filename" << endl ;
    }else{
        utils.displayimage(picture_iter->second.getimage());
    }

}


 


 
