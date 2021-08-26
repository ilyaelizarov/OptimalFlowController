/* Graph interpritation of the network */

#ifndef GRAPH_H
#define GRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include <Eigen/Dense>
#include <vector>
#include <map>

using namespace boost;
using namespace std;
using namespace Eigen;

class GraphNetwork {

	typedef pair<int, int> int_pair;

	// Bundled properties for nodes and edges
	struct nodeProperties {
        	unsigned int id; // Node no.
        	double m_flow; // Flow rate in the node
	};

	struct edgeProperties {
        	unsigned int id; // Edge no.
		double diameter; // Edge diameter
        	double length; // Length of the edge
		unsigned int source;
		// unsigned int target;
	};

        // Undirected graph type
        typedef adjacency_list<vecS, vecS, undirectedS, nodeProperties, edgeProperties> undir_g;

        typedef graph_traits<undir_g>::vertex_descriptor node;
        typedef graph_traits<undir_g>::edge_descriptor branch;
        typedef graph_traits<undir_g>::edge_iterator edge_iterator;

	pair<edge_iterator, edge_iterator> net_edges_total;

	vector<branch> all_edges;
	vector<branch> tree;
	vector<branch> chords;

	unsigned int total_edges_no = 0; 
	unsigned int tree_edges_no = 0;
	unsigned int chords_edges_no = 0;

	public:

	        static map <unsigned int, unsigned int>tree_column_to_id;
                static map <unsigned int, unsigned int>chords_column_to_id;

		// Adjacency matrices for the tree, chords, and a joined one
		static Matrix<int, Dynamic, Dynamic> A_tree_st;
		static Matrix<int, Dynamic, Dynamic> A_chord_st;
		static Matrix<int, Dynamic, Dynamic> A_st;

		// Loop matrix
		static Matrix<int, Dynamic, Dynamic> B_st;

		// Flow rate in the nodes
		static Matrix<double, Dynamic, 1> Q_st;

		undir_g network;

		// Populates network graph and assigns edges properties (length, index
		void Populate (vector<int_pair> net_edges, vector<double> net_diameters, vector<double> net_lengths);

		// Assigns mass flow rates in nodes
		void SetNodesFlow (vector<double> m_flows);

		// Gets the vector of flow rates in the nodes
		Matrix<double, Dynamic, 1> GetNodesFlow (void);

		// Prints network properties
		void Print (void);

		// Finds chords and tree edges
		void SplitTreeAndChords(void);

		// Gets adjacency matrix for chords and tree
		Matrix<int, Dynamic, Dynamic> GetAdjMatrix(void);
		Matrix<int, Dynamic, Dynamic> GetChordAdjMatrix(void);
                Matrix<int, Dynamic, Dynamic> GetTreeAdjMatrix(void);

		// Gets initial flow rate values in chords
		Matrix<double, Dynamic, 1> GetInitialChordsFlow(void);

		// Gets loop matrix
		Matrix<int, Dynamic, Dynamic> GetLoopMatrix(void);  

		// Prints tree
		void PrintTree(void);

};

#endif
