/* 1. Parsers a string with edges, e.g. (1,2),(2,3) to a pair of integers vector
 * 2. Parsers a string with lengths, e.g. 1, 3.23; 4 to a double vector
 */

#include <iostream>
#include <iterator>
#include <string>
#include <regex>

using namespace std;

class ParseParameters {

	private:
        // Expression for splitting the string that describes edges list into pairs of numbers
        const regex c_pair_reg{"\\([0-9]+[,;][0-9]+\\)"};

	// Expression for splitting the pairs into vertexes
        const regex c_vertex_reg{"[0-9]+"};

	// Expression for splitting a string into numbers
	const regex c_number_reg{"[,;]"};

	public:
		void EdgesList (string edge_list_str, vector<pair<int, int>> * edges_vector) {

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

		void LengthList (string length_list_str, vector<double> * lengths_vector) {

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

} ParseParameters;
			

int main() {


	// Tests methods in the ParseParameters class

	string edge_list_str = "(123 ,-222), (43,  0), (70,33), (23,10), (24,5)";

	typedef pair<int, int> int_pair;
	vector<int_pair> net_edges; 

	ParseParameters.EdgesList(edge_list_str, &net_edges);

        for(int_pair i_net_edge: net_edges) {
                cout << "First vertex: " << i_net_edge.first << endl;
                cout << "Second vertex: " << i_net_edge.second << endl;
        }

	vector<double> net_lengths_method;

	ParseParameters.LengthList("-1, 2,4,5; 2.53, 10;175, 2.45, 2e3", &net_lengths_method);

	for(double length: net_lengths_method) {
                cout << "Length: " << length << endl;
        }
	
	return 0;
}	
