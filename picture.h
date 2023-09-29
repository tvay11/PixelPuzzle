#ifndef PICTURE_H
#define PICTURE_H

#include <QPixmap>

class Picture
{
public:
    Picture(const QPixmap& pixmap, int id);
    QPixmap getPixmap() const;
    int getId() const;

private:
    QPixmap m_pixmap;
    int id;
};

#endif // PICTURE_H
