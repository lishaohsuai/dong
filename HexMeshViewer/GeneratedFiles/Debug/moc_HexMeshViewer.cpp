/****************************************************************************
** Meta object code from reading C++ file 'HexMeshViewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../HexMeshViewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HexMeshViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HexMeshViewer_t {
    QByteArrayData data[92];
    char stringdata0[1207];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HexMeshViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HexMeshViewer_t qt_meta_stringdata_HexMeshViewer = {
    {
QT_MOC_LITERAL(0, 0, 13), // "HexMeshViewer"
QT_MOC_LITERAL(1, 14, 9), // "leeUpdate"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "subdivision"
QT_MOC_LITERAL(4, 37, 7), // "average"
QT_MOC_LITERAL(5, 45, 12), // "openMeshFile"
QT_MOC_LITERAL(6, 58, 6), // "readXJ"
QT_MOC_LITERAL(7, 65, 7), // "combine"
QT_MOC_LITERAL(8, 73, 8), // "readSkel"
QT_MOC_LITERAL(9, 82, 10), // "selectCell"
QT_MOC_LITERAL(10, 93, 15), // "selectMutiCells"
QT_MOC_LITERAL(11, 109, 13), // "rconnectNodes"
QT_MOC_LITERAL(12, 123, 8), // "editCell"
QT_MOC_LITERAL(13, 132, 7), // "delCell"
QT_MOC_LITERAL(14, 140, 14), // "drawNormalSkel"
QT_MOC_LITERAL(15, 155, 15), // "reNewDataSignal"
QT_MOC_LITERAL(16, 171, 23), // "genCoarseMeshFinishMesh"
QT_MOC_LITERAL(17, 195, 21), // "addCellUseMouseSignal"
QT_MOC_LITERAL(18, 217, 25), // "addCellUseMouseImplSignal"
QT_MOC_LITERAL(19, 243, 10), // "exportSkel"
QT_MOC_LITERAL(20, 254, 9), // "blendSkel"
QT_MOC_LITERAL(21, 264, 6), // "rotate"
QT_MOC_LITERAL(22, 271, 9), // "rupdateGL"
QT_MOC_LITERAL(23, 281, 10), // "helpString"
QT_MOC_LITERAL(24, 292, 6), // "rBoom4"
QT_MOC_LITERAL(25, 299, 16), // "rdisconnectNodes"
QT_MOC_LITERAL(26, 316, 9), // "clearSkel"
QT_MOC_LITERAL(27, 326, 12), // "rxRotateCell"
QT_MOC_LITERAL(28, 339, 12), // "ryRotateCell"
QT_MOC_LITERAL(29, 352, 12), // "rzRotateCell"
QT_MOC_LITERAL(30, 365, 11), // "rupMoveCell"
QT_MOC_LITERAL(31, 377, 13), // "rdownMoveCell"
QT_MOC_LITERAL(32, 391, 14), // "rfrontMoveCell"
QT_MOC_LITERAL(33, 406, 13), // "rbackMoveCell"
QT_MOC_LITERAL(34, 420, 13), // "rleftMoveCell"
QT_MOC_LITERAL(35, 434, 14), // "rrightMoveCell"
QT_MOC_LITERAL(36, 449, 3), // "PIA"
QT_MOC_LITERAL(37, 453, 12), // "rselectALine"
QT_MOC_LITERAL(38, 466, 22), // "rupdateSelectMutiNodes"
QT_MOC_LITERAL(39, 489, 10), // "rsetxyView"
QT_MOC_LITERAL(40, 500, 10), // "rsetyzView"
QT_MOC_LITERAL(41, 511, 10), // "rsetxzView"
QT_MOC_LITERAL(42, 522, 11), // "rsetxy_View"
QT_MOC_LITERAL(43, 534, 11), // "rsetyz_View"
QT_MOC_LITERAL(44, 546, 11), // "rsetxz_View"
QT_MOC_LITERAL(45, 558, 18), // "rmirrorModeChanged"
QT_MOC_LITERAL(46, 577, 19), // "rshowTriMeshChanged"
QT_MOC_LITERAL(47, 597, 24), // "rshowProjectLinesChanged"
QT_MOC_LITERAL(48, 622, 16), // "raxisShowChanged"
QT_MOC_LITERAL(49, 639, 18), // "rgroundShowChanged"
QT_MOC_LITERAL(50, 658, 16), // "rskelShowChanged"
QT_MOC_LITERAL(51, 675, 18), // "rmirrorShowChanged"
QT_MOC_LITERAL(52, 694, 16), // "rdragModeChanged"
QT_MOC_LITERAL(53, 711, 10), // "rnewNodeOK"
QT_MOC_LITERAL(54, 722, 16), // "rdrawModeChanged"
QT_MOC_LITERAL(55, 739, 14), // "faceNormChange"
QT_MOC_LITERAL(56, 754, 19), // "readOpenComplexSkel"
QT_MOC_LITERAL(57, 774, 17), // "simpleComplexSkel"
QT_MOC_LITERAL(58, 792, 14), // "simpleWithCgal"
QT_MOC_LITERAL(59, 807, 7), // "rspiltX"
QT_MOC_LITERAL(60, 815, 7), // "rspiltY"
QT_MOC_LITERAL(61, 823, 7), // "rspiltZ"
QT_MOC_LITERAL(62, 831, 8), // "rxExpand"
QT_MOC_LITERAL(63, 840, 8), // "ryExpand"
QT_MOC_LITERAL(64, 849, 8), // "rzExpand"
QT_MOC_LITERAL(65, 858, 6), // "rxCrop"
QT_MOC_LITERAL(66, 865, 6), // "ryCrop"
QT_MOC_LITERAL(67, 872, 6), // "rzCrop"
QT_MOC_LITERAL(68, 879, 21), // "rRotateByMouseChanged"
QT_MOC_LITERAL(69, 901, 18), // "rRotateCubeByMouse"
QT_MOC_LITERAL(70, 920, 3), // "V3f"
QT_MOC_LITERAL(71, 924, 15), // "fromVolumeToOff"
QT_MOC_LITERAL(72, 940, 14), // "changeNodeType"
QT_MOC_LITERAL(73, 955, 14), // "scaledJacobian"
QT_MOC_LITERAL(74, 970, 22), // "rVertexSnapModeChanged"
QT_MOC_LITERAL(75, 993, 3), // "arg"
QT_MOC_LITERAL(76, 997, 13), // "rMouseRelease"
QT_MOC_LITERAL(77, 1011, 7), // "project"
QT_MOC_LITERAL(78, 1019, 13), // "exportBroSkel"
QT_MOC_LITERAL(79, 1033, 7), // "fixFlip"
QT_MOC_LITERAL(80, 1041, 7), // "openOff"
QT_MOC_LITERAL(81, 1049, 9), // "shrinking"
QT_MOC_LITERAL(82, 1059, 15), // "showProjectLine"
QT_MOC_LITERAL(83, 1075, 14), // "pickProjPoints"
QT_MOC_LITERAL(84, 1090, 8), // "skelMove"
QT_MOC_LITERAL(85, 1099, 16), // "splitComplexSkel"
QT_MOC_LITERAL(86, 1116, 19), // "rreadTestLineSignal"
QT_MOC_LITERAL(87, 1136, 18), // "rchangeToPolylines"
QT_MOC_LITERAL(88, 1155, 20), // "rfittingCurvesSignal"
QT_MOC_LITERAL(89, 1176, 13), // "ractRMFSignal"
QT_MOC_LITERAL(90, 1190, 9), // "rshowSkel"
QT_MOC_LITERAL(91, 1200, 6) // "rBoom5"

    },
    "HexMeshViewer\0leeUpdate\0\0subdivision\0"
    "average\0openMeshFile\0readXJ\0combine\0"
    "readSkel\0selectCell\0selectMutiCells\0"
    "rconnectNodes\0editCell\0delCell\0"
    "drawNormalSkel\0reNewDataSignal\0"
    "genCoarseMeshFinishMesh\0addCellUseMouseSignal\0"
    "addCellUseMouseImplSignal\0exportSkel\0"
    "blendSkel\0rotate\0rupdateGL\0helpString\0"
    "rBoom4\0rdisconnectNodes\0clearSkel\0"
    "rxRotateCell\0ryRotateCell\0rzRotateCell\0"
    "rupMoveCell\0rdownMoveCell\0rfrontMoveCell\0"
    "rbackMoveCell\0rleftMoveCell\0rrightMoveCell\0"
    "PIA\0rselectALine\0rupdateSelectMutiNodes\0"
    "rsetxyView\0rsetyzView\0rsetxzView\0"
    "rsetxy_View\0rsetyz_View\0rsetxz_View\0"
    "rmirrorModeChanged\0rshowTriMeshChanged\0"
    "rshowProjectLinesChanged\0raxisShowChanged\0"
    "rgroundShowChanged\0rskelShowChanged\0"
    "rmirrorShowChanged\0rdragModeChanged\0"
    "rnewNodeOK\0rdrawModeChanged\0faceNormChange\0"
    "readOpenComplexSkel\0simpleComplexSkel\0"
    "simpleWithCgal\0rspiltX\0rspiltY\0rspiltZ\0"
    "rxExpand\0ryExpand\0rzExpand\0rxCrop\0"
    "ryCrop\0rzCrop\0rRotateByMouseChanged\0"
    "rRotateCubeByMouse\0V3f\0fromVolumeToOff\0"
    "changeNodeType\0scaledJacobian\0"
    "rVertexSnapModeChanged\0arg\0rMouseRelease\0"
    "project\0exportBroSkel\0fixFlip\0openOff\0"
    "shrinking\0showProjectLine\0pickProjPoints\0"
    "skelMove\0splitComplexSkel\0rreadTestLineSignal\0"
    "rchangeToPolylines\0rfittingCurvesSignal\0"
    "ractRMFSignal\0rshowSkel\0rBoom5"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HexMeshViewer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      88,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  454,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  455,    2, 0x0a /* Public */,
       4,    0,  456,    2, 0x0a /* Public */,
       5,    0,  457,    2, 0x0a /* Public */,
       6,    0,  458,    2, 0x0a /* Public */,
       7,    0,  459,    2, 0x0a /* Public */,
       8,    0,  460,    2, 0x0a /* Public */,
       9,    0,  461,    2, 0x0a /* Public */,
      10,    0,  462,    2, 0x0a /* Public */,
      11,    0,  463,    2, 0x0a /* Public */,
      12,    0,  464,    2, 0x0a /* Public */,
      13,    0,  465,    2, 0x0a /* Public */,
      14,    0,  466,    2, 0x0a /* Public */,
      15,    0,  467,    2, 0x0a /* Public */,
      16,    0,  468,    2, 0x0a /* Public */,
      17,    0,  469,    2, 0x0a /* Public */,
      18,    0,  470,    2, 0x0a /* Public */,
      19,    0,  471,    2, 0x0a /* Public */,
      20,    0,  472,    2, 0x0a /* Public */,
      21,    0,  473,    2, 0x0a /* Public */,
      22,    0,  474,    2, 0x0a /* Public */,
      23,    0,  475,    2, 0x0a /* Public */,
      24,    0,  476,    2, 0x0a /* Public */,
      25,    0,  477,    2, 0x0a /* Public */,
      26,    0,  478,    2, 0x0a /* Public */,
      27,    1,  479,    2, 0x0a /* Public */,
      28,    1,  482,    2, 0x0a /* Public */,
      29,    1,  485,    2, 0x0a /* Public */,
      30,    1,  488,    2, 0x0a /* Public */,
      31,    1,  491,    2, 0x0a /* Public */,
      32,    1,  494,    2, 0x0a /* Public */,
      33,    1,  497,    2, 0x0a /* Public */,
      34,    1,  500,    2, 0x0a /* Public */,
      35,    1,  503,    2, 0x0a /* Public */,
      36,    0,  506,    2, 0x0a /* Public */,
      37,    0,  507,    2, 0x0a /* Public */,
      38,    0,  508,    2, 0x0a /* Public */,
      39,    0,  509,    2, 0x0a /* Public */,
      40,    0,  510,    2, 0x0a /* Public */,
      41,    0,  511,    2, 0x0a /* Public */,
      42,    0,  512,    2, 0x0a /* Public */,
      43,    0,  513,    2, 0x0a /* Public */,
      44,    0,  514,    2, 0x0a /* Public */,
      45,    1,  515,    2, 0x0a /* Public */,
      46,    1,  518,    2, 0x0a /* Public */,
      47,    1,  521,    2, 0x0a /* Public */,
      48,    1,  524,    2, 0x0a /* Public */,
      49,    1,  527,    2, 0x0a /* Public */,
      50,    1,  530,    2, 0x0a /* Public */,
      51,    1,  533,    2, 0x0a /* Public */,
      52,    1,  536,    2, 0x0a /* Public */,
      53,    0,  539,    2, 0x0a /* Public */,
      54,    1,  540,    2, 0x0a /* Public */,
      55,    0,  543,    2, 0x0a /* Public */,
      56,    0,  544,    2, 0x0a /* Public */,
      57,    0,  545,    2, 0x0a /* Public */,
      58,    0,  546,    2, 0x0a /* Public */,
      59,    0,  547,    2, 0x0a /* Public */,
      60,    0,  548,    2, 0x0a /* Public */,
      61,    0,  549,    2, 0x0a /* Public */,
      62,    0,  550,    2, 0x0a /* Public */,
      63,    0,  551,    2, 0x0a /* Public */,
      64,    0,  552,    2, 0x0a /* Public */,
      65,    0,  553,    2, 0x0a /* Public */,
      66,    0,  554,    2, 0x0a /* Public */,
      67,    0,  555,    2, 0x0a /* Public */,
      68,    1,  556,    2, 0x0a /* Public */,
      69,    2,  559,    2, 0x0a /* Public */,
      71,    0,  564,    2, 0x0a /* Public */,
      72,    0,  565,    2, 0x0a /* Public */,
      73,    0,  566,    2, 0x0a /* Public */,
      74,    1,  567,    2, 0x0a /* Public */,
      76,    0,  570,    2, 0x0a /* Public */,
      77,    0,  571,    2, 0x0a /* Public */,
      78,    0,  572,    2, 0x0a /* Public */,
      79,    0,  573,    2, 0x0a /* Public */,
      80,    0,  574,    2, 0x0a /* Public */,
      81,    0,  575,    2, 0x0a /* Public */,
      82,    0,  576,    2, 0x0a /* Public */,
      83,    0,  577,    2, 0x0a /* Public */,
      84,    0,  578,    2, 0x0a /* Public */,
      85,    0,  579,    2, 0x0a /* Public */,
      86,    0,  580,    2, 0x0a /* Public */,
      87,    0,  581,    2, 0x0a /* Public */,
      88,    0,  582,    2, 0x0a /* Public */,
      89,    0,  583,    2, 0x0a /* Public */,
      90,    0,  584,    2, 0x0a /* Public */,
      91,    0,  585,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
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
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 70, QMetaType::Double,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   75,
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
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HexMeshViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HexMeshViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->leeUpdate(); break;
        case 1: _t->subdivision(); break;
        case 2: _t->average(); break;
        case 3: _t->openMeshFile(); break;
        case 4: _t->readXJ(); break;
        case 5: _t->combine(); break;
        case 6: _t->readSkel(); break;
        case 7: _t->selectCell(); break;
        case 8: _t->selectMutiCells(); break;
        case 9: _t->rconnectNodes(); break;
        case 10: _t->editCell(); break;
        case 11: _t->delCell(); break;
        case 12: _t->drawNormalSkel(); break;
        case 13: _t->reNewDataSignal(); break;
        case 14: _t->genCoarseMeshFinishMesh(); break;
        case 15: _t->addCellUseMouseSignal(); break;
        case 16: _t->addCellUseMouseImplSignal(); break;
        case 17: _t->exportSkel(); break;
        case 18: _t->blendSkel(); break;
        case 19: _t->rotate(); break;
        case 20: _t->rupdateGL(); break;
        case 21: _t->helpString(); break;
        case 22: _t->rBoom4(); break;
        case 23: _t->rdisconnectNodes(); break;
        case 24: _t->clearSkel(); break;
        case 25: _t->rxRotateCell((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->ryRotateCell((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->rzRotateCell((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->rupMoveCell((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->rdownMoveCell((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->rfrontMoveCell((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->rbackMoveCell((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->rleftMoveCell((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->rrightMoveCell((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->PIA(); break;
        case 35: _t->rselectALine(); break;
        case 36: _t->rupdateSelectMutiNodes(); break;
        case 37: _t->rsetxyView(); break;
        case 38: _t->rsetyzView(); break;
        case 39: _t->rsetxzView(); break;
        case 40: _t->rsetxy_View(); break;
        case 41: _t->rsetyz_View(); break;
        case 42: _t->rsetxz_View(); break;
        case 43: _t->rmirrorModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->rshowTriMeshChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 45: _t->rshowProjectLinesChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 46: _t->raxisShowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 47: _t->rgroundShowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 48: _t->rskelShowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 49: _t->rmirrorShowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 50: _t->rdragModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 51: _t->rnewNodeOK(); break;
        case 52: _t->rdrawModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 53: _t->faceNormChange(); break;
        case 54: _t->readOpenComplexSkel(); break;
        case 55: _t->simpleComplexSkel(); break;
        case 56: _t->simpleWithCgal(); break;
        case 57: _t->rspiltX(); break;
        case 58: _t->rspiltY(); break;
        case 59: _t->rspiltZ(); break;
        case 60: _t->rxExpand(); break;
        case 61: _t->ryExpand(); break;
        case 62: _t->rzExpand(); break;
        case 63: _t->rxCrop(); break;
        case 64: _t->ryCrop(); break;
        case 65: _t->rzCrop(); break;
        case 66: _t->rRotateByMouseChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 67: _t->rRotateCubeByMouse((*reinterpret_cast< V3f(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 68: _t->fromVolumeToOff(); break;
        case 69: _t->changeNodeType(); break;
        case 70: _t->scaledJacobian(); break;
        case 71: _t->rVertexSnapModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 72: _t->rMouseRelease(); break;
        case 73: _t->project(); break;
        case 74: _t->exportBroSkel(); break;
        case 75: _t->fixFlip(); break;
        case 76: _t->openOff(); break;
        case 77: _t->shrinking(); break;
        case 78: _t->showProjectLine(); break;
        case 79: _t->pickProjPoints(); break;
        case 80: _t->skelMove(); break;
        case 81: _t->splitComplexSkel(); break;
        case 82: _t->rreadTestLineSignal(); break;
        case 83: _t->rchangeToPolylines(); break;
        case 84: _t->rfittingCurvesSignal(); break;
        case 85: _t->ractRMFSignal(); break;
        case 86: _t->rshowSkel(); break;
        case 87: _t->rBoom5(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HexMeshViewer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HexMeshViewer::leeUpdate)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HexMeshViewer::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_HexMeshViewer.data,
    qt_meta_data_HexMeshViewer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HexMeshViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HexMeshViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HexMeshViewer.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int HexMeshViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 88)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 88;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 88)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 88;
    }
    return _id;
}

// SIGNAL 0
void HexMeshViewer::leeUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
