/****************************************************************************
** Meta object code from reading C++ file 'PlayerWindow.h'
**
** Created: Mon Nov 21 23:35:46 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PlayerWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PlayerWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PlayerWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   14,   13,   13, 0x0a,
      61,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PlayerWindow[] = {
    "PlayerWindow\0\0,\0"
    "musicSelected(QItemSelection,QItemSelection)\0"
    "printout()\0"
};

const QMetaObject PlayerWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_PlayerWindow,
      qt_meta_data_PlayerWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PlayerWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PlayerWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PlayerWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlayerWindow))
        return static_cast<void*>(const_cast< PlayerWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int PlayerWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: musicSelected((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        case 1: printout(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
