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


struct NodeCami {
	CEdge* m_pEdge;
	NodeCami* m_pAnterior;
	CVertex* m_vertex;
	bool m_startsTram;

	// Constructor por defecto
	NodeCami()
		: m_pEdge(nullptr), m_pAnterior(nullptr), m_vertex(nullptr), m_startsTram(false) {}

	// Constructor con parámetros
	NodeCami(CEdge* pEdge, NodeCami* pAnterior, CVertex* vertex, bool startsTram)
		: m_pEdge(pEdge), m_pAnterior(pAnterior), m_vertex(vertex), m_startsTram(startsTram) {}
};

std::list<NodeCami*> recorregut;

void SalesmanTrackBacktrackingRecursiu(NodeCami* pAnterior, NodeCami* node, CVisits& visits)
{
	// Comprobem si es solucio
	// Si hem arribat al destí
		// Hem passat per tots els vertexs
			// --> es solucio
		// Si visitas completadas y camino más corto tenemos nueva solucion
		// --> guardas solucions

	// Creas node del graph imaginari que estem muntant
	NodeCami nouNode;
	nouNode.m_pAnterior = pAnterior;
	nouNode.m_vertex = nouNode.m_pAnterior->m_pEdge->m_pDestination;

	// Si es la primera vez que llegas al vertice i es visita --> inicio de tramo
	if (nouNode.m_vertex->m_visita == false && nouNode.m_vertex->m_JaHePassat == false) {
		nouNode.m_startsTram = true;
	}

	// Mires quines ramificacions te aquest node
	for (CEdge* pE : nouNode.m_vertex->m_Edges) {
		// Comprobar si el node es pot afegir
		for (std::list<NodeCami*>::const_iterator it = recorregut.begin(); it != recorregut.end(); it++) {
			if ((*it)->m_startsTram != true)
			{
				if 
			}
		}
			// Afegim i fem crida recursiva
			node.m_pEdge = pE;
			LongitudCamiActual += pE->m_Length;
			SalesmanTrackBacktrackingRecursiu(&node, pE->m_pDestination, visits);
			LongitudCamiActual -= pE->m_Length;
		}
}

CTrack SalesmanTrackBacktracking(CGraph& graph, CVisits& visits)
{	
	CVertex* pInici = visits.m_Vertices.front();
	pDesti = visits.m_Vertices.back();

	NodeCami node(nullptr, nullptr, pInici, true);

	SalesmanTrackBacktrackingRecursiu(NULL, &node, visits)
}



/*
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
		pActual->m_visitesVisitades.back() = true; // Posem destí com visitat
		for (bool visita : pActual->m_visitesVisitades)
			if (visita == false) {
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
		if (pActual->m_visita && pActual->m_esPartDeTram[tram] == false) {
			tram++;
			pActual->m_startsTram = tram;
		}
		else if (pActual->m_visita && pActual->m_esPartDeTram[tram] == true) {
			goto ciclo;
		}
		// Marquem aquest vertex com a utilizat en aquest tram
		pActual->m_esPartDeTram[tram] = true;

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
				if (!pE->m_pDestination->m_esPartDeTram[tram]) {
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
		: ciclo
		if (pActual->m_visita && tram == pActual->m_startsTram && pActual->copsHePassat == 1)
		{
			pActual->m_esPartDeTram[tram] = false;
			pActual->m_startsTram = -1;
			tram -= 1;
		}
		// Si es vertex normal/visita, lo quitas de ese tramo y ya
		else
			pActual->m_esPartDeTram[tram] = false;
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
//}

/*
CTrack SalesmanTrackBacktracking(CGraph &graph, CVisits &visits)
{
	CVertex* pInici = visits.m_Vertices.front();
	pDesti = visits.m_Vertices.back();

	CamiActual.SetGraph(&graph);
	CamiActual.Clear();
	CamiMesCurt.SetGraph(&graph);
	CamiMesCurt.Clear();

	LongitudCamiMesCurt = numeric_limits<double>::max();
	LongitudCamiActual = 0.0;

	tram = -1;

	for (CVertex* v : visits.m_Vertices) {
		v->m_visita = true;
		v->m_visitesVisitades.resize(visits.GetNVertices(), false);
	}
	for (CVertex& v : graph.m_Vertices) v.m_esPartDeTram.resize(visits.GetNVertices()-1, false);

	pDesti->m_startsTram = -2;

	pInici->m_esPartDeTram[0] = true;
	pInici->m_visitesVisitades[0] = true;
	SalesmanTrackBacktrackingRecursiu(NULL, pInici, visits);
	
	return CamiMesCurt;
}
*/

/*
#include <limits>
#include <vector>
#include <unordered_set>
#include <list>
using namespace std;

void backtrack(int currentSegmentIndex, CVertex* currentVertex, unordered_set<CVertex*>& visitedInSegment, const vector<CVertex*>& visitVec, CTrack& currentTrack, double currentLength, CTrack& bestTrack, double& minLength) {
	// Prune paths that exceed the current minimum length
	if (currentLength >= minLength) {
		return;
	}

	// Check if all segments have been processed
	if (currentSegmentIndex >= visitVec.size() - 1) {
		if (currentLength < minLength) {
			minLength = currentLength;
			bestTrack.m_Edges = currentTrack.m_Edges;
		}
		return;
	}

	// Check if the current vertex is the end of the current segment
	if (currentVertex == visitVec[currentSegmentIndex + 1]) {
		// Move to the next segment with a new visited set starting from the current vertex
		unordered_set<CVertex*> nextVisited;
		nextVisited.insert(currentVertex);
		backtrack(currentSegmentIndex + 1, currentVertex, nextVisited, visitVec, currentTrack, currentLength, bestTrack, minLength);
		return;
	}

	// Explore all outgoing edges from the current vertex
	for (CEdge* edge : currentVertex->m_Edges) {
		CVertex* nextVertex = edge->m_pDestination;

		// Skip if the next vertex is already visited in this segment
		if (visitedInSegment.find(nextVertex) != visitedInSegment.end()) {
			continue;
		}

		// Check if the next vertex is a future visit beyond the next immediate one
		bool isFutureVisit = false;
		for (size_t i = currentSegmentIndex + 2; i < visitVec.size(); ++i) {
			if (nextVertex == visitVec[i]) {
				isFutureVisit = true;
				break;
			}
		}
		if (isFutureVisit) {
			continue;
		}

		// Temporarily mark the next vertex as visited in this segment
		visitedInSegment.insert(nextVertex);
		currentTrack.m_Edges.push_back(edge);
		double newLength = currentLength + edge->m_Length;

		// Recur with the next vertex
		backtrack(currentSegmentIndex, nextVertex, visitedInSegment, visitVec, currentTrack, newLength, bestTrack, minLength);

		// Undo changes for backtracking
		currentTrack.m_Edges.pop_back();
		visitedInSegment.erase(nextVertex);
	}
}

CTrack SalesmanTrackBacktracking(CGraph& graph, CVisits& visits) {
	CTrack millorCami;
	millorCami.SetGraph(&graph);
	double minLength = numeric_limits<double>::max();

	list<CVertex*>& visitList = visits.m_Vertices;
	if (visitList.size() < 2) {
		return millorCami
	}

	// Convertim les visites de llista a vector
	vector<CVertex*> visitVec(visitList.begin(), visitList.end());

	CTrack camiActual;
	camiActual.SetGraph(&graph);

	// Initialize visited set with the first vertex of the first segment
	unordered_set<CVertex*> initialVisited;
	initialVisited.insert(visitVec[0]);

	// Start backtracking from the first segment
	backtrack(0, visitVec[0], initialVisited, visitVec, camiTrack, 0.0, bestTrack, minLength);

	return bestTrack;
}
*/

// =============================================================================
// SalesmanTrackBacktrackingGreedy =============================================
// =============================================================================

static struct infoCami{
	CTrack cami;
	double longitud;

	// Constructor per defecte (per a inicialitzar l'estructura buida)
	infoCami(CGraph* graph = nullptr) : cami(graph), longitud(0.0) {}
};

static void generarTaula(CGraph& graph, CVisits& visits, std::vector<std::vector<infoCami>>& matriuCamins)
{
	// Para cada visita, mira el camino más corto de todas las visitas a la actual
	size_t i = 0;
	for (auto visit : visits.m_Vertices) {
		DijkstraQueue(graph, visit);

		size_t j = 0;
		for (auto visitDijkstra : visits.m_Vertices) {
			CVertex* vertexActual = visitDijkstra;
			while (vertexActual != visit && vertexActual->m_pDijkstraPrevious != nullptr) {
				matriuCamins[i][j].cami.AddFirst(vertexActual->m_pDijkstraPrevious);
				matriuCamins[i][j].longitud += vertexActual->m_pDijkstraPrevious->m_Length;
				vertexActual = vertexActual->m_pDijkstraPrevious->m_pOrigin; // Es origin->recorre al revés
			}
			j++;
		}
		i++;
	}
}

vector<int> millorCami;

void SalesmanTrackBacktrackingGreedyRecursiu(CGraph& graph, CVisits& visits, std::vector<std::vector<infoCami>>& matriuCamins, std::vector<int>& camiActual, std::vector<bool>& visitat, double LongitudCamiActual)
{
	if (camiActual.size() == visits.m_Vertices.size() - 1) {
		int lastVisit = visits.m_Vertices.size() - 1;
		double finalLongitud = LongitudCamiActual + matriuCamins[camiActual.back()][lastVisit].longitud;
		if (finalLongitud < LongitudCamiMesCurt) {
			LongitudCamiMesCurt = finalLongitud;
			millorCami = camiActual;
			millorCami.push_back(lastVisit);
		}
		return;
	}

	for (size_t i = 0; i < visits.m_Vertices.size() - 1; i++) {
		if (!visitat[i]) {
			int prev = camiActual.empty() ? 0 : camiActual.back();
			double novaLongitud = LongitudCamiActual + matriuCamins[prev][i].longitud;

			// Poda
			if (novaLongitud >= LongitudCamiMesCurt) continue;

			// Marcar visitat i recursió
			visitat[i] = true;
			camiActual.push_back(i);

			SalesmanTrackBacktrackingGreedyRecursiu(graph, visits, matriuCamins, camiActual, visitat, novaLongitud);

			// Backtrack
			visitat[i] = false;
			camiActual.pop_back();
		}
	}
}


CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits)
{
	size_t nVisites = visits.m_Vertices.size();
	std::vector<std::vector<infoCami>> matriuCamins(nVisites, std::vector<infoCami>(nVisites));

	generarTaula(graph, visits, matriuCamins);

	std::vector<int> camiActual = {0};
	std::vector<bool> visitat(nVisites, false);
	visitat[0] = true;
	LongitudCamiMesCurt = numeric_limits<double>::max();

	SalesmanTrackBacktrackingGreedyRecursiu(graph, visits, matriuCamins, camiActual, visitat, 0.0);


	CTrack millorCamiTrack(&graph);
	for (int i = 0; i < millorCami.size() - 1; i++) {
		millorCamiTrack.Append(matriuCamins[millorCami[i]][millorCami[i+1]].cami);
	}

	return millorCamiTrack;
}