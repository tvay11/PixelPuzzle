#ifndef GALLERY_H
#define GALLERY_H

#include <vector>
#include "picture.h"

using namespace std;

class Gallery
{
public:
    Gallery();
    void addPicture(Picture* pic);
    Picture* getPicture(int index) const;
    bool isSorted();
    void printGallery();
    vector<Picture*> album;


    void clearGallery();
};

#endif // GALLERY_H
