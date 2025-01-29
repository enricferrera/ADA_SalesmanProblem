#include "pch.h"
#include "Graph.h"
#include <queue>
#include <iostream>
#include <iomanip> 
#include <random>
#include <chrono>
#include <numeric>
#include <algorithm>

static struct infoCami {
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

// SalesmanTrackProbabilistic ==================================================

CTrack SalesmanTrackProbabilistic(CGraph& graph, CVisits& visits)
{
	size_t nVisites = visits.GetNVertices();
	std::vector<std::vector<infoCami>> matriuCamins(nVisites, std::vector<infoCami>(nVisites));

	generarTaula(graph, visits, matriuCamins);

	// Vector que indica orden -> pos[0] = inicial, pos[nVisites - 1] = final, intermedio puede variar
	std::vector<size_t> visitIndex(nVisites);
	std::iota(visitIndex.begin(), visitIndex.end(), 0);

	// Menys de 2 visites
	if (nVisites < 2)
		return CTrack(&graph);
	// 2 visites
	else if (nVisites == 2) {
		CTrack solucio(&graph);
		solucio.Append(matriuCamins[0][1].cami);
		return solucio;
	}

	// Més de 2 visites
	CTrack millorCami(&graph);
	double longitudMillorCami = std::numeric_limits<double>::infinity();

	int nIntents = 100000;
	for (int intent = 0; intent < nIntents; intent++)
	{
		// Començem cada intent amb una solució aleatoria
		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(visitIndex.begin() + 1, visitIndex.end() - 1, g);

		// Intentem millorar la ruta mitjançant intercambis de vértexs
		bool improved;
		do {
			improved = false;
			double longitudActual = 0.0;
			// Calculem longitud actual
			for (int i = 0; i < nVisites - 1; i++) {
				longitudActual += (matriuCamins[visitIndex[i]][visitIndex[i + 1]].longitud); 
			}

			double millorLongitudIntercanvi = longitudActual;
			std::vector<size_t> millorOrdreIntercanvis = visitIndex;

			for (size_t i = 1; i < nVisites - 2; i++) {
				for (size_t j = i + 1; j < nVisites - 1; j++) {
					std::vector<size_t> nouOrdre = visitIndex;
					std::swap(nouOrdre[i], nouOrdre[j]);

					// Caculamos distancia
					double longitudNova = 0.0;
					for (int i = 0; i < nVisites - 1; i++)
						longitudNova += (matriuCamins[nouOrdre[i]][nouOrdre[i + 1]].longitud);

					// Si la nova ruta es millor, la actualitzem
					if (longitudNova < millorLongitudIntercanvi) {
						millorLongitudIntercanvi = longitudNova;
						millorOrdreIntercanvis = nouOrdre;
					}
				}
			}

			// Si trobem millor, actualitzem l'ordre dels vertexs
			if (millorLongitudIntercanvi < longitudActual) {
				visitIndex = millorOrdreIntercanvis;
				improved = true;
			}
		} while (improved);

		// Calculem longitud final
		double longitudFinal = 0.0;
		for (int i = 0; i < nVisites - 1; i++)
			longitudFinal += (matriuCamins[visitIndex[i]][visitIndex[i + 1]].longitud);

		// Si trobem millor solució la guardem
		if (longitudFinal < longitudMillorCami) {
			longitudMillorCami = longitudFinal;

			millorCami.Clear();
			for (int i = 0; i < nVisites - 1; i++) {
				millorCami.Append(matriuCamins[visitIndex[i]][visitIndex[i + 1]].cami);
			}
		}
	}

	return millorCami;
}


