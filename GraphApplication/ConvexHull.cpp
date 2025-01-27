#include "pch.h"
#include "Graph.h"
#include "GraphApplicationDlg.h"
#include <set>

double PosicioRespeteRecta(CGPoint& a, CGPoint& b, CGPoint& c) {
	return (a.m_Y - b.m_Y) * (c.m_X - b.m_X) - (a.m_X - b.m_X) * (c.m_Y - b.m_Y);
}

double AreaTriangle(CGPoint& a, CGPoint& b, CGPoint c) {
	return abs((a.m_Y - b.m_Y) * (c.m_X - b.m_X) - (a.m_X - b.m_X) * (c.m_Y - b.m_Y)) / 2.0;
}

// Funció per afegir un element al vector només si no hi és
void afegirSiNoExisteix(std::list<CVertex*>& vec, CVertex* vertex) {
	if (std::find(vec.begin(), vec.end(), vertex) == vec.end()) {
		vec.push_back(vertex);  // Només afegeix si no existeix ja
	}
}

// =============================================================================
// CONVEX HULL =================================================================
// =============================================================================

std::list<CVertex*> QuickHullRecursive(CVertex* A, CVertex* B, std::list<CVertex*> points)
{
	if (points.empty()) {
		points.push_back(A);
		points.push_back(B);
		return points;
	}

	CVertex* C = points.front();
	double areaTriangleActual = 0.0, areaTriangleMinim = 0.0;
	for (CVertex* vertexActual : points) {
		areaTriangleActual = AreaTriangle(A->m_Point, B->m_Point, vertexActual->m_Point);
		if (areaTriangleActual > areaTriangleMinim) {
			C = vertexActual;
			areaTriangleMinim = areaTriangleActual;
		}
	}

	// Trobar punts a l'esquera de AC i a la dreta de CB
	std::list<CVertex*> leftPoints, rightPoints;
	for (CVertex* vertexActual : points) {
		if (PosicioRespeteRecta(A->m_Point, C->m_Point,  vertexActual->m_Point) < 0)
			leftPoints.push_back(vertexActual);
		else if (PosicioRespeteRecta(C->m_Point, B->m_Point, vertexActual->m_Point) < 0)
			rightPoints.push_back(vertexActual);
	}

	// Crida recursiva
	std::list<CVertex*> CHLeft = QuickHullRecursive(A, C, leftPoints);
	std::list<CVertex*> CHRight = QuickHullRecursive(C, B, rightPoints);

	// Ajuntem llistes en un set per treure repetits i el tornem a llista
	std::list<CVertex*> finalList;
	for (CVertex* v : CHLeft)
		afegirSiNoExisteix(finalList, v);
	for (CVertex* v : CHRight)
		afegirSiNoExisteix(finalList, v);

	return finalList;
}


// QuickHull ===================================================================

CConvexHull QuickHull(CGraph& graph)
{
	CConvexHull solucio(&graph);

	if (graph.m_Vertices.empty())
		return solucio;

	// Els dos punts AB més a la esquerra abaix més a la dreta a dalt
	CVertex* fartherLeft = &graph.m_Vertices.front(), * fartherRight = &graph.m_Vertices.front();
	for (CVertex& v : graph.m_Vertices) {
		if (v.m_Point.moreLeftDown(fartherLeft->m_Point))
			fartherLeft = &v;
		if (v.m_Point.moreRightUp(fartherRight->m_Point))
			fartherRight = &v;
	}

	// Classificar punts a cada costat de la recta que uneix aquests dos punts
	// Eix y positiu va cap a baix, si es més gran que 0 el posem a downPoints
	std::list<CVertex*> upPoints, downPoints;
	for (CVertex& v : graph.m_Vertices) {
		if (PosicioRespeteRecta(fartherLeft->m_Point, fartherRight->m_Point, v.m_Point) > 0)
			downPoints.push_back(&v);
		else if (PosicioRespeteRecta(fartherLeft->m_Point, fartherRight->m_Point, v.m_Point) < 0)
			upPoints.push_back(&v);
	}

	// Calcular tros superior e inferior de l'envoltant convexa
	std::list<CVertex*> CHUp = QuickHullRecursive(fartherLeft, fartherRight, upPoints);
	std::list<CVertex*> CHDown = QuickHullRecursive(fartherRight, fartherLeft, downPoints);

	// Ajuntem llistes en un set per treure repetits i el tornem a llista
	for (CVertex* v : CHUp)
		afegirSiNoExisteix(solucio.m_Vertices, v);
	for (CVertex* v : CHDown)
		afegirSiNoExisteix(solucio.m_Vertices, v);

	return solucio;
}