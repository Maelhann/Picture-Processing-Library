#include "PicLibrary.hpp"
#include "Colour.hpp"
#include "Utils.hpp"


using namespace std;


void PicLibrary::loadpicture(string path, string filename) {
    Picture picture = Picture(path);
    active_threads.emplace_back(thread([filename, picture, this]() {
        store.insert(pair<string, Picture>(filename, picture));
    }));
}

void PicLibrary::executenexttransformation(string filename) {
    tuple<int, char, int> tuple = getpicture(filename).queuegetnext();
    int angle = get<0>(tuple);
    char dir = get<1>(tuple);
    getpicture(filename).lockpicture();
    switch (get<2>(tuple)) {
        case 6:
            concurrentinvert(filename);
            break;
        case 7:
            concurrentgrayscale(filename);
            break;
        case 8:
            concurrentrotate(angle, filename);
            break;
        case 9:
            concurrentflip(dir, filename);
            break;
        case 10:
            concurrentblur(filename);
            break;
        default:
            break;
    }
    getpicture(filename).queuepop();
    getpicture(filename).unlockpicture();
}

bool PicLibrary::isinlibrary(string filename) {
    auto picture_iter = store.find(filename);
    if (picture_iter == store.end()) {
        return false;
    } else {
        return true;
    }
}

Picture PicLibrary::getpicture(string filename) {
    assert(isinlibrary(filename));
    auto picture_iter = store.find(filename);
    return picture_iter->second;
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
    active_threads.emplace_back(thread([this, filename]() {
        PicLibrary::store.erase(filename);

    }));
}

void PicLibrary::print_picturestore() {
    for (auto &a : store) {
        cout << endl << a.first;
    }
}

void PicLibrary::savepicture(string filename, string path) {
    active_threads.emplace_back(thread([this, path, filename]() {
        utils.saveimage(getpicture(filename).getimage(), path);
    }));
}

void PicLibrary::display(string filename) {
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
    /* ALTERNATIVE IMPLEMENTATIONS COMMENTED AT END OF FILE */
    Picture pic = getpicture(filename);
    Picture cont = Picture(pic.getwidth(), pic.getheight());
    int quarterheight = pic.getheight() / 2;
    int quarterwidth = pic.getwidth() / 2;
    cont.setimage(getpicture(filename).getimage());
    thread first_quarter([quarterheight, quarterwidth, &pic, &cont, this]() {
        for (int y = 1; y < quarterwidth; y++) {
            for (int x = 1; x < quarterheight; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    thread second_quarter([quarterheight, quarterwidth, &pic, &cont, this]() {
        for (int y = quarterwidth; y < pic.getwidth() - 1; y++) {
            for (int x = 1; x < quarterheight; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    thread third_quarter([quarterheight, quarterwidth, &pic, &cont, this]() {
        for (int y = 1; y < quarterwidth; y++) {
            for (int x = quarterheight; x < pic.getheight() - 1; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    thread last_quarter([quarterheight, quarterwidth, &pic, &cont, this]() {
        for (int y = quarterwidth; y < pic.getwidth() - 1; y++) {
            for (int x = quarterheight; x < pic.getheight() - 1; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    first_quarter.join();
    second_quarter.join();
    third_quarter.join();
    last_quarter.join();
    setpicture(filename, cont);
}


void PicLibrary::sequentialblur(string filename) {
    Picture pic = getpicture(filename);
    Picture cont = Picture(pic.getwidth(), pic.getheight());
    cont.setimage(pic.getimage());
    for (int x = 1; x < pic.getwidth() - 1; x++) {
        for (int y = 1; y < pic.getheight() - 1; y++) {
            cont.setpixel(x, y, getaveragecol(pic, x, y));
        }
    }
    setpicture(filename, cont);
}


void PicLibrary::pixelbypixelblur(string filename) {
    Picture pic = getpicture(filename);
    Picture cont = Picture(pic.getwidth(), pic.getheight());
    cont.setimage(pic.getimage());
    vector<thread> threads;
    for (int x = 1; x < pic.getwidth() - 1; x++) {
        for (int y = 1; y < pic.getheight() - 1; y++) {
            threads.emplace_back(thread([&cont, &pic, x, y, this]() {
                (cont.setpixel(x, y, getaveragecol(pic, x, y)));
            }));
        }
    }
    for (thread &th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }
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

void PicLibrary::addtransformation(string filename, int angle, char plane, int opcode) {
    assert(isinlibrary(filename));
    Picture pic = getpicture(filename);
    pic.addcommand(angle, plane, opcode);
    setpicture(filename, pic);
}

void PicLibrary::concurrentinvert(string filename) {
    active_threads.emplace_back(std::thread([this, filename]() {
        invert(filename);
    }));
}

void PicLibrary::concurrentgrayscale(string filename) {
    active_threads.emplace_back(std::thread([this, filename]() {
        grayscale(filename);
    }));
}

void PicLibrary::concurrentrotate(int angle, string filename) {
    active_threads.emplace_back(std::thread([this, angle, filename]() {
        rotate(angle, filename);
    }));
}

void PicLibrary::concurrentflip(char dir, string filename) {
    active_threads.emplace_back(std::thread([this, dir, filename]() {
        flipVH(dir, filename);
    }));
}

void PicLibrary::concurrentblur(string filename) {
    active_threads.emplace_back(std::thread([this, filename]() {
        blur(filename);
    }));
}


void PicLibrary::jointhreads() {
    for (thread &th : active_threads) {
        if (th.joinable()) {
            th.join();
        }
    }

}



/* PREVIOUS BLUR IMPLEMENTATION AND EXPERIMENTS
 *
 * SPLITTING THE IMAGE AND CONCURRENTLY BLURRING 8 EVEN SUBSECTIONS :
 *
 *void PicLibrary::blur(string filename) {
    Picture pic = getpicture(filename);
    Picture cont = Picture(pic.getwidth(), pic.getheight());
    int eigthheight = pic.getheight() / 4;
    int eigthwidth = pic.getwidth() / 2;
    cont.setimage(getpicture(filename).getimage());
    thread first_eighth([eigthwidth, eigthheight, &pic, &cont, this]() {
        for (int y = 1; y < eigthwidth; y++) {
            for (int x = 1; x < eigthheight; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    thread second_eighth([eigthwidth, eigthheight, &pic, &cont, this]() {
        for (int y = 1; y < eigthwidth; y++) {
            for (int x = eigthheight; x < 2 * eigthheight; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    thread third_eighth([eigthwidth, eigthheight, &pic, &cont, this]() {
        for (int y = 1; y < eigthwidth; y++) {
            for (int x = 2 * eigthheight; x < 3 * eigthheight; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    thread fourth_eighth([eigthwidth, eigthheight, &pic, &cont, this]() {
        for (int y = 1; y < eigthwidth; y++) {
            for (int x = 3 * eigthheight; x < pic.getheight() - 1; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    thread fifth_eighth([eigthwidth, eigthheight, &pic, &cont, this]() {
        for (int y = eigthwidth; y < pic.getwidth() - 1; y++) {
            for (int x = 1; x < eigthheight; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    thread sixth_eighth([eigthwidth, eigthheight, &pic, &cont, this]() {
        for (int y = eigthwidth; y < pic.getwidth() - 1; y++) {
            for (int x = eigthheight; x < 2 * eigthheight; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    thread seventh_eighth([eigthwidth, eigthheight, &pic, &cont, this]() {
        for (int y = eigthwidth; y < pic.getwidth() - 1; y++) {
            for (int x = 2 * eigthheight; x < 3 * eigthheight; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    thread last_eighth([eigthwidth, eigthheight, &pic, &cont, this]() {
        for (int y = eigthwidth; y < pic.getwidth()-1; y++) {
            for (int x = 3 * eigthheight; x < pic.getheight() - 1; x++) {
                cont.setpixel(y, x, getaveragecol(pic, y, x));
            }
        }
    });
    first_eighth.join();
    second_eighth.join();
    third_eighth.join();
    fourth_eighth.join();
    fifth_eighth.join();
    sixth_eighth.join();
    seventh_eighth.join();
    last_eighth.join();
    setpicture(filename, cont);
}
 *
 *
 *
 * OPTIMIZATION BY SPLITTING INTO 4 SECTIONS OF COLUMNS AND CONCURRENTLY COMPUTING ROWS :
 *
 * void PicLibrary::blur(string filename) {
    Picture pic = getpicture(filename);
    Picture cont = Picture(pic.getwidth(), pic.getheight());
    int quarter = pic.getwidth() / 4;
    cont.setimage(pic.getimage());

    thread first_quarter([quarter, &pic, &cont, this]() {
        vector<thread> optimization_threads1;
        for (int x = 1; x < quarter; x++) {
            optimization_threads1.emplace_back(std::thread([this, x, &pic, &cont]() {
                for (int y = 1; y < pic.getheight() - 1; y++) {
                    cont.setpixel(x, y, getaveragecol(pic, x, y));
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
                for (int y = 1; y < pic.getheight() - 1; y++) {
                    cont.setpixel(x, y, getaveragecol(pic, x, y));
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
                for (int y = 1; y < pic.getheight() - 1; y++) {
                    cont.setpixel(x, y, getaveragecol(pic, x, y));
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
        for (int x = 3 * quarter; x < pic.getwidth() - 1; x++) {
            optimization_threads2.emplace_back(std::thread([this, x, &pic, &cont]() {
                for (int y = 1; y < pic.getheight() - 1; y++) {
                    cont.setpixel(x, y, getaveragecol(pic, x, y));
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

 *
 * OPTIMIZATION BY HALFING THE PICTURE (SUBSECTION OF TWO ) AND CONCURRENTLY COMPUTING ROW BY ROW
 *
 *void PicLibrary::blur(string filename) {
    Picture pic = getpicture(filename);
    Picture cont = Picture(pic.getwidth(), pic.getheight());
    cont.setimage(pic.getimage());
    thread first_half([&pic, &cont, this]() {
        vector<thread> optimization_threads1;
        for (int x = 1; x < pic.getheight() / 2; x++) {
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

    thread second_half([&pic, &cont, this]() {
        vector<thread> optimization_threads2;
        for (int x = pic.getheight() / 2; x < pic.getheight() - 1; x++) {
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

    first_half.join();
    second_half.join();
    setpicture(filename, cont);

}

 *
 * OPTIMIZATION BY HALVING AND USING AN IFCHECK TO CONCURRENTLY PROCESS LINES ALONG LITTLE SIDE :
 *
 *
void PicLibrary::blur(string filename) {
    Picture pic = getpicture(filename);
    Picture cont = Picture(pic.getwidth(), pic.getheight());
    vector<thread> optimization_threads;
    cont.setimage(pic.getimage());
    if (pic.getheight() <= pic.getwidth()) {
        thread first_half([&optimization_threads, &pic, &cont, this]() {
            for (int x = 1; x < pic.getheight() / 2; x++) {
                optimization_threads.emplace_back(std::thread([this, x, &pic, &cont]() {
                    for (int y = 1; y < pic.getwidth() - 1; y++) {
                        cont.setpixel(y, x, getaveragecol(pic, y, x));
                    }
                }));
            }
        });

        thread second_half([&optimization_threads, &pic, &cont, this]() {
            for (int x = pic.getheight() / 2; x < pic.getheight() - 1; x++) {
                optimization_threads.emplace_back(std::thread([this, x, &pic, &cont]() {
                    for (int y = 1; y < pic.getwidth() - 1; y++) {
                        cont.setpixel(y, x, getaveragecol(pic, y, x));
                    }
                }));
            }
        });
        first_half.join();
        second_half.join();
    } else {
        thread first_half([&optimization_threads, &pic, &cont, this]() {
            for (int x = 1; x < pic.getwidth() / 2; x++) {
                optimization_threads.emplace_back(std::thread([this, x, &pic, &cont]() {
                    for (int y = 1; y < pic.getheight() - 1; y++) {
                        cont.setpixel(x, y, getaveragecol(pic, x, y));
                    }
                }));
            }
        });
        thread second_half([&optimization_threads, &pic, &cont, this]() {
            for (int x = (pic.getwidth() / 2); x < pic.getwidth() - 1; x++) {
                optimization_threads.emplace_back(std::thread([this, x, &pic, &cont]() {
                    for (int y = 1; y < pic.getheight() - 1; y++) {
                        cont.setpixel(x, y, getaveragecol(pic, x, y));
                    }
                }));
            }

        });
        first_half.join();
        second_half.join();
    }
    for (thread &th: optimization_threads) {
        th.join();
    }
    setpicture(filename, cont);

 }

 * LINE-BY-LINE OPTIMIZATION (LARGE SIDE)
 *
 * void PicLibrary::blur(string filename) {
    Picture pic = getpicture(filename);
    Picture cont = Picture(pic.getwidth(), pic.getheight());
    vector<thread> optimization_threads;
    cont.setimage(pic.getimage());

    if (pic.getheight() >= pic.getwidth()) {
        for (int x = 1; x < pic.getheight() - 1; x++) {
            optimization_threads.emplace_back(std::thread([this, x, &pic, &cont]() {
                for (int y = 1; y < pic.getwidth() - 1; y++) {
                    cont.setpixel(y, x, getaveragecol(pic, y, x));
                }
            }));
        }
    } else {
        for (int x = 1; x < pic.getwidth() - 1; x++) {
            optimization_threads.emplace_back(std::thread([this, x, &pic, &cont]() {
                for (int y = 1; y < pic.getheight() - 1; y++) {
                    cont.setpixel(x, y, getaveragecol(pic, x, y));
                }
            }));
        }
    }

    for (thread &th : optimization_threads) {
        th.join();
    }
    setpicture(filename, cont);

    LINE-BY-LINE OPTIMIZATION (LITTLE SIDE)
 *
 * void PicLibrary::blur(string filename) {
    Picture pic = getpicture(filename);
    Picture cont = Picture(pic.getwidth(), pic.getheight());
    vector<thread> optimization_threads;
    cont.setimage(pic.getimage());

    if (pic.getheight() < pic.getwidth()) {
        for (int x = 1; x < pic.getheight() - 1; x++) {
            optimization_threads.emplace_back(std::thread([this, x, &pic, &cont]() {
                for (int y = 1; y < pic.getwidth() - 1; y++) {
                    cont.setpixel(y, x, getaveragecol(pic, y, x));
                }
            }));
        }
    } else {
        for (int x = 1; x < pic.getwidth() - 1; x++) {
            optimization_threads.emplace_back(std::thread([this, x, &pic, &cont]() {
                for (int y = 1; y < pic.getheight() - 1; y++) {
                    cont.setpixel(x, y, getaveragecol(pic, x, y));
                }
            }));
        }
    }

    for (thread &th : optimization_threads) {
        th.join();
    }
    setpicture(filename, cont);

}
 *
 * */




