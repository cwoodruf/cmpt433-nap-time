#ifndef MUSICLIST_H
#define MUSICLIST_H
#include <QString>
#include <QStringList>

class MusicList
{
public:
    MusicList () {}
    ~MusicList () {}
    bool setReadSource (const char* filename);
    QStringList& getMusicList ();
    bool isEmpty ();
private:
    QStringList list;
};

#endif // MUSICLIST_H
