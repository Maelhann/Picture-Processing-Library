#include "PicLibrary.hpp"
#include "Colour.hpp"
#include "Utils.hpp"

using namespace std;

void PicLibrary::loadpicture(string path, string filename) {
    Utils util;
    Picture picture;
    picture.setimage(util.loadimage(path));
    PicLibrary::store.insert(pair<string, Picture>(filename, picture));
}

Picture PicLibrary::getpicture(string filename) {
    auto picture_iter = store.find(filename);
    Utils utils;
    if (picture_iter == store.end()) {
        cout << "\n Error : Couldn't find picture with matching filename" << endl;
        return Picture();
    } else {
        return picture_iter->second;
    }
}

void PicLibrary::setpicture(string filename, Picture picture) {
    auto picture_iter = store.find(filename);
    Utils utils;
    if (picture_iter == store.end()) {
        PicLibrary::store.insert(pair<string, Picture>(filename, picture));
    } else {
        picture_iter->second = picture;
    }
}

void PicLibrary::unloadpicture(string filename) {
    PicLibrary::store.erase(filename);
}

void PicLibrary::print_picturestore() {
    for (auto cursor = PicLibrary::store.begin(); cursor != PicLibrary::store.end(); ++cursor) {
        cout << cursor->first;
    }
}

void PicLibrary::savepicture(string filename, string path) {
    Utils utils;
    utils.saveimage(getpicture(filename).getimage(), path);
}

void PicLibrary::display(string filename) {
    Utils utils;
    utils.displayimage(getpicture(filename).getimage());
}

void PicLibrary::invert(string filename) {
    Picture pic = getpicture(filename);
    for (int x = 0; x < pic.getwidth(); x++) {
        for (int y = 0; y < pic.getheight(); y++) {
            Colour newcolor = pic.getpixel(x, y);
            newcolor.setblue(255 - newcolor.getblue());
            newcolor.setgreen(255 - newcolor.getgreen());
            newcolor.setred(255 - newcolor.getred());
            pic.setpixel(x, y, newcolor);
        }
    }
    setpicture(filename, pic);
}

void PicLibrary::grayscale(string filename) {
    Picture pic = getpicture(filename);
    for (int x = 0; x < pic.getwidth(); x++) {
        for (int y = 0; y < pic.getheight(); y++) {
            Colour newcolor = pic.getpixel(x, y);
            int graycolor = (newcolor.getred() + newcolor.getgreen() + newcolor.getblue()) / 3;
            newcolor.setred(graycolor);
            newcolor.setgreen(graycolor);
            newcolor.setblue(graycolor);
            pic.setpixel(x, y, newcolor);
        }
    }
    setpicture(filename, pic);
}


 


 
