using namespace std;

vector<double> m_flows = {1, 2, -2, -1};

vector<double> A_eq_row = {0};

m_flows::iterator begin_shift = m_flows.begin();

A_eq_row.resize(m_flows.size());


int i_prosumer = 0;
int i_column = 0;

for (/* Iterate over nodes, i_node is an iterator, initially i_node = 0 */ ) {

	i_oddity = 1;
	i_column = i_node;

	for (m_flows::iterator i_m_flow=begin_shift; i_m_flow != begin_shift + 2; i_m_flow++) {

		// Consumption mode, every first branch
		if ( (*i_m_flow > 0) && !(i_oddity % 2) ) {

			A_eq_row.at(i_column) = -pow(*i_m_flow,2);

		// Consumption mode, every second branch
		} else if ( (*i_m_flow > 0) && (i_oddity % 2) ) {
		
			A_eq_row.at(i_column) = pow(*i_m_flow,2);
	
		// Production mode, every first branch
		} else if ( (*i_m_flow < 0) && !(i_oddity % 2) ) {

			A_eq_row.at(i_column) = -1;

		// Production mode, every second branch
		} else {

			A_eq_row.at(i_column) = 1;
		}
	
		i_oddity++;
		i_column++;
	
	}


	A_eq.push_back(A_eq_row);

	fill(A_eq_row.begin(), A_eq_row.end(), 0.0);

	begin_shift++;


}

struct OptimizationDataModel {
	vector<vector<double>> A_eq {
		{-1; 

