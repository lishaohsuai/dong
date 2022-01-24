#pragma once

#include "ComHeader.h"
#include "GeometryKernel.h"
#include <iomanip> 
#include "FunctionBSpline.h"
#include "BezierSolid.h"
#include "skelSimple.h"

enum FileType{
	MESH = 0,
	VTK,
	OFF
};
class FileHandler {

private:
	FileType fType;
	HexV3fMesh *pm_convert_from;
	std::ifstream mesh_iff;
	std::ofstream mesh_off;
	bool checkIsFilled;
	bool isReading;
	std::string filepath_checked;
private:
	bool getCleanLine(std::istream& _ifs, std::string& _string, bool _skipEmptyLines = true) const;
	bool getCleanLine1(std::istream& _ifs, std::string& _string, bool _skipEmptyLines = true) const;
	void trimString(std::string& _string) const;
	void extractQuotedText(std::string& _string) const;
public:
	bool readSkeltonSKEL(OurSkel* skelfile);
public:
	FileHandler(std::string str_file_path, std::string str_file_name);
	FileHandler(std::string path);
	FileHandler() {};
	bool readFile(HexV3fMesh* hexmesh);
	bool read2HexMeshObj_new(HexV3fMesh* hexmesh);
	bool readVtk2HexMeshObj(HexV3fMesh* hexmesh);

	bool writeFile(const std::string& _filename, HexV3fMesh* _mesh);
	bool writeFileMesh(const std::string& _filename, HexV3fMesh* _mesh) const;

	bool read2HexMeshObj(HexV3fMesh *hexmesh, bool isWrite2File, bool _computBottom, bool _topologyCheck);
	bool read2HexMeshObj1(HexV3fMesh *hexmesh, bool isWrite2File, bool _computBottom, bool _topologyCheck);
	
	
	//bool writeFile1(const std::string& _filename, HexV3fMesh* _mesh) const;
	bool write2OffFile(const std::string& _filename, HexV3fMesh* _mesh)const;
	bool write2DatFile(const std::string& _filename, std::vector<HexV3fMesh>& _bc_mesh_vec)const;
	
	bool readVtk2HexMeshObj1(HexV3fMesh * hexmesh);
	bool readVtk2HexMeshObj_smooth(HexV3fMesh * hexmesh, std::vector<V3f> &vn);
	bool readVtk2HexMeshObj_smooth1(HexV3fMesh * hexmesh, std::vector<V3f> &vn);
	bool write2VTKFile(const std::string& _filename, HexV3fMesh* _bc_mesh_vec) const;
	bool write2OFFFile(const std::string& _filename, HexV3fMesh* _bc_mesh_vec) const;
	bool write2OFFFileTri(const std::string& _filename, HexV3fMesh* _bc_mesh_vec) const;
	bool write2AXLFile(const std::string& _filename,std::vector<std::vector<std::vector<std::vector<V3f>>>> _SolidData,
		std::vector<std::vector<double>> _bezier_color);
	bool writepickedpoint(const std::string& _filename,const std::vector<std::vector<VertexHandle>> &pickpoint_vtoline)const;
	bool writepickedpoint(const std::string& _filename, const std::vector<std::vector<V3f>> &pickpoint_vtoline)const;
	bool readpickedpoint(std::vector<std::vector<VertexHandle>> &pickpoint_vtoline);
	bool readpickedpoint(std::vector<std::vector<V3f>> &pickpoint_vtoline);
	bool writepickededge(const std::string& _filename, const std::vector<EdgeHandle> &pickededge)const;
	bool readpickededge(std::vector<EdgeHandle> &pickededge);

	bool writepickededge(const std::string& _filename, const std::vector<EdgeHandle> &pickededge, HexV3fMesh *hexmesh)const;
	bool readpickededge(std::vector<V3f> &edgemid);

	bool readmodelofGodHe(HexV3fMesh *hexmesh);
	bool writepickedface(const std::string& _filename, const std::vector<FaceHandle> &pickeface)const;
	bool readpickedface(std::vector<FaceHandle> &pickedface);
	bool writepickedall(const std::string& _filename, const std::vector<VertexHandle> &pickedvertex, const std::vector<EdgeHandle> &pickededge,
		const std::vector<FaceHandle> &pickedface, const std::vector<CellHandle> &pickedcell)const;
	bool readpickedall(std::vector<VertexHandle> &pickedvertex, std::vector<EdgeHandle> &pickededge,
		std::vector<FaceHandle> &pickedface, std::vector<CellHandle> &pickedcell);
	bool readObj2HexMesh(HexV3fMesh * hexmesh, std::vector<V3f> &vn);
	bool writepickdrawface(const std::string& _filename, const std::vector<int> &_pickdrawface)const;
	bool readpickdrawface(std::vector<int> &_pickdrawface);
	bool writebdy(const std::string& _filename, const std::vector<std::vector<std::pair<int, int>>> &m_bdyquick)const;
	bool readbdy(std::vector<std::vector<std::pair<int, int>>> &m_bdyquick);
	bool writemaxmin(const std::string& _filename, const double max,const double min)const;
public:
	bool readline(std::vector<double> &linecheck);
	bool writeline(const std::string& _filename, std::vector<double> &linecheck);
	bool writev3fline(const std::string& _filename, HexV3fMesh * hexmesh, std::vector<VertexHandle> &pickpoint_v,
		std::vector<std::vector<VertexHandle>> &pickpoint_vtoline);
	bool writecsv(const std::string& _filename, const std::vector<VertexHandle> &pickeface, HexV3fMesh *hexmesh)const;
	bool writecsv(const std::string& _filename, const std::vector<VertexHandle> &pickeface)const;
	bool readcsv(std::vector<VertexHandle> &pickedface, std::vector<V3f> &csv3f);
	bool readcsv(std::vector<VertexHandle> &pickedface);
public:
	bool readbezier(BSplineSolid &m_solids, std::vector<std::vector<double>> &m_bdycoef);
	bool readbeziernew(std::vector<double> &m_color, HexV3fMesh * hexmesh);
	bool writeXJ(const std::string& _filename, HexV3fMesh * _mesh, std::vector<double> &m_color);
	bool readXJ(HexV3fMesh * _mesh, std::vector<double> &m_color);
};

