#include "ObjLoader.h"

bool loadObj(const string &objName) {
	ifstream in(objName);
	if (in.fail)
	{
		std::cout << "Wrong : could not open your obj file. Please Check your file path.\n";
		return false;
	}


}