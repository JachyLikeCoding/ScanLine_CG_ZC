#include "ScanlineZBuffer.h"

void ScanlineZBufferProcessor::initProcessor(Object &_obj) {
	obj = _obj;
	CPT.resize(WINDOW_HEIGHT);
	CET.resize(WINDOW_HEIGHT);
}

//build classified polygon table
void ScanlineZBufferProcessor::buildCPT() {
	cout << "obj.faces.size()��" << obj.faces.size() << endl;
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
	cout << "obj.faces.size()��" << obj.faces.size() << endl;
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


//������µĶ���μ��뵽�����α��У���Ѹö������oxyƽ���ϵ�ͶӰ��ɨ�����ཻ�ı߼��뵽��߱���
bool ScanlineZBufferProcessor::addEdgeToAET(vector<ClassifiedEdge>& CET_y, vector<ActiveEdge>& AET, vector<ClassifiedPolygon>& CPT_y){
	int leftflag(0), rightflag(0);
	for (int p = 0; p < CPT_y.size(); p++) {
		//TODO:remember following delete debug part, CET number wrong!!! 
		cout << "CPT_y.size(): " << CPT_y.size() << endl;
		cout << "CET_y.size(): " << CET_y.size() << endl;
	 
		for (int i = 0; i < CET_y.size(); i++) {
			if (CET_y[i].edge_polygon_id == CPT_y[p].polygon_id) {	//�ҵ����ڸö���εı�
				//�ҵ���ߺ��ұ���ɱ߶�
				for (int edge = 1; edge < CET_y.size(); edge++) {//һ������ο϶��бߵģ��ӵڶ����߿�ʼ�Ƚ�
					if (CET_y[edge].isLeftEdge(CET_y[leftflag])) {//�������־
						leftflag = edge;
					}
					if (CET_y[rightflag].isLeftEdge(CET_y[edge])) {//�����ұ�־
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

	for (int y = obj.maxY; y >= obj.minY; y--) {	//ÿһ��ɨ����
		zbuffer.resize(WINDOW_WIDTH, -INT_MAX);//���ӵ���Զ
		coloridbuffer.resize(WINDOW_WIDTH, -1);//������ɫ��������Ϊ-1��
		//���������α�������µĶ�����漰��ɨ���ߣ�������������α���
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