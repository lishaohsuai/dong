#include "FileHandler.h"
#include "timer.h"

using namespace std;
FileHandler::FileHandler(string str_file_path, string str_file_name)
{
	if (str_file_path.empty()) {
		std::cout << "file path is empty!";
		isReading = false;
		checkIsFilled = false;
		return;
	}
	else if (str_file_name.empty()) {
		std::cout << "file name is empty!";
		isReading = false;
		checkIsFilled = false;
		return;
	}

	if (str_file_name.find("mesh") == string::npos&&
		str_file_name.find("MESH") == string::npos) {
		std::cout << "file type is wrong!";
		isReading = false;
		checkIsFilled = false;
		return;
	}

	std::stringstream stream;
	std::string result;

	stream << str_file_path;
	if (str_file_path.rfind("\\") != str_file_path.length() - 1 &&
		str_file_path.rfind("/") != str_file_path.length() - 1) {
		stream << "\\";
	}
	stream << str_file_name;
	stream >> result;

	filepath_checked = result;
	isReading = true;
	checkIsFilled = false;
}

FileHandler::FileHandler(std::string path)
{
	if (path.empty()) {
		std::cout << "file path is empty!";
		isReading = false;
		checkIsFilled = false;
		return;
	}

	filepath_checked = path;
	string fileType = filepath_checked.substr(filepath_checked.rfind('.') + 1);
	transform(fileType.begin(), fileType.end(), fileType.begin(), ::tolower);
	if (fileType == "mesh") {
		fType = MESH;
	}
	else if (fileType == "vtk") {
		fType = VTK;
	}
	isReading = true;
	checkIsFilled = false;
}

bool FileHandler::readFile(HexV3fMesh* hexmesh){
	bool check = false;
	switch (fType) {
	case MESH:
		check = read2HexMeshObj_new(hexmesh);
		break;
	case VTK:
		check = readVtk2HexMeshObj(hexmesh);
		break;
	default:
		break;
	}
	return check;
}

void FileHandler::extractQuotedText(string& _string) const {

	// Trim Both leading and trailing quote marks
	size_t start = _string.find_first_of("\""); ++start;
	size_t end = _string.find_last_not_of("\"");

	if ((string::npos == start) || (string::npos == end)) {
		_string = "";
	}
	else {
		_string = _string.substr(start, end - start + 1);
	}
}

void FileHandler::trimString(string& _string) const {

	// Trim Both leading and trailing spaces
	size_t start = _string.find_first_not_of(" \t\r\n");
	size_t end = _string.find_last_not_of(" \t\r\n");

	if ((string::npos == start) || (string::npos == end)) {
		_string = "";
	}
	else {
		_string = _string.substr(start, end - start + 1);
	}
}

bool FileHandler::getCleanLine(istream& _ifs, string& _string, bool _skipEmptyLines) const {

	while (true) {
		// Get the current line:
		getline(_ifs, _string);
		// Remove whitespace at beginning and end
		trimString(_string);
		// Check if string is not empty ( otherwise we continue
		if (_string.size() != 0) {
			// Check if string is a comment ( starting with # )
			if (_string[0] != '#') {
				return true;
			}
		}
		else {
			if (!_skipEmptyLines)
				return true;
		}
		if (_ifs.eof() || _string.compare("End")) {
			cerr << "End of file reached while searching for input!" << endl;
			return false;
		}
	}
	return false;
}

bool FileHandler::getCleanLine1(istream& _ifs, string& _string, bool _skipEmptyLines) const {

	// While we are not at the end of the file
	while (true) {

		// Get the current line:
		getline(_ifs, _string);

		// Remove whitespace at beginning and end
		trimString(_string);

		// Check if string is not empty ( otherwise we continue
		if (_string.size() != 0) {

			// Check if string is a comment ( starting with # )
			if (_string[0] != '#') {
				return true;
			}

		}
		else {
			if (!_skipEmptyLines)
				return true;
		}

	}

	return false;
}
//read mesh file to HexV3fMesh
bool FileHandler::read2HexMeshObj1(HexV3fMesh *hexmesh, bool isWrite2File, bool _computBottom, bool _topologyCheck)
{
	if (!isReading) {
		return false;
	}
	this->pm_convert_from = hexmesh;

	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	stringstream sstr;
	string line;
	string s_tmp;
	uint64_t c = 0u;

	int v_arr[8];

	pm_convert_from = hexmesh;

	//*************************
	//header
	bool header_found = true;

	getCleanLine(mesh_iff, line);
	sstr.str(line);

	// Check header
	sstr >> s_tmp;
	transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "MESHVERSIONFORMATTED") {
		header_found = false;
		mesh_iff.close();
		std::cerr << "The specified file might not be in .mesh format!" << std::endl;
		return false;
	}

	sstr.clear();
	getCleanLine(mesh_iff, line);
	sstr.str(line);

	// Get Dimension
	string s_dim;
	sstr >> s_tmp;
	sstr >> s_dim;
	transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "DIMENSION" || s_dim != "3") {
		mesh_iff.close();
		std::cerr << "Other dimension for files are not supported at the moment!" << std::endl;
		return false;
	}
	if (!header_found) {
		sstr.clear();
		sstr.str(line);
	}
	else {
		sstr.clear();
		getCleanLine(mesh_iff, line);
		sstr.str(line);
	}

	sstr >> s_tmp;
	sstr >> c;
	std::transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "VERTICES") {
		mesh_iff.close();
		std::cerr << "No vertex section defined!" << std::endl;
		return false;
	}
	else {
		for (uint64_t i = 0u; i < c; ++i) {
			int check;
			getCleanLine(mesh_iff, line);
			sstr.clear();
			sstr.str(line);
			V3f v = V3f(0.00, 0.00, 0.00);
			sstr >> v.x;
			sstr >> v.y;
			sstr >> v.z;
			sstr >> check;
			if (check == 0)
				v.is_valid = false;
			pm_convert_from->add_vertex(v);
		}
	}

	int cellnum;
	while (true)
	{
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		string::size_type idx;
		idx = line.find("Hexahedra");
		cout << line << endl;
		if (idx != string::npos)
		{
			sstr >> s_tmp;
			sstr >> cellnum;
			break;
		}
	}

	int temp_inde = 0;
	for (unsigned int f = 0; f < cellnum; ++f) {
		if (f % 1000 == 0)
			cout << f << ":" << cellnum << endl;
		vector<VertexHandle> temp_vetex;

		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);

		for (int i = 0; i < 8; ++i) {
			sstr >> temp_inde;
			temp_vetex.push_back(VertexHandle(temp_inde - 1));
		}


		CellHandle cell_now = pm_convert_from->add_cell(temp_vetex);


	}
	mesh_iff.close();

	return true;
}

bool FileHandler::read2HexMeshObj(HexV3fMesh *hexmesh, bool isWrite2File, bool _computBottom, bool _topologyCheck)
{
	if (!isReading) {
		return false;
	}
	this->pm_convert_from = hexmesh;

	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;
	string s_tmp;
	uint64_t c = 0u;


	int v_arr[8];

	pm_convert_from = hexmesh;

	//*************************
	//header
	bool header_found = true;

	getCleanLine(mesh_iff, line);
	sstr.str(line);

	// Check header
	sstr >> s_tmp;
	transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "MESHVERSIONFORMATTED") {
		//iff.close();
		header_found = false;
		mesh_iff.close();
		std::cerr << "The specified file might not be in .mesh format!" << std::endl;
		return false;
	}

	sstr.clear();
	getCleanLine(mesh_iff, line);
	sstr.str(line);

	// Get Dimension
	string s_dim;
	sstr >> s_tmp;

	sstr >> s_dim;
	transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "DIMENSION" || s_dim != "3") {
		mesh_iff.close();
		std::cerr << "Other dimension for files are not supported at the moment!" << std::endl;
		return false;
	}

	if (!header_found) {
		sstr.clear();
		sstr.str(line);
	}
	else {
		sstr.clear();
		getCleanLine(mesh_iff, line);
		sstr.str(line);
	}

	sstr >> s_tmp;
	std::transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "VERTICES") {
		mesh_iff.close();
		std::cerr << "No vertex section defined!" << std::endl;
		return false;
	}
	else {
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		sstr >> c;
		for (uint64_t i = 0u; i < c; ++i) {
			int check;
			getCleanLine(mesh_iff, line);
			sstr.clear();
			sstr.str(line);
			V3f v = V3f(0.00, 0.00, 0.00);
			sstr >> v.x;
			sstr >> v.y;
			sstr >> v.z;
			sstr >> check;
			if (check == 0)
				v.is_valid = false;
			pm_convert_from->add_vertex(v);
		}
	}


	while (true)
	{
		getCleanLine(mesh_iff, line);
		if (line == "Hexahedra")break;
	}
	getCleanLine(mesh_iff, line);
	sstr.clear();
	sstr.str(line);
	sstr >> c;
	int temp_inde = 0;
	for (unsigned int f = 0; f < c; ++f) {
		if (f % 1000 == 0)
			cout << f << ":" << c << endl;
		vector<VertexHandle> temp_vetex;

		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);

		for (int i = 0; i < 8; ++i) {
			sstr >> temp_inde;
			temp_vetex.push_back(VertexHandle(temp_inde - 1));
		}


		CellHandle cell_now = pm_convert_from->add_cell(temp_vetex);


	}
	mesh_iff.close();

	return true;
}

bool FileHandler::write2OffFile(const std::string& _filename, HexV3fMesh* _mesh) const {

	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}

	// Write header
	off << "OFF" << std::endl;
	off << _mesh->vertices_.size() << " " << _mesh->faces_.size() << " 0" << std::endl;
	uint64_t n_vertices(_mesh->vertices_.size());

	// write vertices
	for (uint64_t v_it = 0; v_it < n_vertices; ++v_it) {

		off << std::fixed << std::setprecision(6) <<
			_mesh->vertices_[v_it].x << " "
			<< _mesh->vertices_[v_it].y << " "
			<< _mesh->vertices_[v_it].z << " "
			<< std::endl;
	}
	//�������ģʽ
	off.unsetf(ios::fixed);
	uint64_t n_faces(_mesh->faces_.size());

	for (uint64_t c_it = 0; c_it < n_faces; ++c_it) {

		off << "4 ";
		std::vector<VertexHandle> ves = _mesh->faces_[c_it].vs_f;

		for (std::vector<VertexHandle>::iterator it_ves = ves.begin(); it_ves
			!= ves.end(); ++it_ves) {
			off << (it_ves->idx()) << " ";

		}
		off << std::endl;
	}

	off << "End" << std::endl;
	off.close();

	return true;
}

/**
 * @description: bezier volume write 
 * @param {*}
 * @return {*}
 */
bool FileHandler::write2DatFile(const std::string & _filename, std::vector<HexV3fMesh>& _bc_mesh_vec) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}

	// Write header
	off << "TITLE = \"title\" VARIABLES = \"X\", \"Y\", \"Z\", \"elementcolor\"" << "\n";
	uint16_t bc_index = 0;
	uint32_t color[4] = { 0xD2B48C,
	0xB22222,
	0x87CEEB,
	0x228B22
	};
	for (bc_index = 0; bc_index < _bc_mesh_vec.size(); ++bc_index) {
		off << "ZONE T =" << "zone_" << (int)bc_index
			<< ", N = " << (int)_bc_mesh_vec[bc_index].vertices_.size()
			<< ", E = " << (int)_bc_mesh_vec[bc_index].cells_.size() << ", ET = BRICK, F = FEPOINT\n";

		uint64_t n_vertices(_bc_mesh_vec[bc_index].vertices_.size());

		// write vertices
		for (uint64_t v_it = 0; v_it < n_vertices; ++v_it) {
			if (bc_index == 1)
			{
				off << std::fixed << std::setprecision(6)
					<< _bc_mesh_vec[bc_index].vertices_[v_it].x << " "
					<< _bc_mesh_vec[bc_index].vertices_[v_it].y << " "
					<< _bc_mesh_vec[bc_index].vertices_[v_it].z << " "
					<< (double)(color[0]) << "\n";
			}
			else if (bc_index == 2)
			{
				off << std::fixed << std::setprecision(6)
					<< _bc_mesh_vec[bc_index].vertices_[v_it].x << " "
					<< _bc_mesh_vec[bc_index].vertices_[v_it].y << " "
					<< _bc_mesh_vec[bc_index].vertices_[v_it].z << " "
					<< (double)(color[3]) << "\n";
			}
			else
			{
				off << std::fixed << std::setprecision(6)
					<< _bc_mesh_vec[bc_index].vertices_[v_it].x << " "
					<< _bc_mesh_vec[bc_index].vertices_[v_it].y << " "
					<< _bc_mesh_vec[bc_index].vertices_[v_it].z << " "
					<< (double)(color[bc_index % 4]) << "\n";
			}
		}

		off.unsetf(ios::fixed);
		//write hex

		uint64_t n_cells(_bc_mesh_vec[bc_index].cells_.size());
		for (uint64_t v_it = 0; v_it < n_cells; ++v_it) {
			off << (int)(_bc_mesh_vec[bc_index].cells_[v_it].vertex_[0] + 1);
			for (uint8_t v_index = 1; v_index < 8; ++v_index) {
				off << " " << (int)(_bc_mesh_vec[bc_index].cells_[v_it].vertex_[v_index] + 1);
			}
			off << "\n";
		}
	}
	off.close();
	return true;
}

bool FileHandler::readVtk2HexMeshObj(HexV3fMesh * hexmesh)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;
	string s_tmp;
	uint64_t c = 0u;


	int v_arr[8];

	pm_convert_from = hexmesh;

	//*************************
	//header
	bool header_found = true;

	getCleanLine(mesh_iff, line);
	getCleanLine(mesh_iff, line);
	getCleanLine(mesh_iff, line);
	sstr.str(line);

	// Check header
	sstr >> s_tmp;
	transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "ASCII") {
		//iff.close();
		header_found = false;
		mesh_iff.close();
		std::cerr << "The specified file might not be in .vtk format!" << std::endl;
		return false;
	}
	sstr.clear();

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	sstr.clear();

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	std::string type;
	uint64_t count_point;
	sstr >> type;
	sstr >> count_point;
	sstr >> type;

	for (uint64_t i = 0u; i < count_point; ++i) {
		//if (i % 1000 == 0)
			//cout << "count_point: " << i << ": " << count_point << endl;
		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		V3f v = V3f(0.00, 0.00, 0.00);
		sstr >> v.x;
		sstr >> v.y;
		sstr >> v.z;
		pm_convert_from->add_vertex(v);
	}


	sstr.clear();
	getCleanLine(mesh_iff, line);
	sstr.str(line);
	//std::string type;
	uint64_t cell_point;
	sstr >> type;
	sstr >> cell_point;
	sstr >> type;

	int temp_inde;


	for (unsigned int f = 0; f < cell_point; ++f) {
		//cout << f << ":" << 62858 << endl;
		if (f % 1000 == 0)
			cout << "count_point: " << f << ": " << cell_point << endl;
		vector<VertexHandle> temp_vetex;
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		sstr >> temp_inde;
		for (int i = 0; i < 8; ++i) {
			sstr >> temp_inde;
			temp_vetex.push_back(VertexHandle(temp_inde));
		}


		CellHandle cell_now = pm_convert_from->add_cell(temp_vetex);


	}
	mesh_iff.close();

	return true;
}

bool FileHandler::write2VTKFile(const std::string & _filename, HexV3fMesh* _mesh) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}

	// Write header
	off << "# vtk DataFile Version 2.0" << std::endl;
	off << "mesh vtk data - converted from .off" << std::endl;
	off << "ASCII" << std::endl;
	off << "DATASET UNSTRUCTURED_GRID" << std::endl;
	uint64_t n_vertices(_mesh->vertices_.size());
	off << "POINTS ";
	off << n_vertices << " double" << std::endl;

	// write vertices
	for (uint64_t v_it = 0; v_it < n_vertices; ++v_it) {

		off << _mesh->vertices_[v_it].x << " "
			<< _mesh->vertices_[v_it].y << " "
			<< _mesh->vertices_[v_it].z << std::endl;
	}


	uint64_t n_cells(_mesh->cells_.size());

	uint32_t cout_ = 0;
	for (uint64_t c_it = 0; c_it < n_cells; ++c_it) {
		if (_mesh->cells_[c_it].is_valid) ++cout_;
	}
	off << "CELLS ";
	off << cout_ << " " << cout_ * 9 << std::endl;

	for (uint64_t c_it = 0; c_it < n_cells; ++c_it) {
		if (!_mesh->cells_[c_it].is_valid) continue;

		std::vector<VertexHandle> ves = _mesh->cells_[c_it].vertex_;

		off << 8 << " ";

		for (std::vector<VertexHandle>::iterator it_ves = ves.begin(); it_ves
			!= ves.end(); ++it_ves) {
			off << ((it_ves->idx())) << " ";

		}
		off << std::endl;
	}

	off << "CELL_TYPES  ";
	off << cout_ << std::endl;

	for (int i = 0; i < cout_; ++i)
		off << 12 << std::endl;

	off.close();

	return true;
}

/**
 * @description: 输出 四边形
 */
bool cmp(const pair<V3f, int>& a, const pair<V3f, int>& b) {
	return a.second < b.second;
}
bool FileHandler::write2OFFFile(const std::string& _filename, HexV3fMesh* _mesh) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}

	// Write header
	off << "OFF" << std::endl;
	uint64_t n_vertices = 1;
	_mesh->frameface.clear();
	_mesh->frame.clear();
	_mesh->choseframe(); // 边界边
	_mesh->choseframeface(); // 边界面
	std::map<V3f, int> v; // 所有的边界面的顶点
	
	for (auto it : _mesh->frameface) {
		for (auto& iit : it) {
			if (v[iit] == 0) {
				v[iit] = n_vertices;
				n_vertices++;
			}
		}
	}
	off << v.size() << " " << _mesh->frameface.size() << " 0" << "\n";
	vector<pair<V3f, int>> vec(v.begin(), v.end());
	//对线性的vector进行排序

	sort(vec.begin(), vec.end(), cmp);
	// write vertices
	for (auto &it : vec) {
		off << it.first.x << " "
			<< it.first.y << " "
			<< it.first.z << std::endl;
	}
	for (auto it : _mesh->frameface) {
		off << it.size();
		for (auto& iit : it) {
			off << " " << v[iit] - 1;
		}
		off << "\n";
	}
	off.close();

	return true;
}

//bool cmp(const pair<V3f, int>& a, const pair<V3f, int>& b) {
//	return a.second < b.second;
//}
bool FileHandler::write2OFFFileTri(const std::string& _filename, HexV3fMesh* _mesh) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);
	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}

	// Write header
	off << "OFF" << std::endl;
	uint64_t n_vertices = 1;
	_mesh->frameface.clear();
	_mesh->frame.clear();
	_mesh->choseframe(); // 边界边
	_mesh->choseframeface(); // 边界面
	std::map<V3f, int> v; // 所有的边界面的顶点

	for (auto it : _mesh->frameface) {
		for (auto& iit : it) {
			if (v[iit] == 0) {
				v[iit] = n_vertices;
				n_vertices++;
			}
		}
	}
	off << v.size() << " " << _mesh->frameface.size() * 2 << " 0" << "\n";
	vector<pair<V3f, int>> vec(v.begin(), v.end());
	//对线性的vector进行排序

	sort(vec.begin(), vec.end(), cmp);
	// write vertices
	for (auto& it : vec) {
		off << it.first.x << " "
			<< it.first.y << " "
			<< it.first.z << std::endl;
	}
	for (auto it : _mesh->frameface) {
		off << 3;
		vector<V3f> tmp;
		for (auto& iit : it) {
			tmp.emplace_back(iit);
		}
		off << " " << v[tmp[0]] - 1;
		off << " " << v[tmp[1]] - 1;
		off << " " << v[tmp[2]] - 1;
		off << "\n";
		off << 3;
		off << " " << v[tmp[2]] - 1;
		off << " " << v[tmp[3]] - 1;
		off << " " << v[tmp[0]] - 1;
		off << "\n";
	}
	off.close();

	return true;
}

bool FileHandler::write2AXLFile(const std::string& _filename, std::vector<std::vector<std::vector<std::vector<V3f>>>> _SolidData,
	std::vector<std::vector<double>> _bezier_color)
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}
	off << "<axl>" << "\n";
	int i = 0;
	int v_sum = 4 * 4 * 4;
	for (std::vector<std::vector<std::vector<std::vector<V3f>>>>::iterator _SolidData_it = _SolidData.begin(); _SolidData_it != _SolidData.end();
		_SolidData_it++)
	{
		off << "<volume type=" << '"' << "bspline" << '"' << " name=" << '"' << "bv_0" << '"' << " color=" << '"';
		off << _bezier_color[i][0] << " " << _bezier_color[i][1] << " " << _bezier_color[i][2] << '"' << '>' << "\n";
		i++;
		off << "  <number>4 4 4</number>" << "\n";
		off << "  <order>4 4 4</order>" << "\n";
		off << "  <knots>0 0 0 0 1 1 1 1 </knots>" << "\n";
		off << "  <knots>0 0 0 0 1 1 1 1 </knots>" << "\n";
		off << "  <knots>0 0 0 0 1 1 1 1 </knots>" << "\n";
		off << "  <points>";
		int j = 0;
		std::vector<std::vector<std::vector<V3f>>> solid = *_SolidData_it;
		for (std::vector<std::vector<std::vector<V3f>>>::iterator solid_it = solid.begin(); solid_it != solid.end(); solid_it++)
		{
			std::vector<std::vector<V3f>> face = *solid_it;
			for (std::vector<std::vector<V3f>>::iterator face_it = face.begin(); face_it != face.end(); face_it++)
			{
				std::vector<V3f> line = *face_it;
				for (std::vector<V3f>::iterator line_it = line.begin(); line_it != line.end(); line_it++)
				{
					V3f vertex = *line_it;
					j++;
					if (j == v_sum)
					{
						off << std::fixed << std::setprecision(6)
							<< vertex.x << " "
							<< vertex.y << " "
							<< vertex.z;
					}
					else
					{
						off << std::fixed << std::setprecision(6)
							<< vertex.x << " "
							<< vertex.y << " "
							<< vertex.z << " "
							<< "\n";
					}
				}
			}
		}
		off << "</points>" << "\n";
		off << "</volume>" << "\n";
	}
	off << "<script></script>" << "\n";
	off << "<camera>" << "\n";
	off << " <Parameters fieldOfView=" << '"' << "0.785398" << '"' << " Type=" << '"' << "PERSPECTIVE" << '"' << "\n";
	off << "zNearCoefficient=" << '"' << "0.005" << '"' << " orthoCoef=" << '"' << "0.414214" << '"' << " zClippingCoefficient=" << '"' << "1.73205" << '"' << "\n";
	off << "/>" << "\n";
	off << " <Stereo distToScreen=" << '"' << "0.5" << '"' << " focusDistance=" << '"' << "241.421" << '"' << " physScreenWidth=" << '"' << "0.4" << '"' << "\n";
	off << "IODist=" << '"' << "0.062" << '"' << " />" << "\n";
	off << " <ManipulatedCameraFrame>" << "\n";
	off << "  <position x=" << '"' << "-8.36938" << '"' << " y=" << '"' << "0.880508" << '"' << " z=" << '"' << "7.02155" << '"' << " />" << "\n";
	off << "  <orientation q0=" << '"' << "-0.498947" << '"' << " q1=" << '"' << "0.060419" << '"' << " q2=" << '"' << "0.842227" << '"' << " q3=" << '"' << "-0.195076" << '"' << " />" << "\n";
	off << "  <ManipulatedParameters transSens=" << '"' << "1" << '"' << " rotSens=" << '"' << "1" << '"' << " wheelSens=" << '"' << "1" << '"' << "\n";
	off << "spinSens=" << '"' << "0.3" << '"' << " />" << "\n";
	off << "  <ManipulatedCameraParameters flySpeed=" << '"' << "1" << '"' << " >" << "\n";
	off << "   <flyUpVector x=" << '"' << "0" << '"' << " y=" << '"' << "1" << '"' << " z=" << '"' << "0" << '"' << " />" << "\n";
	off << "  </ ManipulatedCameraParameters>" << "\n";
	off << " </ManipulatedCameraFrame>" << "\n";
	off << "</camera>" << "\n";
	off << "</axl>" << "\n";
}

bool FileHandler::writeFile(const std::string& _filename, HexV3fMesh* _mesh) {
	string fileType = _filename.substr(_filename.rfind('.') + 1);
	transform(fileType.begin(), fileType.end(), fileType.begin(), ::tolower);
	if (fileType == "mesh") {
		fType = MESH;
	}
	else if (fileType == "vtk") {
		fType = VTK;
	}
	else if (fileType == "off") {
		fType = OFF;
	}
	bool check = false;
	switch (fType) {
	case MESH:
		check = writeFileMesh(_filename, _mesh);
		break;
	case VTK:
		check = write2VTKFile(_filename, _mesh);
		break;
	case OFF:
		// check = write2OFFFile(_filename, _mesh); // 四边形网格
		check = write2OFFFileTri(_filename, _mesh);
		break;
	default:
		break;
	}
	return check;
}

bool FileHandler::writeFileMesh(const std::string& _filename, HexV3fMesh* _mesh) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}

	// Write header
	off << "MeshVersionFormatted 1" << std::endl;
	off << "Dimension 3" << std::endl;
	uint64_t n_vertices(_mesh->vertices_.size());
	off << "Vertices " << n_vertices << std::endl;

	// write vertices
	for (uint64_t v_it = 0; v_it < n_vertices; ++v_it) {

		off << _mesh->vertices_[v_it].x << " "
			<< _mesh->vertices_[v_it].y << " "
			<< _mesh->vertices_[v_it].z << " ";
		if (_mesh->vertices_[v_it].is_valid == true)
			off << 1 << " ";
		else
			off << 0 << " ";
		off << endl; // "-1"
	}
	uint64_t n_faces(_mesh->faces_.size());
	off << "Quadrilaterals "<< n_faces << std::endl;

	for (uint64_t c_it = 0; c_it < n_faces; ++c_it) {


		std::vector<VertexHandle> ves = _mesh->faces_[c_it].vs_f;

		for (std::vector<VertexHandle>::iterator it_ves = ves.begin(); it_ves
			!= ves.end(); ++it_ves) {
			off << ((it_ves->idx()) + 1) << " ";

		}
		off << "1" << std::endl;
	}

	uint64_t n_cells(_mesh->cells_.size());
	off << "Hexahedra ";
	uint32_t cout_ = 0;
	for (uint64_t c_it = 0; c_it < n_cells; ++c_it) {
		if (_mesh->cells_[c_it].is_valid) ++cout_;
	}
	off << cout_ << std::endl;

	for (uint64_t c_it = 0; c_it < n_cells; ++c_it) {
		if (!_mesh->cells_[c_it].is_valid) continue;

		std::vector<VertexHandle> ves = _mesh->cells_[c_it].vertex_;

		for (std::vector<VertexHandle>::iterator it_ves = ves.begin(); it_ves
			!= ves.end(); ++it_ves) {
			off << ((it_ves->idx()) + 1) << " ";

		}
		off << "1" << std::endl;
	}


	off << "End" << std::endl;
	off.close();

	return true;
}

bool FileHandler::writepickedpoint(const std::string& _filename,
	const std::vector<std::vector<VertexHandle>> &pickpoint_vtoline) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}


	// write picked point
	off << pickpoint_vtoline.size() << endl;
	for (int v_it = 0; v_it < pickpoint_vtoline.size(); ++v_it)
	{
		off << pickpoint_vtoline[v_it].size() << endl;
		for (int i = 0; i < pickpoint_vtoline[v_it].size(); i++)
		{
			off << pickpoint_vtoline[v_it][i].idx() << " ";
		}
		off << endl;
	}

	off.close();

	return true;
}

bool FileHandler::writepickedpoint(const std::string& _filename,
	const std::vector<std::vector<V3f>> &pickpoint_vtoline) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}


	// write picked point
	off << pickpoint_vtoline.size() << endl;
	for (int v_it = 0; v_it < pickpoint_vtoline.size(); ++v_it)
	{
		off << pickpoint_vtoline[v_it].size() << endl;
		for (int i = 0; i < pickpoint_vtoline[v_it].size(); i++)
		{
			off << pickpoint_vtoline[v_it][i].x << " ";
			off << pickpoint_vtoline[v_it][i].y << " ";
			off << pickpoint_vtoline[v_it][i].z << endl;
		}
		//off << endl;
	}

	off.close();

	return true;
}

bool FileHandler::readpickedpoint(std::vector<std::vector<VertexHandle>> &pickpoint_vtoline)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	int pickpoint_vtolin_size;
	sstr >> pickpoint_vtolin_size;

	for (int i = 0; i < pickpoint_vtolin_size; i++)
	{
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		int pickpoint_size;
		sstr >> pickpoint_size;
		std::vector<VertexHandle> temp_v;

		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		for (int j = 0; j < pickpoint_size; j++)
		{
			int v_id;
			sstr >> v_id;
			VertexHandle v(v_id);
			temp_v.push_back(v);
		}
		pickpoint_vtoline.push_back(temp_v);
	}

	mesh_iff.close();

	return true;
}

bool FileHandler::readpickedpoint(std::vector<std::vector<V3f>> &pickpoint_vtoline)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	int pickpoint_vtolin_size;
	sstr >> pickpoint_vtolin_size;

	for (int i = 0; i < pickpoint_vtolin_size; i++)
	{
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		int pickpoint_size;
		sstr >> pickpoint_size;
		std::vector<V3f> temp_v;

		for (int j = 0; j < pickpoint_size; j++)
		{
			getCleanLine(mesh_iff, line);
			sstr.str("");
			sstr.clear();
			sstr.str(line);
			V3f v = V3f(0.00, 0.00, 0.00);
			sstr >> v.x;
			sstr >> v.y;
			sstr >> v.z;
			temp_v.push_back(v);
		}
		pickpoint_vtoline.push_back(temp_v);
	}

	mesh_iff.close();

	return true;
}

bool FileHandler::writepickededge(const std::string & _filename, const std::vector<EdgeHandle>& pickededge) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}


	// write picked point
	off << pickededge.size() << endl;

	for (int i = 0; i < pickededge.size(); i++)
	{
		off << pickededge[i].idx() << " ";
	}
	off << endl;


	off.close();

	return true;


}

bool FileHandler::readpickededge(std::vector<EdgeHandle>& pickededge)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	int pickededge_size;
	sstr >> pickededge_size;

	getCleanLine(mesh_iff, line);
	sstr.clear();
	sstr.str(line);
	for (int j = 0; j < pickededge_size; j++)
	{
		int e_id;
		sstr >> e_id;
		EdgeHandle e(e_id);
		pickededge.push_back(e);
	}

	mesh_iff.close();

	return true;
}

bool FileHandler::writepickededge(const std::string & _filename, const std::vector<EdgeHandle>& pickededge, HexV3fMesh *hexmesh) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}


	// write picked point
	off << pickededge.size() << endl;

	for (int i = 0; i < pickededge.size(); i++)
	{
		V3f edgemid = hexmesh->cal_edge_mid(pickededge[i]);
		off << edgemid.x << " " << edgemid.y << " " << edgemid.z << endl;
	}

	off.close();

	return true;


}

bool FileHandler::readpickededge(std::vector<V3f>& edgemid)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	int pickededge_size;
	sstr >> pickededge_size;

	for (int j = 0; j < pickededge_size; j++)
	{
		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		V3f v = V3f(0.00, 0.00, 0.00);
		sstr >> v.x;
		sstr >> v.y;
		sstr >> v.z;
		edgemid.push_back(v);
	}

	mesh_iff.close();

	return true;
}

bool FileHandler::readmodelofGodHe(HexV3fMesh * hexmesh)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;
	pm_convert_from = hexmesh;

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	int vertex_size;
	sstr >> vertex_size;
	int houdu = 2;
	double hhh = 100;
	for (int j = 0; j < vertex_size; j++)
	{
		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		V3f v = V3f(0.00, 0.00, 0.00);
		sstr >> v.x;
		//if (v.x < 0.5)
		//{
			//hhh = 0.27;
			/*sstr >> v.z;
			sstr >> v.y;*/
			//v.y = v.y - 0.05;
			/*pm_convert_from->add_vertex(v);
			for (int i = 0; i < houdu; i++)
			{
				v = v + V3f(0, hhh / houdu, 0);
				pm_convert_from->add_vertex(v);
			}*/
			//}
			//else
			//{
				//hhh = 0.12;
		sstr >> v.y;
		sstr >> v.z;
		//v.z = v.z - 2;
		//v.y = v.y-2.9-0.09555;
		//v.x = v.x + 6.3-0.04645+0.08;
		pm_convert_from->add_vertex(v);
		for (int i = 0; i < houdu; i++)
		{
			v = v + V3f(0, 0, hhh / houdu);
			pm_convert_from->add_vertex(v);
		}
		//}

		/*v = v + V3f(0, 0, 0.75);
		pm_convert_from->add_vertex(v);
		v = v + V3f(0, 0, 0.75);
		pm_convert_from->add_vertex(v);
		v = v + V3f(0, 0, 0.75);
		pm_convert_from->add_vertex(v);*/
	}
	getCleanLine(mesh_iff, line);
	sstr.clear();
	sstr.str(line);
	int cell_num;
	sstr >> cell_num;
	int temp_inde;

	for (int i = 0; i < cell_num; i++)
	{
		vector<VertexHandle> temp_vetex;
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		sstr >> temp_inde;
		for (int k = 0; k < 4; ++k) {
			sstr >> temp_inde;
			temp_vetex.push_back(VertexHandle((houdu + 1) * temp_inde));
		}
		for (int j = 0; j < houdu; j++)
		{
			vector<VertexHandle> temp_vetex1;
			for (int k = 0; k < 4; ++k) {
				temp_vetex1.push_back(VertexHandle(temp_vetex[k] + j));
			}
			for (int k = 0; k < 4; ++k) {
				temp_vetex1.push_back(VertexHandle(temp_vetex[k] + j + 1));
			}
			pm_convert_from->add_cell(temp_vetex1);
		}
		temp_vetex.swap(vector<VertexHandle>());


		/*vector<VertexHandle> temp_vetex1;
		for (int i = 0; i < 4; ++i) {
			temp_vetex1.push_back(VertexHandle(temp_vetex[i] + 1));
		}
		for (int i = 0; i < 4; ++i) {
			temp_vetex1.push_back(VertexHandle(temp_vetex[i] + 2));
		}
		pm_convert_from->add_cell(temp_vetex1);

		vector<VertexHandle> temp_vetex2;
		for (int i = 0; i < 4; ++i) {
			temp_vetex2.push_back(VertexHandle(temp_vetex[i] + 2));
		}
		for (int i = 0; i < 4; ++i) {
			temp_vetex2.push_back(VertexHandle(temp_vetex[i] + 3));
		}
		pm_convert_from->add_cell(temp_vetex2);

		vector<VertexHandle> temp_vetex3;
		for (int i = 0; i < 4; ++i) {
			temp_vetex3.push_back(VertexHandle(temp_vetex[i] + 3));
		}
		for (int i = 0; i < 4; ++i) {
			temp_vetex3.push_back(VertexHandle(temp_vetex[i] + 4));
		}
		pm_convert_from->add_cell(temp_vetex3);	*/
	}
	mesh_iff.close();

	return true;
}

bool FileHandler::writepickedface(const std::string & _filename, const std::vector<FaceHandle>& pickedface) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}


	// write picked point
	off << pickedface.size() << endl;

	for (int i = 0; i < pickedface.size(); i++)
	{
		off << pickedface[i].idx() << " ";
	}
	off << endl;


	off.close();

	return true;


}

bool FileHandler::readpickedface(std::vector<FaceHandle>& pickedface)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	int pickedface_size;
	sstr >> pickedface_size;

	getCleanLine(mesh_iff, line);
	sstr.clear();
	sstr.str(line);
	for (int j = 0; j < pickedface_size; j++)
	{
		int f_id;
		sstr >> f_id;
		FaceHandle f(f_id);
		pickedface.push_back(f);
	}

	mesh_iff.close();

	return true;
}

bool FileHandler::writepickedall(const std::string& _filename, const std::vector<VertexHandle> &pickedvertex, const std::vector<EdgeHandle> &pickededge,
	const std::vector<FaceHandle> &pickedface, const std::vector<CellHandle> &pickedcell)const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}
	//��
	off << pickedvertex.size() << endl;
	for (int i = 0; i < pickedvertex.size(); i++)
	{
		off << pickedvertex[i].idx() << " ";
	}
	off << endl;

	//��
	off << pickededge.size() << endl;
	for (int i = 0; i < pickededge.size(); i++)
	{
		off << pickededge[i].idx() << " ";
	}
	off << endl;

	//��
	off << pickedface.size() << endl;
	for (int i = 0; i < pickedface.size(); i++)
	{
		off << pickedface[i].idx() << " ";
	}
	off << endl;

	//��
	off << pickedcell.size() << endl;
	for (int i = 0; i < pickedcell.size(); i++)
	{
		off << pickedcell[i].idx() << " ";
	}
	off << endl;

	off.close();

	return true;
}

bool FileHandler::readpickedall(std::vector<VertexHandle> &pickedvertex, std::vector<EdgeHandle> &pickededge,
	std::vector<FaceHandle> &pickedface, std::vector<CellHandle> &pickedcell)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;

	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	int pickedvertex_size;
	sstr >> pickedvertex_size;
	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	for (int j = 0; j < pickedvertex_size; j++)
	{
		int v_id;
		sstr >> v_id;
		VertexHandle v(v_id);
		pickedvertex.push_back(v);
	}

	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	int pickededge_size;
	sstr >> pickededge_size;

	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	for (int j = 0; j < pickededge_size; j++)
	{
		int e_id;
		sstr >> e_id;
		EdgeHandle e(e_id);
		pickededge.push_back(e);
	}

	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	int pickedface_size;
	sstr >> pickedface_size;
	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	for (int j = 0; j < pickedface_size; j++)
	{
		int f_id;
		sstr >> f_id;
		FaceHandle f(f_id);
		pickedface.push_back(f);
	}

	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	int pickedcell_size;
	sstr >> pickedcell_size;

	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	for (int j = 0; j < pickedcell_size; j++)
	{
		int c_id;
		sstr >> c_id;
		CellHandle c(c_id);
		pickedcell.push_back(c);
	}

	mesh_iff.close();

	return true;
}

/**
 * @description: can only read obj with vertex normal and face with four point
 * @param {*}
 * @return {*}
 */
bool FileHandler::readObj2HexMesh(HexV3fMesh *hexmesh, std::vector<V3f> &vn)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);
	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;
	string s_tmp;
	uint64_t c = 0u;


	int v_arr[8];

	pm_convert_from = hexmesh;

	//*************************
	//header
	bool header_found = true;

	getCleanLine(mesh_iff, line);
	getCleanLine(mesh_iff, line);
	getCleanLine(mesh_iff, line);
	getCleanLine(mesh_iff, line);
	getCleanLine(mesh_iff, line);
	getCleanLine(mesh_iff, line);
	getCleanLine(mesh_iff, line);


	getCleanLine(mesh_iff, line);
	sstr.str(line);
	std::string type;
	uint64_t vertices_num;
	sstr >> type;
	sstr >> type;
	sstr >> vertices_num;

	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	uint64_t faces_num;
	sstr >> type;
	sstr >> type;
	sstr >> faces_num;

	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);

	V3f center(0, 0, 0);

	for (uint64_t i = 0u; i < vertices_num; ++i) {

		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		sstr >> type;
		V3f vn_ = V3f(0.00, 0.00, 0.00);// vertex normal
		sstr >> vn_.x;
		sstr >> vn_.y;
		sstr >> vn_.z;
		vn.push_back(vn_);

		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		sstr >> type;
		V3f v = V3f(0.00, 0.00, 0.00);// vertex point
		sstr >> v.x;
		sstr >> v.y;
		sstr >> v.z;
		pm_convert_from->add_vertex(v);
		center = center + v;
	}
	center = center / vertices_num;
	pm_convert_from->add_vertex(center);

	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);

	int temp_inde;


	for (unsigned int f = 0; f < faces_num; ++f) {
		vector<VertexHandle> temp_vetex;
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		sstr >> type;
		for (int i = 0; i < 4; ++i) {
			sstr >> temp_inde;
			temp_vetex.push_back(VertexHandle(temp_inde - 1));
			sstr >> type;
		}
		for (int i = 0; i < 4; ++i) {
			temp_vetex.push_back(VertexHandle(vertices_num));
		}
		CellHandle cell_now = pm_convert_from->add_cell(temp_vetex);
	}
	mesh_iff.close();

	return true;
}

bool FileHandler::writepickdrawface(const std::string & _filename, const std::vector<int> &_pickdrawface) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}


	// write picked point
	off << _pickdrawface.size() << endl;

	for (int i = 0; i < _pickdrawface.size(); i++)
	{
		off << _pickdrawface[i] << " ";
	}
	off << endl;


	off.close();

	return true;


}

bool FileHandler::readpickdrawface(std::vector<int> &_pickdrawface)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	int pickedface_size;
	sstr >> pickedface_size;

	getCleanLine(mesh_iff, line);
	sstr.clear();
	sstr.str(line);
	for (int j = 0; j < pickedface_size; j++)
	{
		int f_id;
		sstr >> f_id;
		_pickdrawface.push_back(f_id);
	}

	mesh_iff.close();

	return true;
}

bool FileHandler::writebdy(const std::string& _filename, const std::vector<std::vector<std::pair<int, int>>> &m_bdyquick)const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}


	// write picked point
	off << m_bdyquick.size() << endl;

	for (int i = 0; i < m_bdyquick.size(); i++)
	{
		std::vector<std::pair<int, int>> temp = m_bdyquick[i];
		off << temp.size() << endl;
		for (int j = 0; j < temp.size(); ++j)
		{
			off << temp[j].first << " " << temp[j].second << endl;
		}

	}
	off << endl;


	off.close();

	return true;


}

bool FileHandler::readbdy(std::vector<std::vector<std::pair<int, int>>> &m_bdyquick)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;

	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	int bdyquick_size;
	sstr >> bdyquick_size;


	for (int j = 0; j < bdyquick_size; j++)
	{
		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		int tempsize;
		sstr >> tempsize;
		std::vector<std::pair<int, int>> temp;
		for (int i = 0; i < tempsize; ++i)
		{
			getCleanLine(mesh_iff, line);
			sstr.str("");
			sstr.clear();
			sstr.str(line);
			int a, b;
			sstr >> a >> b;
			temp.push_back(pair<int, int>(a, b));
		}
		m_bdyquick.push_back(temp);
	}

	mesh_iff.close();

	return true;
}

/**
 * @description: read triangle vtk? 
 * @param {*}
 * @return {*}
 */
bool FileHandler::readVtk2HexMeshObj1(HexV3fMesh * hexmesh)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;
	string s_tmp;
	uint64_t c = 0u;


	int v_arr[8];

	pm_convert_from = hexmesh;

	//*************************
	//header
	bool header_found = true;

	getCleanLine(mesh_iff, line);
	getCleanLine(mesh_iff, line);
	getCleanLine(mesh_iff, line);
	sstr.str(line);

	// Check header
	sstr >> s_tmp;
	transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "ASCII") {
		//iff.close();
		header_found = false;
		mesh_iff.close();
		std::cerr << "The specified file might not be in .vtk format!" << std::endl;
		return false;
	}
	sstr.clear();

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	sstr.clear();

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	std::string type;
	uint64_t count_point;
	sstr >> type;
	sstr >> count_point;
	sstr >> type;
	V3f center(0, 0, 0);
	for (uint64_t i = 0u; i < count_point; ++i) {

		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		V3f v = V3f(0.00, 0.00, 0.00);
		sstr >> v.x;
		sstr >> v.y;
		sstr >> v.z;
		pm_convert_from->add_vertex(v);
		center = center + v;
	}
	center = center / count_point;
	pm_convert_from->add_vertex(center);

	sstr.clear();
	getCleanLine(mesh_iff, line);
	sstr.str(line);
	//std::string type;
	uint64_t cell_point;
	sstr >> type;
	sstr >> cell_point;
	sstr >> type;

	int temp_inde;


	for (unsigned int f = 0; f < cell_point; ++f) {
		//cout << f << ":" << 62858 << endl;
		vector<VertexHandle> temp_vetex;
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		sstr >> temp_inde;
		for (int i = 0; i < 3; ++i) {
			sstr >> temp_inde;
			temp_vetex.push_back(VertexHandle(temp_inde));
			if (i == 2)
				temp_vetex.push_back(VertexHandle(temp_inde));
		}
		for (int i = 0; i < 4; i++)
			temp_vetex.push_back(VertexHandle(count_point));


		CellHandle cell_now = pm_convert_from->add_cell(temp_vetex);


	}
	mesh_iff.close();

	return true;
}

bool FileHandler::writemaxmin(const std::string& _filename, const double max, const double min)const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}


	// write picked point
	off << max << endl;
	off << min << endl;

	off.close();

	return true;


}

bool FileHandler::readline(std::vector<double> &linecheck)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;
	string s_tmp;

	while (line != "End")
	{
		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		sstr >> s_tmp;
		double a;
		sstr >> a;
		linecheck.push_back(a);
	}

	mesh_iff.close();

	return true;
}

bool FileHandler::writeline(const std::string & _filename, std::vector<double> &linecheck)
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}


	for (int i = 0; i < linecheck.size(); i++)
	{
		if (i != linecheck.size() - 1)
			off << linecheck[i] << ",";
		else
			off << linecheck[i];
	}
	off << endl;


	off.close();

	return true;


}

bool FileHandler::readVtk2HexMeshObj_smooth(HexV3fMesh *hexmesh, std::vector<V3f> &vn)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;
	string s_tmp;
	uint64_t c = 0u;


	int v_arr[8];

	pm_convert_from = hexmesh;

	//*************************
	//header
	bool header_found = true;

	getCleanLine1(mesh_iff, line);
	getCleanLine1(mesh_iff, line);
	getCleanLine1(mesh_iff, line);
	//getCleanLine(mesh_iff, line);

	getCleanLine1(mesh_iff, line);
	sstr.str(line);
	std::string type;
	uint64_t vertices_num;
	sstr >> type;
	sstr >> vertices_num;
	int linenum = vertices_num / 3;
	bool ishad = false;
	int tempnum = vertices_num % 3;
	if (tempnum != 0)
	{
		linenum++;
		ishad = true;
	}

	V3f center(0, 0, 0);

	for (uint64_t i = 0u; i < linenum; ++i) {

		if (i == linenum - 1 && ishad == true)
		{
			getCleanLine1(mesh_iff, line);
			sstr.str("");
			sstr.clear();
			sstr.str(line);
			for (int j = 0; j < tempnum; j++)
			{
				V3f v = V3f(0.00, 0.00, 0.00);
				sstr >> v.x;
				sstr >> v.y;
				sstr >> v.z;
				pm_convert_from->add_vertex(v);
				center = center + v;
			}
		}
		else
		{
			getCleanLine1(mesh_iff, line);
			sstr.str("");
			sstr.clear();
			sstr.str(line);
			for (int j = 0; j < 3; j++)
			{
				V3f v = V3f(0.00, 0.00, 0.00);
				sstr >> v.x;
				sstr >> v.y;
				sstr >> v.z;
				pm_convert_from->add_vertex(v);
				center = center + v;
			}
		}
	}
	center = center / vertices_num;
	pm_convert_from->add_vertex(center);

	getCleanLine1(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	sstr >> type;
	int faces_num;
	sstr >> faces_num;

	int temp_inde;

	for (unsigned int f = 0; f < faces_num; ++f) {
		if (f % 100 == 0)
			std::cout << "face num: " << f << " " << faces_num << endl;
		vector<VertexHandle> temp_vetex;
		getCleanLine1(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		sstr >> type;
		for (int i = 0; i < 4; ++i) {
			sstr >> temp_inde;
			temp_vetex.push_back(VertexHandle(temp_inde));
		}
		for (int i = 0; i < 4; ++i) {
			temp_vetex.push_back(VertexHandle(vertices_num));
		}
		V3f a; V3f b; V3f n;
		a = pm_convert_from->vertices_[temp_vetex[1]] - pm_convert_from->vertices_[temp_vetex[0]];
		b = pm_convert_from->vertices_[temp_vetex[3]] - pm_convert_from->vertices_[temp_vetex[0]];
		n = a % b;
		n = n.normalize();
		//n = n * -1;
		vn.push_back(n);
		CellHandle cell_now = pm_convert_from->add_cell(temp_vetex);
	}
	mesh_iff.close();

	return true;
}

bool FileHandler::readVtk2HexMeshObj_smooth1(HexV3fMesh *hexmesh, std::vector<V3f> &vn)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;
	string s_tmp;
	uint64_t c = 0u;


	int v_arr[8];

	pm_convert_from = hexmesh;

	//*************************
	//header
	bool header_found = true;

	getCleanLine1(mesh_iff, line);
	getCleanLine1(mesh_iff, line);
	getCleanLine1(mesh_iff, line);
	//getCleanLine(mesh_iff, line);

	getCleanLine1(mesh_iff, line);
	sstr.str(line);
	std::string type;
	uint64_t vertices_num;
	sstr >> type;
	sstr >> vertices_num;
	int linenum = vertices_num / 3;
	bool ishad = false;
	int tempnum = vertices_num % 3;
	if (tempnum != 0)
	{
		linenum++;
		ishad = true;
	}

	V3f center(0, 0, 0);

	for (uint64_t i = 0u; i < linenum; ++i) {

		if (i == linenum - 1 && ishad == true)
		{
			getCleanLine1(mesh_iff, line);
			sstr.str("");
			sstr.clear();
			sstr.str(line);
			for (int j = 0; j < tempnum; j++)
			{
				V3f v = V3f(0.00, 0.00, 0.00);
				sstr >> v.x;
				sstr >> v.y;
				sstr >> v.z;
				pm_convert_from->add_vertex(v);
				center = center + v;
			}
		}
		else
		{
			getCleanLine1(mesh_iff, line);
			sstr.str("");
			sstr.clear();
			sstr.str(line);
			for (int j = 0; j < 3; j++)
			{
				V3f v = V3f(0.00, 0.00, 0.00);
				sstr >> v.x;
				sstr >> v.y;
				sstr >> v.z;
				pm_convert_from->add_vertex(v);
				center = center + v;
			}
		}
	}
	center = center / vertices_num;
	pm_convert_from->add_vertex(center);

	getCleanLine1(mesh_iff, line);
	getCleanLine1(mesh_iff, line);
	getCleanLine1(mesh_iff, line);
	getCleanLine1(mesh_iff, line);
	getCleanLine1(mesh_iff, line);
	getCleanLine1(mesh_iff, line);

	getCleanLine1(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	sstr >> type;
	int faces_num;
	sstr >> faces_num;

	int temp_inde;

	for (unsigned int f = 0; f < faces_num; ++f) {
		if (f % 100 == 0)
			std::cout << "face num: " << f << " " << faces_num << endl;
		vector<VertexHandle> temp_vetex;
		getCleanLine1(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		sstr >> type;
		for (int i = 0; i < 4; ++i) {
			sstr >> temp_inde;
			temp_vetex.push_back(VertexHandle(temp_inde));
		}
		for (int i = 0; i < 4; ++i) {
			temp_vetex.push_back(VertexHandle(vertices_num));
		}
		V3f a; V3f b; V3f n;
		a = pm_convert_from->vertices_[temp_vetex[1]] - pm_convert_from->vertices_[temp_vetex[0]];
		b = pm_convert_from->vertices_[temp_vetex[3]] - pm_convert_from->vertices_[temp_vetex[0]];
		n = a % b;
		n = n.normalize();
		//n = n * -1;
		vn.push_back(n);
		CellHandle cell_now = pm_convert_from->add_cell(temp_vetex);
	}
	mesh_iff.close();

	return true;
}

bool FileHandler::writev3fline(const std::string& _filename, HexV3fMesh * hexmesh, std::vector<VertexHandle> &pickpoint_v,
	std::vector<std::vector<VertexHandle>> &pickpoint_vtoline)
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}

	off << pickpoint_v.size() << endl;

	for (int i = 0; i < pickpoint_v.size(); ++i)
	{
		//if (i < 40)
		off << i + 1 << " " << hexmesh->vertices_[pickpoint_v[i]].x << " "
			<< hexmesh->vertices_[pickpoint_v[i]].y << endl;
		//else
			//off << i + 1 << " " << hexmesh->vertices_[pickpoint_v[i]].x << " "
			//<< hexmesh->vertices_[pickpoint_v[i]].y << endl;
	}

	off << pickpoint_vtoline.size() << endl;

	int count = 1;
	for (int i = 0; i < pickpoint_vtoline.size(); ++i)
	{
		off << pickpoint_vtoline[i].size() << endl;
		for (int j = 0; j < pickpoint_vtoline[i].size(); ++j)
		{
			//if(i == 0)
			off << j + 1 << " " << hexmesh->vertices_[pickpoint_vtoline[i][j]].x << " "
				<< hexmesh->vertices_[pickpoint_vtoline[i][j]].y << " " << count++ << endl;
			//else
				//off << j + 1 << " " << hexmesh->vertices_[pickpoint_vtoline[i][j]].x << " "
				//<< hexmesh->vertices_[pickpoint_vtoline[i][j]].y << " " << count++ << endl;
		}
	}



	off.close();

	return true;


}

bool FileHandler::writecsv(const std::string & _filename, const std::vector<VertexHandle>& csv, HexV3fMesh *hexmesh) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}


	// write picked point
	off << csv.size() << endl;

	for (int i = 0; i < csv.size(); i++)
	{
		off << csv[i].idx() << " ";
		off << hexmesh->vertices_[csv[i]].x << " " << hexmesh->vertices_[csv[i]].y
			<< " " << hexmesh->vertices_[csv[i]].z << endl;
	}
	off << endl;


	off.close();

	return true;


}

bool FileHandler::writecsv(const std::string & _filename, const std::vector<VertexHandle>& csv) const
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}


	// write picked point
	off << csv.size() << endl;

	for (int i = 0; i < csv.size(); i++)
	{
		off << csv[i].idx() << " ";
	}
	off << endl;


	off.close();

	return true;


}

bool FileHandler::readcsv(std::vector<VertexHandle>& csv, std::vector<V3f> &csv3f)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	int csv_size;
	sstr >> csv_size;

	for (int j = 0; j < csv_size; j++)
	{
		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		int csv_id;
		sstr >> csv_id;
		VertexHandle c(csv_id);
		csv.push_back(c);
		V3f v;
		sstr >> v.x; sstr >> v.y; sstr >> v.z;
		csv3f.push_back(v);
	}

	mesh_iff.close();

	return true;
}

bool FileHandler::readcsv(std::vector<VertexHandle>& csv)
{
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;

	getCleanLine(mesh_iff, line);
	sstr.str(line);
	int csv_size;
	sstr >> csv_size;

	getCleanLine(mesh_iff, line);
	sstr.clear();
	sstr.str(line);
	for (int j = 0; j < csv_size; j++)
	{
		int csv_id;
		sstr >> csv_id;
		VertexHandle c(csv_id);
		csv.push_back(c);
	}

	mesh_iff.close();

	return true;
}

/**
 * @description: read mesh core
 * @param {*}
 * @return {*}
 */
bool FileHandler::read2HexMeshObj_new(HexV3fMesh *hexmesh)
{
	if (!isReading) {
		return false;
	}
	this->pm_convert_from = hexmesh;
	// mesh_iff应该是存储着网格
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;
	string s_tmp;
	uint64_t c = 0u;

	int v_arr[8];

	pm_convert_from = hexmesh;

	while (1)
	{
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		string::size_type idx;
		idx = line.find("Vertices");
		if (idx != string::npos)
		{
			cout << line << endl;
			if (line == "Vertices")
			{
				getCleanLine(mesh_iff, line);
				sstr.clear();
				sstr.str(line);
				sstr >> c;
				break;
			}
			else
			{
				sstr >> s_tmp;
				sstr >> c; // c 中存储着顶点个数
				break;
			}
		}
	}
	// 3 维顶点 有4个数字 最后一个数字是有效数字，如果是无效的那么这个点
	for (uint64_t i = 0u; i < c; ++i)
	{
		int check;
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		V3f v = V3f(0.00, 0.00, 0.00);
		sstr >> v.x;
		sstr >> v.y;
		sstr >> v.z;
		sstr >> check;
		if (check == 0)
			v.is_valid = false;
		pm_convert_from->add_vertex(v);
	}
	// 读取cell个数
	while (true)
	{
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		string::size_type idx;
		idx = line.find("Hexahedra");
		if (idx != string::npos)
		{
			cout << line << endl;
			if (line == "Hexahedra")
			{
				getCleanLine(mesh_iff, line);
				sstr.clear();
				sstr.str(line);
				sstr >> c;
				break;
			}
			else
			{
				sstr >> s_tmp;
				sstr >> c;
				break;
			}
			break;
		}
	}
	int temp_inde = 0;
	for (unsigned int f = 0; f < c; ++f) {
		if (f % 1000 == 0)
			cout << f << ":" << c << endl;
		vector<VertexHandle> temp_vetex;
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		for (int i = 0; i < 8; ++i) {
			sstr >> temp_inde;
			temp_vetex.push_back(VertexHandle(temp_inde - 1));
		}
		CellHandle cell_now = pm_convert_from->add_cell(temp_vetex);
	}
	mesh_iff.close();
	return true;
}
/**
 * @description: read bezier solid
 * @param {*}
 * @return {*}
 */
bool FileHandler::readbezier(BSplineSolid &m_solids, std::vector<std::vector<double>> &m_bdycoef)
{
	BSplineSolid m_solids1;
	m_solids = m_solids1;
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;

	getCleanLine(mesh_iff, line);
	sstr.str("");
	sstr.clear();
	sstr.str(line);
	int bezier_size;
	sstr >> bezier_size;
	m_solids.Control_Point.resize(bezier_size);
	m_solids.Local_V3f.resize(bezier_size);
	m_solids.Local_to_Global.resize(bezier_size);
	m_solids.Local_Order.resize(bezier_size);
	m_solids.Local_Node.resize(bezier_size);
	m_bdycoef.resize(bezier_size);
	//SolidData.resize(bezier_size);

	for (int j = 0; j < bezier_size; j++)
	{
		if (j > 0)
		{
			getCleanLine(mesh_iff, line);
			sstr.str("");
			sstr.clear();
			sstr.str(line);
		}
		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		int u_id, v_id, w_id;
		sstr >> u_id >> v_id >> w_id;//u,v,w����Ĵ���
		m_solids.Local_Order[j].push_back(u_id);
		m_solids.Local_Order[j].push_back(v_id);
		m_solids.Local_Order[j].push_back(w_id);

		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		int u_con_num, v_con_num, w_con_num;//���Ƶ����
		sstr >> u_con_num >> v_con_num >> w_con_num;
		m_solids.Control_Point[j].resize(w_con_num);
		//SolidData[j].resize(w_con_num);
		for (int i = 0; i < w_con_num; ++i)
		{
			m_solids.Control_Point[j][i].resize(v_con_num);
			//SolidData[j][i].resize(v_con_num);
		}
		for (int i = 0; i < w_con_num; ++i)
			for (int k = 0; k < v_con_num; ++k)
			{
				m_solids.Control_Point[j][i][k].resize(u_con_num);
				//SolidData[j][i][k].resize(u_con_num);
			}

		m_solids.Local_Node[j].resize(3);//�ڵ�����
		//u����
		int node = u_id + u_con_num + 1;
		m_solids.Local_Node[j][0].resize(node);
		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		for (int n_i = 0; n_i < node; ++n_i)
			sstr >> m_solids.Local_Node[j][0][n_i];
		//v����
		node = v_id + v_con_num + 1;
		m_solids.Local_Node[j][1].resize(node);
		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		for (int n_i = 0; n_i < node; ++n_i)
			sstr >> m_solids.Local_Node[j][1][n_i];
		//w����
		node = w_id + w_con_num + 1;
		m_solids.Local_Node[j][2].resize(node);
		getCleanLine(mesh_iff, line);
		sstr.str("");
		sstr.clear();
		sstr.str(line);
		for (int n_i = 0; n_i < node; ++n_i)
			sstr >> m_solids.Local_Node[j][2][n_i];

		int v_num = 0;
		for (int u = u_con_num - 1; u >= 0; u--)
			for (int v = 0; v < v_con_num; v++)
				for (int w = w_con_num - 1; w >= 0; w--)
					/*for (int w = 0; w < w_con_num; w++)
						for (int v = v_con_num - 1; v >= 0; v--)
							for (int u = 0; u < u_con_num; u++)*/
				{
					getCleanLine(mesh_iff, line);
					sstr.str("");
					sstr.clear();
					sstr.str(line);
					V3f control_point;//���Ƶ�
					sstr >> control_point.x >> control_point.y >> control_point.z;
					m_solids.Local_V3f[j].push_back(control_point);
					double cal;
					sstr >> cal;
					m_bdycoef[j].push_back(cal);
					m_solids.Control_Point[j][w][v][u] = v_num++;
					//SolidData[j][w][v][u] = control_point;
				}
	}

	mesh_iff.close();

	return true;
}

bool FileHandler::readbeziernew(std::vector<double> &m_color, HexV3fMesh * hexmesh)
{
	if (!isReading) {
		return false;
	}
	this->pm_convert_from = hexmesh;

	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;
	string s_tmp;
	uint64_t c = 0u;


	int v_arr[8];

	pm_convert_from = hexmesh;

	getCleanLine(mesh_iff, line);
	sstr.clear();
	sstr.str(line);
	getCleanLine(mesh_iff, line);
	sstr.clear();
	sstr.str(line);
	sstr >> c;

	for (uint64_t i = 0u; i < c; ++i)
	{
		double col;
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		V3f v = V3f(0.00, 0.00, 0.00);
		sstr >> v.x;
		sstr >> v.y;
		sstr >> v.z;
		sstr >> col;
		pm_convert_from->add_vertex(v);
		m_color.push_back(col);
	}

	getCleanLine(mesh_iff, line);
	sstr.clear();
	sstr.str(line);
	getCleanLine(mesh_iff, line);
	sstr.clear();
	sstr.str(line);
	sstr >> c;

	int temp_inde = 0;
	for (unsigned int f = 0; f < c; ++f) {
		vector<VertexHandle> temp_vetex;
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);

		for (int i = 0; i < 8; ++i) {
			sstr >> temp_inde;
			temp_vetex.push_back(VertexHandle(temp_inde - 1));
		}


		CellHandle cell_now = pm_convert_from->add_cell(temp_vetex);


	}
	mesh_iff.close();

	return true;
}

bool FileHandler::writeXJ(const std::string& _filename, HexV3fMesh * _mesh, std::vector<double> &m_color)
{
	std::ofstream off(_filename.c_str(), std::ios::out);

	if (!off.good()) {
		std::cerr << "Error: Could not open file " << _filename << " for writing!" << std::endl;
		off.close();
		return false;
	}

	// Write header
	off << "MeshVersionFormatted 1" << std::endl;
	off << "Dimension 3" << std::endl;
	uint64_t n_vertices(_mesh->vertices_.size());
	off << "Vertices" << std::endl;
	off << n_vertices << std::endl;

	// write vertices
	for (uint64_t v_it = 0; v_it < n_vertices; ++v_it) {

		off << _mesh->vertices_[v_it].x << " "
			<< _mesh->vertices_[v_it].y << " "
			<< _mesh->vertices_[v_it].z << " "
			<< m_color[v_it] << " ";
		if (_mesh->vertices_[v_it].is_valid == true)
			off << 1 << " ";
		else
			off << 0 << " ";
		off << "-1" << endl;
	}
	uint64_t n_faces(_mesh->faces_.size());
	off << "Quadrilaterals" << std::endl;
	off << n_faces << std::endl;

	for (uint64_t c_it = 0; c_it < n_faces; ++c_it) {


		std::vector<VertexHandle> ves = _mesh->faces_[c_it].vs_f;

		for (std::vector<VertexHandle>::iterator it_ves = ves.begin(); it_ves
			!= ves.end(); ++it_ves) {
			off << ((it_ves->idx()) + 1) << " ";

		}
		off << "1" << std::endl;
	}

	uint64_t n_cells(_mesh->cells_.size());
	off << "Hexahedra" << std::endl;
	uint32_t cout_ = 0;
	for (uint64_t c_it = 0; c_it < n_cells; ++c_it) {
		if (_mesh->cells_[c_it].is_valid) ++cout_;
	}
	off << cout_ << std::endl;

	for (uint64_t c_it = 0; c_it < n_cells; ++c_it) {
		if (!_mesh->cells_[c_it].is_valid) continue;

		std::vector<VertexHandle> ves = _mesh->cells_[c_it].vertex_;

		for (std::vector<VertexHandle>::iterator it_ves = ves.begin(); it_ves
			!= ves.end(); ++it_ves) {
			off << ((it_ves->idx()) + 1) << " ";

		}
		off << "1" << std::endl;
	}


	off << "End" << std::endl;
	off.close();

	return true;
}



/**************************************************
@brief   : GU��Ӧ������������ÿ�������ɫ
@author  : lee
@input   ��none
@output  ��none
@time    : none
**************************************************/
bool FileHandler::readXJ(HexV3fMesh * _mesh, std::vector<double> &m_color)
{
	if (!isReading) {
		return false;
	}
	this->pm_convert_from = _mesh;

	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);

	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}

	stringstream sstr;
	string line;
	string s_tmp;
	uint64_t c = 0u;


	int v_arr[8];

	pm_convert_from = _mesh;

	while (1)
	{
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		string::size_type idx;
		idx = line.find("Vertices");
		if (idx != string::npos)
		{
			cout << line << endl;
			if (line == "Vertices")
			{
				getCleanLine(mesh_iff, line);
				sstr.clear();
				sstr.str(line);
				sstr >> c;
				break;
			}
			else
			{
				sstr >> s_tmp;
				sstr >> c;
				break;
			}
		}
	}
	m_color.resize(c);
	for (uint64_t i = 0u; i < c; ++i)
	{
		if (i % 5000 == 0)
			cout << i << ":" << c << endl;
		int check;
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		V3f v = V3f(0.00, 0.00, 0.00);
		sstr >> v.x;
		sstr >> v.y;
		sstr >> v.z;
		sstr >> m_color[i];
		sstr >> check;
		if (check == 0)
			v.is_valid = false;
		pm_convert_from->add_vertex(v);
	}

	//sort(m_color.begin(), m_color.end());
	//std::cout << "MIN" << m_color[0] << " MAX" << m_color[m_color.size() - 1] << std::endl;

	while (true)
	{
		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);
		string::size_type idx;
		idx = line.find("Hexahedra");
		if (idx != string::npos)
		{
			cout << line << endl;
			if (line == "Hexahedra")
			{
				getCleanLine(mesh_iff, line);
				sstr.clear();
				sstr.str(line);
				sstr >> c;
				break;
			}
			else
			{
				sstr >> s_tmp;
				sstr >> c;
				break;
			}
			break;
		}
	}

	int temp_inde = 0;
	for (unsigned int f = 0; f < c; ++f) {
		if (f % 5000 == 0)
			cout << f << ":" << c << endl;
		vector<VertexHandle> temp_vetex;

		getCleanLine(mesh_iff, line);
		sstr.clear();
		sstr.str(line);

		for (int i = 0; i < 8; ++i) {
			sstr >> temp_inde;
			temp_vetex.push_back(VertexHandle(temp_inde - 1));
		}


		CellHandle cell_now = pm_convert_from->add_cell(temp_vetex);


	}
	mesh_iff.close();

	return true;
}


bool FileHandler::readSkeltonSKEL(OurSkel* skelfile) {
	OurSkel tempskel;
	if (!isReading)	return false;
	mesh_iff = ifstream(filepath_checked.c_str(), ios::in);
	if (!mesh_iff.good()) {
		std::cerr << "Error: Could not open file " << filepath_checked << " for reading!" << std::endl;
		mesh_iff.close();
		return false;
	}
	stringstream sstr;
	string line;
	string s_tmp;
	int nVertices;
	// Check header
	getCleanLine(mesh_iff, line);//读一行，预处理（去掉首尾的空格）
	bool check = false;
	if (line == "ID Cx Cy Cz Length Width Height #NEIGHBORS NEIGHBORS_LIST") {
		check = true;
	}
	getCleanLine(mesh_iff, line);//读一行，预处理（去掉首尾的空格）
	sstr.str(line);//这一行转换成字符串形式
	//sstr >> s_tmp;//这一串字符赋值给s_tmp去操作
	nVertices = std::atoi(sstr.str().c_str());
	std::cout << "读入skel头文件标记的节点总数  " << nVertices << std::endl;
	sstr.clear();
	tempskel.SkelAll.resize(nVertices);
	int countLine = 0;
	while (!mesh_iff.eof()) {
		if (countLine % 10 == 0) {
			std::cout << "index " << countLine << "\n";
		}
		getline(mesh_iff, line);
		if (line.length() < 1) continue;
		double x, y, z, tempradius;
		int tempdegree;
		//存放分割后的字符串
		vector<string> res;
		string word = line.c_str();
		//暂存从word中读取的字符串 
		string result;
		//将字符串读到inp输入流中 
		stringstream inp(word);
		while (inp >> result)
			res.push_back(result);
		int idx = atoi(res[0].c_str());
		if (countLine != idx) {
			std::cout << "the read line's idx not equal to the top of line " << std::endl;
			mesh_iff.close();
			return false;
		}
		x = atof(res[1].c_str());
		y = atof(res[2].c_str());
		z = atof(res[3].c_str());
		tempradius = atof(res[4].c_str());
		if (check) {
			tempradius += atof(res[5].c_str());
			tempradius += atof(res[6].c_str());
			tempdegree = atoi(res[7].c_str());
		}
		else {
			tempdegree = atoi(res[5].c_str());
		}
		
		tempskel.SkelAll[idx].index = idx;
		tempskel.SkelAll[idx].pos = V3f(x, y, z);
		tempskel.SkelAll[idx].is_valid = true;
		tempskel.SkelAll[idx].radius = tempradius;
		tempskel.SkelAll[idx].Indegree = tempdegree;
		vector<int> neires;
		if (check) {
			for (int i = 1; i <= tempdegree; i++) {
				neires.push_back(atoi(res[7 + i].c_str()));
			}
		}
		else {
			for (int i = 1; i <= tempdegree; i++) {
				neires.push_back(atoi(res[5 + i].c_str()));
			}
		}

		tempskel.SkelAll[idx].neighbor.assign(neires.begin(), neires.end());
		++countLine;
	}
	mesh_iff.close();
	tempskel.CalBoundingBox();

	skelfile->SkelAll = tempskel.SkelAll;
	skelfile->r = tempskel.r;
	skelfile->center = tempskel.center;

	return true;
}
