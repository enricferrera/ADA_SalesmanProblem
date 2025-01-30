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
	CBBNode(int index, CVisits& visits)
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

void ExpandLength(CBBNode* actual, int nVisites, std::vector<std::vector<infoCami>>& matriuCamins, std::priority_queue<CBBNode*, std::vector<CBBNode*>, comparatorCBBNode>& cuaPrioritat)
{
	// Si es nodo final no lo expandimos, no tiene sentido
	if (actual->m_index == nVisites - 1) {
		return;
	}

	// Expandimos nodos no visitados
	for (int i = 0; i < nVisites; i++)
	{
		if (!actual->m_visitesVisitades[i])
		{
			cuaPrioritat.push(new CBBNode(i, actual, matriuCamins[actual->m_index][i].longitud));
		}
	}
}

CTrack SalesmanTrackBranchAndBound1(CGraph& graph, CVisits& visits)
{

	const size_t nVisites = visits.GetNVertices();
	if (nVisites < 2) return CTrack(&graph);

	std::vector<std::vector<infoCami>> matriuCamins(nVisites, std::vector<infoCami>(nVisites));
	generarTaula(graph, visits, matriuCamins);

	std::priority_queue<CBBNode*, std::vector<CBBNode*>, comparatorCBBNode> cuaPrioritat;
	cuaPrioritat.push(new CBBNode(0, visits));

	double millorLongitud = std::numeric_limits<double>::infinity();
	CTrack millorCami(&graph);

	while (!cuaPrioritat.empty()) {
		CBBNode* actual = cuaPrioritat.top();
		cuaPrioritat.pop();

		// Si index es el nodo final, compruebas si se han hecho todas las visitas para ver si es solucion
		if (actual->m_index == nVisites - 1)
		{
			bool totsVisitats = true;
			for (size_t i = 0; i < nVisites; i++)
			{
				if (!actual->m_visitesVisitades[i]) // No visitat
				{
					totsVisitats = false;
					break;
				}
			}
			if (totsVisitats)
				goto reconstruccioCami;
		}

		// Expandim i pasem al seguent node
		ExpandLength(actual, nVisites, matriuCamins, cuaPrioritat);
		continue;

		// Reconstrucció camí
		reconstruccioCami:
		std::vector<int> indexsCami;
		CBBNode* aux = actual;
		while (aux != nullptr) {
			indexsCami.push_back(aux->m_index);
			aux = aux->m_pFather;
		}
		std::reverse(indexsCami.begin(), indexsCami.end());

		for (size_t i = 0; i < indexsCami.size() - 1; ++i) {
			millorCami.Append(matriuCamins[indexsCami[i]][indexsCami[i + 1]].cami);
		}

		delete actual;
		return millorCami;
	}
	
	// Si no trobem cap, retornem ruta buida
	return CTrack(&graph);
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
