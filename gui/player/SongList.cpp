#include "SongList.h"
//#include <QFile>
//#include <QTextStream>
#include <QProcess>
#include <QDebug>
#include <string>
void getMusicList (QString peer, QString& musicList)
{
	QProcess getmusiclist;
	getmusiclist.start("getmusiclist",QStringList() << peer);
	getmusiclist.waitForFinished();
	musicList = getmusiclist.readAllStandardOutput();
}

SongList::SongList () {
    refresh ();
}

SongList::~SongList () {
    clear ();
}

bool SongList::setReadSource (const char* f) {
    QString filename (f);
    QString list;
    
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

void SongList::refresh () {
    clear ();
    songs.clear ();
    QProcess napshared;
    napshared.start("napconfig",QStringList() << "napshared");
    napshared.waitForFinished();
    QString dir (napshared.readAllStandardOutput());
    readDir (dir);
}

void SongList::readDir (QString& str) {
    QDir dir;
    //dir.setPath(currDir->text()); //set directory
    dir.cd(str); //switch to another directory
    //currDir->setText(dir.absolutePath());
    readFiles(dir);
}

void SongList::readFiles (QDir& dir) {
    QStringList strings;
    strings << "*.mp3" ;
    QFileInfoList fileInfoList = dir.entryInfoList(strings,   //filename filter，"*", "*.tar.gz"
        QDir::Files,  //file attribute filter,directory,file，read/write
        QDir::NoSort  //or QDir::Name sort, name, modified time, size, dirfirst
    );

    for(int i=0; i<fileInfoList.count(); i++) {
        QFileInfo tmpFileInfo = fileInfoList.at(i);
        readFileInfo (tmpFileInfo);
        /*if((tmpFileInfo.isDir())) {
            QIcon icon = QIcon::fromTheme("edit-undo");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem(icon, fileName); //show icon and name on list
            ui->listWidget->addItem(tmp);
            tmp->setCheckState(Qt::Checked);
            //ui->listWidget->setItemWidget(tmp, new QCheckBox);
        } else if(tmpFileInfo.isFile()) {
            QIcon icon = QIcon::fromTheme("edit-cut");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem (icon, fileName);
            ui->listWidget->addItem(tmp);
        } */
    }

    QProcess getpeers;
    QString peers;

    getpeers.start("getpeers");
    getpeers.waitForFinished();
    peers = QString(getpeers.readAllStandardOutput());
    QStringList peersList = peers.split("\n",QString::SkipEmptyParts);
    //qDebug () << peersList;

    QString musicFromPeers;
    // for (QStringList::iterator it = peersList.begin (); it != peersList.end (); it++) {
    foreach (QString peer, peersList) {
        musicFromPeers.clear ();
	qDebug () << peer;
        //getMusicList (peer.toLocal8Bit().data(), musicFromPeers);
        getMusicList (peer, musicFromPeers);
        if (musicFromPeers.length () != 0) {
            QStringList musics = musicFromPeers.split ('\n');
	    foreach (QString filename, musics) {
                    Song* song = new Song;
		    song->path = filename;
		    QStringList parts = filename.split("/");
                    if (parts.count () == 0) {
			song->filename = filename;
                    } else {
                        qDebug () << parts[parts.count () - 1];
                        qDebug () << parts[0];
                        song->filename = peer + "\n" + parts[parts.count () - 1];
                    }
                    song->title = "no title";
                    song->artist = "no artist";
                    song->inPlaylist = true;
                    songs.push_back (song);
            }
        }
    }
}

bool SongList::readFileInfo (QFileInfo& fileInfo) {
    qDebug () << fileInfo.fileName ();
    Song* song = new Song;
    song->path = fileInfo.absoluteFilePath ();
    song->filename = fileInfo.fileName ();
    song->title = "no title";
    song->artist = "no artist";
    song->inPlaylist = true;
    songs.push_back (song);
    
    QProcess madplay;
    QStringList arguments;
    arguments << "-T" << song->path;
    madplay.start("madplay", arguments);
    if (!madplay.waitForStarted())
        return false;

    madplay.closeWriteChannel();

    if (!madplay.waitForFinished())
        return false;

    QByteArray returnValue = madplay.readAllStandardError();
    QString result (returnValue);
    QStringList infos = result.split("\n");
    if (infos.count () <= 1) {
        qDebug () << "reading tag error";
        return false;
    }
    for (QStringList::const_iterator it = infos.begin () + 1; it != infos.end (); it++) {
        QString info = *it;
        info = info.trimmed ();
        if (info.startsWith ("Title")) {
            song->title = info.remove (0, 7);
        } else if (info.startsWith ("Artist")) {
            song->artist = info.remove (0, 8);
        }
    }
    return true;  
}

int SongList::getSize () {
    return songs.count ();
}

Song* SongList::getSongInfo (int index) {
    return songs.at (index);
}

bool SongList::isEmpty () {
    return (songs.count () == 0);
}

void SongList::clear () {
    QVector<Song*>::iterator it;
    for (it = songs.begin (); it != songs.end (); it++) {
        delete *it;
    }
    songs.clear ();
}
