/* Populates a network graph from edges and lengths lists */

#include "numerical.h"
#include "parser.h"
#include "graph.h"
#include <boost/graph/adjacency_list.hpp>
#include <iostream>

using namespace boost;

int main() {

	ParseParameters Parse;
	GraphNetwork Graph;

	// Graph for making a loop matrix
	// 0 - 1 - 2 - 5 --- 7
	//     |   |   |     |
	//     4 - 3 - 6 --- 8

	// Parameters
	// string edge_list_str = "(0, 1), (1,  2), (2,5), (5,6), (6,3), (2,3), (4,3), (1,4)";
	string edge_list_str = "(0, 1), (1,  2), (2,5), (5,6), (6,3), (2,3), (4,3), (1,4), (5,7), (7,8), (8,6)";

	
	string diameter_list_str = "15e-3, 0.020, 0.030, 0.040, 0.025, 0.010, 0.025, 0.05, 0.01 , 0.015, 0.032";
	// string length_list_str = "7, 4, 3, 4; 5,6, 10e3, 8";
        string length_list_str = "50, 25, 100, 85; 75, 60, 120, 150, 80, 50, 20";


	// Inputs
	std::vector<double> m_flows = {1, 2, 3, 4, -4, -3, -3, 2, 3};

	std::cout << "Edges list:" << std::endl;
	std::cout << edge_list_str << std::endl;

	std::cout << "Diameters list:" << std::endl;
	std::cout << diameter_list_str << std::endl;

        std::cout << "Lengths list:" << std::endl;
        std::cout << length_list_str << std::endl;

	typedef pair<int, int> int_pair;
	
	// Data containers for edges, diameters, and length lists
	vector<int_pair> net_edges;
	vector<double> net_diameters;
	vector<double> net_lengths;

	// Parse edges list, put data into a container
	Parse.EdgesList(edge_list_str, &net_edges);

	// Parse diameters list, put data into a container
	Parse.DiameterList(diameter_list_str, &net_diameters);

	// Parse length list, put data into a container
	Parse.LengthList(length_list_str, &net_lengths);

	std::cout << "Lengths from the vector: " << std::endl;
	std::cout << net_diameters.at(0) << std::endl;

	// Populate a network graph
	Graph.Populate(net_edges, net_diameters, net_lengths);
	
	// Assign mass flow rates in the nodes
	Graph.SetNodesFlow(m_flows);

	std::cout << std::endl;

	// Print graph properties
	Graph.Print();

        // Next inputs
        m_flows = {0.00069, 0.00111, 0.0016, 0.0027, -0.0022, -0.0011, 0.0019, 0.00055, 0.00083};

        // Assign mass flow rates in the nodes
        Graph.SetNodesFlow(m_flows);

	Graph.Print();

	Graph.SplitTreeAndChords();

	Graph.PrintTree();

	std::cout << "A_chord: " << std::endl;

	std::cout << Graph.GetChordAdjMatrix() << std::endl;

	std::cout << "A_tree: " << std::endl;

	std::cout << Graph.GetTreeAdjMatrix() << std::endl;

	std::cout << "X_c_0: " << std::endl;

	std::cout << Graph.GetInitialChordsFlow() << std::endl;

	std::cout << "Q:" << std::endl;

	std::cout << Graph.GetNodesFlow() << std::endl;

	std::cout << "B: " << std::endl;

	std::cout << Graph.GetLoopMatrix() << std::endl;

	Newton NumericalMethods;

	std::cout << NumericalMethods.GetFlowBranches(Graph.GetTreeAdjMatrix(), Graph.GetChordAdjMatrix(),
			Graph.GetNodesFlow(), Graph.GetInitialChordsFlow()) << std::endl;

	std::cout << "Pressure drop residuals in the cycles:" << std::endl;

	std::cout << NumericalMethods.GetResVec(Graph.GetLoopMatrix(),
		NumericalMethods.GetFlowBranches(Graph.GetTreeAdjMatrix(),
		       Graph.GetChordAdjMatrix(),
                       Graph.GetNodesFlow(),
		       Graph.GetInitialChordsFlow()),
		&net_diameters,
		&net_lengths) << std::endl;


}
