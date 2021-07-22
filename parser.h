/* Parsers string list parameters of the model into data containers */

#ifndef PARSER_H
#define PARSER_H

#include <regex>

using namespace std;

class ParseParameters {

        // Expression for splitting the string that describes edges list into pairs of numbers
        const regex c_pair_reg{"\\([0-9]+[,;][0-9]+\\)"};

	// Expression for splitting the pairs into vertexes
        const regex c_vertex_reg{"[0-9]+"};

	// Expression for splitting a string into numbers
	const regex c_number_reg{"[,;]"};

	public:
		/* Parses a string with the edges list to populate a vector with the pairs of vertixes
		 * The string must be in the following format:
		 * (vertex1, vertex2), (vertex2, vertex3)
		 * e.g. (1, 2), (2,3)
		 */
		void EdgesList (string edge_list_str, vector<pair<int, int>> * edges_vector);

		/* Parses a string with the lengths list to populate a vector with lengths for every edge
		 * The string must be length1, length2, length3
		 * E.g. 1,2, 3, 4
		 */
		void LengthList (string length_list_str, vector<double> * lengths_vector);

		/* Parses a string with the diameters list to populate a vector with diameters for every edge
		 * Uses the same format as for LengthList()
		 */
		void DiameterList (string diameter_list_str, vector<double> * diameters_vector);
	       
};

#endif		
