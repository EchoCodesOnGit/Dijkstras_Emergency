#include <iostream>
using namespace std;
#include "Dijkstra.h"
#include "ccpLocations.h"

/*
 * TODO:
 * 1. Add weights to the edges of the vertices
 * 2. Hardcode all vertices and edges with their respective weights and have the frontend
 * take in user input to see which CCP they're at and then provided a list of closest
 * hospitals sorted by distance (closest to furthest)
 *
 * FIXME:
 * For some reason whenever the shortest path is ran, it always returns Helen Keller Hospital
 *      -EFFORTS TRIED:
 *          -Swapping the newGraph->addDirectEdge with newGraph->addUndirEdge
 *          -switching the declaration of the code blocks
 *          -messing with the compare function in Dijkstra.h (didn't work)
 *      -MAYBE TRY:
 *          -debugging with the hasEdge, getEdges
 *
 * NOTES:
 *          -We could probably get away with
 */

int menu();
string promptLocation();
vector<string> findShortestPath(Graph*, string);
int main() {
    Graph* newGraph = new Graph();
    //adding the ccp locations to the graph
    newGraph->addVertex("Commons");
    newGraph->addVertex("Wesleyan Hall");
    newGraph->addVertex("Flowers Hall");
    newGraph->addVertex("Mattielou Hall");
    //adding the hospital vertices to the graph
    newGraph->addVertex("North Alabama Medical Center");
    newGraph->addVertex("Helen Keller Hospital");

    //Commons to Hospitals
    newGraph->addDirectEdge(
            (newGraph->getVertex("Commons")),
            (newGraph->getVertex("North Alabama Medical Center")),
            {2.1,2.2,2.3}
            );
    newGraph->addDirectEdge(
            (newGraph->getVertex("Commons")),
            (newGraph->getVertex("Helen Keller Hospital")),
            {5.5,5.6,6.2}
            );

    //Wesleyan to Hospitals
    newGraph->addDirectEdge(
            (newGraph->getVertex("Wesleyan Hall")),
            (newGraph->getVertex("North Alabama Medical Center")),
            {2.2,2.2,2.4}
            );
    newGraph->addDirectEdge(
            (newGraph->getVertex("Wesleyan Hall")),
            (newGraph->getVertex("Helen Keller Hospital")),
            {5.8,6.5}
            );

    //Flowers to Hospitals
    newGraph->addUndirEdge(
            (newGraph->getVertex("Flowers Hall")),
            (newGraph->getVertex("North Alabama Medical Center")),
            {2.5,2.7}
            );
    newGraph->addUndirEdge(
            (newGraph->getVertex("Flowers Hall")),
            (newGraph->getVertex("Helen Keller Hospital")),
            {5.7,6.4}
            );

    //Mattielou to Hospitals
    newGraph->addUndirEdge(
            (newGraph->getVertex("Mattielou Hall")),
            (newGraph->getVertex("North Alabama Medical Center")),
            {2.3,2.5,2.7}
            );
    newGraph->addUndirEdge(
            (newGraph->getVertex("Mattielou Hall")),
            (newGraph->getVertex("Helen Keller Hospital")),
            {5.9,6.6}
        );
    /****************************************************/

    //init the ccpLocs with the previously defined graph
    ccpLocations ccpLocs(newGraph);
    //adding ccps
    ccpLocs.addCCPLocation("Commons");
    ccpLocs.addCCPLocation("Wesleyan Hall");
    ccpLocs.addCCPLocation("Flowers Hall");
    ccpLocs.addCCPLocation("Mattielou Hall");
    //adding hospitals
    ccpLocs.addHospital("North Alabama Medical Center");
    ccpLocs.addHospital("Helen Keller Hospital");

    /****************************************************/

    int choice = menu();
    while(choice != -1) {
        string location;
        vector<string> givenPath;
        switch (choice) {
            case 1:
                location = promptLocation();
                break;
            case 2:
                if (location.empty()) {
                    cout << "Please enter your location first.\n";
                    location = promptLocation();
                }
                givenPath = ccpLocs.shortestPathToHospital(location);
                for (const auto& path: givenPath) {
                    cout << path << endl;
                }
                break;
            default:
                cout << "ERROR: Invalid choice\n";
                menu();
                break;
        }
        choice = menu();
    }

    cout << "Have a nice day!\n";
    exit(1);

    return 0;
}

int menu(){
    int choice;

    cout << "---------------------------\n";
    cout << "CHOOSE ANY OF THE FOLLOWING\n";
    cout << "---------------------------\n";

    cout << "1. Enter Your Location\n";
    cout << "2. Find Shortest Path\n";
    cout << "-1. Exit\n";

    cin >> choice;

    return choice;
}

string promptLocation(){
    cout << "Locations:\n";
    cout << "\t1. Commons\n";
    cout << "\t2. Wesleyan Hall\n";
    cout << "\t3. Flowers Hall\n";
    cout << "\t4. Mattielou Hall\n";

    cin.ignore();
    string location;
    getline(cin, location);

    if(location != "Commons" &&
       location != "Wesleyan Hall" &&
       location != "Flowers Hall" &&
       location != "Mattielou Hall"){
        cout << "Enter a valid CCP location\n";
        promptLocation();
    }

    return location;
}

vector<string> findShortestPath(Graph* graph, const string& location){
    string loc = location;
    ccpLocations locats(graph);
    while(loc.empty()) {
        cout << "Cannot find the shortest path without a given location";
        loc = promptLocation();
    }
    return locats.shortestPathToHospital(loc);
}