/* Graph interpritation of the network */

#ifndef GRAPH_H
#define GRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include <vector>

using namespace boost;
using namespace std;

class GraphNetwork {

	typedef pair<int, int> int_pair;

	// Bundled properties for nodes and edges
	struct nodeProperties {
        	unsigned int id; // Node no.
        	double m_flow; // Flow rate in the node
	};

	struct edgeProperties {
        	unsigned int id; // Edge no.
        	double length; // Length of the edge
	};
	
	// Undirected graph type
	typedef adjacency_list<vecS, vecS, undirectedS, nodeProperties, edgeProperties> undir_g;

	typedef graph_traits<undir_g>::vertex_descriptor node;
	typedef graph_traits<undir_g>::edge_iterator edge_iterator;

	pair<edge_iterator, edge_iterator> net_edges_total;

	public:

		undir_g network;		

		// Populates network graph and assigns edges properties (length, index
		void Populate (vector<int_pair> net_edges, vector<double> net_lengths);

		// Assigns mass flow rates in nodes
		void NodesFlow (double m_flows[]);

		// Prints network properties
		void Print (void);
};

#endif
