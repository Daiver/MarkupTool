/****************************************************************************
** Meta object code from reading C++ file 'markupwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MarkupTool/markupwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'markupwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MarkupWindow_t {
    QByteArrayData data[19];
    char stringdata0[395];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MarkupWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MarkupWindow_t qt_meta_stringdata_MarkupWindow = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MarkupWindow"
QT_MOC_LITERAL(1, 13, 28), // "on_loadFacesAction_triggered"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 23), // "on_actionNext_triggered"
QT_MOC_LITERAL(4, 67, 23), // "on_actionPast_triggered"
QT_MOC_LITERAL(5, 91, 32), // "on_spinLandmarkSize_valueChanged"
QT_MOC_LITERAL(6, 124, 4), // "arg1"
QT_MOC_LITERAL(7, 129, 29), // "on_comboBoxBodyPart_activated"
QT_MOC_LITERAL(8, 159, 5), // "index"
QT_MOC_LITERAL(9, 165, 24), // "on_actionClear_triggered"
QT_MOC_LITERAL(10, 190, 29), // "on_actionLoad_shape_triggered"
QT_MOC_LITERAL(11, 220, 29), // "on_actionSave_shape_triggered"
QT_MOC_LITERAL(12, 250, 30), // "on_listFiles_itemDoubleClicked"
QT_MOC_LITERAL(13, 281, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(14, 298, 4), // "item"
QT_MOC_LITERAL(15, 303, 25), // "on_actionDelete_triggered"
QT_MOC_LITERAL(16, 329, 7), // "checked"
QT_MOC_LITERAL(17, 337, 35), // "on_actionUpdate_from_JSON_tri..."
QT_MOC_LITERAL(18, 373, 21) // "on_editButton_clicked"

    },
    "MarkupWindow\0on_loadFacesAction_triggered\0"
    "\0on_actionNext_triggered\0"
    "on_actionPast_triggered\0"
    "on_spinLandmarkSize_valueChanged\0arg1\0"
    "on_comboBoxBodyPart_activated\0index\0"
    "on_actionClear_triggered\0"
    "on_actionLoad_shape_triggered\0"
    "on_actionSave_shape_triggered\0"
    "on_listFiles_itemDoubleClicked\0"
    "QListWidgetItem*\0item\0on_actionDelete_triggered\0"
    "checked\0on_actionUpdate_from_JSON_triggered\0"
    "on_editButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MarkupWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    1,   82,    2, 0x08 /* Private */,
       7,    1,   85,    2, 0x08 /* Private */,
       9,    0,   88,    2, 0x08 /* Private */,
      10,    0,   89,    2, 0x08 /* Private */,
      11,    0,   90,    2, 0x08 /* Private */,
      12,    1,   91,    2, 0x08 /* Private */,
      15,    0,   94,    2, 0x08 /* Private */,
      15,    1,   95,    2, 0x08 /* Private */,
      17,    0,   98,    2, 0x08 /* Private */,
      18,    1,   99,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   16,

       0        // eod
};

void MarkupWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MarkupWindow *_t = static_cast<MarkupWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_loadFacesAction_triggered(); break;
        case 1: _t->on_actionNext_triggered(); break;
        case 2: _t->on_actionPast_triggered(); break;
        case 3: _t->on_spinLandmarkSize_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->on_comboBoxBodyPart_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_actionClear_triggered(); break;
        case 6: _t->on_actionLoad_shape_triggered(); break;
        case 7: _t->on_actionSave_shape_triggered(); break;
        case 8: _t->on_listFiles_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 9: _t->on_actionDelete_triggered(); break;
        case 10: _t->on_actionDelete_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->on_actionUpdate_from_JSON_triggered(); break;
        case 12: _t->on_editButton_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MarkupWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MarkupWindow.data,
      qt_meta_data_MarkupWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MarkupWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MarkupWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MarkupWindow.stringdata0))
        return static_cast<void*>(const_cast< MarkupWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MarkupWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
