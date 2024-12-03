//Dijkstra h file
//@author Ethan Smith
//date:11/27/24
//uses the Graph h file to calculate the
//shortest path between two points.
#ifndef GROUPPROJ3_DIJKSTRA_H
#define GROUPPROJ3_DIJKSTRA_H

#include "Graph.h"

class Dijkstra {
private:
    unordered_map<Vertex*, double> dists;
    unordered_map<Vertex*, Vertex*> prev;
    unordered_map<Vertex*, bool> visited;

public:

    Dijkstra() {
        dists.clear();
        prev.clear();
        visited.clear();
    }
/* name: findDijkstra()
 * description: Find the shortest distance
 *              using Dijkstra's algorithm
 * @author Ethan Smith
 */
    //run the Dijkstra algorithm to find the shortest dist
    pair<vector<Vertex*>, double> findDijkstra(Graph* graph, Vertex* start, Vertex* end) {

        //set all dists to infinity and predecessors to nullptr
        for (Vertex* v : graph->getVertices()) {
            dists[v] = numeric_limits<double>::infinity();
            prev[v] = nullptr;
            visited[v] = false; //make sure all vertices marked as not visited
        }
        dists[start] = 0;

        //this lambda creates the priority queue with custom comparator
        auto compare = [](pair<Vertex*, double> a, pair<Vertex*, double> b) {
            return a.second > b.second;
        };

        priority_queue<
                pair<Vertex*, double>,
                vector<pair<Vertex*, double>>,
                decltype(compare)> pq(compare);

        pq.emplace(start, 0.0);

        //Dijkstra algorithm loop
        while (!pq.empty()) {
            Vertex* currVert = pq.top().first;
            pq.pop();

            //skip if the vertex has been visited
            if (visited[currVert]) {
                continue;
            }
            visited[currVert] = true;

            //if the currVert is at the end, we go ahead and stop
            if (currVert == end) {
                break;
            }

            //process each edge from the current vertex
            for (Edge* edge : (graph->getEdgesFrom(currVert))) {
                Vertex* vertNeighbor = edge->getToVert();
                double weight = edge->getWeight();

                //relax the edge and update the distance if a shorter path is found
                if (!visited[vertNeighbor] && (dists[currVert] + weight < dists[vertNeighbor])) {
                    dists[vertNeighbor] = dists[currVert] + weight;
                    prev[vertNeighbor] = currVert;
                    pq.emplace(vertNeighbor, dists[vertNeighbor]);
                }
            }
        }

        //rebuild start to end pathway
        vector<Vertex*> path;
        for (Vertex* at = end; at != nullptr; at = prev[at]) {
            path.insert(path.begin(), at); //insert front to reverse
        }

        //check if start vert is the first in the path (valid path found)
        if (path.empty() || path.front() != start) {
            return { {}, numeric_limits<double>::infinity() }; //no path valid
        }

        //return path and dist to the end
        return {path, dists[end]};
    }
};

#endif //GROUPPROJ3_DIJKSTRA_H
