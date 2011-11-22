#include "MusicList.h"
#include <QFile>
#include <QTextStream>

bool MusicList::setReadSource (const char* f) {
    list.clear ();
    QString filename (f);
    
    QFile file (filename);
    if (!file.open (QIODevice::ReadOnly)) {
        return false;
    }
    QTextStream stream (&file);
    QString line;
    while (!stream.atEnd ()) {
        line = stream.readLine ();
        list.append (line);
    }
    file.close ();
    return true;
}

QStringList& MusicList::getMusicList () {
    return list;
}

bool MusicList::isEmpty () {
    return (list.size () == 0);
}
