#include <iostream>
#include "gallery.h"
#include <QDebug>

using namespace std;

Gallery::Gallery()
{
    std::vector<Picture*> album(35, nullptr);

}

void Gallery::addPicture(Picture* pic) {

    album.push_back(pic);
}


Picture* Gallery::getPicture(int index) const
{
    if (index >= 0 && index < album.size())
        return album[index];

    return nullptr;
}
void Gallery::printGallery() {
    for (auto pic : album) {
        if (pic) {
            qDebug() << "Picture ID: " << pic->getId();
        } else {
            qDebug() << "Null picture in gallery.";
        }
    }
}

bool Gallery::isSorted() {
    for (size_t i = 0; i < album.size() - 1; ++i) {
        if (album[i]->getId() > album[i + 1]->getId()) {
            return false;
        }
    }
    return true;
}

void Gallery::clearGallery() {
    album.clear();
}
