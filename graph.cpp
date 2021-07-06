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
// Flow rate in a node
struct nodeFlowRate {
	float m_flow;
};

// weights at edges
typedef boost::property<boost::edge_weight_t, double> EdgeWeightProperty;

// adjacency lists: undirected and directed
typedef adjacency_list<listS, vecS, undirectedS, nodeFlowRate, EdgeWeightProperty> MyGraph;

typedef graph_traits<MyGraph>::edge_descriptor MyEdge;
typedef graph_traits<MyGraph>::vertex_descriptor MyVertex;

// Graph for making a loop matrix
// 1 - 2 - 3
//     |   |
//     4 - 5

int main() {

	// Undirected graph for storing input data
	MyGraph g;

	// Graph definition
	add_edge(1, 2, 1, g);
	add_edge(2, 3, 2, g);
	add_edge(3, 4, 3, g);
	add_edge(4, 5, 4, g);
	add_edge(5, 2, 1, g);

	// extra cycle
	add_edge(3, 6, 2, g);
	add_edge(6, 7, 1, g);
	add_edge(4, 7, 2, g);

	// Mass flow rates in the nodes
	g[1].m_flow = 2;
	g[2].m_flow = 1;
	g[3].m_flow = -0.5;
	g[4].m_flow = -0.5;
	g[5].m_flow =  -2;
	g[6].m_flow = 1;
	g[7].m_flow = -1;

        write_graphviz(std::cout, g);

	property_map < MyGraph, edge_weight_t >::type weight = get(edge_weight, g);
	std::vector<MyEdge> spanning_tree;

	kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));

	std::cout << "Print the edges in the MST:" << std::endl;
  for (std::vector<MyEdge>::iterator ei = spanning_tree.begin();
       ei != spanning_tree.end(); ++ei) {
    std::cout << source(*ei, g) << " <--> " << target(*ei, g)
      << " with weight of " << weight[*ei]
      << std::endl;
  }


}
