#include "Basic.h"
#include "ObjLoader.h"
#include "DataStructure.h"
#include "display.h"

int main(int argc, char* argv[]) {
	string inputfile = "models/test.obj";
	Object obj;
	bool flag;
	flag = obj.loadObj(inputfile);
	obj.test();
	cout << "load success? " << flag << endl;

	//display(argc, argv);

	system("pause");
	return 0;
}