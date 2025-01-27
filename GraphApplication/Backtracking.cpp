#include "pch.h"
#include "Graph.h"
#include <set>
#include <stack>

CTrack* CamiMesCurt(NULL);
double LongitudCamiMesCurt;
CVertex* pDesti;
CTrack* CamiActual(NULL);
double LongitudCamiActual;
int tram = 0;

struct NodeCami {
	CEdge* m_pEdge;
	NodeCami* m_pAnterior;
};

// =============================================================================
// SalesmanTrackBacktracking ===================================================
// =============================================================================

void SalesmanTrackBacktrackingRecursiu(NodeCami* pAnterior, CVertex* pActual)
{
	if (pActual == pDesti) {
		if (LongitudCamiActual < LongitudCamiMesCurt) {
			CamiMesCurt->Clear();
			while (pAnterior) {
				CamiMesCurt->m_Edges.push_front(pAnterior->m_pEdge);
				pAnterior = pAnterior->m_pAnterior;
			}
			LongitudCamiMesCurt = LongitudCamiActual;
		}
	}
	else if (LongitudCamiActual < LongitudCamiMesCurt) {
		if (pActual->m_visita == true)
			tram++;
		pActual->esPartDeTram[tram] = true; //Vertex part de Tram actual
		NodeCami node;
		node.m_pAnterior = pAnterior;
		for (CEdge* pE : pActual->m_Edges) {
			if (!pE->m_pDestination->esPartDeTram[tram]) {
				LongitudCamiActual += pE->m_Length;
				SalesmanTrackBacktrackingRecursiu(&node, pE->m_pDestination);
				LongitudCamiActual -= pE->m_Length;
			}
		}
		if (pActual->m_visita == true)
			tram--;
		pActual->esPartDeTram[tram] = false;

	}
}

CTrack SalesmanTrackBacktracking(CGraph &graph, CVisits &visits)
{
	CVertex* pInici = visits.m_Vertices.front();
	pDesti = visits.m_Vertices.back();

	LongitudCamiMesCurt = numeric_limits<double>::max();
	LongitudCamiActual = 0.0;

 	for (CVertex* v : visits.m_Vertices) v->m_visita = true;
	SalesmanTrackBacktrackingRecursiu(NULL, pInici);
	
	return *CamiMesCurt;
}




// =============================================================================
// SalesmanTrackBacktrackingGreedy =============================================
// =============================================================================


CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits)
{
	/*
	CVertex* pInici = graph.GetVertex("INICI");
	CVertex* pDesti = graph.GetVertex("DESTI");

	for (CVertex* visita : visits.m_Vertices)
	{
		DijkstraQueue(graph, visita);
	}
	*/
	return CTrack(&graph);
}


