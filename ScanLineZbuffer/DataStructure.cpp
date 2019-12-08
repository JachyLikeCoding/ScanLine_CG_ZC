#include"DataStructure.h"

ActiveEdge::ActiveEdge(ClassifiedEdge &leftEdge, ClassifiedEdge &rightEdge, ClassifiedPolygon &CP) {
	xl = leftEdge.x;
	xr = rightEdge.x;
	dxl = leftEdge.dx;
	dyl = leftEdge.dy;
	dxr = leftEdge.dx;
	dyr = rightEdge.dy;
	edge_polygon_id = CP.polygon_id;
	if (abs(CP.c) > 1e-5) {
		dzx = -CP.a / CP.c;
		dzy = CP.b / CP.c;
		zl = dzx * xl - dzy * leftEdge.maxY - CP.d / CP.c;
	}
	else {
		dzx = -INT_MAX;
		dzy = INT_MAX;
		zl = -INT_MAX;
	}
}