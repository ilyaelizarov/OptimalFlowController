/* Tests numerical methods for the controller */

#include "numerical.h"
#include <Eigen/Dense>
#include <iostream>

using namespace std;
using namespace Eigen;

int main() {

	Newton NumericalMethods;

        // Loop matrix
        Matrix<int, 3, 6> B;
        B << 1, 0, 0, 1, -1, 0,
             0, 1, 0, 0,  1, 1,
             0, 0, 0, 1,  0, 1;

	cout << "B:" << endl;
	cout << B << endl;

        // Resistance coefficient matrix
        DiagonalMatrix<double, 6> S;
        S.diagonal() << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;

	cout << "S:" << endl;
	// cout << S << endl;

       // Initial flow rate matrix
        Matrix<double, 6, 1> X_0;
        X_0 << 2.0, 3.0, 4.0, 5.0, 6.0, 7.0;

	cout << "X_0:" << endl;
	cout << X_0 << endl;

       // Residuals vector
        Matrix<double, 3, 1> dH;
        dH << 10, 20, 30;

	cout << "dH:" << endl;
	cout << dH << endl;

	Matrix<double, Dynamic, 1> X_next;
	
	X_next = NumericalMethods.SolveFlowBranches(B, S, X_0, dH);

	cout << "X_next:" << endl;
	cout << X_next << endl;

}
