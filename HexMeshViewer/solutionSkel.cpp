#include "solutionSkel.h"
#include "singalFather.h"
SolutionSkel::SolutionSkel() :QObject(nullptr) {
	solutionOldBMesh = new BMesh::Solution();
	mirrorNode = NULL;
	selectNode = NULL;
	step = 1;
	state = haveNotGenJN;
	bdragMode = false;
	bRotateByMouse = false;
	bVertexSnap = false;
	bmirrorMode = false;
}

/**
 * @description: ��������
 * @param {type}
 * @return {type}
 */
SolutionSkel::~SolutionSkel() {}

/**
 * @description: ��ȡ�Ǽܽṹ
 * @param {type}
 * @return {type}
 */
bool SolutionSkel::readSkel(std::string filename) {
	bool check = solutionOldBMesh->readSkel(filename);
	if (check) {
		return true;
	}
	return false;
}

/**
 * @description: gen node Hex
 * @param {type}
 * @return {type}
 */
bool SolutionSkel::genNodeHex() {
	bool check = solutionOldBMesh->genNodeHex();
	if (check) {
		//solutionOldBMesh->deleteNodeTraceAllNode();
		return true;
	}
	return false;
}

/**
 * @description: gen view Hex
 * @param {type}
 * @return {type}
 */
void SolutionSkel::genViewHex(HexV3fMesh &mesh) {
	// add vertex and add cell
	for (int i = 0; i < solutionOldBMesh->v.size(); i++) {
		vector<VertexHandle> temp_vetex;
		for (int j = 0; j < 8; j++) {
			V3f v = V3f(0.00, 0.00, 0.00);
			v.x = floor(solutionOldBMesh->v[i]->root->p[j].x * 1000 + 0.5) * 1.0 / 1000.0;
			v.y = floor(solutionOldBMesh->v[i]->root->p[j].y * 1000 + 0.5) * 1.0 / 1000.0;
			v.z = floor(solutionOldBMesh->v[i]->root->p[j].z * 1000 + 0.5) * 1.0 / 1000.0;
			
			temp_vetex.push_back(mesh.add_vertex(v));
		}
		std::cout << "===" << std::endl;
		mesh.add_cell(temp_vetex);
		temp_vetex.clear();
	}
	std::cout << "[DEBUG] solutionOldBMesh vNum " << solutionOldBMesh->v.size() << std::endl;
}

/**
 * @description: get root
 * @param {type}
 * @return {type}
 */
BMesh::Node * SolutionSkel::getRoot() {
	return solutionOldBMesh->v[0];
}

/**
 * @description: get node vec
 * @param {type}
 * @return {type}
 */
vector<BMesh::Node*> SolutionSkel::getNodeVec() {
	return solutionOldBMesh->v;
}

/**
 * @description: get root
 * @param {type}
 * @return {type}
 */
BMesh::Node *SolutionSkel::selectWhichNode(V3f &p) {
	BMesh::Point3D a(p.x, p.y, p.z);
	selectNode = solutionOldBMesh->selectWhichNode(a);
	return selectNode;
}

/**
 * @description: connect a and b
 * @param {type}
 * @return {type}
 */
bool SolutionSkel::connectNodes(BMesh::Node* a, BMesh::Node* b) {
	bool rlt = solutionOldBMesh->connectNodes(a, b);
	return rlt;
}

bool SolutionSkel::disconnectNodes(BMesh::Node* a, BMesh::Node* b) {
	bool rlt = solutionOldBMesh->disconnectNodes(a, b);
	return rlt;
}

/**
 * @description: get root
 * @param {type}
 * @return {type}
 */
void SolutionSkel::reditNode(double x, double y, double z, double angleX, double angleY, double angleZ, double length, double width, double height) {
	BMesh::Point3D p(x, y, z);
	std::vector<double> lwh;
	lwh.emplace_back(length);
	lwh.emplace_back(width);
	lwh.emplace_back(height);
	solutionOldBMesh->editNode(selectNode, p, lwh);
	solutionOldBMesh->editNodeAngle(selectNode, angleX, angleY, angleZ);
	SingalFather::getInstance()->reDrawCube();
}

/**
 * @description: edit cube angle
 * @param direction 0 x, 1 y, 2 z
 * @param plus 1 +, -1 -
 * @return {type}
 */
void SolutionSkel::rotateCubeByMouse(V3f axis, double angle) {
	if(state == haveGenJN){
		clearJNCube();
	}
	if(selectNode == NULL) {
		return;
	}
	if(selectNodes.size() > 1) {
		// calc Bounding box center
		BMesh::Point3D p(0, 0, 0);
		for(int i=0; i<selectNodes.size(); i++) {
			p += selectNodes[i]->getPosition();
		}
		p /= selectNodes.size();
		for(int i=0; i<selectNodes.size(); i++) {
			selectNode = selectNodes[i];
			glm::vec3 axiss;
			axiss.x = axis.x;
			axiss.y = axis.y;
			axiss.z = axis.z;
			glm::quat inq = inAngleAxis_(axiss, angle);
			glm::vec3 ou = eulerAngles(inq);
			solutionOldBMesh->editNodeAngleByQuat(selectNode, inq);
		}
	} else {
		glm::vec3 axiss;
		axiss.x = axis.x;
		axiss.y = axis.y;
		axiss.z = axis.z;
		glm::quat inq = inAngleAxis_(axiss, angle);
		glm::vec3 ou = eulerAngles(inq);
		solutionOldBMesh->editNodeAngleByQuat(selectNode, inq);
		//solutionOldBMesh->editNodeAngle(selectNode, ou.x * 180.0 / M_PI, 
				//ou.y * 180.0 / M_PI, ou.z * 180.0 / M_PI);
	}
}

bool SolutionSkel::changeNodeType() {
	for (auto it : selectNodes) {
		it->setNodeType(BMesh::BN);
	}
	return true;
}


glm::quat SolutionSkel::inAngleAxis_(glm::vec3 RotationAxis, double RotationAngle) {
	RotationAngle = RotationAngle * DEGTORAD;
	RotationAxis = normalize(RotationAxis);
	glm::quat t;
	t.x = RotationAxis.x * sin(RotationAngle / 2);
	t.y = RotationAxis.y * sin(RotationAngle / 2);
	t.z = RotationAxis.z * sin(RotationAngle / 2);
	t.w = cos(RotationAngle / 2);
	return t;
}

void SolutionSkel::rotateCube(int direction, int plus){
	if(state == haveGenJN){
		clearJNCube();
	}
	if(selectNode == NULL) {
		return;
	}
	if(selectNodes.size() > 1) {
		// calc Bounding box center
		BMesh::Point3D p(0, 0, 0);
		for(int i=0; i<selectNodes.size(); i++) {
			p += selectNodes[i]->getPosition();
		}
		p /= selectNodes.size();
		for(int i=0; i<selectNodes.size(); i++) {
			selectNode = selectNodes[i];
			if(direction == 0) {
				if(plus == 1) {
					solutionOldBMesh->editNodeAngle(selectNode, step, 0, 0, p);
				}else if(plus == -1){
					solutionOldBMesh->editNodeAngle(selectNode, -step, 0, 0, p);
				}
			}else if(direction == 1) {
				if(plus == 1) {
					solutionOldBMesh->editNodeAngle(selectNode, 0, step, 0, p);
				}else if(plus == -1) {
					solutionOldBMesh->editNodeAngle(selectNode, 0, -step, 0, p);
				}
			}else if(direction == 2) {
				if(plus == 1) {
					solutionOldBMesh->editNodeAngle(selectNode, 0, 0, step, p);
				}else if(plus == -1){
					solutionOldBMesh->editNodeAngle(selectNode, 0, 0, -step, p);
				}
			}
		}
	} else {
		if(direction == 0) {
			if(plus == 1) {
				solutionOldBMesh->editNodeAngle(selectNode, step, 0, 0);
			}else if(plus == -1){
				solutionOldBMesh->editNodeAngle(selectNode, -step, 0, 0);
			}
		}else if(direction == 1) {
			if(plus == 1) {
				solutionOldBMesh->editNodeAngle(selectNode, 0, step, 0);
			}else if(plus == -1) {
				solutionOldBMesh->editNodeAngle(selectNode, 0, -step, 0);
			}
		}else if(direction == 2) {
			if(plus == 1) {
				solutionOldBMesh->editNodeAngle(selectNode, 0, 0, step);
			}else if(plus == -1){
				solutionOldBMesh->editNodeAngle(selectNode, 0, 0, -step);
			}
		}
	}
}

/**
 * @description: moveCube
 * @param {*}
 * @return {*}
 */
void SolutionSkel::moveCube(int direction, int plus){
	if(state == haveGenJN){
		clearJNCube();
	}
	if(selectNode == NULL) {
		return;
	}
	if(selectNodes.size() > 1) {
		for(int i=0; i<selectNodes.size(); i++) {
			selectNode = selectNodes[i];
			if(direction == 0) {
				if(plus == 1) {
					BMesh::Point3D p = selectNode->getPosition();
					p += BMesh::Point3D(step, 0, 0);
					solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
				}else if(plus == -1){
					BMesh::Point3D p = selectNode->getPosition();
					p += BMesh::Point3D(-step, 0, 0);
					solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
				}
			}else if(direction == 1) {
				if(plus == 1) {
					BMesh::Point3D p = selectNode->getPosition();
					p += BMesh::Point3D(0, step, 0);
					solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
				}else if(plus == -1){
					BMesh::Point3D p = selectNode->getPosition();
					p += BMesh::Point3D(0, -step, 0);
					solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
				}
			}else if(direction == 2) {
				if(plus == 1) {
					BMesh::Point3D p = selectNode->getPosition();
					p += BMesh::Point3D(0, 0, step);
					solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
				}else if(plus == -1){
					BMesh::Point3D p = selectNode->getPosition();
					p += BMesh::Point3D(0, 0, -step);
					solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
				}
			}
		}		
	}
	else {
		if(direction == 0) {
			if(plus == 1) {
				BMesh::Point3D p = selectNode->getPosition();
				p += BMesh::Point3D(step, 0, 0);
				solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
			}else if(plus == -1){
				BMesh::Point3D p = selectNode->getPosition();
				p += BMesh::Point3D(-step, 0, 0);
				solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
			}
		}else if(direction == 1) {
			if(plus == 1) {
				BMesh::Point3D p = selectNode->getPosition();
				p += BMesh::Point3D(0, step, 0);
				solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
			}else if(plus == -1){
				BMesh::Point3D p = selectNode->getPosition();
				p += BMesh::Point3D(0, -step, 0);
				solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
			}
		}else if(direction == 2) {
			if(plus == 1) {
				BMesh::Point3D p = selectNode->getPosition();
				p += BMesh::Point3D(0, 0, step);
				solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
			}else if(plus == -1){
				BMesh::Point3D p = selectNode->getPosition();
				p += BMesh::Point3D(0, 0, -step);
				solutionOldBMesh->editNode(selectNode, p, selectNode->getLengthWidthHeight());
			}
		}
	}
}

/**
 * @description: Split a node uniformly along the x-axis direction
 * @param {Node} *a
 * @return {*}
 */
void SolutionSkel::spiltX(){
	if(selectNodes.size() > 1) {
		int size = selectNodes.size();
		for(int i=0; i<size; i++) {
			selectNodes.push_back(solutionOldBMesh->spiltX(selectNodes[i]));
		}
		for (int i = 0; i < selectNodes.size(); i++) {
			solutionOldBMesh->deleteNodeTopology(selectNodes[i]);
		}
		std::cout << "tmp\n";
	}else {
		if(selectNode != NULL) {
			selectNodes.clear();
			selectNodes.push_back(solutionOldBMesh->spiltX(selectNode));
			selectNodes.push_back(selectNode);
		}
	}
}
void SolutionSkel::spiltY(){
	if(selectNodes.size() > 1) {
		int size = selectNodes.size();
		for(int i=0; i<size; i++) {
			selectNodes.push_back(solutionOldBMesh->spiltY(selectNodes[i]));
		}
		for (int i = 0; i < selectNodes.size(); i++) {
			solutionOldBMesh->deleteNodeTopology(selectNodes[i]);
		}
		std::cout << "tmp\n";
	}else {
		if(selectNode != NULL) {
			selectNodes.clear();
			selectNodes.push_back(solutionOldBMesh->spiltY(selectNode));
			selectNodes.push_back(selectNode);
		}
	}
}
void SolutionSkel::spiltZ(){
	if(selectNodes.size() > 1) {
		int size = selectNodes.size();
		for(int i=0; i<size; i++) {
			selectNodes.push_back(solutionOldBMesh->spiltZ(selectNodes[i]));
		}
		for (int i = 0; i < selectNodes.size(); i++) {
			solutionOldBMesh->deleteNodeTopology(selectNodes[i]);
		}
	}else {
		if(selectNode != NULL) {
			selectNodes.clear();
			selectNodes.push_back(solutionOldBMesh->spiltZ(selectNode));
			selectNodes.push_back(selectNode);
		}
	}
}

/**
 * @description: refineJN
 * @param {type}
 * @return {type}
 */
void SolutionSkel::refineJN(){
	solutionOldBMesh->refineJN();
	SingalFather::getInstance()->reDrawCube();
}

/**
 * @description: rscaleNode
 * @param {type}
 * @return {type}
 */
void SolutionSkel::rscaleNode(double deltaR, int type){
	if (state == haveGenJN) {
		clearJNCube();
	}
	if (type != -1) {
		BMesh::Point3D mirrorP;
		bool find = false;
		double x = selectNode->getPosition().x;
		double y = selectNode->getPosition().y;
		double z = selectNode->getPosition().z;
		for (int i = 0; i < selectNode->children.size(); i++) {
			BMesh::Node* xMirrorNodeFather = selectNode->children[i];
			if (find) {
				break;
			}
			for (int j = 0; j < xMirrorNodeFather->children.size(); j++) {
				BMesh::Node* xMirrorNode = xMirrorNodeFather->children[j];
				BMesh::Point3D a = xMirrorNode->getPosition();
				if (type == 0) {
					BMesh::Point3D ax(xMirrorNodeFather->getPosition().x - a.x, a.y, a.z);
					if (abs(xMirrorNodeFather->getPosition().x - a.x + xMirrorNodeFather->getPosition().x - selectNode->getPosition().x) < 1e-7 &&
						abs(xMirrorNode->getPosition().y - selectNode->getPosition().y) < 1e-7 &&
						abs(xMirrorNode->getPosition().z - selectNode->getPosition().z) < 1e-7) {
						find = true;
						mirrorNode = xMirrorNode;
						mirrorP = BMesh::Point3D(xMirrorNodeFather->getPosition().x - (x - xMirrorNodeFather->getPosition().x), y, z);
						//solutionOldBMesh->editNode(mirrorNode, mirrorP, r);
						break;
					}
				}
				else if (type == 1) {
					BMesh::Point3D ax(a.x, xMirrorNodeFather->getPosition().y - a.y, a.z);
					if (abs(xMirrorNode->getPosition().x - selectNode->getPosition().x) < 1e-7 &&
						abs(xMirrorNodeFather->getPosition().y - a.y + xMirrorNodeFather->getPosition().y - selectNode->getPosition().y) < 1e-7 &&
						abs(xMirrorNode->getPosition().z - selectNode->getPosition().z) < 1e-7) {
						find = true;
						mirrorNode = xMirrorNode;
						mirrorP = BMesh::Point3D(x, xMirrorNodeFather->getPosition().y - (y - xMirrorNodeFather->getPosition().y), z);
						//solutionOldBMesh->editNode(mirrorNode, mirrorP, r);
						break;
					}
				}
				else if (type == 2) {
					BMesh::Point3D ax(a.x, a.y, xMirrorNodeFather->getPosition().z - a.z);
					if (abs(xMirrorNode->getPosition().x - selectNode->getPosition().x) < 1e-7 &&
						abs(xMirrorNode->getPosition().y - selectNode->getPosition().y) < 1e-7 &&
						abs(xMirrorNodeFather->getPosition().z - a.z + xMirrorNodeFather->getPosition().z - selectNode->getPosition().z) < 1e-7) {
						find = true;
						mirrorNode = xMirrorNode;
						mirrorP = BMesh::Point3D(x, y, xMirrorNodeFather->getPosition().z - (z - xMirrorNodeFather->getPosition().z));
						//solutionOldBMesh->editNode(mirrorNode, mirrorP, r);
						break;
					}
				}
				else if (type == 3) {
					BMesh::Point3D ax(xMirrorNodeFather->getPosition().x - a.x,
						xMirrorNodeFather->getPosition().y - a.y,
						xMirrorNodeFather->getPosition().z - a.z);
					if (abs(xMirrorNodeFather->getPosition().x - a.x + xMirrorNodeFather->getPosition().x - selectNode->getPosition().x) < 1e-7 &&
						abs(xMirrorNodeFather->getPosition().y - a.y + xMirrorNodeFather->getPosition().y - selectNode->getPosition().y) < 1e-7 &&
						abs(xMirrorNodeFather->getPosition().z - a.z + xMirrorNodeFather->getPosition().z - selectNode->getPosition().z) < 1e-7) {
						find = true;
						mirrorNode = xMirrorNode;
						mirrorP = BMesh::Point3D(xMirrorNodeFather->getPosition().x - (x - xMirrorNodeFather->getPosition().x),
							xMirrorNodeFather->getPosition().y - (y - xMirrorNodeFather->getPosition().y),
							xMirrorNodeFather->getPosition().z - (z - xMirrorNodeFather->getPosition().z));
						//solutionOldBMesh->editNode(mirrorNode, mirrorP, r);
						break;
					}
				}
			}
		}
		if (selectNode != NULL) {
			BMesh::Point3D p = selectNode->getPosition();
			std::vector<double> ss = selectNode->getLengthWidthHeight();
			ss[0] = ss[0] * deltaR; ss[1] = ss[1] * deltaR; ss[2] = ss[2] * deltaR;
			solutionOldBMesh->editNode(selectNode, p, ss);
		}
		if (find && mirrorNode != NULL) {
			BMesh::Point3D p = mirrorNode->getPosition();
			std::vector<double> ss = selectNode->getLengthWidthHeight();
			ss[0] = ss[0] * deltaR; ss[1] = ss[1] * deltaR; ss[2] = ss[2] * deltaR;
			solutionOldBMesh->editNode(mirrorNode, p, ss);
		}
		
	} else {
		if (selectNode != NULL) {
			BMesh::Point3D p = selectNode->getPosition();
			std::vector<double> ss = selectNode->getLengthWidthHeight();
			ss[0] = ss[0] * deltaR; ss[1] = ss[1] * deltaR; ss[2] = ss[2] * deltaR;
			solutionOldBMesh->editNode(selectNode, p, ss);
			
		}
	}

	if(state == haveGenJN){
		generateLinkBody();
	}else {
		// SingalFather::getInstance()->reDrawCube();
		SingalFather::getInstance()->supdateGL();
	}
}

/**
 * @description: set step
 * @param {*}
 * @return {*}
 */
void SolutionSkel::rsetStepDouble(double step){
	this->step = step;
}

/**
 * @description: reDrawCubeR
 * @param {type}
 * @return {type}
 */
void SolutionSkel::reDrawCubeR() {
	SingalFather::getInstance()->sdrawNormalSkel();
	std::cout << "[DEBUG] reDraw here" << std::endl;
}

void SolutionSkel::setMesh(HexV3fMesh *mesh_) {
	mesh = mesh_;
}

void SolutionSkel::pipeline() {
	solutionOldBMesh->pipeline();
	SingalFather::getInstance()->genCoarseMeshFinish();
}

void SolutionSkel::pipeline2() {
	solutionOldBMesh->pipelineReCon();
	SingalFather::getInstance()->sdrawNormalSkel();
}

void pointModel(double model[16], double point[4], double rlt[4]) {
	rlt[0] = 0; rlt[1] = 0; rlt[2] = 0; rlt[3] = 0;
	rlt[0] = model[0] * point[0] + model[1] * point[1] + model[2] * point[2] + model[3] * point[3];
	rlt[1] = model[4] * point[0] + model[5] * point[1] + model[6] * point[2] + model[7] * point[3];
	rlt[2] = model[8] * point[0] + model[9] * point[1] + model[10] * point[2] + model[11] * point[3];
	rlt[3] = model[12] * point[0] + model[13] * point[1] + model[14] * point[2] + model[15] * point[3];
}


/**
 * @description: check sniff 
 * @param {const} BMesh
 * @param {const} BMesh
 * @return {*} not equal return true equal return false
 */
bool SolutionSkel::changeSnap(const BMesh::Point3D&pOld, const BMesh::Point3D &pNew, BMesh::Point3D &out) {
	bool check = false;
	if(step >= 1) {
		// 按1计算
		int pOldx = pOld.x;
		int pOldy = pOld.y;
		int pOldz = pOld.z;

		int pNewx = pNew.x;
		int pNewy = pNew.y;
		int pNewz = pNew.z;
		out = BMesh::Point3D(pNewx, pNewy, pNewz);
		if(pOldx != pNewx  || pOldy != pNewy || pOldz != pNewz) {
			check = true;
			return check;
		}
	}else if(step > 0 && step < 1){
		std::stringstream ss0;
		ss0 << step;
		std::string s0;
		ss0 >> s0;
		int count = 0;
		for(int i=0; i<s0.size(); i++) {
			if(count >= 1 && s0[i] != '0') {
				break;
			}
			if(s0[i] == '.') {
				count = 1;
				continue;
			}
			if(s0[i] == '0' && count >= 1) {
				count++;
			}
		}

		double pOx = bigStringToZero(pOld.x, count);
		double pNx = bigStringToZero(pNew.x, count);

		double pOy = bigStringToZero(pOld.y, count);
		double pNy = bigStringToZero(pNew.y, count);

		double pOz = bigStringToZero(pOld.z, count);
		double pNz = bigStringToZero(pNew.z, count);
		out = BMesh::Point3D(pNx, pNy, pNz);
		if(pOx != pNx) {
			check = true;
			return check;
		}
		if(pOy != pNy) {
			check = true;
			return check;
		}
		if(pOz != pNz) {
			check = true;
			return check;
		}
	}
	return check;
}

/**
 * @description: let string bigger than step count to be zero
 * @param {double} p
 * @param {int} count
 * @return {*}
 */
double SolutionSkel::bigStringToZero(double p, int count){
	std::stringstream ss;
	ss << p;
	std::string sx; 
	ss >> sx;
	int lcount = 0;
	for(int i=0; i<sx.size(); i++) {
		if(sx[i] == '.') {
			lcount = 1;
			continue;
		}
		if (lcount > count) {
			sx[i] = '0';
		}
		if(lcount >= 1) {
			lcount++;
		}
	}
	std::stringstream s2;
	s2 << sx;
	double rlt;
	s2 >> rlt;
	return rlt;
}


/**
 * @description: move cell
 *				type: 0: x 1:y 2:z 3:o -1:only this move
 * @param {type}
 * @return {type}
 */
void SolutionSkel::rdeltaEditNode(double x, double y, double z, int type) {
	BMesh::Point3D newP(x, y, z);
	std::vector<double> r = selectNode->getLengthWidthHeight();
	BMesh::Point3D oldP = selectNode->getPosition();
	if (bVertexSnap) {
		newP = newP + oldP + snifDelta;
	}
	else {
		newP = newP + oldP;
	}
	BMesh::Point3D snapPoint;
	if(bVertexSnap) { // should change 
		if(changeSnap(oldP, newP, snapPoint)) {
			snifDelta = newP - snapPoint;
			newP = snapPoint;
		}
		else{
			snifDelta = (newP - oldP);
			return;
		}
	}
	BMesh::Point3D mirrorP;
	bool find = false;
	if(state == haveGenJN){
		clearJNCube();
	}
	if (type != -1) {
		if (type == 4) {
			BMesh::Point3D pp(selectNode->getPosition().x, selectNode->getPosition().y, selectNode->getPosition().z);
			pp.y = pp.y * -1;
			mirrorNode = selectWhichNode_(pp);
			if (mirrorNode) {
				find = true;
				pp = newP;
				pp.y = pp.y * -1;
				solutionOldBMesh->editNode(mirrorNode, pp, r);
			}
		}
		else {
			for (int i = 0; i < selectNode->children.size(); i++) {
				BMesh::Node* xMirrorNodeFather = selectNode->children[i];
				if (find) {
					break;
				}
				for (int j = 0; j < xMirrorNodeFather->children.size(); j++) {
					BMesh::Node* xMirrorNode = xMirrorNodeFather->children[j];
					BMesh::Point3D a = xMirrorNode->getPosition();
					if (type == 0) {
						BMesh::Point3D ax(xMirrorNodeFather->getPosition().x - a.x, a.y, a.z);
						if (abs(xMirrorNodeFather->getPosition().x - a.x + xMirrorNodeFather->getPosition().x - selectNode->getPosition().x) < 1e-7 &&
							abs(xMirrorNode->getPosition().y - selectNode->getPosition().y) < 1e-7 &&
							abs(xMirrorNode->getPosition().z - selectNode->getPosition().z) < 1e-7) {
							find = true;
							mirrorNode = xMirrorNode;
							mirrorP = BMesh::Point3D(xMirrorNodeFather->getPosition().x - (x - xMirrorNodeFather->getPosition().x), y, z);
							solutionOldBMesh->editNode(mirrorNode, mirrorP, r);
							break;
						}
					}
					else if (type == 1) {
						BMesh::Point3D ax(a.x, xMirrorNodeFather->getPosition().y - a.y, a.z);
						if (abs(xMirrorNode->getPosition().x - selectNode->getPosition().x) < 1e-7 &&
							abs(xMirrorNodeFather->getPosition().y - a.y + xMirrorNodeFather->getPosition().y - selectNode->getPosition().y) < 1e-7 &&
							abs(xMirrorNode->getPosition().z - selectNode->getPosition().z) < 1e-7) {
							find = true;
							mirrorNode = xMirrorNode;
							mirrorP = BMesh::Point3D(x, xMirrorNodeFather->getPosition().y - (y - xMirrorNodeFather->getPosition().y), z);
							solutionOldBMesh->editNode(mirrorNode, mirrorP, r);
							break;
						}
					}
					else if (type == 2) {
						BMesh::Point3D ax(a.x, a.y, xMirrorNodeFather->getPosition().z - a.z);
						if (abs(xMirrorNode->getPosition().x - selectNode->getPosition().x) < 1e-7 &&
							abs(xMirrorNode->getPosition().y - selectNode->getPosition().y) < 1e-7 &&
							abs(xMirrorNodeFather->getPosition().z - a.z + xMirrorNodeFather->getPosition().z - selectNode->getPosition().z) < 1e-7) {
							find = true;
							mirrorNode = xMirrorNode;
							mirrorP = BMesh::Point3D(x, y, xMirrorNodeFather->getPosition().z - (z - xMirrorNodeFather->getPosition().z));
							solutionOldBMesh->editNode(mirrorNode, mirrorP, r);
							break;
						}
					}
					else if (type == 3) {
						BMesh::Point3D ax(xMirrorNodeFather->getPosition().x - a.x,
							xMirrorNodeFather->getPosition().y - a.y,
							xMirrorNodeFather->getPosition().z - a.z);
						if (abs(xMirrorNodeFather->getPosition().x - a.x + xMirrorNodeFather->getPosition().x - selectNode->getPosition().x) < 1e-7 &&
							abs(xMirrorNodeFather->getPosition().y - a.y + xMirrorNodeFather->getPosition().y - selectNode->getPosition().y) < 1e-7 &&
							abs(xMirrorNodeFather->getPosition().z - a.z + xMirrorNodeFather->getPosition().z - selectNode->getPosition().z) < 1e-7) {
							find = true;
							mirrorNode = xMirrorNode;
							mirrorP = BMesh::Point3D(xMirrorNodeFather->getPosition().x - (x - xMirrorNodeFather->getPosition().x),
								xMirrorNodeFather->getPosition().y - (y - xMirrorNodeFather->getPosition().y),
								xMirrorNodeFather->getPosition().z - (z - xMirrorNodeFather->getPosition().z));
							solutionOldBMesh->editNode(mirrorNode, mirrorP, r);
							break;
						}
					}
				}
			}
		}
		
	}
	else {
		mirrorNode = NULL;
	}
	if (!find) {
		mirrorNode = NULL;
	}
	solutionOldBMesh->editNode(selectNode, newP, r);
	if(state == haveGenJN){
		generateLinkBody();
	}else{
		SingalFather::getInstance()->supdateGL();
	}
}

BMesh::Node * SolutionSkel::selectWhichNode_(BMesh::Point3D &p) {
	BMesh::Point3D t(p.x, p.y, p.z);
	return solutionOldBMesh->selectWhichNode(t);
}


void SolutionSkel::xExpand() {
	if(selectNode != NULL) {
		BMesh::Point3D p = selectNode->getPosition();
		std::vector<double> ss = selectNode->getLengthWidthHeight();
		ss[0] = ss[0] + step;
		solutionOldBMesh->editNode(selectNode, p, ss);
	}
}
void SolutionSkel::yExpand() {
	if(selectNode != NULL) {
		BMesh::Point3D p = selectNode->getPosition();
		std::vector<double> ss = selectNode->getLengthWidthHeight();
		ss[1] = ss[1] + step;
		solutionOldBMesh->editNode(selectNode, p, ss);
	}
}
void SolutionSkel::zExpand() {
	if(selectNode != NULL) {
		BMesh::Point3D p = selectNode->getPosition();
		std::vector<double> ss = selectNode->getLengthWidthHeight();
		ss[2] = ss[2] + step;
		solutionOldBMesh->editNode(selectNode, p, ss);
	}
}
void SolutionSkel::xCrop() {
	if(selectNode != NULL) {
		BMesh::Point3D p = selectNode->getPosition();
		std::vector<double> ss = selectNode->getLengthWidthHeight();
		if((ss[0] - step) < 0) {
			QMessageBox::information(NULL, "ERROR", "it will be too small", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return;
		}
		ss[0] = ss[0] - step;
		solutionOldBMesh->editNode(selectNode, p, ss);
	}
}
void SolutionSkel::yCrop() {
	if(selectNode != NULL) {
		BMesh::Point3D p = selectNode->getPosition();
		std::vector<double> ss = selectNode->getLengthWidthHeight();
		if((ss[1] - step) < 0) {
			QMessageBox::information(NULL, "ERROR", "it will be too small", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return;
		}
		ss[1] = ss[1] - step;
		solutionOldBMesh->editNode(selectNode, p, ss);
	}
}
void SolutionSkel::zCrop() {
	if(selectNode != NULL) {
		BMesh::Point3D p = selectNode->getPosition();
		std::vector<double> ss = selectNode->getLengthWidthHeight();
		if((ss[2] - step) < 0) {
			QMessageBox::information(NULL, "ERROR", "it will be too small", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return;
		}
		ss[2] = ss[2] - step;
		solutionOldBMesh->editNode(selectNode, p, ss);
	}
}



/**
 * @description: add Node
 * @param {type}
 * @return {type}
 */
void SolutionSkel::raddNode(double x, double y, double z, double angleX, double angleY, double angleZ, double length, double width, double height) {
	BMesh::Point3D p(x, y, z);
	if (length == width && width == height && length == 1.0000123) {
		std::vector<double> lwh = selectNode->getLengthWidthHeight();
		solutionOldBMesh->addNode(selectNode, p, angleX, angleY, angleZ, lwh[0], lwh[1], lwh[2]);
		if(bmirrorMode) {
			BMesh::Point3D pp(p);
			pp.y = pp.y * -1;
			BMesh::Point3D tt(selectNode->getPosition());
			tt.y = tt.y * -1;
			BMesh::Node * rlt = selectWhichNode_(tt);
			if(rlt) {
				solutionOldBMesh->addNode(rlt, pp, angleX, angleY, angleZ, lwh[0], lwh[1], lwh[2]);
				if(bdragMode) {
					selectNode = solutionOldBMesh->v[solutionOldBMesh->v.size() - 2];
					SingalFather::getInstance()->snewNodeOK();
				}
			}
		} else if(bdragMode) {
			selectNode = solutionOldBMesh->v[solutionOldBMesh->v.size() - 1];
			SingalFather::getInstance()->snewNodeOK();
		}
	}
	else {
		solutionOldBMesh->addNode(selectNode, p, angleX, angleY, angleZ, length, width, height);
	}
	SingalFather::getInstance()->reDrawCube();
}

/**
 * @description: delete Node
 * @param {type}
 * @return {type}
 */
void SolutionSkel::rdelNode() {
	std::cout << "[DEBUG] delete node here " << std::endl;
	bool check = solutionOldBMesh->delNode(selectNode);
	if (!check) {
		std::cout << "[DEBUG] delete false" << std::endl;
	}
	selectNode = nullptr;
	SingalFather::getInstance()->reDrawCube();
}

/**
 * @description: clear skel
 * @param {type}
 * @return {type}
 */
void SolutionSkel::clearSkel() {
	while (solutionOldBMesh->v.size()) {
		solutionOldBMesh->delNode(solutionOldBMesh->v[0]);
	}
}

/**
 * @description: auto change cube's angle
 * @param {type}
 * @return {type}
 */
void SolutionSkel::changeAngleAuto(){
	if(state == haveGenJN){
		clearJNCube();
	}
	solutionOldBMesh->pipeline1();
	if(state == haveGenJN){
		solutionOldBMesh->pipeline2();
	}
	SingalFather::getInstance()->sdrawNormalSkel();
}

/**
 * @description: generate link body
 * @param {*}
 * @return {*}
 */
void SolutionSkel::generateLinkBody(){
	solutionOldBMesh->pipeline2();
	state = haveGenJN;
	SingalFather::getInstance()->sdrawNormalSkel();
}

void SolutionSkel::simpleWithCgal() {
	QString path = QFileDialog::getOpenFileName(NULL, "open", ""
		, "OFF(*.off)");
	solutionOldBMesh->setTriOffPath((std::string)path.toLocal8Bit().data());
	solutionOldBMesh->simpleWithCgal();
	QMessageBox::information(NULL, "Information", "off to skel is finished, Lucky Guy!",
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	SingalFather::getInstance()->sshowSkelSingal();
}


/**
 * @description: clearJNCube
 * @param {*}
 * @return {*}
 */
void SolutionSkel::clearJNCube(){
	for(auto it=solutionOldBMesh->v.begin(); it != solutionOldBMesh->v.end();){
		if((*it)->getNodeType() == BMesh::NODETYPE::JN){
			// rebuild connect
			BMesh::Node *a = (*it)->children[0];
			BMesh::Node *b = (*it)->children[1];
			a->addChild(b);
			a->deleteChild((*it));
			b->addChild(a);
			b->deleteChild((*it));
			it = solutionOldBMesh->v.erase(it);
		}else{
			it++;
		}
	}
	for(auto it=solutionOldBMesh->v.begin(); it != solutionOldBMesh->v.end(); it++){
		BMesh::Hex *b = new BMesh::Hex();
		(*b) = *(*it)->root;
		delete (*it)->root;
		(*it)->root = b;
	}
}

/**
 * @description: topological Split
 * @param {*}
 * @return {*}
 */
void SolutionSkel::topologicalSplit(){
	solutionOldBMesh->pipeline3();
	SingalFather::getInstance()->genCoarseMeshFinish();
}


void SolutionSkel::saveSkel(std::string name) {
	solutionOldBMesh->saveSkel(name);
}

void SolutionSkel::getALine(std::vector<BMesh::Node*>& ret) {
	selectNodes.clear();
	solutionOldBMesh->getALine(selectNode, ret);
	for(int i=0; i<ret.size(); i++){
		selectNodes.push_back(ret[i]);
	}
}


void SolutionSkel::changeToPolylines() {
	solutionOldBMesh->changeToPolylines();
}