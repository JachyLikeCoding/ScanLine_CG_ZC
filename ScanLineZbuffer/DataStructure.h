#pragma once
#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#include "Basic.h"

//�������α�
class ClassifiedPolygon {
public:
	int polygon_id;		//����α��
	int a, b, c, d;			//���������ƽ��ķ���ϵ��
	int dy;						//��Խ��ɨ������
	ivec3 color;			//�������ɫ
	ClassifiedPolygon *nxtPolygon;
	ClassifiedPolygon(int _polygon_id, GLfloat _a, GLfloat _b, GLfloat _c, GLfloat _d, int _dy, ivec3 _color, ClassifiedPolygon *_nxtPolygon) :
		polygon_id(_polygon_id), a(_a), b(_b), c(_c), d(_d), dy(_dy), nxtPolygon(_nxtPolygon){}
	
};

//����߱�
class ClassifiedEdge {
public:
	int x;					//�ߵ��϶˵��x����
	int dx;					//��������ɨ���߽����x�����
	int dy;					//�߿�Խ��ɨ������Ŀ
	int edge_polygon_id;		//����������εı��
	int maxY;			//�ߵ��϶˵�y����
	ClassifiedEdge *nxtEdge;
	ClassifiedEdge() : x(0), dx(0), dy(-1), maxY(0), edge_polygon_id(-1), nxtEdge(nullptr) {}
	ClassifiedEdge(int _x, int _dx, int _dy, int _maxY, int _edge_polygon_id = -1, ClassifiedEdge *_next = nullptr) :
		x(_x), dx(_dx), dy(_dy), maxY(_maxY), edge_polygon_id(_edge_polygon_id), nxtEdge(_next) {}
	bool isLeftEdge(ClassifiedEdge &edge);
	bool isSamePolygon(ClassifiedEdge &edge);
};


//��߱����ͶӰ����α߽���ɨ�����ཻ�ı߶�
class ActiveEdge {
public:
	int xl, xr;					//���ҽ����x����
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

//�����α�
class ActivePolygon {
public:
	int polygon_id;		//����α��
	GLfloat a, b, c, d;			//���������ƽ��ķ���ϵ��
	int remain_dy;		//��Խ��"ʣ��"ɨ������
	ivec3 color;			//�������ɫ
	ActivePolygon *nxtActivePolygon;

	ActivePolygon(int _polygon_id, GLfloat _a, GLfloat _b, GLfloat _c, GLfloat _d, int _dy, ivec3 _color, ActivePolygon *_next) :
		polygon_id(_polygon_id), a(_a), b(_b), c(_c), d(_d), remain_dy(_dy), nxtActivePolygon(_next) {}
};



#endif // !DATASTRUCTURE_H
