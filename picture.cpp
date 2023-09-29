#include "picture.h"

using namespace std;

Picture::Picture(const QPixmap& pixmap, int id)
        : m_pixmap(pixmap),
          id(id)
{
}

QPixmap Picture::getPixmap() const {
    return m_pixmap;
}

int Picture::getId() const {
    return id;
}
