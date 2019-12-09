#include "ScanlineZBuffer.h"

void ScanlineZBufferProcessor::initProcessor(Object &_obj) {
	obj = _obj;
	CPT.resize(WINDOW_HEIGHT);
	CET.resize(WINDOW_HEIGHT);
}

//build classified polygon table
void ScanlineZBufferProcessor::buildCPT() {
	cout << "obj.faces.size()：" << obj.faces.size() << endl;
	for (int i = 0; i < obj.faces.size(); i++) {
		int maxY;
		GLfloat maxZ;
		GLfloat a, b, c, d;
		int dy;
		vec3 color;
		
		obj.CalFace(i, a, b, c, d, maxY, maxZ, dy, color);
		obj.CalFaceEdges(i);
		ClassifiedPolygon newCP(i, a, b, c, d, dy, color, nullptr);

		cout << "maxY:" << maxY << endl;
		CPT[maxY].push_back(newCP);
	}
}


void ScanlineZBufferProcessor::buildCET() {
	cout << "obj.faces.size()：" << obj.faces.size() << endl;
	for (int i = 0; i < obj.faces.size(); i++) {
		obj.CalFaceEdges(i);
	}
	for (auto edge : obj.edges) {
		int maxY = edge.maxY;
		CET[maxY].push_back(edge);
	}
}


bool ScanlineZBufferProcessor::addPolygonToAPT(int maxY, vector<ActivePolygon> &APT, vector<vector<ClassifiedPolygon>> &CPT) {
	for (int i = 0; i < CPT[maxY].size(); i++){
		ClassifiedPolygon cp = CPT[maxY][i];
		ActivePolygon ap(cp.polygon_id, cp.a, cp.b, cp.c, cp.d, maxY, cp.color, NULL);
		APT.push_back(ap);
	}
	//just for debug
	cout << "----------------scanline y = " << maxY << "---------------------------"<< endl;
	return true;
}


//如果有新的多边形加入到活化多边形表中，则把该多边形在oxy平面上的投影和扫描线相交的边加入到活化边表中
bool ScanlineZBufferProcessor::addEdgeToAET(vector<ClassifiedEdge>& CET_y, vector<ActiveEdge>& AET, vector<ClassifiedPolygon>& CPT_y){
	int leftflag(0), rightflag(0);
	for (int p = 0; p < CPT_y.size(); p++) {
		//TODO:remember following delete debug part, CET number wrong!!! 
		cout << "CPT_y.size(): " << CPT_y.size() << endl;
		cout << "CET_y.size(): " << CET_y.size() << endl;
	 
		for (int i = 0; i < CET_y.size(); i++) {
			if (CET_y[i].edge_polygon_id == CPT_y[p].polygon_id) {	//找到属于该多边形的边
				//找到左边和右边组成边对
				for (int edge = 1; edge < CET_y.size(); edge++) {//一个多边形肯定有边的，从第二条边开始比较
					if (CET_y[edge].isLeftEdge(CET_y[leftflag])) {//更新左标志
						leftflag = edge;
					}
					if (CET_y[rightflag].isLeftEdge(CET_y[edge])) {//更新右标志
						rightflag = edge;
					}
				}
				if (leftflag == rightflag) {
					return false;
				}
				else {
					ActiveEdge AE(CET_y[leftflag], CET_y[rightflag], CPT_y[p]);
					AET.push_back(AE);
					return true;
				}
			}
		}
	}
	return false;
}


void ScanlineZBufferProcessor::ScanlineZBuffer(Object &_obj) {
	initProcessor(_obj);
	buildCPT();
	buildCET();
	cout << "minY: " << obj.minY << endl;
	cout << "maxY: " << obj.maxY << endl;

	for (int y = obj.maxY; y >= obj.minY; y--) {	//每一行扫描线
		zbuffer.resize(WINDOW_WIDTH, -INT_MAX);//离视点最远
		coloridbuffer.resize(WINDOW_WIDTH, -1);//背景颜色（索引设为-1）
		//检查分类多边形表，如果有新的多边形涉及该扫描线，把它放入活化多边形表中
		if (CET[y].empty()) {
			continue;
		}
		else {
			if (addPolygonToAPT(y, APT, CPT)) {
				addEdgeToAET(CET[y], AET, CPT[y]);
			}
		}
		printAPT(APT);
		printAET(AET);
	}
	//clear();
}


void ScanlineZBufferProcessor::resizeWindow(int width, int height) {
	winHeight = height;
	winWidth = width;
	CPT.clear();
	CET.clear();
	CPT.resize(height);
	CET.resize(height);
	zbuffer.clear();
	//TODO:Check zbuffer size
	zbuffer.resize(width);
}


void clear() {
}


//just for debug:
void ScanlineZBufferProcessor::test() {
	cout << "CET size:" << CET.size() << endl;
	cout << "CPT size:" << CPT.size() << endl;
}


void ScanlineZBufferProcessor::printAET(vector<ActiveEdge> &AET) {
	cout << "AET size:" << AET.size() << "\n";
	cout << "AET edge_polygon_id:  ";
	for (int i = 0; i < AET.size(); i++) {
		cout << AET[i].edge_polygon_id << "\t";
	}
	cout << endl;
	cout << "AET dxl & dxr:  \n";
	for (int i = 0; i < AET.size(); i++) {
		cout << AET[i].dxl << "\t" << AET[i].dxr <<endl;
	}
	cout << endl;
}

void ScanlineZBufferProcessor::printAPT(vector<ActivePolygon> &APT) {
	cout << "APT size:" << APT.size() << "\n";
	cout << "APT polygon_id:  ";
	for (int i = 0; i < APT.size(); i++) {
		cout << APT[i].polygon_id << "\t";
	}
	cout << endl;
}