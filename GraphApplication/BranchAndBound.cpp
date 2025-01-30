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
	CBBNode* m_pFather;
	double m_Length;
public:
	CBBNode(int index, CVisits& visits)
		: m_index(index)
		, m_pFather(NULL)
		, m_Length(0.0)
	{
		m_visitesVisitades.resize(visits.m_Vertices.size(), false);
		m_visitesVisitades[0] = true;
	}
	CBBNode(int index, CBBNode* pFather, double pathLength)
		: m_index(index)
		, m_pFather(pFather)
		, m_Length(pFather->m_Length + pathLength)
	{
		m_visitesVisitades = pFather->m_visitesVisitades;
		m_visitesVisitades[index] = true;
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

// =============================================================================
// SalesmanTrackBranchAndBound2 ================================================
// =============================================================================

// CBBLinkNode ===============================================================
class CBBNode2 {
public:
	int m_index;
	std::vector<bool> m_visitesVisitades;
	CBBNode2* m_pFather;
	double m_Length;
	double m_cotaInferior;
public:
	CBBNode2(int index, CVisits& visits)
		: m_index(index)
		, m_pFather(NULL)
		, m_Length(0.0)
		, m_cotaInferior(0.0)
	{
		m_visitesVisitades.resize(visits.m_Vertices.size(), false);
		m_visitesVisitades[0] = true;
	}
	CBBNode2(int index, CBBNode2* pFather, double pathLength)
		: m_index(index)
		, m_pFather(pFather)
		, m_Length(pFather->m_Length + pathLength)
		, m_cotaInferior(0.0)
	{
		m_visitesVisitades = pFather->m_visitesVisitades;
		m_visitesVisitades[index] = true;
	}
};

// comparator ==================================================================
struct comparatorCBBNode2 {
	bool operator()(const CBBNode2* s1, const CBBNode2* s2) {
		return s1->m_cotaInferior > s2->m_cotaInferior;
	}
};

// cota inferior ===============================================================
void PreCalcularMinims(const std::vector<std::vector<infoCami>>& matriuCamins, std::vector<double>& distanciesMinimes)
{
	for (size_t j = 0; j < matriuCamins.size(); ++j) {
		for (size_t i = 0; i < matriuCamins[j].size() - 1; ++i) {
			if (i != j)
				distanciesMinimes[j] = std::min(distanciesMinimes[j], matriuCamins[i][j].longitud);
		}
	}
}

double CalculaCotaInferior(const CBBNode2* node, std::vector<double>& distanciesMinimes)
{
	double cotaInferior = 0.0;
	// Busquem el camí minim a tots els nodes no visitats
	for (size_t i = 0; i < node->m_visitesVisitades.size(); i++) {
		if (!node->m_visitesVisitades[i])
			cotaInferior += distanciesMinimes[i];
	}

	return node->m_Length - cotaInferior;
}

// expand ==================================================================
void ExpandBB2(CBBNode2* actual, int nVisites
	, std::vector<std::vector<infoCami>>& matriuCamins
	, std::vector<double>& distanciesMinimes
	, std::priority_queue<CBBNode2*, std::vector<CBBNode2*>, comparatorCBBNode2>& cuaPrioritat)
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
			CBBNode2* nouNode = new CBBNode2(i, actual, matriuCamins[actual->m_index][i].longitud);
			nouNode->m_cotaInferior = CalculaCotaInferior(nouNode, distanciesMinimes);
			cuaPrioritat.push(nouNode);
		}
	}
}


CTrack SalesmanTrackBranchAndBound2(CGraph& graph, CVisits &visits)
{
	const size_t nVisites = visits.GetNVertices();
	if (nVisites < 2) return CTrack(&graph);

	std::vector<std::vector<infoCami>> matriuCamins(nVisites, std::vector<infoCami>(nVisites));
	generarTaula(graph, visits, matriuCamins);

	// Distancies mínimes per al càlcul de la cota inferior
	std::vector<double> distanciesMinimes(nVisites, std::numeric_limits<double>::max());
	PreCalcularMinims(matriuCamins, distanciesMinimes);

	std::priority_queue<CBBNode2*, std::vector<CBBNode2*>, comparatorCBBNode2> cuaPrioritat;
	cuaPrioritat.push(new CBBNode2(0, visits));

	CTrack millorCami(&graph);
	double millorCota = std::numeric_limits<double>::max();

	while (!cuaPrioritat.empty()) {
		CBBNode2* actual = cuaPrioritat.top();
		cuaPrioritat.pop();

		if (actual->m_cotaInferior >= millorCota) {
			delete actual;
			continue;
		}

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
				millorCota = actual->m_cotaInferior;
				goto reconstruccioCami;
		}

		// Expandim i pasem al seguent node
		ExpandBB2(actual, nVisites, matriuCamins, distanciesMinimes, cuaPrioritat);
		continue;

		// Reconstrucció camí
		reconstruccioCami:
		millorCami.Clear();
		std::vector<int> indexsCami;
		CBBNode2* aux = actual;
		while (aux != nullptr) {
			indexsCami.push_back(aux->m_index);
			aux = aux->m_pFather;
		}
		std::reverse(indexsCami.begin(), indexsCami.end());

		for (size_t i = 0; i < indexsCami.size() - 1; ++i) {
			millorCami.Append(matriuCamins[indexsCami[i]][indexsCami[i + 1]].cami);
		}
	}

	// Si no trobem cap, retornem ruta buida
	return millorCami;
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
