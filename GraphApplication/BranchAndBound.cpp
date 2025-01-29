#include "pch.h"
#include "Graph.h"
#include <queue>
#include <iostream>
#include <iomanip> 


struct infoCami {
	CTrack cami;
	double longitud;

	// Constructor per defecte (per a inicialitzar l'estructura buida)
	infoCami(CGraph* graph = nullptr) : cami(graph), longitud(0.0) {}
};

void generarTaula(CGraph& graph, CVisits& visits, std::vector<std::vector<infoCami>>& matriuCamins)
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

// =============================================================================
// SalesmanTrackBranchAndBound1 ================================================
// =============================================================================

// CBBLinkNode ===============================================================
class CBBNode {
public:
	int m_index;
	std::vector<bool> m_visitesVisitades;
	int m_nVisitesVisitades;
	CBBNode* m_pFather;
	double m_Length;
	unsigned m_CntRef;
public:
	CBBNode(int index, CVertex* pDestination, CVisits& visits)
		: m_index(index)
		, m_nVisitesVisitades(1)
		, m_pFather(NULL)
		, m_Length(0.0)
		, m_CntRef(1)
	{
		m_visitesVisitades.resize(visits.m_Vertices.size(), false);
		m_visitesVisitades[0] = true;
	}
	CBBNode(int index, CBBNode* pFather, double pathLength)
		: m_index(index)
		, m_nVisitesVisitades(++pFather->m_nVisitesVisitades)
		, m_pFather(pFather)
		, m_Length(pFather->m_Length + pathLength)
		, m_CntRef(1)
	{
		m_visitesVisitades = pFather->m_visitesVisitades;
		m_visitesVisitades[index] = true;
		++m_pFather->m_CntRef;
	}
	void Unlink()
	{
		if (--m_CntRef == 0) {
			if (m_pFather) m_pFather->Unlink();
			delete this;
		}
	}
};

// comparator ==================================================================
struct comparatorCBBNode {
	bool operator()(const CBBNode* s1, const CBBNode* s2) {
		return s1->m_Length > s2->m_Length;
	}
};



CTrack SalesmanTrackBranchAndBound1(CGraph& graph, CVisits& visits)
{
	int nVisites = visits.m_Vertices.size();
	int lastIndex = nVisites - 1;

	std::vector<std::vector<infoCami>> matriuCamins(nVisites, std::vector<infoCami>(nVisites));
	generarTaula(graph, visits, matriuCamins);

	priority_queue<CBBNode*, std::vector<CBBNode*>, comparatorCBBNode> queue;
	queue.push(new CBBNode(0, visits.m_Vertices.back(), visits));

	double millorLongitud= std::numeric_limits<double>::infinity();
	CTrack millorCami(&graph);

	// Aproach doesn't make sense, hay que añadir todos los nodos incluso los del final
	while (!queue.empty())
	{
		CBBNode* nodeActual = queue.top();
		queue.pop();

		// Si el último nodo no es el final, aún no hem completat el camí valid
		if (nodeActual->m_nVisitesVisitades != nVisites && nodeActual->m_index != lastIndex) {
			// Ramifiquem
			for (size_t i = 0; i < nVisites; i++)
			{
				double longitudCami = matriuCamins[nodeActual->m_index][i].longitud;
				double longitudTotal = nodeActual->m_Length + longitudCami;

				if (!nodeActual->m_visitesVisitades[i]) {
					if (i == lastIndex) {
						bool totsAltresVisitats = true;
						// Resto visitas nodo visitado?
						for (size_t j = 0; j < nVisites; j++)
							if (j != lastIndex && !nodeActual->m_visitesVisitades[j]) {
								totsAltresVisitats = false;
								break;
							}
						if (totsAltresVisitats) {
							CBBNode* nouNode = new CBBNode(static_cast<int>(i), nodeActual, longitudCami);
							queue.push(nouNode);
						}
					}
					else {
							CBBNode* nouNode = new CBBNode(static_cast<int>(i), nodeActual, longitudCami);
							queue.push(nouNode);
					}
				}
			}
		}

		// Compruebas si el nodo es solución
		if (nodeActual->m_nVisitesVisitades == nVisites	&& nodeActual->m_index == lastIndex) {
			if (nodeActual->m_Length < millorLongitud) {
				millorLongitud = nodeActual->m_Length;
				millorCami.Clear();

				//Reconstrucció camí
				std::vector<int> indexsCami;
				CBBNode* aux = nodeActual;
				while (aux != nullptr) {
					indexsCami.push_back(aux->m_index);
					aux = aux->m_pFather;
				}
				std::reverse(indexsCami.begin(), indexsCami.end());

				for (size_t i = 0; i < indexsCami.size() - 1; ++i) {
					millorCami.Append(matriuCamins[indexsCami[i]][indexsCami[i + 1]].cami);
				}
			}
			//continue;
		}
	}
	return millorCami;
}

// SalesmanTrackBranchAndBound2 ===================================================

CTrack SalesmanTrackBranchAndBound2(CGraph& graph, CVisits &visits)
{
	size_t nVisites = visits.m_Vertices.size();
	std::vector<std::vector<infoCami>> matriuCamins(nVisites, std::vector<infoCami>(nVisites));

	generarTaula(graph, visits, matriuCamins);

	return CTrack(&graph);
}

// SalesmanTrackBranchAndBound3 ===================================================


CTrack SalesmanTrackBranchAndBound3(CGraph& graph, CVisits &visits)
{
	size_t nVisites = visits.m_Vertices.size();
	std::vector<std::vector<infoCami>> matriuCamins(nVisites, std::vector<infoCami>(nVisites));

	generarTaula(graph, visits, matriuCamins);

	return CTrack(&graph);
}

// SalesmanTrackBranchAndBound4 ===================================================


CTrack SalesmanTrackBranchAndBound4(CGraph& graph, CVisits& visits)
{
	return CTrack(&graph);
}
