//casualty collection point locations h file
//@author Emily Monroe
//date:11/29/24
//Creates hospital nodes and ccp nodes
// for the vertices in the graph. It also
// finds the closest hospital to the ccp
// locations using the graph and Dijkstra files
#ifndef ccpLocationsH
#define ccpLocationsH

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
        unordered_set<Vertex*> hospitalNodes;
        unordered_set<Vertex*> ccpNodes;
    
    public:
        // Constructor creates graph of points and hospitals
        ccpLocations (Graph* graph)
        {
            this->graph = graph;
        }
    
        //name:addHospital
        //description: creates hospital nodes when called
        void addHospital(const string& hospitalLabel) 
        {
            Vertex* hospital = graph->getVertex(hospitalLabel);
            if (hospital) 
            {
                hospitalNodes.insert(hospital);
            }
        }
    
        //name:addCCPLocations
        //description: creates ccp nodes when called
        void addCCPLocation(const string& ccpLabel) 
        {
            Vertex* ccp = graph->getVertex(ccpLabel);
            if (ccp) 
            {
                ccpNodes.insert(ccp);
            }
        }
    
        //name:shortestPathToHospital
        //description:Finds the shortest path to the nearest hospital from ccp
        //using dijkstra algorithm from the file
        vector<string> shortestPathToHospital(const string& ccpLabel) 
        {
            Vertex* ccp = graph->getVertex(ccpLabel);
            if (!ccp) 
            {
                return {"ccp location not found"};
            }
    
            if (hospitalNodes.empty())
            {
                return {"No hospitals available"};
            }
    
            Dijkstra dijkstra;
            double shortestDistance = numeric_limits<double>::max();
            vector<Vertex*> bestPath;
    
            //iterate over all hospital nodes to find the closest to the ccp
            for (Vertex* hospital : hospitalNodes)
            {
                auto [path, distance] = dijkstra.findDijkstra(graph, ccp, hospital);
                if (!path.empty() && distance < shortestDistance)
                {
                    shortestDistance = distance;
                    bestPath = path;
                }
            }
    
            if (bestPath.empty())
            {
                return {"No hospital reachable"};
            }
    
            // converts path to (label)
            vector<string> result;
            for (Vertex* vertex : bestPath)
            {
                result.push_back(vertex->getLabel());
            }
    
            return result;

        }
};

#endif //EMERGENCY_SERVICE_H
