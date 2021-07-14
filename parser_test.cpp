/* Test methods of the ParseParameters class */

 #include "parser.h"
#include <iostream>

using namespace std;

int main() {

	ParseParameters Parse;

	string edge_list_str = "(123 ,222), (43,  0), (70,33), (23,10), (24,5)";

	typedef pair<int, int> int_pair;
	vector<int_pair> net_edges; 

	Parse.EdgesList(edge_list_str, &net_edges);

        for(int_pair i_net_edge: net_edges) {
                cout << "First vertex: " << i_net_edge.first << endl;
                cout << "Second vertex: " << i_net_edge.second << endl;
        }

	vector<double> net_lengths_method;

	Parse.LengthList("-1, 2,4,5; 2.53, 10;175, 2.45, 2e3", &net_lengths_method);

	for(double length: net_lengths_method) {
                cout << "Length: " << length << endl;
        }
	
	return 0;
}
