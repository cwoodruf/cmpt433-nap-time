#ifndef MUSICLIST_H
#define MUSICLIST_H
#include <QString>
#include <QVector>
#include <QDir>
#include <QFileInfoList>

struct Song {
    QString title;
    QString artist;
    QString filename;
    QString path;
    bool inPlaylist;
};

class SongList
{
public:
    SongList ();
    ~SongList ();
    bool setReadSource (const char* filename);
    bool isEmpty ();
    int getSize ();
    Song* getSongInfo (int index);
    void refresh ();
private:
    void clear ();
    void readDir (QString& str);
    void readFiles (QDir& dir);
    bool readFileInfo (QFileInfo& fileInfo);
    QVector<Song*> songs;
};

#endif // MUSICLIST_H
