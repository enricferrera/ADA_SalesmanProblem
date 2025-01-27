#include "pch.h"
#include "Graph.h"
#include <queue>

// =============================================================================
// Dijkstra ====================================================================
// =============================================================================

void Dijkstra(CGraph& graph, CVertex* pStart)
{
	// Inicialització de vertexs
	for (CVertex &v : graph.m_Vertices) {
			v.m_DijkstraDistance = DBL_MAX;
			v.m_DijkstraVisit = false;
	}

	pStart->m_DijkstraDistance = 0;
	pStart->m_DijkstraVisit = true;

	CVertex* pActual = pStart;
	while (pActual != NULL)
	{
		for (CEdge* e : pActual->m_Edges) {
			double distanciaActual = pActual->m_DijkstraDistance + e->m_Length;

			if (e->m_pDestination->m_DijkstraDistance > distanciaActual)
				e->m_pDestination->m_DijkstraDistance = distanciaActual;
		}

		double distanciaMinima = DBL_MAX;
		for (CVertex &v : graph.m_Vertices) {
			if (v.m_DijkstraDistance < distanciaMinima && v.m_DijkstraVisit == false)
			{
				distanciaMinima = v.m_DijkstraDistance;
				pActual = &v;
			}
		}
		pActual->m_DijkstraVisit = true;
		if (distanciaMinima == DBL_MAX)
			pActual = NULL;
	}
}

// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================

void DijkstraQueue(CGraph& graph, CVertex *pStart)
{
	//Comparador per al minheap
	struct comparator {
		bool operator()(CVertex* v1, CVertex* v2) {
			return v1->m_DijkstraDistance >= v2->m_DijkstraDistance;
		}
	};

	priority_queue<CVertex*, std::vector<CVertex*>, comparator> queue;

	// Inicialització de vertexs
	for (CVertex& v : graph.m_Vertices) {
		v.m_DijkstraDistance = DBL_MAX;
		v.m_DijkstraVisit = false;
	}

	pStart->m_DijkstraDistance = 0;

	queue.push(pStart);
	while (!queue.empty())
	{
		CVertex* actualVertex = queue.top();
		queue.pop();

		if (!actualVertex->m_DijkstraVisit) {
			for (CEdge* e : actualVertex->m_Edges) {
				double distanciaActual = actualVertex->m_DijkstraDistance + e->m_Length;

				if (e->m_pDestination->m_DijkstraDistance > distanciaActual) {
					e->m_pDestination->m_DijkstraDistance = distanciaActual;
					queue.push(e->m_pDestination);
					e->m_pDestination->m_pDijkstraPrevious = e;
				}
			}
		}
		actualVertex->m_DijkstraVisit = true;
	}
}
