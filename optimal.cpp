#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

vector<double> m_flows = {2, 2, -2, -1};

vector< vector<double> > A_eq;
vector<double> A_eq_row = {0};

int i_oddity = 0;
int i_column = 0;

int num_loops = 3;

int main(void) {

	vector<double>::iterator begin_shift = m_flows.begin();

	A_eq_row.resize(m_flows.size());

	for (int i_loop = 0; i_loop != num_loops; i_loop++) {

		i_oddity = 1;
		i_column = i_loop;

		for (vector<double>::iterator i_m_flow=begin_shift; i_m_flow != begin_shift + 2; i_m_flow++) {

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

for (vector< vector<double> >::iterator i_counter = A_eq.begin();
	 i_counter != A_eq.end(); i_counter++) {

	for (vector<double>::iterator i_row_counter = (*i_counter).begin(); i_row_counter != (*i_counter).end();
		i_row_counter++) {

		cout << *i_row_counter << ' ';

	}

	cout << endl;

}


}
