#include "pch.h"
#include "Graph.h"
#include <set>
#include <stack>

CTrack CamiMesCurt(NULL);
double LongitudCamiMesCurt;
CVertex* pDesti;
CTrack CamiActual(NULL);
double LongitudCamiActual;
int tram = -1; // En la primera iteración siempre pasa a 0

struct NodeCami {
	CEdge* m_pEdge;
	NodeCami* m_pAnterior;
};

// =============================================================================
// SalesmanTrackBacktracking ===================================================
// =============================================================================

void SalesmanTrackBacktrackingRecursiu(NodeCami* pAnterior, CVertex* pActual, CVisits& visits)
{
	// ¿Hem arribat al destí?
	if (pActual == pDesti) {
		// ¿Hem passat per totes les visites?
		bool visitesCompletades = true;
		pActual->m_JaHePassat = true; // Posem destí com visitat
		for (CVertex* v : visits.m_Vertices)
			if (v->m_JaHePassat == false) {
				visitesCompletades = false;
				break;
			}
		
		// Si totes les visites i cami més curt
		if (visitesCompletades && LongitudCamiActual < LongitudCamiMesCurt) {
			CamiMesCurt.Clear();
			while (pAnterior) {
				CamiMesCurt.m_Edges.push_front(pAnterior->m_pEdge);
				pAnterior = pAnterior->m_pAnterior;
			}
			LongitudCamiMesCurt = LongitudCamiActual;
		}
	}
	//Sinó marquem aquest node com part del tram i ramifiquem amb crida recursiva
	if (LongitudCamiActual < LongitudCamiMesCurt) {
		bool visitaJaVisitada = false;
		if (pActual->m_visita == true && pActual->m_JaHePassat == false && pActual != pDesti)
			tram++;
		//Visita por la que ya hemos pasado -> variable local para indicarlo
		else if (pActual->m_visita == true && pActual->m_JaHePassat == true)
			visitaJaVisitada = true;

		pActual->esPartDeTram[tram] = true; //Vertex part de Tram actual
		pActual->m_JaHePassat = true;
		NodeCami node;
		node.m_pAnterior = pAnterior;
		for (CEdge* pE : pActual->m_Edges) {
			if (!pE->m_pDestination->esPartDeTram[tram]) {
				node.m_pEdge = pE;
				LongitudCamiActual += pE->m_Length;
				SalesmanTrackBacktrackingRecursiu(&node, pE->m_pDestination, visits);
				LongitudCamiActual -= pE->m_Length;
			}
		}
		pActual->esPartDeTram[tram] = false;
		if (!visitaJaVisitada)
			tram--;
		pActual->m_JaHePassat = false;
	}
}

CTrack SalesmanTrackBacktracking(CGraph &graph, CVisits &visits)
{
	CVertex* pInici = visits.m_Vertices.front();
	pDesti = visits.m_Vertices.back();

	CamiActual.SetGraph(&graph);
	CamiMesCurt.SetGraph(&graph);

	LongitudCamiMesCurt = numeric_limits<double>::max();
	LongitudCamiActual = 0.0;

 	for (CVertex* v : visits.m_Vertices) v->m_visita = true;
	for (CVertex& v : graph.m_Vertices) v.esPartDeTram.resize(visits.GetNVertices()-1, false);

	SalesmanTrackBacktrackingRecursiu(NULL, pInici, visits);
	
	return CamiMesCurt;
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


