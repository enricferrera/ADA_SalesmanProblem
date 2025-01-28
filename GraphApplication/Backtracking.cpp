#include "pch.h"
#include "Graph.h"
#include <set>
#include <stack>
#include <list>

#define NO_PERTANY -1

CTrack CamiMesCurt(NULL);
double LongitudCamiMesCurt;
CVertex* pDesti;
CTrack CamiActual(NULL);
double LongitudCamiActual;
int tram = -1; // En la primera iteración siempre pasa a 0
std::list<CVertex*> recorrido;

struct NodeCami {
	CEdge* m_pEdge;
	NodeCami* m_pAnterior;
};

// =============================================================================
// SalesmanTrackBacktracking ===================================================
// =============================================================================

void SalesmanTrackBacktrackingRecursiu(NodeCami* pAnterior, CVertex* pActual, CVisits& visits)
{
	// COMPROBACIÓ CAMÍ
	// ¿Hem arribat al destí?
	if (pActual == pDesti) {
		// ¿Hem passat per totes les visites?
		bool visitesCompletades = true;
		pActual->m_JaHePassat = true; // Posem destí com visitat
		for (CVertex* v : visits.m_Vertices)
			if (v->m_startsTram == NO_PERTANY) {
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

	// PAS ENDAVANT - PAS ENDARRERE
	if (LongitudCamiActual < LongitudCamiMesCurt) {
		// Si no hem passat en general i es visita, cambiem de tram i marquem que aquest node comença aquest tram
		if (tram == -1)
			tram == 0
		if (pActual->m_visita && pActual->m_JaHePassat == false) {
			tram++;
			pActual->m_startsTram = tram;
		}
		// Marquem aquest vertex com a utilizat en aquest tram
		pActual->esPartDeTram[tram] = true;
		pActual->m_JaHePassat = true;

		NodeCami node; node.m_pAnterior = pAnterior;

		// Mirem tots els edges y mirem si el destí es una opció
		//	- Recorrem fins que trobem el que ha iniciat aquest tram
		//	- Mirem que no haguem passat per aquest vertex en aquest tram
		for (CEdge* pE : pActual->m_Edges) {
			std::list<CVertex*>::const_reverse_iterator it = recorrido.rbegin();
			// Recorremos hasta que lleguemos al iniciador del tramo o a
			// Esto esta mal -> Recorremos hasta 
			// Comprobar que ese vertex no esta en el tramo
			//while (it != recorrido.rend() && (*it)->m_startsTram == tram)
			//{
				// Comprobamos que no sea parte del tramo
				if (!pE->m_pDestination->esPartDeTram[tram]) {
					node.m_pEdge = pE;
					LongitudCamiActual += pE->m_Length;
					SalesmanTrackBacktrackingRecursiu(&node, pE->m_pDestination, visits);
					LongitudCamiActual -= pE->m_Length;
				}
			}
		//}
		// Si hechamos para atras hemos de quitar el nodo de ese tramo y si es uno de los que inicia lo ponemos a -1; Si estas en el tramo 1 y has iniciado el tramo 1, lo quitas del tramo 1, quitas que ha iniciado el tramo 1 y reduces el tramo --> Esto es para las visitas
		// Sinó es una visita, lo quitas del tramo y ya
		// En general has de quitar que pertenece al tramo y que has pasado por el
		if (pActual->m_visita && tram == pActual->m_startsTram)
		{
			pActual->esPartDeTram[tram] = false;
			pActual->m_startsTram = -1;
			tram -= 1;
		}
		// Si es vertex normal/visita, lo quitas de ese tramo y ya
		else
			pActual->esPartDeTram[tram] = false;
		pActual->m_JaHePassat = false;
	}

	// Comprobar destino y todas las visitas recorridas
		// True Copiar camino

	// Paso adelante
		// Añadir al stack, marcar como visto y añadir al tramo
		// ¿Para añadir con que no sea parte del tramo ya vale?

	// Paso atras
		// Quitar del stack, desmarcar como visto y quitar del tramo


	/*
		Major problema, al comprobar si un vertice es apto para continuar, hemos de ver si en ese tramo aún no hemos pasado. Cómo lo hacemos? Miramos en la pila hasta la última visita que habíamos visitado por primera vez. Y si paso por un nodo visita que no marca el comienzo del tramo.
		Como cojones hago eso?
			- Recorrer hacía atras los vertices hasta encontrar uno que sea visita
			- Variable en Vertice que indique que tramo marca esa visita. Ej: Si ese vertice visita marca el comienzo del tramo 2, y estamos en el 3, al recorrer el camino hacía atras, lo haremos hasta el vertice que tenga el indicador de iniciador de tramo en 3.
			- A parte no queremos pasar dos veces por el mismo vertice en el mismo tramo, necesitamos marcar a que tramos pertenece el vertice
	*/

	// Usaré la structura nodo para reacer el camino
	// Y usaré la lista de vertices para comprobar tramos y visitas
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

	pDesti->m_startsTram = -2;

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


