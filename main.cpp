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
                for (const auto &path: givenPath) {
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

    string location;
    cin >> location;

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