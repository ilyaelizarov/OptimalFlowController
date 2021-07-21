/* Populates a network graph from edges and lengths lists */

#include "parser.h"
#include "graph.h"
#include <boost/graph/adjacency_list.hpp>
#include <iostream>

using namespace boost;

int main() {

	ParseParameters Parse;
	GraphNetwork Graph;

	// Graph for making a loop matrix
	// 0 - 1 - 2 - 5
	//     |   |   |
	//     4 - 3 - 6

	// Parameters
	string edge_list_str = "(0, 1), (1,  2), (2,5), (5,6), (6,3), (2,3), (4,3), (1,4)";
	string length_list_str = "7, 4, 3, 4; 5,6, 10e3, 8";

	// Inputs
	std::vector<double> m_flows = {1, 2, 3, 4, -4, -3, -3};

	std::cout << "Edges list:" << std::endl;
	std::cout << edge_list_str << std::endl;

        std::cout << "Lengths list:" << std::endl;
        std::cout << length_list_str << std::endl;

	typedef pair<int, int> int_pair;
	
	// Data containers for edges and length lists
	vector<int_pair> net_edges;
	vector<double> net_lengths;

	// Parse edges list, put data into a container
	Parse.EdgesList(edge_list_str, &net_edges);

	// Parse length list, put data into a container
	Parse.LengthList(length_list_str, &net_lengths);

	// Populate a network graph
	Graph.Populate(net_edges, net_lengths);
	
	// Assign mass flow rates in the nodes
	Graph.NodesFlow(m_flows);

	std::cout << std::endl;

	// Print graph properties
	Graph.Print();

        // Next inputs
        m_flows = {5, 4, 1, 0, -8, -4, -7};

        // Assign mass flow rates in the nodes
        Graph.NodesFlow(m_flows);

	Graph.Print();

	Graph.SplitTreeAndChords();

	Graph.PrintTree();

	std::cout << Graph.ChordAdjMatrix() << std::endl;

	std::cout << Graph.TreeAdjMatrix() << std::endl;
}
