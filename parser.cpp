/* Parsers string list parameters of the model into data containers */

#include "parser.h"
#include <iterator>
#include <string>

using namespace std;

void ParseParameters::EdgesList (string edge_list_str, vector<pair<int, int>> * edges_vector) {

	// Erase all spaces in the string
        edge_list_str.erase(remove(edge_list_str.begin(), edge_list_str.end(), ' '), edge_list_str.end());

	// Matching sequence iterator for the pair
        sregex_token_iterator i_pair(edge_list_str.cbegin(), edge_list_str.cend(), c_pair_reg, 0);
        sregex_token_iterator i_pair_last;

	// Parsing every pair to get vertexes
	for(; i_pair!=i_pair_last; ++i_pair) {
		string pair_str = *i_pair;

		// Matching sequence iterator for the vertex
		sregex_token_iterator i_vertex(pair_str.cbegin(), pair_str.cend(), c_vertex_reg, 0);

		int first_vertex = stoi(*i_vertex);
		int second_vertex = stoi(*(++i_vertex));

		edges_vector->push_back(make_pair(first_vertex, second_vertex));

	}
}

void ParseParameters::LengthList (string length_list_str, vector<double> * lengths_vector) {

	// Erase all spaces in the string
        length_list_str.erase(remove(length_list_str.begin(), length_list_str.end(), ' '), length_list_str.end());

	// Matching sequence iterator for the length
        sregex_token_iterator i_length(length_list_str.cbegin(), length_list_str.cend(), c_number_reg, -1);
        sregex_token_iterator i_length_last;

	// Parsing every length number
        for(; i_length!=i_length_last; ++i_length) {

		double length = stod(*i_length);

       		lengths_vector->push_back(length);

        }

}

void ParseParameters::DiameterList (string diameter_list_str, vector<double> * diameters_vector) {ParseParameters::LengthList(diameter_list_str, diameters_vector);}
