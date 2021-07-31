#include "ortools/linear_solver/linear_solver.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;
using namespace operations_research;

vector<double> m_flows = {1, 2, -2, -1};

vector< vector<double> > A_eq;
vector<double> A_eq_row = {0};

int i_oddity = 0;
int i_column = 0;

// Number of loops
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

// Print matrix A_eq
for (vector< vector<double> >::iterator i_counter = A_eq.begin();
	 i_counter != A_eq.end(); i_counter++) {

	for (vector<double>::iterator i_row_counter = (*i_counter).begin(); i_row_counter != (*i_counter).end();
		i_row_counter++) {

		cout << *i_row_counter << ' ';

	}

	cout << endl;

}


// Create a solver
unique_ptr<MPSolver> solver(MPSolver::CreateSolver("GLOP"));

// Create the tunnel constraints
vector<double> lbounds{0.0039, 0.0039, 0.01, 0.01};
vector<double> hbounds{2.5e7, 2.5e7, 0.1714, 0.45};

vector<const MPVariable*> x(4);
for (int i_opt_var =0; i_opt_var < 4; ++i_opt_var) {
        x[i_opt_var] = solver->MakeIntVar(lbounds[i_opt_var], hbounds[i_opt_var] , "");
}

// Create the equality constraints 
vector<double> bounds{0, 0, 0};

for (int i_constr = 0; i_constr < 3; ++i_constr) {
	MPConstraint* constraint1 = solver->MakeRowConstraint(bounds[i_constr],
			bounds[i_constr], "");
	for (int i_constr_coef =0; i_constr_coef < 4; ++i_constr_coef) {
		constraint1->SetCoefficient(x[i_constr_coef],
			       A_eq[i_constr][i_constr_coef]);
	}
}

// Create objective function
vector<double> obj_coeffs;

obj_coeffs.push_back(pow(abs(m_flows[0]), 3));
obj_coeffs.push_back(pow(abs(m_flows[1]), 3));
obj_coeffs.push_back(abs(m_flows[2]));
obj_coeffs.push_back(abs(m_flows[3]));

MPObjective* objective = solver->MutableObjective();

for (int i_obj; i_obj < 4; ++i_obj) {
	objective->SetCoefficient(x[i_obj], obj_coeffs[i_obj]);
}

objective->SetMinimization();

// Call the solver
const MPSolver::ResultStatus result_status = solver->Solve();

// Print results
if (result_status != MPSolver::OPTIMAL) {
  LOG(FATAL) << "The problem does not have an optimal solution.";
}
LOG(INFO) << "Solution:";
LOG(INFO) << "Optimal objective value = " << objective->Value();

for (int j = 0; j < 4; ++j) {
  LOG(INFO) << "x[" << j << "] = " << x[j]->solution_value();
}

}
