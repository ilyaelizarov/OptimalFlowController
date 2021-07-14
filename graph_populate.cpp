/* Populates a network graph from edges and lengths lists */

#include "parser.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>

using namespace boost;

/* Bundled properties */

struct nodeProperties {
        uint32_t id; // Node no.
        double m_flow; // Flow rate in the node
};

struct edgeProperties {
	unsigned int id; // Edge no.
	double length; // Length of the edge
};

// Network without flow directions
typedef adjacency_list<vecS, vecS, undirectedS, nodeProperties, edgeProperties> undir_g;
undir_g network;

typedef graph_traits<undir_g>::edge_descriptor branch;

int main() {

	ParseParameters Parse;

	// Graph for making a loop matrix
	// 1 - 2 - 3 - 6
	//     |   |   |
	//     5 - 4 - 7

	// Parameters
	string edge_list_str = "(1, 2), (2,  3), (3,6), (6,7), (7,4), (3,4), (5,4), (2,5)";
	string length_list_str = "7, 4, 3, 4; 5,6, 10e3, 8";

	std::cout << "Edges list:" << std::endl;
	std::cout << edge_list_str << std::endl;

        std::cout << "Lengths list:" << std::endl;
        std::cout << length_list_str << std::endl;


	typedef pair<int, int> int_pair;
	
	// Data containers for edges and length lists
	vector<int_pair> net_edges;
	vector<double> net_length;

	// Parse edges list, put data into a container
	Parse.EdgesList(edge_list_str, &net_edges);

	// Parse length list, put data into a container
	Parse.LengthList(length_list_str, &net_length);

	unsigned int i_length = 0;
	
	// Populate network graph
	for(int_pair i_net_edge: net_edges) {

		add_edge(i_net_edge.first, i_net_edge.second,
			edgeProperties{i_length, net_length.at(i_length)},
			network);

		i_length++;
	}

        write_graphviz(std::cout, network);

	typedef graph_traits<undir_g>::edge_iterator edge_iterator;

	std::pair<edge_iterator, edge_iterator> net_edges_total = edges(network);

	for (edge_iterator i_edge = net_edges_total.first;
			i_edge != net_edges_total.second;
		       		++i_edge) {
		
		std::cout << source(*i_edge, network) << " <--> " 
			<< target(*i_edge, network) << " with weight of "
		       	<< network[*i_edge].length << " with id "
		       	<< network[*i_edge].id << std::endl;
	
	}


}
