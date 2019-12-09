#pragma once
#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#include "Basic.h"

//分类多边形表
class ClassifiedPolygon {
public:
	int polygon_id;		//多边形编号
	int a, b, c, d;			//多边形所在平面的方程系数
	int dy;						//跨越的扫描线数
	ivec3 color;			//多边形颜色
	ClassifiedPolygon *nxtPolygon;
	ClassifiedPolygon(int _polygon_id, GLfloat _a, GLfloat _b, GLfloat _c, GLfloat _d, int _dy, ivec3 _color, ClassifiedPolygon *_nxtPolygon) :
		polygon_id(_polygon_id), a(_a), b(_b), c(_c), d(_d), dy(_dy), nxtPolygon(_nxtPolygon){}
	
};

//分类边表
class ClassifiedEdge {
public:
	int x;					//边的上端点的x坐标
	int dx;					//相邻两条扫描线焦点的x坐标差
	int dy;					//边跨越的扫描线数目
	int edge_polygon_id;		//边所属多边形的编号
	int maxY;			//边的上端点y坐标
	ClassifiedEdge *nxtEdge;
	ClassifiedEdge() : x(0), dx(0), dy(-1), maxY(0), edge_polygon_id(-1), nxtEdge(nullptr) {}
	ClassifiedEdge(int _x, int _dx, int _dy, int _maxY, int _edge_polygon_id = -1, ClassifiedEdge *_next = nullptr) :
		x(_x), dx(_dx), dy(_dy), maxY(_maxY), edge_polygon_id(_edge_polygon_id), nxtEdge(_next) {}
	bool isLeftEdge(ClassifiedEdge &edge);
	bool isSamePolygon(ClassifiedEdge &edge);
};


//活化边表：存放投影多边形边界与扫描线相交的边对
class ActiveEdge {
public:
	int xl, xr;					//左、右交点的x坐标
	int dxl, dxr;
	int dyl, dyr;
	int edge_polygon_id;
	ivec3 color;
	GLfloat zl;
	GLfloat dzx;
	GLfloat dzy;
	ActiveEdge *nextAE;

	ActiveEdge(ClassifiedEdge &leftEdge, ClassifiedEdge &rightEdge, ClassifiedPolygon &CP);
};

//活化多边形表
class ActivePolygon {
public:
	int polygon_id;		//多边形编号
	GLfloat a, b, c, d;			//多边形所在平面的方程系数
	int remain_dy;		//跨越的"剩余"扫描线数
	ivec3 color;			//多边形颜色
	ActivePolygon *nxtActivePolygon;

	ActivePolygon(int _polygon_id, GLfloat _a, GLfloat _b, GLfloat _c, GLfloat _d, int _dy, ivec3 _color, ActivePolygon *_next) :
		polygon_id(_polygon_id), a(_a), b(_b), c(_c), d(_d), remain_dy(_dy), nxtActivePolygon(_next) {}
};



#endif // !DATASTRUCTURE_H
