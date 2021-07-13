/* Parsers a string with edges, e.g. (1,2),(2,3) to a pair type */

#include <iostream>
#include <iterator>
#include <string>
#include <regex>

using namespace std;

int main() {

	string edge_list_str = "(123 ,222), (43,  0), (70,33), (23,10)";

	// Erase all spaces in the string
	edge_list_str.erase(remove(edge_list_str.begin(), edge_list_str.end(), ' '), edge_list_str.end());	
	cout << edge_list_str << endl;

	// Splits the string into pairs for the format (number,number)
	regex reg_pair("\\([0-9]+[,;][0-9]+\\)");

	// Splits the pair into a pair of edges
	regex reg_vertex("[0-9]+");

	typedef pair<int, int> int_pair;
	vector<int_pair> net_edges; 
	typedef vector<int_pair> vector_pair;
	// vector_pair net_edges;
	//

	sregex_token_iterator pos_pair(edge_list_str.cbegin(), edge_list_str.cend(), reg_pair, 0);
        sregex_token_iterator end_pair;

	int i = 0;
	for( ; pos_pair!=end_pair; ++pos_pair) {
		
		string pair_str = *pos_pair;
		cout << ++i << endl;
		cout << pair_str << endl;


		sregex_token_iterator pos_edge(pair_str.begin(), pair_str.end(), reg_vertex, 0);
//		sregex_token_iterator end_edge;

//		cout << *(++pos_pair) << endl;

		int m_firstVertex =  stoi(*pos_edge);
//		pos_edge++;
		int m_secondVertex = stoi(*(++pos_edge));

		net_edges.push_back(make_pair(m_firstVertex, m_secondVertex));
	}

/*	for(vector_pair::iterator inet_edge = net_edge.begin();
		inet_edge != net_edge.end(); ++inet_edge) { */ 

	for(int_pair inet_edge: net_edges) {
		cout << "First vertex: " << inet_edge.first << endl;
		cout << "Second vertex: " << inet_edge.second << endl;
	}

	return 0;
}	
