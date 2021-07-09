/* The code takes a graph and forms an adjacency matrix A */

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <iostream>
#include <stack>

using namespace boost;

/* Bundled properties */

struct nodeProperties {
	uint32_t id; // Node no.
	double m_flow; // Flow rate in the node
};

struct edgeProperties {
	uint32_t id; // Edge no.
	double length; // Length of the branch
};

// Hot part of the network without flow directions
typedef adjacency_list<listS, vecS, undirectedS, nodeProperties, edgeProperties> HotNetworkUndir;

typedef graph_traits<HotNetworkUndir>::edge_descriptor Branch;
typedef graph_traits<HotNetworkUndir>::vertex_descriptor Node;

// Graph for making a loop matrix
// 1 - 2 - 3 - 6
//     |   |   |
//     4 - 5 - 7

int main() {

	// Undirected graph for storing input data
	HotNetworkUndir g;

	// Populating a graph
	add_edge(1, 2, {1, 1}, g);
	add_edge(2, 3, {2, 2}, g);
	add_edge(3, 4, {3, 3}, g);
	add_edge(4, 5, {4, 4}, g);
	add_edge(5, 2, {5, 5}, g);

	// extra cycle
	add_edge(3, 6, {6, 6}, g);
	add_edge(6, 7, {7, 7}, g);
	add_edge(4, 7, {8, 8}, g);

	// Mass flow rates in the nodes
	g[1].m_flow = 2;
	g[2].m_flow = 1;
	g[3].m_flow = -0.5;
	g[4].m_flow = -0.5;
	g[5].m_flow =  -2;
	g[6].m_flow = 1;
	g[7].m_flow = -1;

        write_graphviz(std::cout, g);

//	property_map<HotNetworkUndir, edge_weight_t>::type weight = get(edge_weight, g);
	std::vector<Branch> spanning_tree;

	kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree), weight_map(get(&edgeProperties::length, g)));

	std::cout << "Print the edges in the MST:" << std::endl;
  for (std::vector<Branch>::iterator ei = spanning_tree.begin();
       ei != spanning_tree.end(); ++ei) {
    std::cout << source(*ei, g) << " <--> " << target(*ei, g)
      << " with weight of " << g[*ei].length
      << " with id " << g[*ei].id
      << std::endl;
  }


}
