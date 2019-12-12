#pragma once
#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#include "Basic.h"

//�������α�
class ClassifiedPolygon {
public:
	int polygon_id;		//����α��
	GLfloat a, b, c, d;			//���������ƽ��ķ���ϵ��
	int dy;						//��Խ��ɨ������
	vec3 color;			//�������ɫ
	ClassifiedPolygon(int _polygon_id, GLfloat _a, GLfloat _b, GLfloat _c, GLfloat _d, int _dy, vec3 _color) :
		polygon_id(_polygon_id), a(_a), b(_b), c(_c), d(_d), dy(_dy), color(_color){}
	
};

//����߱�
class ClassifiedEdge {
public:
	GLfloat x;					//�ߵ��϶˵��x����
	GLfloat dx;					//��������ɨ���߽����x�����
	int dy;					//�߿�Խ��ɨ������Ŀ
	int edge_polygon_id;		//����������εı��
	int maxY;			//�ߵ��϶˵�y����

	ClassifiedEdge() : x(0), dx(0), dy(-1), maxY(0), edge_polygon_id(-1){}
	ClassifiedEdge(int _x, int _dx, int _dy, int _maxY, int _edge_polygon_id = -1) : 
		x(_x), dx(_dx), dy(_dy), maxY(_maxY), edge_polygon_id(_edge_polygon_id){}
	bool isLeftEdge(ClassifiedEdge &edge);
	bool isSamePolygon(ClassifiedEdge &edge);
};


//��߱����ͶӰ����α߽���ɨ�����ཻ�ı߶�
class ActiveEdge {
public:
	GLfloat xl, xr;					//���ҽ����x����
	GLfloat dxl, dxr;
	GLfloat dyl, dyr;
	int edge_polygon_id;
	vec3 color;
	GLfloat dzx;
	GLfloat dzy;
	GLfloat zl;

	ActiveEdge(ClassifiedEdge &leftEdge, ClassifiedEdge &rightEdge, ClassifiedPolygon &CP);
};

//�����α�
class ActivePolygon {
public:
	int polygon_id;		//����α��
	GLfloat a, b, c, d;			//���������ƽ��ķ���ϵ��
	int remain_dy;		//��Խ��"ʣ��"ɨ������
	//GLfloat color[3];	//�������ɫ
	vec3 color;
	ActivePolygon(int _polygon_id, GLfloat _a, GLfloat _b, GLfloat _c, GLfloat _d, int _dy, vec3 _color) :
		polygon_id(_polygon_id), a(_a), b(_b), c(_c), d(_d), remain_dy(_dy), color(_color){}
};



#endif // !DATASTRUCTURE_H
