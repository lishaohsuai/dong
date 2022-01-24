/****************************************************************************
** Meta object code from reading C++ file 'CuttingWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CuttingWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CuttingWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CuttingWidget_t {
    QByteArrayData data[66];
    char stringdata0[941];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CuttingWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CuttingWidget_t qt_meta_stringdata_CuttingWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CuttingWidget"
QT_MOC_LITERAL(1, 14, 15), // "xslidderChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 5), // "value"
QT_MOC_LITERAL(4, 37, 15), // "yslidderChanged"
QT_MOC_LITERAL(5, 53, 15), // "zslidderChanged"
QT_MOC_LITERAL(6, 69, 31), // "construct_vertex_texture_signal"
QT_MOC_LITERAL(7, 101, 13), // "addCubeSignal"
QT_MOC_LITERAL(8, 115, 16), // "deleteCubeSignal"
QT_MOC_LITERAL(9, 132, 14), // "editCubeSignal"
QT_MOC_LITERAL(10, 147, 16), // "selectCubeSignal"
QT_MOC_LITERAL(11, 164, 21), // "selectMutiNodesSignal"
QT_MOC_LITERAL(12, 186, 17), // "connectNodeSignal"
QT_MOC_LITERAL(13, 204, 20), // "disconnectNodeSignal"
QT_MOC_LITERAL(14, 225, 15), // "reNewDataSignal"
QT_MOC_LITERAL(15, 241, 1), // "x"
QT_MOC_LITERAL(16, 243, 1), // "y"
QT_MOC_LITERAL(17, 245, 1), // "z"
QT_MOC_LITERAL(18, 247, 6), // "angleX"
QT_MOC_LITERAL(19, 254, 6), // "angleY"
QT_MOC_LITERAL(20, 261, 6), // "angleZ"
QT_MOC_LITERAL(21, 268, 6), // "length"
QT_MOC_LITERAL(22, 275, 5), // "width"
QT_MOC_LITERAL(23, 281, 6), // "height"
QT_MOC_LITERAL(24, 288, 7), // "genMesh"
QT_MOC_LITERAL(25, 296, 22), // "addCubeWithMouseSignal"
QT_MOC_LITERAL(26, 319, 13), // "setStepSignal"
QT_MOC_LITERAL(27, 333, 21), // "changeAngleAutoSignal"
QT_MOC_LITERAL(28, 355, 22), // "generateLinkBodySignal"
QT_MOC_LITERAL(29, 378, 15), // "topoSplitSignal"
QT_MOC_LITERAL(30, 394, 12), // "refineSignal"
QT_MOC_LITERAL(31, 407, 17), // "selectALineSignal"
QT_MOC_LITERAL(32, 425, 12), // "xyViewSignal"
QT_MOC_LITERAL(33, 438, 12), // "yzViewSignal"
QT_MOC_LITERAL(34, 451, 12), // "xzViewSignal"
QT_MOC_LITERAL(35, 464, 13), // "xyView_Signal"
QT_MOC_LITERAL(36, 478, 13), // "yzView_Signal"
QT_MOC_LITERAL(37, 492, 13), // "xzView_Signal"
QT_MOC_LITERAL(38, 506, 15), // "axisShowChanged"
QT_MOC_LITERAL(39, 522, 17), // "groundShowChanged"
QT_MOC_LITERAL(40, 540, 15), // "skelShowChanged"
QT_MOC_LITERAL(41, 556, 17), // "mirrorShowChanged"
QT_MOC_LITERAL(42, 574, 17), // "mirrorModeChanged"
QT_MOC_LITERAL(43, 592, 15), // "dragModeChanged"
QT_MOC_LITERAL(44, 608, 18), // "showTriMeshChanged"
QT_MOC_LITERAL(45, 627, 23), // "showProjectLinesChanged"
QT_MOC_LITERAL(46, 651, 14), // "drawModeChange"
QT_MOC_LITERAL(47, 666, 12), // "spiltXSignal"
QT_MOC_LITERAL(48, 679, 12), // "spiltYSignal"
QT_MOC_LITERAL(49, 692, 12), // "spiltZSignal"
QT_MOC_LITERAL(50, 705, 13), // "xExpandSignal"
QT_MOC_LITERAL(51, 719, 13), // "yExpandSignal"
QT_MOC_LITERAL(52, 733, 13), // "zExpandSignal"
QT_MOC_LITERAL(53, 747, 11), // "xCropSignal"
QT_MOC_LITERAL(54, 759, 11), // "yCropSignal"
QT_MOC_LITERAL(55, 771, 11), // "zCropSignal"
QT_MOC_LITERAL(56, 783, 18), // "roateByMouseChange"
QT_MOC_LITERAL(57, 802, 3), // "arg"
QT_MOC_LITERAL(58, 806, 20), // "vertexSnapModeChange"
QT_MOC_LITERAL(59, 827, 11), // "boom4Signal"
QT_MOC_LITERAL(60, 839, 20), // "simpleWithCgalSignal"
QT_MOC_LITERAL(61, 860, 18), // "readTestLineSignal"
QT_MOC_LITERAL(62, 879, 18), // "fittingCurveSignal"
QT_MOC_LITERAL(63, 898, 19), // "fittingCurvesSignal"
QT_MOC_LITERAL(64, 918, 12), // "actRMFSignal"
QT_MOC_LITERAL(65, 931, 9) // "pipeline2"

    },
    "CuttingWidget\0xslidderChanged\0\0value\0"
    "yslidderChanged\0zslidderChanged\0"
    "construct_vertex_texture_signal\0"
    "addCubeSignal\0deleteCubeSignal\0"
    "editCubeSignal\0selectCubeSignal\0"
    "selectMutiNodesSignal\0connectNodeSignal\0"
    "disconnectNodeSignal\0reNewDataSignal\0"
    "x\0y\0z\0angleX\0angleY\0angleZ\0length\0"
    "width\0height\0genMesh\0addCubeWithMouseSignal\0"
    "setStepSignal\0changeAngleAutoSignal\0"
    "generateLinkBodySignal\0topoSplitSignal\0"
    "refineSignal\0selectALineSignal\0"
    "xyViewSignal\0yzViewSignal\0xzViewSignal\0"
    "xyView_Signal\0yzView_Signal\0xzView_Signal\0"
    "axisShowChanged\0groundShowChanged\0"
    "skelShowChanged\0mirrorShowChanged\0"
    "mirrorModeChanged\0dragModeChanged\0"
    "showTriMeshChanged\0showProjectLinesChanged\0"
    "drawModeChange\0spiltXSignal\0spiltYSignal\0"
    "spiltZSignal\0xExpandSignal\0yExpandSignal\0"
    "zExpandSignal\0xCropSignal\0yCropSignal\0"
    "zCropSignal\0roateByMouseChange\0arg\0"
    "vertexSnapModeChange\0boom4Signal\0"
    "simpleWithCgalSignal\0readTestLineSignal\0"
    "fittingCurveSignal\0fittingCurvesSignal\0"
    "actRMFSignal\0pipeline2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CuttingWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      53,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  279,    2, 0x06 /* Public */,
       4,    1,  282,    2, 0x06 /* Public */,
       5,    1,  285,    2, 0x06 /* Public */,
       6,    0,  288,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  289,    2, 0x0a /* Public */,
       8,    0,  290,    2, 0x0a /* Public */,
       9,    0,  291,    2, 0x0a /* Public */,
      10,    0,  292,    2, 0x0a /* Public */,
      11,    0,  293,    2, 0x0a /* Public */,
      12,    0,  294,    2, 0x0a /* Public */,
      13,    0,  295,    2, 0x0a /* Public */,
      14,    9,  296,    2, 0x0a /* Public */,
      24,    0,  315,    2, 0x0a /* Public */,
      25,    0,  316,    2, 0x0a /* Public */,
      26,    0,  317,    2, 0x0a /* Public */,
      27,    0,  318,    2, 0x0a /* Public */,
      28,    0,  319,    2, 0x0a /* Public */,
      29,    0,  320,    2, 0x0a /* Public */,
      30,    0,  321,    2, 0x0a /* Public */,
      31,    0,  322,    2, 0x0a /* Public */,
      32,    0,  323,    2, 0x0a /* Public */,
      33,    0,  324,    2, 0x0a /* Public */,
      34,    0,  325,    2, 0x0a /* Public */,
      35,    0,  326,    2, 0x0a /* Public */,
      36,    0,  327,    2, 0x0a /* Public */,
      37,    0,  328,    2, 0x0a /* Public */,
      38,    1,  329,    2, 0x0a /* Public */,
      39,    1,  332,    2, 0x0a /* Public */,
      40,    1,  335,    2, 0x0a /* Public */,
      41,    1,  338,    2, 0x0a /* Public */,
      42,    1,  341,    2, 0x0a /* Public */,
      43,    1,  344,    2, 0x0a /* Public */,
      44,    1,  347,    2, 0x0a /* Public */,
      45,    1,  350,    2, 0x0a /* Public */,
      46,    0,  353,    2, 0x0a /* Public */,
      47,    0,  354,    2, 0x0a /* Public */,
      48,    0,  355,    2, 0x0a /* Public */,
      49,    0,  356,    2, 0x0a /* Public */,
      50,    0,  357,    2, 0x0a /* Public */,
      51,    0,  358,    2, 0x0a /* Public */,
      52,    0,  359,    2, 0x0a /* Public */,
      53,    0,  360,    2, 0x0a /* Public */,
      54,    0,  361,    2, 0x0a /* Public */,
      55,    0,  362,    2, 0x0a /* Public */,
      56,    1,  363,    2, 0x0a /* Public */,
      58,    1,  366,    2, 0x0a /* Public */,
      59,    0,  369,    2, 0x0a /* Public */,
      60,    0,  370,    2, 0x0a /* Public */,
      61,    0,  371,    2, 0x0a /* Public */,
      62,    0,  372,    2, 0x0a /* Public */,
      63,    0,  373,    2, 0x0a /* Public */,
      64,    0,  374,    2, 0x0a /* Public */,
      65,    0,  375,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,   15,   16,   17,   18,   19,   20,   21,   22,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   57,
    QMetaType::Void, QMetaType::Int,   57,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CuttingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CuttingWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->xslidderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->yslidderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->zslidderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->construct_vertex_texture_signal(); break;
        case 4: _t->addCubeSignal(); break;
        case 5: _t->deleteCubeSignal(); break;
        case 6: _t->editCubeSignal(); break;
        case 7: _t->selectCubeSignal(); break;
        case 8: _t->selectMutiNodesSignal(); break;
        case 9: _t->connectNodeSignal(); break;
        case 10: _t->disconnectNodeSignal(); break;
        case 11: _t->reNewDataSignal((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6])),(*reinterpret_cast< double(*)>(_a[7])),(*reinterpret_cast< double(*)>(_a[8])),(*reinterpret_cast< double(*)>(_a[9]))); break;
        case 12: _t->genMesh(); break;
        case 13: _t->addCubeWithMouseSignal(); break;
        case 14: _t->setStepSignal(); break;
        case 15: _t->changeAngleAutoSignal(); break;
        case 16: _t->generateLinkBodySignal(); break;
        case 17: _t->topoSplitSignal(); break;
        case 18: _t->refineSignal(); break;
        case 19: _t->selectALineSignal(); break;
        case 20: _t->xyViewSignal(); break;
        case 21: _t->yzViewSignal(); break;
        case 22: _t->xzViewSignal(); break;
        case 23: _t->xyView_Signal(); break;
        case 24: _t->yzView_Signal(); break;
        case 25: _t->xzView_Signal(); break;
        case 26: _t->axisShowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->groundShowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->skelShowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->mirrorShowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->mirrorModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->dragModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->showTriMeshChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->showProjectLinesChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->drawModeChange(); break;
        case 35: _t->spiltXSignal(); break;
        case 36: _t->spiltYSignal(); break;
        case 37: _t->spiltZSignal(); break;
        case 38: _t->xExpandSignal(); break;
        case 39: _t->yExpandSignal(); break;
        case 40: _t->zExpandSignal(); break;
        case 41: _t->xCropSignal(); break;
        case 42: _t->yCropSignal(); break;
        case 43: _t->zCropSignal(); break;
        case 44: _t->roateByMouseChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 45: _t->vertexSnapModeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 46: _t->boom4Signal(); break;
        case 47: _t->simpleWithCgalSignal(); break;
        case 48: _t->readTestLineSignal(); break;
        case 49: _t->fittingCurveSignal(); break;
        case 50: _t->fittingCurvesSignal(); break;
        case 51: _t->actRMFSignal(); break;
        case 52: _t->pipeline2(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CuttingWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CuttingWidget::xslidderChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CuttingWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CuttingWidget::yslidderChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CuttingWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CuttingWidget::zslidderChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CuttingWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CuttingWidget::construct_vertex_texture_signal)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CuttingWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_CuttingWidget.data,
    qt_meta_data_CuttingWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CuttingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CuttingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CuttingWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CuttingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 53)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 53;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 53)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 53;
    }
    return _id;
}

// SIGNAL 0
void CuttingWidget::xslidderChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CuttingWidget::yslidderChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CuttingWidget::zslidderChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CuttingWidget::construct_vertex_texture_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
