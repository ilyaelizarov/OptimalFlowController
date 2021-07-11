/* Parsers a string with edges, e.g. (1,2),(2,3) to a pair type */

#include <iostream>
#include <iterator>
#include <string>
#include <regex>

using namespace std;

int main() {

	string str = "(123 ;222), (43,  0), (70,33), (23,10)";

	// Erase all spaces in the string
	str.erase(remove(str.begin(), str.end(), ' '), str.end());	
	cout << str << endl;

	// Splits the string into pairs for the format (number,number)
	regex reg_pair("\\([0-9]+[,;][0-9]+\\)");
	sregex_token_iterator pos_pair(str.cbegin(), str.cend(), reg_pair, 0);
	sregex_token_iterator end_pair;

	// Splits the pair into a pair of edges
	regex reg_vertex("[0-9]+");
	typedef pair<int, int> int_pair;
	vector<int_pair> net_edges; 
	// typedef vector<int_pair> vector_pair;
	//vector_pair net_edges;

	for( ; pos_pair!=end_pair; ++pos_pair) {
		string pair_str = *pos_pair;
		sregex_token_iterator pos_edge(pair_str.cbegin(), pair_str.cend(), reg_vertex, 0);
		
		net_edges.push_back(make_pair(stoi(*pos_edge), stoi(*(++pos_edge))));
	}

/*	for(vector_pair::iterator inet_edges = net_edges.begin();
		inet_edges != net_edges.end(); ++inet_edges) { */

	for(int_pair inet_edge: net_edges) {
		cout << "First vertex: " << inet_edge.first << endl;
		cout << "Second vertex: " << inet_edge.second << endl;
	}

	return 0;
}	
