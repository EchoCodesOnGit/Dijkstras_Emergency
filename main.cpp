#include <iostream>
using namespace std;
#include "Dijkstra.h"
#include "ccpLocations.h"

int menu();
void printEdges(Graph* graph);
string promptLocation();
vector<string> findShortestPath(Graph* graph, ccpLocations locats, const string& location);
int main() {
    Graph* newGraph = new Graph();
    //adding the ccp locations to the graph
    newGraph->addVertex("Commons");
    newGraph->addVertex("Muscle Shoals Sound Studio");
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
            {5.5,5.6,10.0}
            );

    //Wesleyan to Hospitals
    newGraph->addDirectEdge(
            (newGraph->getVertex("Muscle Shoals Sound Studio")),
            (newGraph->getVertex("North Alabama Medical Center")),
            {3.7,4.2}
            );
    newGraph->addDirectEdge(
            (newGraph->getVertex("Muscle Shoals Sound Studio")),
            (newGraph->getVertex("Helen Keller Hospital")),
            {2.5,2.6}
            );

    //Flowers to Hospitals
    newGraph->addDirectEdge(
            (newGraph->getVertex("Flowers Hall")),
            (newGraph->getVertex("North Alabama Medical Center")),
            {2.5,2.7,2.8}
            );
    newGraph->addDirectEdge(
            (newGraph->getVertex("Flowers Hall")),
            (newGraph->getVertex("Helen Keller Hospital")),
            {5.7}
            );

    //Mattielou to Hospitals
    newGraph->addDirectEdge(
            (newGraph->getVertex("Mattielou Hall")),
            (newGraph->getVertex("North Alabama Medical Center")),
            {2.3,2.5,2.7}
            );
    newGraph->addDirectEdge(
            (newGraph->getVertex("Mattielou Hall")),
            (newGraph->getVertex("Helen Keller Hospital")),
            {5.9}
        );
    /****************************************************/

    //init the ccpLocs with the previously defined graph
    ccpLocations ccpLocs(newGraph);
    //adding ccps
    ccpLocs.addCCPLocation("Commons");
    ccpLocs.addCCPLocation("Muscle Shoals Sound Studio");
    ccpLocs.addCCPLocation("Flowers Hall");
    ccpLocs.addCCPLocation("Mattielou Hall");
    //adding hospitals

    ccpLocs.addHospital("North Alabama Medical Center");
    ccpLocs.addHospital("Helen Keller Hospital");

    /****************************************************/

    //printEdges(newGraph);

    int choice = menu();
    while(choice != -1) {
        string location;
        vector<string> givenPath;
        if(choice == 1) {
            location = promptLocation();
            if (location.empty()) {
                cout << "Please enter your location first.\n";
                location = promptLocation();
            }
            givenPath = findShortestPath(newGraph, ccpLocs, location);
            cout << "The closest hospital from the CCP at " << givenPath[0] << " is the " << givenPath[givenPath.size()-1] << ":\n\t";
            cout << "PATH: ";
            for (const auto& path : givenPath) {
                if(path != "Helen Keller Hospital" && path != "North Alabama Medical Center")
                    cout << path << "->";
                else
                    cout << path;
            }
            pair<double, vector<double>> locWeights = newGraph->getWeightPair(
                                                                              newGraph->getVertex(givenPath[0]),
                                                                              newGraph->getVertex(givenPath[givenPath.size()-1])
                                                                              );
            cout << "\n\tThe shortest distance between " << givenPath[0] << " and " << givenPath[givenPath.size()-1] << " is " << locWeights.first << " miles\n";
            cout << "\tAlternate routes are the following distance(s): ";
            for(double temp : locWeights.second){
                if(temp!=locWeights.second[locWeights.second.size()-1])
                    cout << temp << " miles, ";
                else
                    cout << temp << " miles";
            }
            cout << '\n';
        } else {
            cout << "ERROR: Invalid choice\n";
            menu();
        }

        choice = menu();
    }

    cout << "Have a nice day!\n";
    exit(1);

    return 0;
}
/* name: menu()
 * description: This function creates a menu for
 *              the user to select if they want
 *              the shortest path from their CCP
 *              or if they want to exit the program.
 * @author Ethan Smith
 */
int menu() {
    int choice;

    while (true) { // Loop until valid input
        cout << "---------------------------\n";
        cout << "CHOOSE ANY OF THE FOLLOWING\n";
        cout << "---------------------------\n";
        cout << "1. Find Shortest Path From Location\n";
        cout << "-1. Exit\n";
        cout << "Enter your choice: ";

        if (cin >> choice) { // Validate input
            if (choice == 1 || choice == -1) {
                return choice; // Valid input, exit the function
            } else {
                cout << "Make sure to enter a valid number\n";
            }
        } else {
            //non-num inputs
            cin.clear(); //wipe exit flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //toss bad inputs
            cout << "Make sure to enter a valid number\n";
        }
    }
}

/*
 * name: promptLocation()
 *
 * Description: This is a simple function that just
 *              asks for the user's location
 *
 * @author Ethan Smith
 */

string promptLocation() {
    cout << "Locations:\n";
    cout << "\t1. Commons\n";
    cout << "\t2. Muscle Shoals Sound Studio\n";
    cout << "\t3. Flowers Hall\n";
    cout << "\t4. Mattielou Hall\n";

    string location;

    cin.ignore();
    while(true) {
        cout << "Enter your location: ";
        getline(cin, location);

        if (location == "Commons" ||
            location == "Muscle Shoals Sound Studio" ||
            location == "Flowers Hall" ||
            location == "Mattielou Hall") {
            break;
        }

        cout << "Enter a valid CCP location.\n";
    }

    return location;
}

/*
 * name: findShortestPath
 *
 * Description: This function class upon the shortestPathToHospital function in the ccpLocations.h file. From there
 *              it runs the defined Dijkstra algorithm.
 *
 * @author Ethan Smith
 */
vector<string> findShortestPath(Graph* graph, ccpLocations locats, const string& location){
    auto loc = location;
    if(loc.empty()) {
        cout << "Cannot find the shortest path without a given location";
        loc = promptLocation();
    }

    return locats.shortestPathToHospital(loc);
}

/*
 * name:printEdges
 * Description: The function iterates through each edge of a given graph and prints out the
 *              starting edge and its ending edge which it's connected to.
 *
 * @author Ethan Smith, Emily Monroe
 */
/*
void printEdges(Graph* graph) {
    unordered_set<Edge*> edges = graph->getEdges();

    vector<Edge*> sortedEdges(edges.begin(), edges.end());
    sort(sortedEdges.begin(), sortedEdges.end());


    destroy(edges.begin(), edges.end());

    for (Edge* edge : edges) {
        cout << "Edge from " << edge->getFromVert()->getLabel()
             << " to " << edge->getToVert()->getLabel()
             << " with weight " << edge->getWeight() << endl;
    }
}
*/