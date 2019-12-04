#pragma once
#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#include "Basic.h"

class ClassifiedEdge {
public:
	int x;					//�ߵ��϶˵��x����
	int dx;					//��������ɨ���߽����x�����
	int dy;					//�߿�Խ��ɨ������Ŀ
	int edge_polygon_id;		//����������εı��
	int maxY;			//�ߵ��϶˵�y����
	ClassifiedEdge *nxtEdge;
	ClassifiedEdge() : x(0), dx(0), dy(-1), maxY(0), edge_polygon_id(-1), nxtEdge(nullptr){}
	ClassifiedEdge(int _x, int _dx, int _dy, int _maxY, int _edge_polygon_id = -1, ClassifiedEdge *_next = nullptr) :
		x(_x), dx(_dx), dy(_dy), maxY(_maxY), edge_polygon_id(_edge_polygon_id), nxtEdge(_next) {}
};

class ClassifiedPolygon {
public:
	int polygon_id;		//����α��
	int a, b, c, d;			//���������ƽ��ķ���ϵ��
	int dy;						//��Խ��ɨ������
	ivec3 color;			//�������ɫ
	ClassifiedPolygon *nxtPolygon;
	ClassifiedPolygon(int _polygon_id, int _a, int _b, int _c, int _d, int _dy, ivec3 _color, ClassifiedPolygon *_nxtPolygon) :
		polygon_id(_polygon_id), a(_a), b(_b), c(_c), d(_d), dy(_dy), nxtPolygon(_nxtPolygon){}
};

//��߱����ͶӰ����α߽���ɨ�����ཻ�ı߶�
class ActiveEdge {
public:
	int xl, xr;					//���ҽ����x����
	int dxl, dxr;
	int dyl, dyr;

};

class ActivePolygon {
public:
	int polygon_id;		//����α��
	int a, b, c, d;			//���������ƽ��ķ���ϵ��
	int remain_dy;		//��Խ��"ʣ��"ɨ������
	ivec3 color;			//�������ɫ
	ActivePolygon *nxtActivePolygon;

	ActivePolygon(int _polygon_id, int _a, int _b, int _c, int _d, int _dy, ivec3 _color, ActivePolygon *_next) :
		polygon_id(_polygon_id), a(_a), b(_b), c(_c), d(_d), remain_dy(_dy), nxtActivePolygon(_next) {}
};



#endif // !DATASTRUCTURE_H
