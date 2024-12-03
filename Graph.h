/*
 * As a general whole, these classes were grabbed from a sample c++ source file provided
 * to us during the CS355 course that was originally used for minimum spanning trees. We just
 * noticed that it was a good blueprint for the graph class that would be used for the Dijkstra's
 * algorithm project.
 *
 * @author Ethan Smith, Emily Monroe
 * @version 11.23.24
 */
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <limits>
#include <queue>
using namespace std;

#ifndef GROUPPROJ3_GRAPH_H
#define GROUPPROJ3_GRAPH_H
/********************************
 * Vertex class: Creates the
 * vertices used in the graph.
 * @author: Emily Monroe
 * @version 11.23.2024
 * @since 1.0
 * ********************************/
class Vertex{
private:
    string label;
public:
     Vertex(const string& vertexLabel){
        label = vertexLabel;
    }

    string getLabel() { return this->label; }
};

/*
 * This class is to act as an edge, the line between to vertices.
 * It holds the vertex its travelling from and the vertex its travelling to,
 * as well as it's weight
 *
 * @author Ethan Smith
 * @version 11.23.2024
 * @since 1.0
 */
class Edge{
    private:
        Vertex* fromVert;
        Vertex* toVert;
        vector<double> weightSet;

    public:
        Edge(Vertex* from, Vertex* to, vector<double> edgeWeight){
            fromVert = from;
            toVert = to;
            weightSet = edgeWeight;
        }

        Vertex* getToVert() { return this->toVert; }
        Vertex* getFromVert() { return this->fromVert; }
        double getWeight() const { return this->weightSet[0]; }
        vector<double> getWeightSet() const { return this->weightSet; }
};

class Graph {
    private:
        unordered_map<Vertex*, vector<Edge*> *> fromEdges;
        unordered_map<Vertex*, vector<Edge*> *> toEdges;
        unordered_map<Vertex*, vector<Edge*> *> undirEdges;

        class ComparePairs{
            public:
                bool operator()(const pair<Vertex*, double>& a, const pair<Vertex*, double>& b) const{
                    return a.second > b.second;
                }
        };

    public:
        virtual ~Graph() {
            unordered_set<Vertex *> distinctVerts;
            unordered_set<Edge *> distinctEdges;

            //parsing the set of vertices and edges
            for (auto &keyVal: fromEdges) {
                distinctVerts.insert(keyVal.first);
                for (Edge *temp: *keyVal.second) {
                    distinctEdges.insert(temp);
                }
            }
            for (auto &keyVal: toEdges) {
                distinctVerts.insert(keyVal.first);
                for (Edge *temp: *keyVal.second) {
                    distinctEdges.insert(temp);
                }
            }

            //clear maps
            fromEdges.clear();
            toEdges.clear();

            //free edges and verts
            for (Edge *temp: distinctEdges) {
                delete temp;
            }
            for (Vertex *temp: distinctVerts) {
                delete temp;
            }
        }

        /*
         * This addEdge function simply creates a new Vertex object and
         * add it to the fromEdges/toEdges unordered_map private fields.
         *
         * @author Ethan Smith
         * @version 11.23.2024
         * @param newLabel : const string&
         * @returns Vertex*
         */
        Vertex* addVertex(const string &newLabel) {
            //create new Vertex obj
            auto* newVertex = new Vertex(newLabel);

            //each vertex must live as a key in both maps
            fromEdges[newVertex] = new vector<Edge*>();
            toEdges[newVertex] = new vector<Edge*>();

            return newVertex;
        }

        /*
         * This function adds a direct edge to the graph given its staring vertex,
         * ending vertex, and its weight
         *
         * @author Ethan Smith
         * @version 11.23.2024
         * @param fromVertex : Vertex*, toVertex : Vertex*, weight : double->1.0
         * @returns Edge*
         */
        Edge* addDirectEdge(Vertex* fromVertex, Vertex* toVertex, vector<double> weight = {1.0}){
            //only add unique labels
            if(hasEdge(fromVertex, toVertex)){
                return nullptr;
            }

            Edge* newDirEdge = new Edge(fromVertex, toVertex, weight);

            fromEdges[fromVertex]->push_back(newDirEdge);
            toEdges[toVertex]->push_back(newDirEdge);

            return newDirEdge;
        }

        /*
         * This function adds a new undirected edge by utilizing the previously defined
         * addDirectedEdge method
         *
         * @author Ethan Smith
         * @version 11.23.2024
         * @see Graph::addDirectedEdge
         * @param vertA : Vertex*, vertB : Vertex*, weight : double->1.0
         * @returns pair<Edge*, Edge*>
         * @deprecated This function was originally written in this file simply for generalization of a graph, however,
         *             since we are dealing with just the user's perspective and onl going from a CCP to a hospital, it is not needed.
         *
         *  pair<Edge*, Edge*> addUndirEdge(Vertex* vertA, Vertex* vertB, double weight = 1.0){
         *      Edge* edge1 = new Edge(vertA, vertB, weight);
         *      Edge* edge2 = new Edge(vertB, vertA, weight);
         *      pair<Edge*, Edge*> edgePair(edge1, edge2);
         *      return edgePair;
         *   }
        */

        /*
         * This function returns the set of edges throughout the graph
         *
         * @author Ethan Smith
         * @version 11.23.2024
         * @returns unordered_set<Edge*>
         */
        unordered_set<Edge*> getEdges() const{
            unordered_set<Edge*> edgeSet;
            for(auto& keyVal : fromEdges) {
                vector<Edge*>* edges = keyVal.second;
                for(Edge* edge : *edges){
                    edgeSet.insert(edge);
                }
            }
            return edgeSet;
        }

        /*
         * This function wasn't originally here and was added on the provided version date. All it does
         * is take the two provided vertexs and returns the single weight from vertA to vertB as well as
         * the set of alternate possible weights for the two vertices
         *
         * @author Ethan Smith
         * @version 12.2.2024
         * @returns pair<double, vector<double>>
         * @params Vertex*, Vertex*
         */
        pair<double, vector<double>> getWeightPair(Vertex* vertA, Vertex* vertB){
            vector<Edge*> edgesFromVert1 = getEdgesFrom(vertA);
            for(Edge* edge : edgesFromVert1){
                if(edge->getToVert() == vertB){
                    pair<double, vector<double>> weights;
                    weights.first = edge->getWeight();
                    weights.second = edge->getWeightSet();
                    return weights;
                }
            }

            return {-1.0, {-11.0}};
        }

    /*
     * This function had to be rewritten as a result of the implementation of the
     * getWeightPair() function. All this function does is return the edges going out
     * from the provided Vertex* arg.
     *
     * @author Ethan Smith
     * @version 12.2.2024
     * @return const vector<Edge*>&
     * @param Vertex*
     */
    const vector<Edge*>& getEdgesFrom(Vertex* fromVertex) const {
        auto iter = fromEdges.find(fromVertex);
        if (iter != fromEdges.end() && iter->second != nullptr) {
            return *(iter->second);
        }
        cout << "Vertex not found or no edges associated with it.";
        return {};
    }

    /*
     * All this does is return the edges to an "argumented" Vertex*
     *
     * @author Ethan Smith
     * @version 11.23.2024
     * @return const vector<Edge*>*
     * @param Vertex*
     */
    const vector<Edge*>* getEdgesTo(Vertex* toVertex) const{
            return toEdges.at(toVertex);
        }

        /*
         * This returns the vertex the that is
         * being searched for returns null if
         * vertex doesn't exist
         * @author Emily Monroe
         *
         */
        Vertex* getVertex(const string &vertexLabel) {
            for (auto &keyValue: fromEdges) {
                Vertex* vertex = keyValue.first;
                if (vertex->getLabel() == vertexLabel) {
                    return keyValue.first;
                }
            }
            return nullptr;
        }

        /*
         * This returns the vertices of the
         * graph in a vector form
         * @author Emily Monroe
         */
        vector<Vertex*> getVertices() const{
            vector<Vertex*> vertices;
            for (auto &keyValue: fromEdges)
            {
                vertices.push_back(keyValue.first);
            }
            return vertices;
        }
        /*
           * This returns true only if graph has
           * edge from fromVertex to toVertex
           * @author Emily Monroe
           */
        bool hasEdge(Vertex* fromVertex, Vertex *toVertex) const
        {
            if (0 == fromEdges.count(fromVertex)) {
                return false;
            }

            //search list of edges for edge that goes to toVertex
            vector<Edge*> &edges = *fromEdges.at(fromVertex);
            for(Edge* edge: edges)
            {
                if (edge->getToVert() == toVertex)
                {
                    return true;
                }
            }
            return false;
        }

};


#endif //GROUPPROJ3_GRAPH_H
