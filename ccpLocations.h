//casualty collection point locations h file
//Author @emily monroe
//date:11/29/24
//uses the other files and creates hospital nodes
//and ccp nodes. it also finds the closest hospital
//to the ccp locations.
#ifndef ccp_Locations_H
#define ccp_Locations_H

#include "Graph.h"
#include "Dijkstra.h"
#include <unordered_set>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

class ccpLocations {
    private:
        Graph* graph;
        unordered_set<Vertex*> hospital_nodes;
        unordered_set<Vertex*> ccp_nodes;
    
    public:
        // Constructor creates graph of points and hospitals
        ccpLocations(Graph* graph) 
        {
            this->graph = graph;
        }
    
        // name:addHospital
        //description: creates hospital nodes when called
        void addHospital(const string& hospitalLabel) 
        {
            Vertex* hospital = graph->GetVertex(hospitalLabel);
            if (hospital) 
            {
                hospital_nodes.insert(hospital);
            }
        }
    
        // name:addCCPLocations
        //description: creates ccp nodes when called
        void addCCPLocation(const string& ccpLabel) 
        {
            Vertex* ccp = graph->GetVertex(ccpLabel);
            if (ccp) 
            {
                ccp_nodes.insert(ccp);
            }
        }
    
        // name:shortestPathToHospital
        // description:Finds the shortest path to the nearest hospital from ccp
        //using dijkstra algorithm from the file
        vector<string> shortestPathToHospital(const string& ccpLabel) 
        {
            Vertex* ccp = graph->GetVertex(ccpLabel);
            if (!ccp) 
            {
                return {"ccp location not found"};
            }
    
            if (hospital_nodes.empty()) 
            {
                return {"No hospitals available"};
            }
    
            Dijkstra dijkstra;
            double shortest_distance = numeric_limits<double>::max();
            vector<Vertex*> best_path;
    
            // Iterates over all hospital nodes to find the closest to the ccp
            for (Vertex* hospital : hospital_nodes) 
            {
                auto [path, distance] = dijkstra.findDijkstra(graph, emergency, hospital);
                if (!path.empty() && distance < shortest_distance) 
                {
                    shortest_distance = distance;
                    best_path = path;
                }
            }
    
            if (best_path.empty()) 
            {
                return {"No hospital reachable"};
            }
    
            // converts path to (label)
            vector<string> result;
            for (Vertex* vertex : best_path) 
            {
                result.push_back(vertex->getLabel());
            }
    
            return result;
        }
};

#endif //EMERGENCY_SERVICE_H
