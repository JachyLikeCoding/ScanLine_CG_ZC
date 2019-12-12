#include "ScanlineZBuffer.h"

void ScanlineZBufferProcessor::initProcessor(Object &_obj) {
	obj = _obj;
	CPT.clear();
	CET.clear();
	CPT.resize(WINDOW_HEIGHT);
	CET.resize(WINDOW_HEIGHT);
	APT.clear();
	AET.clear();
	framebuffer.resize(3 * WINDOW_HEIGHT * WINDOW_WIDTH, 0);
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
		ClassifiedPolygon newCP(i, a, b, c, d, dy, color);

		cout << "maxY:" << maxY << endl;
		CPT[maxY].push_back(newCP);
	}
}


void ScanlineZBufferProcessor::buildCET() {
	cout << "obj.faces.size()：" << obj.faces.size() << endl;
	/*for (int i = 0; i < obj.faces.size(); i++) {
		obj.CalFaceEdges(i);
	}*/
	for (int i = 0; i < obj.edges.size(); i++) {
		int maxY = obj.edges[i].maxY;
		CET[maxY].push_back(obj.edges[i]);
	}
}


bool ScanlineZBufferProcessor::addPolygonToAPT(int maxY, vector<ActivePolygon> &APT, vector<vector<ClassifiedPolygon>> &CPT) {
	for (int i = 0; i < CPT[maxY].size(); i++){
		ClassifiedPolygon cp = CPT[maxY][i];
		ActivePolygon ap(cp.polygon_id, cp.a, cp.b, cp.c, cp.d, maxY, cp.color);
		APT.push_back(ap);
	}
	//just for debug
	cout << "----------------scanline y = " << maxY << "---------------------------"<< endl;
	return true;
}


//如果有新的多边形加入到活化多边形表中，则把该多边形在oxy平面上的投影和扫描线相交的边加入到活化边表中
void ScanlineZBufferProcessor::addEdgeToAET(vector<ClassifiedEdge>& CET_y, vector<ActiveEdge>& AET, vector<ClassifiedPolygon>& CPT_y){
	cout << "CPT_y.size(): " << CPT_y.size() << endl;
	cout << "CET_y.size(): " << CET_y.size() << endl;

	for (int p = 0; p < CPT_y.size(); p++) {
		int leftflag(-1), rightflag(-1);
		for (int i = 0; i < CET_y.size(); i++) {
			if (CET_y[i].edge_polygon_id == CPT_y[p].polygon_id) {	//找到属于该多边形的边
				//找到左边和右边组成边对
				cout << "CET_y[" << i << "].edge_polygon_id == CPT_y[" << p << "].polygon_id = " << CET_y[i].edge_polygon_id << endl;
				if ((leftflag == -1) || (rightflag == -1)) {
					leftflag = rightflag = i;
				}
				else {
					cout << "CET_y[i].edge_polygon_id:" << CET_y[i].edge_polygon_id << endl;
					cout << "CET_y[leftflag].edge_polygon_id:" << CET_y[leftflag].edge_polygon_id << endl;
					if (CET_y[i].isLeftEdge(CET_y[leftflag])) {//更新左标志
						leftflag = i;
					}
					if (CET_y[rightflag].isLeftEdge(CET_y[i])) {//更新右标志
						rightflag = i;
					}
				}
			}
		}
		if (leftflag != rightflag) {
			ActiveEdge AE(CET_y[leftflag], CET_y[rightflag], CPT_y[p]);
			AET.push_back(AE);
		}
		else {
			//cout << "没有加边对!" << endl;
		}
	}
}


void ScanlineZBufferProcessor::ScanlineZBuffer(Object &_obj) {
	initProcessor(_obj);
	buildCPT();
	buildCET();
	cout << "minY: " << obj.minY << endl;
	cout << "maxY: " << obj.maxY << endl;
	//计时
	time_t t_start = clock();

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
			updateBuffer(AET,y);
			//update APT & AET
			update_APTAET(APT, AET, CET[y]);
		}
		printAPT(APT);
		printAET(AET);
	}

	time_t t_end = clock();
	GLfloat deltaT = (t_end - t_start + 0.0) / CLOCKS_PER_SEC;
	/*system("cls");*/
	cout << "模型顶点数: " << obj.vertexes.size() << endl;
	cout << "模型面片数：" << obj.faces.size() << endl;
	cout << "扫描线Z-Buffer算法耗时：" << deltaT << endl;
	//clear data
	clearDS();
}

void  ScanlineZBufferProcessor::update_APTAET(vector<ActivePolygon> &APT, vector<ActiveEdge> &AET, vector<ClassifiedEdge> &CET_y) {
	for (int i = 0; i < AET.size(); i++) {
		APT[i].remain_dy -= 1;
		if ((APT[i].remain_dy < 0)) {//delete the polygon and its edge pair
			APT.erase(APT.begin() + i);
			AET.erase(AET.begin() + i);
			i -= 1;
		}
		else {//update AET
			  //should find another left edge
			if ((--AET[i].dyl) < 0) {
				for (int e = 0; e < CET_y.size(); e++) {
					if ((CET_y[e].edge_polygon_id == AET[i].edge_polygon_id) && (abs(CET_y[e].x - AET[i].xl)) < 1) {
						AET[i].xl = CET_y[e].x;
						AET[i].dxl = CET_y[e].dx;
						AET[i].dyl = CET_y[e].dy;
					}
				}
			}
			//should find another right edge
			if ((--AET[i].dyr) < 0) {
				for (int e = 0; e < CET_y.size(); e++) {
					if ((CET_y[e].edge_polygon_id == AET[i].edge_polygon_id) && (abs(CET_y[e].x - AET[i].xr)) < 1) {
						AET[i].xr = CET_y[e].x;
						AET[i].dxr = CET_y[e].dx;
						AET[i].dyr = CET_y[e].dy;
					}
				}
			}
			//update AET
			AET[i].xl += AET[i].dxl;
			AET[i].xr += AET[i].dxr;
			AET[i].zl += (AET[i].dzx * AET[i].dxl + AET[i].dzy);
		}
	}
}


void ScanlineZBufferProcessor::resizeWindow(int width, int height) {
	winHeight = height;
	winWidth = width;
	CPT.clear();
	CET.clear();
	CPT.resize(height);
	CET.resize(height);
	zbuffer.clear();
	zbuffer.resize(width);
}

//update z-buffer and color buffer
void ScanlineZBufferProcessor::updateBuffer(vector<ActiveEdge> &AET, int y) {
	for (int i = 0; i < AET.size(); i++) {
		GLfloat zx = AET[i].xl;
		for (int x = int(AET[i].xl)+1; x <= AET[i].xr; x++) {
			zx += AET[i].dzx;
			if (zx > zbuffer[x]) {
				//更新深度缓存
				zbuffer[x] = zx;
				coloridbuffer[x] = AET[i].edge_polygon_id;
				vec3 color = getColor(coloridbuffer[x], APT);
				//更新帧缓存
				framebuffer[3 * (y * WINDOW_WIDTH + x)] = color[0];
				framebuffer[3 * (y * WINDOW_WIDTH + x) + 1] = color[1];
				framebuffer[3 * (y * WINDOW_WIDTH + x) + 2] = color[2];
			}
		}
	}
}

void ScanlineZBufferProcessor::drawScanline(int y, vector<ActivePolygon> &APT, vector<ActiveEdge> &AET) {
	glDrawPixels(winWidth, winHeight, GL_RGB, GL_FLOAT, &framebuffer[0]);
	/*for (int x = 0; x < WINDOW_WIDTH; x++) {
		vec3 pixelcolor;
		getColor(coloridbuffer[x], pixelcolor, APT);
		glColor3f(pixelcolor[0], pixelcolor[1], pixelcolor[2]);
		glVertex3f(x, y, zbuffer[x]);
	}*/
}

vec3 ScanlineZBufferProcessor::getColor(int polygon_id,  vector<ActivePolygon> &APT) {
	vec3 color;
	for (int i = 0; i < APT.size(); i++) {
		if (APT[i].polygon_id == polygon_id)
		{
			color = APT[i].color;
		}
	}
	return color;
}

void ScanlineZBufferProcessor::clearDS() {
	AET.clear();
	APT.clear();
	CET.clear();
	CPT.clear();
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