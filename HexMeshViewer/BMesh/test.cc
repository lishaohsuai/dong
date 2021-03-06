/*
 * @Author: your name
 * @Date: 2020-10-05 20:25:58
 * @LastEditTime: 2020-12-02 14:02:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/src/test.cc
 */
#include "test.hpp"
using namespace BMesh;
/**
 * @description: 对common类的一些简单的测试
 * @param {type}
 * @return {type}
 */
void Unitest_common::main_common() {
    /*
    Point3D a(3, 4, 0);
    Point3D o(0, 0, 0);
    // 参照答案 5
    std::cout << "[DEBUG] common distance " << common::distance(a, o) << std::endl;
    Point3D in(5, 5, 5);
    Point3D out;
    common::rotationThreeAngle(45, 0, 0, in, out);
    // 参照答案 (5, 0, 5*1.414)
    std::cout << "[DEBUG] common ratation " << out.x << " " << out.y << " " << out.z << std::endl;
    Point3D in1(5, 5, 5);
    Point3D out1;
    common::rotationThreeAngle(0, 0, 45, in1, out1);
    // 参照答案 (5, 0, 5*1.414)
    std::cout << "[DEBUG] common ratation - " << out1.x << " " << out1.y << " " << out1.z << std::endl;
    std::vector<Lvector> v;
    v.push_back(Lvector(Point3D(0, 0, 0), Point3D(1, 0, 0)));
    v.push_back(Lvector(Point3D(0, 0, 0), Point3D(0, 1, 0)));
    v.push_back(Lvector(Point3D(0, 0, 0), Point3D(0, 0, 1)));
    std::vector<double> abg;
    common::optimization(v, abg);
    // 参照答案 (0,0,0); 测试通过
    std::cout << "[DEBUG] common angle " << abg[0] << " " << abg[1] << " " << abg[2] << std::endl;
    v.clear();
    v.push_back(Lvector(Point3D(0, 0, 0), Point3D(1, 0, 0)));
    v.push_back(Lvector(Point3D(0, 0, 0), Point3D(1, 1, 0)));
    v.push_back(Lvector(Point3D(0, 0, 0), Point3D(1, 1, 1)));
    common::optimization(v, abg);
    // 参照答案 (0,0,45) 可能有错误(好烦哟~)
    std::cout << "[DEBUG] common angle " << abg[0] << " " << abg[1] << " " << abg[2] << std::endl;
    v.clear();
    v.push_back(Lvector(Point3D(0, 0, 0), Point3D(1, 0, 0)));
    v.push_back(Lvector(Point3D(0, 0, 0), Point3D(1, 1, 0)));
    common::optimization(v, abg);
    // 参照答案 (0,0,45) 可能有错误(好烦哟~)
    std::cout << "[DEBUG] common angle " << abg[0] << " " << abg[1] << " " << abg[2] << std::endl;
    */
}

/**
 * @description: 查看读取的骨架信息是否正确
 * @param {type}
 * @return {type}
 */
void Unitest_common::main_genNodeHex(std::string filename) {
    Solution s;
    s.readSkel(filename);
    // 遍历graph
    // 校验每个节点的id和他的孩子的id 应该和文件一样
    for (int i = 0; i < s.v.size(); i++) {
        DBG_D("my id is %d my son id is", s.v[i]->id);
        for (int j = 0; j < s.v[i]->children.size(); j++) {
            DBG_S(" ");
            DBG_S(s.v[i]->children[j]->id);
        }
        DBG_S("\n");
    }
    s.genGraph();
    s.genNodeHex();
    s.genIntersectionPointInHex();
    s.genlinkPoints();
    // 遍历graph
    // 校验每个节点的id和他的孩子的id 应该和文件一样
    // for (int i = 0; i < s.v.size(); i++) {
    //     std::cout << "my id is " << s.v[i]->id << " my son id is ";
    //     for (int j = 0; j < s.v[i]->children.size(); j++) {
    //         std::cout << " " << s.v[i]->children[j]->id;
    //     }
    //     std::cout << std::endl;
    // }
    s.checkNodeFaceExistBigFaceSmallFace();
    s.deleteNodeTraceAllNode();
    for (int i = 0; i < s.v.size(); i++) {
        DBG_D("my id is %d my son id is", s.v[i]->id);
        for (int j = 0; j < s.v[i]->children.size(); j++) {
            DBG_S(" ");
            DBG_S(s.v[i]->children[j]->id);
        }
        DBG_S("\n");
    }
    s.writeMesh("mesh/fourpoints_2.mesh");
    // s.writeVtk("vtk/fourpoints_2.vtk");
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
void Unitest_common::main_point() {
    Point3D a(1, 1, 1);
    // 参考 1 1 1
    std::cout << a.getXYZ();
    Point3D b(2, 2, 2);
    Point3D c;
    c = a - b;
    // -1 -1 -1
    std::cout << c.getXYZ();
    Point3D d = a + b;
    // 3 3 3
    std::cout << d.getXYZ();
    d = a;
    // 1 1 1
    std::cout << d.getXYZ();
    d = Point3D(5, 5, 5);
    // 5 5 5
    std::cout << d.getXYZ();
    Point3D e(d);
    // 5 5 5
    std::cout << e.getXYZ();
    // 1 1 1
    std::cout << "[DEBUG -] " << sub_point().getXYZ();
    // 1 1 1
    std::cout << "[DEBUG --] " << sub1_point().getXYZ();
}

Point3D Unitest_common::sub_point() { return Point3D(1, 1, 1); }

Point3D Unitest_common::sub1_point() {
    Point3D c(1, 1, 1);
    return c;
}

/**
 * @description: 切分面
 * @param {type}
 * @return {type}
 */
void Unitest_common::main_splitFace() {
    Face f(Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(1, 1, 0), Point3D(0, 1, 0));
    std::vector<Point3D> p;
    std::vector<std::pair<Point3D, Face>> out;
    // 参考输出 0
    // std::cout << "[DEBUG] " << Geometry::splitFaceUseIntersectionPoints(f, p, out) << std::endl;
    // p.push_back(Point3D(0.2, 0.2, 0));
    // // 参考输出 0
    // std::cout << "[DEBUG] " << Geometry::splitFaceUseIntersectionPoints(f, p, out) << std::endl;
    p.push_back(Point3D(0.2, 0.2, 0));
    p.push_back(Point3D(0.8, 0.2, 0));
    p.push_back(Point3D(0.2, 0.8, 0));
    p.push_back(Point3D(0.8, 0.8, 0));
    // 参考输出 1
    std::cout << "[DEBUG 1] " << Geometry::splitFaceUseIntersectionPoints(f, p, out) << std::endl;
    /*
    * 参考输出 更改函数
        [DEBUG 1] 1
        [DEBUG]
        0.500000 1.000000 0.000000
        0.000000 1.000000 0.000000
        0.000000 0.500000 0.000000
        0.500000 0.500000 0.000000
        [DEBUG]
        0.500000 0.500000 0.000000
        0.000000 0.500000 0.000000
        0.000000 0.000000 0.000000
        0.500000 0.000000 0.000000
        [DEBUG]
        1.000000 1.000000 0.000000
        0.500000 1.000000 0.000000
        0.500000 0.500000 0.000000
        1.000000 0.500000 0.000000
        [DEBUG]
        1.000000 0.500000 0.000000
        0.500000 0.500000 0.000000
        0.500000 0.000000 0.000000
        1.000000 0.000000 0.000000
    */
    for (int i = 0; i < out.size(); i++) {
        std::cout << "[DEBUG] \n" << out[i].second.getPointsString();
    }
    // p.clear();
    // p.push_back(Point3D(0.2, 0.2, 0));
    // p.push_back(Point3D(0.4, 0.2, 0));
    // p.push_back(Point3D(0.6, 0.3, 0));
    // std::cout << "[DEBUG] " << Geometry::splitFaceUseIntersectionPoints(f, p, out) << std::endl;
    /*
    [DEBUG]
    1.000000 0.000000 0.000000
    1.000000 1.000000 0.000000
    0.500000 1.000000 0.000000
    0.500000 0.000000 0.000000
    [DEBUG]
    0.500000 0.000000 0.000000
    0.500000 1.000000 0.000000
    0.300000 1.000000 0.000000
    0.300000 0.000000 0.000000
    [DEBUG]
    0.300000 0.000000 0.000000
    0.300000 1.000000 0.000000
    0.000000 1.000000 0.000000
    0.000000 0.000000 0.000000
    */
    // for (int i = 0; i < out.size(); i++) {
    //     std::cout << "[DEBUG] \n" << out[i].second.getPointsString();
    // }
}

/**
 * @description: 线面是否有交点
 * @param {type}
 * @return {type}
 */

void Unitest_common::main_intersection() {
    Line l(Point3D(1, 1, 1), Point3D(0, 0, 0));
    Face f(Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(1, 1, 0), Point3D(0, 1, 0));
    Point3D p;
    // 1  0,0,0
    std::cout << "[DEBUG] intersection " << Geometry::intersectionLine3DFace3D(l, f, p) << " " << p.getXYZ();
    l = Line(Point3D(1, 1, 1), Point3D(0.5, 0.5, 0.5));
    // 0  0,0,0
    std::cout << "[DEBUG] intersection " << Geometry::intersectionLine3DFace3D(l, f, p) << " " << p.getXYZ();
}

void Unitest_common::main_test() {
    double a = -0.0000;
    double b = 0.0000;
    // yes
    if (a == b) {
        std::cout << "yes" << std::endl;
    }
    std::unordered_map<std::string, int> m;
    m["str"] = 1;
    std::cout << m["str"] << " " << m["dd"] << std::endl;
    int aa = 0;
    int dd = 0;
    int bb = ++aa;
    int cc = dd++;
    // 1 0
    std::cout << bb << " " << cc;
    // 测试 空是否返回true
    std::vector<int> v;
    if (v.empty()) {
        std::cout << "[DEBUG] vector is empty!!!" << v.empty() << std::endl;
    }
    int a1 = 100;
    int b1 = 200;
    std::swap(a1, b1);
    // 200 100
    std::cout << "[DEBUG] a1 b1 " << a1 << " " << b1 << std::endl;
}

/**
 * @description: 测试node类
 * @param {type}
 * @return {type}
 */
void Unitest_common::main_testNode() {
    /*
    Node *a[100];
    a[0] = new Node(1, 1, 1, 1, BN);
    a[1] = new Node(2, 2, 2, 2, BN);
    a[2] = new Node(3, 3, 3, 3, BN);
    a[0]->addChild(a[1]);
    a[0]->addChild(a[2]);
    for (int i = 0; i < 3; i++) {
        std::cout << "[DEBUG] " << a[i]->id << std::endl;
    }
    for (int i = 0; i < a[0]->children.size(); i++) {
        std::cout << "[DEBUG] a[0]'s children " << a[0]->children[i]->id << std::endl;
    }
    if (a[0]->deleteChild(a[1])) {
        std::cout << "[DEBUG] 删除节点成功" << std::endl;
    }
    if (a[0]->deleteChild(a[2])) {
        std::cout << "[DEBUG] 删除节点成功" << std::endl;
    }
    if (a[0]->deleteChild(a[2])) {
        std::cout << "[DEBUG] 删除节点成功" << std::endl;
    } else {
        std::cout << "[DEBUG] 删除节点失败" << std::endl;
    }
    for (int i = 0; i < a[0]->children.size(); i++) {
        std::cout << "[DEBUG] a[0]'s children " << a[0]->children[i]->id << std::endl;
    }
    */
}

/**
 * @description: 对面的判断
 * @param {type}
 * @return {type}
 */
void Unitest_common::main_testFace() {
    Face a(Point3D(0, 0, 0), Point3D(0, 1, 0), Point3D(1, 1, 0), Point3D(1, 0, 0));
    Face b(Point3D(0, 0, 0), Point3D(0, 1, 0), Point3D(1, 1, 0), Point3D(1, 0, 0));
    // equal
    if (a == b) {
        std::cout << "[DEBUG] equal" << std::endl;
    } else {
        std::cout << "[DEBUG] not equal" << std::endl;
    }
}

/**
 * @description:  探测面的包含问题
 * @param {type}
 * @return {type}
 */
void Unitest_common::main_testFaceContain() {
    Face a(Point3D(0, 0, 0), Point3D(0, 1, 0), Point3D(1, 1, 0), Point3D(1, 0, 0));
    Face b(Point3D(0, 0.5, 0), Point3D(1, 0.5, 0), Point3D(1, 1, 0), Point3D(0, 1, 0));
    Face d(Point3D(0, 1, 0), Point3D(1, 1, 0), Point3D(1, 2, 0), Point3D(0, 2, 0));
    std::vector<Point3D> c;

    // std::cout << "[DEBUG] \n" << (Geometry::isAFaceContainBFace(a, b, c)?"true":"false") << "\n" <<c[0].getXYZ() <<
    // c[1].getXYZ()
    //           << std::endl;
    std::cout << "[DEBUG] \n"
              << (Geometry::isAFaceContainBFace(a, d, c) == 1 ? "true" : "false") << "\n"
              << c[0].getXYZ() << c[1].getXYZ() << std::endl;
}

/**
 * @description: 六面体分裂
 * @param {type}
 * @return {type}
 */
void Unitest_common::main_hexFaceSplit() {
    /*
    Hex a = Hex(Point3D(0, 0, 0), 1);
    Hex b = Hex(Point3D(0, 0, 0), 1);
    std::vector<Point3D> diffPoint;
    diffPoint.push_back(Point3D(1, -1, 0));
    diffPoint.push_back(Point3D(1, 1, 0));
    std::vector<Point3D> samePoint;
    samePoint.push_back(Point3D(1, -1, -1));
    samePoint.push_back(Point3D(1, 1, -1));
    if (Geometry::splitHexUseALine(a, b, diffPoint, samePoint, 4)) {
        std::cout << "[DEBUG] can split" << std::endl;
    } else {
        std::cout << "[DEBUG] can't split" << std::endl;
    }
    for (int i = 0; i < 8; i++) {
        std::cout << a.p[i].getXYZ();
    }
    std::cout << "---\n";
    for (int i = 0; i < 8; i++) {
        std::cout << b.p[i].getXYZ();
    }
    */
}

/**
 * @description: 交错情况
 * @param {type}
 * @return {type}
 */
void Unitest_common::main_interlaceFace() {
    Face a(Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(1, 1, 0), Point3D(0, 1, 0));
    Face b(Point3D(0, 1, 0), Point3D(0.5, 1, 0), Point3D(0.5, -1, 0), Point3D(0, -1, 0));
    Face c(Point3D(0, 1, 0), Point3D(0, 0, 0), Point3D(-1, 0, 0), Point3D(-1, 1, 0));
    // 标准答案 [DEBUG] a interlace b
    if (Geometry::isFacetoFace(a, b)) {
        std::cout << "[DEBUG] a interlace b" << std::endl;
    } else {
        std::cout << "[DEBUG] a not interlace b" << std::endl;
    }
    // 标准答案 [DEBUG] a interlace b
    if (Geometry::isFacetoFace(b, a)) {
        std::cout << "[DEBUG] a interlace b" << std::endl;
    } else {
        std::cout << "[DEBUG] a not interlace b" << std::endl;
    }
    // 标准答案 [DEBUG] a not interlace c
    if (Geometry::isFacetoFace(a, c)) {
        std::cout << "[DEBUG] a interlace c" << std::endl;
    } else {
        std::cout << "[DEBUG] a not interlace c" << std::endl;
    }
    // 标准答案 [DEBUG] a not interlace c
    if (Geometry::isFacetoFace(c, a)) {
        std::cout << "[DEBUG] a interlace c" << std::endl;
    } else {
        std::cout << "[DEBUG] a not interlace c" << std::endl;
    }
    // 标准答案 [DEBUG] b not interlace c
    if (Geometry::isFacetoFace(c, b)) {
        std::cout << "[DEBUG] b interlace c" << std::endl;
    } else {
        std::cout << "[DEBUG] b not interlace c" << std::endl;
    }
    // 标准答案 [DEBUG] b not interlace c
    if (Geometry::isFacetoFace(b, c)) {
        std::cout << "[DEBUG] b interlace c" << std::endl;
    } else {
        std::cout << "[DEBUG] b not interlace c" << std::endl;
    }
}

/**
 * @description: 体交错测试
 * @param {*}
 * @return {*}
 */
void Unitest_common::main_interlaceHex() {
    Point3D v11[8] = {Point3D(0, 0, 1), Point3D(1, 0, 1), Point3D(1, 1, 1), Point3D(0, 1, 1),
                      Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(1, 1, 0), Point3D(0, 1, 0)};
    std::vector<Point3D> v12(v11, v11 + 8);
    Hex a(v12);
    Point3D v21[8] = {Point3D(0, 1, 0),  Point3D(0, -1, 0),  Point3D(0.5, -1, 0),  Point3D(0.5, 1, 0),
                      Point3D(0, 1, -1), Point3D(0, -1, -1), Point3D(0.5, -1, -1), Point3D(0.5, 1, -1)};
    std::vector<Point3D> v22(v21, v21 + 8);
    Hex b(v22);
    Point3D v31[8] = {Point3D(0, 0, 1), Point3D(0, 1, 1), Point3D(-1, 1, 1), Point3D(-1, 0, 1),
                      Point3D(0, 0, 0), Point3D(0, 1, 0), Point3D(-1, 1, 0), Point3D(-1, 0, 0)};
    std::vector<Point3D> v32(v31, v31 + 8);
    Hex c(v32);
    std::vector<Point3D> differentPoint;
    std::vector<Point3D> samePoint;
    int ai;
    int bi;
    // 标准答案 6
    // std::cout << "[DEBUG] hex return id " << Geometry::isAHexFaceContainBHexFace(&a, &b, differentPoint, samePoint,
    // ai, bi) << std::endl;
    // 标准答案 2
    // std::cout << "[DEBUG] hex return id " << Geometry::isAHexFaceContainBHexFace(&a, &c, differentPoint, samePoint,
    // ai, bi) << std::endl;
    // 标准答案 3
    // std::cout << "[DEBUG] hex return id " << Geometry::isAHexFaceContainBHexFace(&b, &c, differentPoint, samePoint,
    // ai, bi) << std::endl;
    // 开始分裂测试 将标准答案 2 3 注释掉
    // Hex *newHex = new Hex();
    // std::cout << "[DEBUG] 不同点显示" << std::endl;
    // for(int i=0; i<differentPoint.size(); i++){
    //     std::cout << differentPoint[i].getXYZ();
    // }
    // std::cout << "[DEBUG] 相同点显示" << std::endl;
    // for(int i=0; i<samePoint.size(); i++){
    //     std::cout << samePoint[i].getXYZ();
    // }

    // // 输出顶点在 show.py 中校验通过
    // if(Geometry::splitHexUseALine(b, *newHex, differentPoint, samePoint, ai, bi)){
    //     std::cout << "[DEBUG] 新生成的8个顶点" << std::endl;
    //     for(int i=0; i<8; i++){
    //         std::cout << newHex->p[i].getXYZ();
    //     }
    //     std::cout << "===b===" << std::endl;
    //     for(int i=0; i<8; i++){
    //         std::cout << b.p[i].getXYZ();
    //     }
    // }
    // else{
    //     std::cout << "[DEBUG] can't be splited!\n";
    // }
    /*
    std::cout << "[DEBUG] hex return id "
              << Geometry::isAHexFaceContainBHexFace(&b, &a, differentPoint, samePoint, ai, bi) << std::endl;
    Hex *newHex = new Hex();
    std::cout << "[DEBUG] 不同点显示" << std::endl;
    for (int i = 0; i < differentPoint.size(); i++) {
        std::cout << differentPoint[i].getXYZ();
    }
    std::cout << "[DEBUG] 相同点显示" << std::endl;
    for (int i = 0; i < samePoint.size(); i++) {
        std::cout << samePoint[i].getXYZ();
    }

    // 输出顶点在 show.py 中校验通过
    if (Geometry::splitHexUseALine(a, *newHex, differentPoint, samePoint, ai, bi)) {
        std::cout << "[DEBUG] 新生成的8个顶点" << std::endl;
        for (int i = 0; i < 8; i++) {
            std::cout << newHex->p[i].getXYZ();
        }
        std::cout << "===a===" << std::endl;
        for (int i = 0; i < 8; i++) {
            std::cout << a.p[i].getXYZ();
        }
    } else {
        std::cout << "[DEBUG] can't be splited!\n";
    }
    */
}

/**
 * @description: printf 测试
 * @param {*}
 * @return {*}
 */
void Unitest_common::main_printf() {
    printf("hello\n");
    printf("hello %d\n", 1);
    int x = 2;
    printf("hello"
           " world%d\n",
           x);
    DBG_D("hello %d", 1);
}