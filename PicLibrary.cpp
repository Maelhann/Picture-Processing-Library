#include "PicLibrary.hpp"
#include "Colour.hpp"
#include "Utils.hpp"


using namespace std;

void PicLibrary::loadpicture(string path, string filename) {
    Picture picture = Picture(path);
    store.insert(pair<string, Picture>(filename, picture));
}

Picture PicLibrary::getpicture(string filename) {
    auto picture_iter = store.find(filename);
    if (picture_iter == store.end()) {
        cout << endl << " Error : Couldn't find picture with matching filename" << endl;
        return Picture();
    } else {
        return picture_iter->second;
    }
}

void PicLibrary::setpicture(string filename, Picture picture) {
    auto picture_iter = store.find(filename);
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
    Picture cont = Picture(pic.getwidth(), pic.getheight());
    int quarter = pic.getheight() / 4;
    cont.setimage(pic.getimage());

    thread first_quarter([quarter, &pic, &cont, this]() {
        vector<thread> optimization_threads1;
        for (int x = 1; x < quarter; x++) {
            optimization_threads1.emplace_back(std::thread([this, x, &pic, &cont]() {
                for (int y = 1; y < pic.getwidth() - 1; y++) {
                    cont.setpixel(y, x, getaveragecol(pic, y, x));
                }
            }));
        }
        for (thread &th : optimization_threads1) {
            if (th.joinable()) {
                th.join();
            }
        }
    });

    thread second_quarter([quarter, &pic, &cont, this]() {
        vector<thread> optimization_threads2;
        for (int x = quarter; x < 2 * quarter; x++) {
            optimization_threads2.emplace_back(std::thread([this, x, &pic, &cont]() {
                for (int y = 1; y < pic.getwidth() - 1; y++) {
                    cont.setpixel(y, x, getaveragecol(pic, y, x));
                }
            }));
        }
        for (thread &th : optimization_threads2) {
            if (th.joinable()) {
                th.join();
            }
        }
    });

    thread third_quarter([quarter, &pic, &cont, this]() {
        vector<thread> optimization_threads2;
        for (int x = 2 * quarter; x < 3 * quarter; x++) {
            optimization_threads2.emplace_back(std::thread([this, x, &pic, &cont]() {
                for (int y = 1; y < pic.getwidth() - 1; y++) {
                    cont.setpixel(y, x, getaveragecol(pic, y, x));
                }
            }));
        }
        for (thread &th : optimization_threads2) {
            if (th.joinable()) {
                th.join();
            }
        }
    });

    thread last_quarter([quarter, &pic, &cont, this]() {
        vector<thread> optimization_threads2;
        for (int x = 3 * quarter; x < pic.getheight() - 1; x++) {
            optimization_threads2.emplace_back(std::thread([this, x, &pic, &cont]() {
                for (int y = 1; y < pic.getwidth() - 1; y++) {
                    cont.setpixel(y, x, getaveragecol(pic, y, x));
                }
            }));
        }
        for (thread &th : optimization_threads2) {
            if (th.joinable()) {
                th.join();
            }
        }
    });
    first_quarter.join();
    second_quarter.join();
    third_quarter.join();
    last_quarter.join();
    setpicture(filename, cont);
}

Colour PicLibrary::getaveragecol(Picture pic, int x, int y) {
    Colour avg = Colour(0, 0, 0);
    int rval = 0;
    int bval = 0;
    int gval = 0;
    for (int i = x - 1; i < x + 2; i++) {
        rval += pic.getpixel(i, y - 1).getred() + pic.getpixel(i, y).getred()
                + pic.getpixel(i, y + 1).getred();
        bval += pic.getpixel(i, y - 1).getblue() + pic.getpixel(i, y).getblue()
                + pic.getpixel(i, y + 1).getblue();
        gval += pic.getpixel(i, y - 1).getgreen() + pic.getpixel(i, y).getgreen()
                + pic.getpixel(i, y + 1).getgreen();

    }
    avg.setred(rval / 9);
    avg.setblue(bval / 9);
    avg.setgreen(gval / 9);
    return avg;
}


void PicLibrary::concurrentinvert(string filename) {
    active_threads.emplace_back(std::thread([this, filename]() {
        getpicture(filename).lockpicture();
        invert(filename);
        getpicture(filename).unlockpicture();
    }));
}

void PicLibrary::concurrentgrayscale(string filename) {
    active_threads.emplace_back(std::thread([this, filename]() {
        getpicture(filename).lockpicture();
        grayscale(filename);
        getpicture(filename).unlockpicture();
    }));
}

void PicLibrary::concurrentrotate(int angle, string filename) {
    active_threads.emplace_back(std::thread([this, angle, filename]() {
        getpicture(filename).lockpicture();
        rotate(angle, filename);
        getpicture(filename).unlockpicture();
    }));
}

void PicLibrary::concurrentflip(char dir, string filename) {
    active_threads.emplace_back(std::thread([this, dir, filename]() {
        getpicture(filename).lockpicture();
        flipVH(dir, filename);
        getpicture(filename).unlockpicture();
    }));
}

void PicLibrary::concurrentblur(string filename) {
    active_threads.emplace_back(std::thread([this, filename]() {
        getpicture(filename).lockpicture();
        blur(filename);
        getpicture(filename).unlockpicture();
    }));
}


void PicLibrary::jointhreads() {
    for (thread &th : active_threads) {
        if (th.joinable()) {
            th.join();
        }
    }

}

 


 
