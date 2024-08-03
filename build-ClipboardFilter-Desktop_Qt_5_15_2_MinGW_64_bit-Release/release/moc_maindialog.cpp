/****************************************************************************
** Meta object code from reading C++ file 'maindialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../maindialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'maindialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainDialog_t {
    QByteArrayData data[10];
    char stringdata0[186];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainDialog_t qt_meta_stringdata_MainDialog = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainDialog"
QT_MOC_LITERAL(1, 11, 18), // "on_clipboardChange"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 22), // "on_stateButton_clicked"
QT_MOC_LITERAL(4, 54, 31), // "on_characterListBox_textChanged"
QT_MOC_LITERAL(5, 86, 4), // "arg1"
QT_MOC_LITERAL(6, 91, 22), // "on_clearButton_clicked"
QT_MOC_LITERAL(7, 114, 23), // "on_insertButton_clicked"
QT_MOC_LITERAL(8, 138, 23), // "on_removeButton_clicked"
QT_MOC_LITERAL(9, 162, 23) // "on_updataButton_clicked"

    },
    "MainDialog\0on_clipboardChange\0\0"
    "on_stateButton_clicked\0"
    "on_characterListBox_textChanged\0arg1\0"
    "on_clearButton_clicked\0on_insertButton_clicked\0"
    "on_removeButton_clicked\0on_updataButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    1,   51,    2, 0x08 /* Private */,
       6,    0,   54,    2, 0x08 /* Private */,
       7,    0,   55,    2, 0x08 /* Private */,
       8,    0,   56,    2, 0x08 /* Private */,
       9,    0,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_clipboardChange(); break;
        case 1: _t->on_stateButton_clicked(); break;
        case 2: _t->on_characterListBox_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->on_clearButton_clicked(); break;
        case 4: _t->on_insertButton_clicked(); break;
        case 5: _t->on_removeButton_clicked(); break;
        case 6: _t->on_updataButton_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_MainDialog.data,
    qt_meta_data_MainDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MainDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
