/*
 * This class was created to run and process Dijkstra's algorithm for finding
 * the shortest distance between two distinct points
 */

#ifndef GROUPPROJ3_DIJKSTRA_H
#define GROUPPROJ3_DIJKSTRA_H

#include "Graph.h"

class Dijkstra {
private:
    unordered_map<Vertex *, double> dists;
    unordered_map<Vertex *, Vertex*> prev;
    unordered_map<Vertex *, bool> visited;

public:
    //find shortest dist
    pair<vector<Vertex*>, double> findDijkstra(Graph* graph, Vertex* start, Vertex* end) {

        //init dists to infinity
        for (Vertex *v: graph->getVertices()) {
            dists[v] = numeric_limits<double>::infinity(); //init all verts to infinity
            prev[v] = nullptr; //init all predecessors to nullptr
        }
        dists[start] = 0; //set source to 0

        //lambda function to compute the min-heap/priority-queue
        auto compare = [](pair<Vertex*, double> a, pair<Vertex*, double> b) {
            return a.second > b.second;
        };

        priority_queue<
                pair<Vertex*, double>,
                vector<pair<Vertex*, double>>,
                decltype(compare)> pq(compare);

        pq.emplace(start, 0.0);

        while (!pq.empty()) {
            Vertex* currVert = pq.top().first;
            pq.pop();

            if (currVert == end) {
                break;
            }

            //if visited, skip the vert
            if (visited[currVert]) {
                continue;
            }
            visited[currVert] = true;

            //parse neighbors
            for (Edge* edge : *(graph->getEdgesFrom(currVert))) {
                Vertex* vertNeighbor = edge->getToVert();
                double weight = edge->getWeight();

                if (!visited[vertNeighbor] && (((dists[currVert] + weight) < dists[vertNeighbor]))) {
                    dists[vertNeighbor] = dists[currVert] + weight;
                    prev[vertNeighbor] = currVert; //track previous vert
                    pq.emplace(vertNeighbor, dists[vertNeighbor]);
                }
            }
        }

        // Reconstruct the path
        vector<Vertex*> path;
        for (Vertex* at = end; at != nullptr; at = prev[at]) {
            path.insert(path.begin(), at);
        }

        //check if start vert is at the beginning
        if (path.empty() || path.front() != start) {
            path.clear(); //path dne
        }

        return {path, dists[end]};
    }
};

#endif //GROUPPROJ3_DIJKSTRA_H
