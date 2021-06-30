/* Solves a system of linearized equations for the second Kirchhoff's law 
 * with the Newton's method */

#include <iostream>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class Newton {

	public:
		void solve(Matrix<int, Dynamic, Dynamic> LoopMat,
			   DiagonalMatrix<double, Dynamic> ResMat,
			   DiagonalMatrix<double, Dynamic> FlowRateMat,
			   Matrix<double, Dynamic, 1> ResVec) const {

		// Matrices with integers must be casted to double or float
		Matrix<double, Dynamic, Dynamic> KirchhoffMat = 2*LoopMat.cast<double>()*
			ResMat*FlowRateMat*LoopMat.cast<double>().transpose();

		cout << KirchhoffMat << endl;

		cout << KirchhoffMat.colPivHouseholderQr().solve(-ResVec) << endl;

	}
};

int main() {

	// Loop matrix
	Matrix<int, 3, 5> B;
	B << 1, 0, 0, 1, -1,
	     0, 1, 0, 0,  1,
	     0, 0, 0, 1,  0;

	// Resistance coefficient matrix
	DiagonalMatrix<double, 5> S;
	S.diagonal() << 1.0, 2.0, 3.0, 4.0, 5.0;

	// Flow rate matrix
	DiagonalMatrix<double, 5> X;
	X.diagonal() << 2.0, 3.0, 4.0, 5.0, 6.0;
	Newton sol;

	// Residual vector
	Matrix<double, 3, 1> dH;
	dH << 10, 20, 30;

	// Solves for x in Ax=b
	sol.solve(B, S, X, dH);

}
