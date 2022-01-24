#include "HexMeshViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	HexMeshViewer w;
	w.setWindowIcon(QIcon("./Images/Mesh.ico"));
	w.show();
	return a.exec();
}
