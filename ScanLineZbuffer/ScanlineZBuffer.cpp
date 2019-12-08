#include "ScanlineZBuffer.h"

void ScanlineZBufferProcessor::initProcessor(Object &_obj) {
	obj = _obj;
	CPT.resize(WINDOW_HEIGHT, NULL);
	CET.resize(WINDOW_HEIGHT, NULL);
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
		ClassifiedPolygon *newCP = new ClassifiedPolygon(i, a, b, c, d, dy, color, nullptr);

		cout << "maxY:" << maxY << endl;

		if (CPT[maxY] == NULL) {
			CPT[maxY] = newCP;
		}
		else
		{
			newCP->nxtPolygon = CPT[maxY];
			CPT[maxY] = newCP;
		}
	}
}


void ScanlineZBufferProcessor::buildCET() {
	cout << "obj.faces.size()��" << obj.faces.size() << endl;
	for (int i = 0; i < obj.faces.size(); i++) {
		obj.CalFaceEdges(i);
	}
	for (auto edge : obj.edges) {
		if (CET[edge->maxY] == NULL) {
			CET[edge->maxY] = edge;
		}
		else {
			edge->nxtEdge = CET[edge->maxY];
			CET[edge->maxY] = edge;
		}
	}
}

void ScanlineZBufferProcessor::buildAET(int y) {

}

void ScanlineZBufferProcessor::buildAPT(int y) {
	ActivePolygon *ap = NULL;
	
}

void ScanlineZBufferProcessor::updateZBuffer(int y) {

}

void clear() {
	

}

void addPolygonToAPT(int maxY, vector<ActivePolygon *> &APT, vector<ClassifiedPolygon *> &CPT) {
	ClassifiedPolygon *cp;
	while (CPT[maxY] != NULL) {
		cp = CPT[maxY];
		ActivePolygon *ap = new ActivePolygon(cp->polygon_id, cp->a, cp->b, cp->c, cp->d, maxY, cp->color, NULL);
		APT.push_back(ap);
		CPT[maxY] = CPT[maxY]->nxtPolygon;
	}
	//just for debug
	cout << "----------------scanline y = " << maxY << endl;
	cout << "APT.size: " << APT.size() << endl;
	for (int i = 0; i < APT.size(); i++) {
		cout << APT[i]->polygon_id << ", ";
	}
	cout << endl;
}


//������µĶ���μ��뵽�����α��У���Ѹö������oxyƽ���ϵ�ͶӰ��ɨ�����ཻ�ı߼��뵽��߱���
void addEdgeToAET(vector<ClassifiedEdge> &CET_y,  vector<ActiveEdge> &AET, vector<ClassifiedPolygon> &CPT_y) {
	for (int i = 0; i < CPT_y.size(); i++) {
		addEdge( AET, CET_y, CPT_y[i]);
	}
}

void addEdge(vector<ActiveEdge> &AET, vector<ClassifiedEdge> &CET_y, ClassifiedPolygon &CP) {
	for (int i = 0; i < CET_y.size(); i++) {
		if (CET_y[i].edge_polygon_id == CP.polygon_id) {	//�ҵ����ڸö���εı�
			//TODO: ����д��ô����ߣ�
		}
	}
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
		if (CET[y] == NULL) {
			continue;
		}
		else {
			addPolygonToAPT(y, APT, CPT);
			addActivatedEdgeTable(CET[y], AET, CPT[y]);
		}

	}

	//clear();
}


void ScanlineZBufferProcessor::resizeWindow(int width, int height) {
	winHeight = height;
	winWidth = width;
	CPT.clear();
	CET.clear();
	CPT.resize(height, NULL);
	CET.resize(height, NULL);
	zbuffer.clear();
	//TODO:Check zbuffer size
	zbuffer.resize(height * width, NULL);
}


//just for debug:
void ScanlineZBufferProcessor::test() {
	cout << "CET size:" << CET.size() << endl;
	cout << "CPT size:" << CPT.size() << endl;
}