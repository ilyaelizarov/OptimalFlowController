/* Graph interpritation of the network */

#include "graph.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/range.hpp>
#include <vector>
#include <iostream>

using namespace boost;
using namespace std;

void GraphNetwork::Populate (vector<int_pair> net_edges, vector<double> net_lengths) {
	
	unsigned int i_length = 0;

	for (int_pair i_net_edge: net_edges) {

		add_edge(i_net_edge.first, i_net_edge.second,
                        edgeProperties{i_length, net_lengths.at(i_length)},
                        network);

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

