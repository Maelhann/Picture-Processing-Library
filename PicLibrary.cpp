#include "PicLibrary.hpp"
#include "Colour.hpp"
#include "Utils.hpp"

using namespace std;

void PicLibrary::loadpicture(string path, string filename) {
    Utils util;
    Picture picture = Picture(path);
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
        cout << endl << cursor->first;
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

void PicLibrary::rotate(int angle, string filename) {
    switch (angle) {
        case 90 :
            rotateonce(filename);
            break;
        case 180 :
            rotateonce(filename);
            rotateonce(filename);
            break;
        case 270:
            rotateonce(filename);
            rotateonce(filename);
            rotateonce(filename);
            break;
        default:
            cout << "Error :  Invalid angle" << endl;
            break;
    }
}

void PicLibrary::rotateonce(string filename) {
    Picture pic = getpicture(filename);
    Picture cont = Picture(pic.getheight(), pic.getwidth());
    for (int x = 0; x < cont.getwidth(); x++) {
        for (int y = 0; y < cont.getheight(); y++) {
            cont.setpixel(pic.getheight() - 1 - x, y, pic.getpixel(y, x));
        }
    }
    setpicture(filename, cont);
}

void PicLibrary::flipVH(char plane, string filename) {
    Picture pic = getpicture(filename);
    Picture cont = Picture(pic.getwidth(), pic.getheight());
    if (plane == 'H') {
        for (int x = 0; x < cont.getwidth(); x++) {
            for (int y = 0; y < cont.getheight(); y++) {
                cont.setpixel(cont.getwidth() - x - 1, y, pic.getpixel(x, y));
            }
        }
    } else {
        for (int x = 0; x < cont.getwidth(); x++) {
            for (int y = 0; y < cont.getheight(); y++) {
                cont.setpixel(x, cont.getheight() - 1 - y, pic.getpixel(x, y));
            }
        }
    }
    setpicture(filename, cont);
}

void PicLibrary::blur(string filename) {
    Picture pic = getpicture(filename);
    for (int x = 1; x < pic.getwidth() - 1; x++) {
        for (int y = 1; y < pic.getheight() - 1; y++) {
            pic.setpixel(x, y, getaveragecol(pic, x, y));
        }
    }
    setpicture(filename, pic);

}

Colour PicLibrary::getaveragecol(Picture pic, int x, int y) {
    Colour avg = Colour(0, 0, 0);
    int rval = 0;
    int bval = 0;
    int gval = 0;
    for (int i = x - 1; i < x + 2; i++) {
        for (int j = y - 1; j < y + 2; j++) {
            rval += pic.getpixel(i, j).getred();
            bval += pic.getpixel(i, j).getblue();
            gval += pic.getpixel(i, j).getgreen();
        }
    }
    avg.setred(rval / 9);
    avg.setblue(bval / 9);
    avg.setgreen(gval / 9);
    return avg;
}


 


 
