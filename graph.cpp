/* Graph interpritation of the network */

#include "graph.h"
#include "udgcd.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/range.hpp>
#include <Eigen/Dense>
#include <vector>
#include <map>
#include <iostream>

using namespace boost;
using namespace std;
using namespace Eigen;

map<unsigned int, unsigned int> GraphNetwork::tree_column_to_id;
map<unsigned int, unsigned int> GraphNetwork::chords_column_to_id;

Matrix<int, Dynamic, Dynamic> GraphNetwork::A_tree_st;
Matrix<int, Dynamic, Dynamic> GraphNetwork::A_chord_st;
Matrix<int, Dynamic, Dynamic> GraphNetwork::A_st;
Matrix<double, Dynamic, 1> GraphNetwork::Q_st;
Matrix<int, Dynamic, Dynamic> GraphNetwork::B_st;


void GraphNetwork::Populate (vector<int_pair> net_edges, vector<double> net_diameters, vector<double> net_lengths) {
	
	unsigned int i_length = 0;

	for (int_pair i_net_edge: net_edges) {

		bool connected;
		branch current_branch;

		tie(current_branch, connected) = add_edge(i_net_edge.first, i_net_edge.second,
                        edgeProperties{i_length, net_diameters.at(i_length), net_lengths.at(i_length)},
                        network);

		this->all_edges.push_back(current_branch);

		i_length++;
	}
}

void GraphNetwork::SetNodesFlow(vector<double> m_flows) {

	unsigned int i_flows = 0;

	for (node i_vertex : make_iterator_range(vertices(network))) {

                network[i_vertex].m_flow = m_flows.at(i_flows);
                i_flows++;
        }
}

Matrix<double, Dynamic, 1> GraphNetwork::GetNodesFlow(void) {

	unsigned int i_row = 0;

	Matrix<double, Dynamic, 1> Q = Matrix<double, Dynamic, 1>::Zero(num_vertices(network), 1);

	for (node i_vertex : make_iterator_range(vertices(network))) {

		Q(i_vertex, 0) = network[i_vertex].m_flow;
		
		i_row++;
	}

	Q_st = Q;

	return Q;

}

		
void GraphNetwork::SplitTreeAndChords(void) {

	this->tree_edges_no = 0;
	this->chords_edges_no = 0;

	kruskal_minimum_spanning_tree(network, back_inserter(this->tree), weight_map(get(&edgeProperties::length, network)));

	// Get total edges number
	this->total_edges_no = this->all_edges.size();

	sort(this->all_edges.begin(), this->all_edges.end());
        sort(this->tree.begin(), this->tree.end());

	this->tree_edges_no = this->tree.size();

        set_difference(this->all_edges.begin(), this->all_edges.end(),
		       	this->tree.begin(), this->tree.end(),
                inserter(this->chords, chords.begin()));

	this->chords_edges_no = this->chords.size();

}

Matrix<int, Dynamic, Dynamic> GraphNetwork::GetChordAdjMatrix(void) {

	int i_column = 0;

	Matrix<int, Dynamic, Dynamic> A_chord = Matrix<int, Dynamic, Dynamic>::Zero(num_vertices(network), chords_edges_no) ;
       
	for (vector<branch>::iterator i_chord=this->chords.begin(); i_chord !=this->chords.end(); ++i_chord) {

                chords_column_to_id.insert(pair<int, int>(i_column, network[*i_chord].id));

		node source_chord = source(*i_chord, network);
		node target_chord = target(*i_chord, network);

//		cout << "Flow rate in the source: " << network[source_chord].m_flow << endl;

		if (network[source_chord].m_flow >= network[target_chord].m_flow) {

			A_chord(source_chord, i_column) = 1;
			A_chord(target_chord, i_column) = -1;
			network[*i_chord].source = source_chord;
			// network[*i_chord].target = target_chord;

		} else {
			
			A_chord(source_chord, i_column) = -1;
                        A_chord(target_chord, i_column) = 1;
			network[*i_chord].source = target_chord;
			// network[*i_chord].target = source_chord;
		}

		i_column++;

	}


	A_chord_st = A_chord;

	return A_chord;

}

Matrix<double, Dynamic, 1> GraphNetwork::GetInitialChordsFlow(void) {

	Matrix<double, Dynamic, 1> X_c_0 = Matrix<double, Dynamic, 1>::Zero(chords_edges_no, 1);

	int i_edges_chord = 0;

	for (vector<branch>::iterator i_chord=this->chords.begin(); i_chord !=this->chords.end(); ++i_chord) {

		node source_chord = source(*i_chord, network);
		node target_chord = target(*i_chord, network);

		if (network[source_chord].m_flow == 0) {
		
			X_c_0(i_edges_chord, 0) = network[target_chord].m_flow;
		
		} else if (network[target_chord].m_flow == 0) {
			
			X_c_0(i_edges_chord, 0) = network[source_chord].m_flow;
		
		} else if (network[source_chord].m_flow >= network[target_chord].m_flow) {

			X_c_0(i_edges_chord, 0) = network[source_chord].m_flow;

		} else {
		
			X_c_0(i_edges_chord, 0) = network[target_chord].m_flow;
		
		}
		
		i_edges_chord++;
	
	}

	return X_c_0;

}


Matrix<int, Dynamic, Dynamic> GraphNetwork::GetAdjMatrix(void) {

	Matrix<int, Dynamic, Dynamic> A = Matrix<int, Dynamic, Dynamic>::Zero(num_vertices(network), total_edges_no);

	for (vector<branch>::iterator i_branch=this->all_edges.begin(); i_branch != this->all_edges.end(); ++i_branch) {

                node source_branch = source(*i_branch, network);
                node target_branch = target(*i_branch, network);

                if (network[source_branch].m_flow >= network[target_branch].m_flow) {

                        A(source_branch, network[*i_branch].id) = 1;
                        A(target_branch, network[*i_branch].id) = -1;

                } else {

                       A(source_branch, network[*i_branch].id) = -1;
                       A(target_branch, network[*i_branch].id) = 1;
                }

        }

        A_st = A;

        return A;

}

Matrix<int, Dynamic, Dynamic> GraphNetwork::GetTreeAdjMatrix(void) {

	int i_column = 0;

        Matrix<int, Dynamic, Dynamic> A_tree = Matrix<int, Dynamic, Dynamic>::Zero(num_vertices(network), tree_edges_no);
        
	for (vector<branch>::iterator i_tree=this->tree.begin(); i_tree !=this->tree.end(); ++i_tree) {

                tree_column_to_id.insert(pair<int, int>(i_column, network[*i_tree].id));

//		cout << "Column " << i_column << " corresponds to edge " << tree_column_to_id[i_column] << endl;

                node source_tree = source(*i_tree, network);
                node target_tree = target(*i_tree, network);


                if (network[source_tree].m_flow >= network[target_tree].m_flow) {

                        A_tree(source_tree, i_column) = 1;
                        A_tree(target_tree, i_column) = -1;
                        network[*i_tree].source = source_tree;
                        // network[*i_tree].target = target_tree;


                } else {

                       A_tree(source_tree, i_column) = -1;
                       A_tree(target_tree, i_column) = 1;
                       network[*i_tree].source = target_tree;
                       // network[*i_tree].target = source_tree;


                }

                i_column++;

        }


	A_tree_st = A_tree;

        return A_tree;

}

Matrix<int, Dynamic, Dynamic> GraphNetwork::GetLoopMatrix(void) {

	unsigned int i_cycle = 0;
	unsigned int i_vertex = 0;

	node first_node_cycle;

	pair<node, node> verteces;
	branch branch;

	// set null output
	streambuf * orig_buf = cout.rdbuf();
    	cout.rdbuf(NULL);

	vector<vector<node>> cycles = udgcd::findCycles<undir_g, node>(network);

	// restore output
	cout.rdbuf(orig_buf);

        Matrix<int, Dynamic, Dynamic> B = Matrix<int, Dynamic, Dynamic>::Zero(cycles.size(), num_edges(network)) ;

	for (vector< vector<node> >::iterator i_column = cycles.begin();
			i_column != cycles.end(); i_column++) {

		for (vector<node>::iterator i_row = (*i_column).begin();
				i_row != (*i_column).end();
					i_row++) {

			i_vertex++;

			if (i_vertex == 1) {

				first_node_cycle = *i_row;
				verteces.first = *i_row;

			} else if (i_vertex % 2 == 0) {
				
				verteces.second = *i_row;

				// Inspect an edge
				// cout << "(" << verteces.first << "," << verteces.second << ")" << endl;
				branch = edge(verteces.first, verteces.second, network).first;
				// cout << "Investigated edge is: " << network[branch].id << endl;
				if (network[branch].source == verteces.first) {

					B(i_cycle, network[branch].id) = 1;

				} else {

					B(i_cycle, network[branch].id) = -1;
				};


				verteces.first = *i_row;

			} else if (i_vertex % 3 == 0) {

				verteces.second = *i_row;

				// Inspect an edge
                                // cout << "(" << verteces.first << "," << verteces.second << ")" << endl;
                                branch = edge(verteces.first, verteces.second, network).first;
                                // cout << "Investigated edge is: " << network[branch].id << endl;
                                if (network[branch].source == verteces.first) {

                                        B(i_cycle, network[branch].id) = 1;

                                } else {

                                        B(i_cycle, network[branch].id) = -1;
                                };


				
				verteces.first = *i_row;

			};


			if ( i_row == (*i_column).end() - 1) {

				verteces.second = first_node_cycle;
				verteces.first = *i_row;
				
				// Inspect an edge
                                // cout << "(" << verteces.first << "," << verteces.second << ")" << endl;
                                branch = edge(verteces.first, verteces.second, network).first;
                                // cout << "Investigated edge is: " << network[branch].id << endl;
                                if (network[branch].source == verteces.first) {

                                        B(i_cycle, network[branch].id) = 1;

                                } else {

                                        B(i_cycle, network[branch].id) = -1;
                                };



			}

			 // cout << "Node in the cycle: " << *i_row << endl;
		}

		i_vertex = 0;
		// cout << "New cycle" << endl;
		i_cycle++;
	}


	B_st = B;

	return B;
}

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

