/* Graph interpritation of the network */

#include "graph.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/range.hpp>
#include <Eigen/Dense>
#include <vector>
#include <iostream>

using namespace boost;
using namespace std;
using namespace Eigen;

void GraphNetwork::Populate (vector<int_pair> net_edges, vector<double> net_lengths) {
	
	unsigned int i_length = 0;

	for (int_pair i_net_edge: net_edges) {

		bool connected;
		branch current_branch;

		tie(current_branch, connected) = add_edge(i_net_edge.first, i_net_edge.second,
                        edgeProperties{i_length, net_lengths.at(i_length)},
                        network);

		this->all_edges.push_back(current_branch);

		i_length++;
	}
}

void GraphNetwork::NodesFlow(vector<double> m_flows) {

	unsigned int i_flows = 0;

	for (node i_vertex : make_iterator_range(vertices(network))) {

                network[i_vertex].m_flow = m_flows.at(i_flows);
                i_flows++;
        }
}

void GraphNetwork::SplitTreeAndChords(void) {

	this->tree_edges_no = 0;
	this->chords_edges_no = 0;

	kruskal_minimum_spanning_tree(network, back_inserter(this->tree), weight_map(get(&edgeProperties::length, network)));

	sort(this->all_edges.begin(), this->all_edges.end());
        sort(this->tree.begin(), this->tree.end());

	this->tree_edges_no = this->tree.size();

        set_difference(this->all_edges.begin(), this->all_edges.end(),
		       	this->tree.begin(), this->tree.end(),
                inserter(this->chords, chords.begin()));

	this->chords_edges_no = this->chords.size();

}

Matrix<int, Dynamic, Dynamic> GraphNetwork::ChordAdjMatrix(void) {

	this->i_edges_global = 0;

	Matrix<int, Dynamic, Dynamic> A_chord = Matrix<int, Dynamic, Dynamic>::Zero(num_vertices(network), chords_edges_no) ;
       
	for (vector<branch>::iterator i_chord=this->chords.begin(); i_chord !=this->chords.end(); ++i_chord) {

		node source_chord = source(*i_chord, network);
		node target_chord = target(*i_chord, network);

		if (network[source_chord].m_flow >= network[target_chord].m_flow) {

			A_chord(source_chord, this->i_edges_global) = 1;
			A_chord(target_chord, this->i_edges_global) = -1;

		} else {
			
			A_chord(source_chord, this->i_edges_global) = -1;
                        A_chord(target_chord, this->i_edges_global) = 1;
		}

		this->i_edges_global++;

	}

	
	return A_chord;

}

Matrix<int, Dynamic, Dynamic> GraphNetwork::TreeAdjMatrix(void) {

	// Change this later
	this->i_edges_global = 0;

        Matrix<int, Dynamic, Dynamic> A_tree = Matrix<int, Dynamic, Dynamic>::Zero(num_vertices(network), tree_edges_no) ;



        for (vector<branch>::iterator i_tree=this->tree.begin(); i_tree !=this->tree.end(); ++i_tree) {

                node source_tree = source(*i_tree, network);
                node target_tree = target(*i_tree, network);

                if (network[source_tree].m_flow >= network[target_tree].m_flow) {

                        A_tree(source_tree, this->i_edges_global) = 1;
                        A_tree(target_tree, this->i_edges_global) = -1;

                } else {

                        A_tree(source_tree, this->i_edges_global) = -1;
                       A_tree(target_tree, this->i_edges_global) = 1;
                }

                this->i_edges_global++;

        }


        return A_tree;

}


/* void GraphNetwork::AdjacencyMatrix()
 * 
 * 1. Iterate through tree edges e.g. in PrintTree()
 * 1.1 Compare m_flow in source and target vertexes
 * 1.2 Write in A matrix for column with edge.id put 1, -1 in rows that correspond to vertixes
 * 2. Iterate through chord edges
 */

void GraphNetwork::PrintTree(void) {

	cout << "Tree:" << endl;
	for (vector<branch>::iterator i_tree=this->tree.begin(); i_tree !=this->tree.end(); ++i_tree) {

			cout << *i_tree << ' ' << network[*i_tree].id << endl;
			
	}

	cout << "Chords: " << endl;
        for (vector<branch>::iterator i_chord=this->chords.begin(); i_chord !=this->chords.end(); ++i_chord) {

                        cout << *i_chord << ' ' << network[*i_chord].id << endl;
        }


}

void GraphNetwork::Print(void) {

	net_edges_total = edges(network);

        for (edge_iterator i_edge = net_edges_total.first;
                        i_edge != net_edges_total.second;
                                ++i_edge) {
                
		cout << source(*i_edge, network) << " <--> " 
                     << target(*i_edge, network) << " with weight of "
                     << network[*i_edge].length << " with id "
                     << network[*i_edge].id << " m_flow in node "
                     << source(*i_edge, network) << " is "
                     << network[source(*i_edge, network)].m_flow << " m_flow in node "
                     << target(*i_edge, network) << " is "
                     << network[target(*i_edge, network)].m_flow << endl;
        
	}
}

