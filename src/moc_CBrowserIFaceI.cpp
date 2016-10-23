/****************************************************************************
** Meta object code from reading C++ file 'CBrowserIFaceI.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CBrowserIFaceI.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CBrowserIFaceI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CBrowserIFace_t {
    QByteArrayData data[10];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CBrowserIFace_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CBrowserIFace_t qt_meta_stringdata_CBrowserIFace = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CBrowserIFace"
QT_MOC_LITERAL(1, 14, 7), // "newProc"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 8), // "readProc"
QT_MOC_LITERAL(4, 32, 9), // "printProc"
QT_MOC_LITERAL(5, 42, 8), // "quitProc"
QT_MOC_LITERAL(6, 51, 10), // "goBackProc"
QT_MOC_LITERAL(7, 62, 13), // "goForwardProc"
QT_MOC_LITERAL(8, 76, 11), // "hscrollProc"
QT_MOC_LITERAL(9, 88, 11) // "vscrollProc"

    },
    "CBrowserIFace\0newProc\0\0readProc\0"
    "printProc\0quitProc\0goBackProc\0"
    "goForwardProc\0hscrollProc\0vscrollProc"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CBrowserIFace[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    0,   60,    2, 0x0a /* Public */,
       9,    0,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CBrowserIFace::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CBrowserIFace *_t = static_cast<CBrowserIFace *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newProc(); break;
        case 1: _t->readProc(); break;
        case 2: _t->printProc(); break;
        case 3: _t->quitProc(); break;
        case 4: _t->goBackProc(); break;
        case 5: _t->goForwardProc(); break;
        case 6: _t->hscrollProc(); break;
        case 7: _t->vscrollProc(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CBrowserIFace::staticMetaObject = {
    { &CQMainWindow::staticMetaObject, qt_meta_stringdata_CBrowserIFace.data,
      qt_meta_data_CBrowserIFace,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CBrowserIFace::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CBrowserIFace::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CBrowserIFace.stringdata0))
        return static_cast<void*>(const_cast< CBrowserIFace*>(this));
    return CQMainWindow::qt_metacast(_clname);
}

int CBrowserIFace::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
