#include "pch.h"
#include "Graph.h"
// SalesmanTrackGreedy =========================================================

CTrack SalesmanTrackGreedy(CGraph& graph, CVisits &visits)
{
	CTrack* solution = new CTrack(&graph);

	CVertex* previousVertex = visits.m_Vertices.front();
	CVertex* actualVertex = visits.m_Vertices.front();
	CVertex* lastVertex = visits.m_Vertices.back();

	visits.m_Vertices.pop_front();
	visits.m_Vertices.pop_back();

	while (!visits.m_Vertices.empty())
	{
		DijkstraQueue(graph, actualVertex);

		double minimunDistance = DBL_MAX;
		list<CVertex*>::iterator it = visits.m_Vertices.begin();
		list<CVertex*>::iterator iteradorActual;
		for (it; it != visits.m_Vertices.end(); ++it) {
			if ((*it)->m_DijkstraDistance < minimunDistance) {
				iteradorActual = it;
				actualVertex = (*it);
				minimunDistance = (*it)->m_DijkstraDistance;
			}
		}

		CVertex* assistantVertex = actualVertex;
		CTrack trackAuxiliar(&graph);
		while (assistantVertex->m_pDijkstraPrevious->m_pOrigin != previousVertex)
		{
			trackAuxiliar.AddFirst(assistantVertex->m_pDijkstraPrevious);
			assistantVertex = assistantVertex->m_pDijkstraPrevious->m_pOrigin;
		}
		trackAuxiliar.AddFirst(assistantVertex->m_pDijkstraPrevious);
		solution->Append(trackAuxiliar);

		assistantVertex = assistantVertex->m_pDijkstraPrevious->m_pOrigin;

		previousVertex = actualVertex;
		visits.m_Vertices.erase(iteradorActual);
	}

	// Lo hacemos para el último que hemos sacado de la lista
	DijkstraQueue(graph, previousVertex);
	CTrack trackAuxiliar(&graph);
	while (lastVertex->m_pDijkstraPrevious->m_pOrigin != previousVertex)
	{
		trackAuxiliar.AddFirst(lastVertex->m_pDijkstraPrevious);
		lastVertex = lastVertex->m_pDijkstraPrevious->m_pOrigin;
	}
	trackAuxiliar.AddFirst(lastVertex->m_pDijkstraPrevious);
	solution->Append(trackAuxiliar);

	return *solution;
}