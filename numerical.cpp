/* Solves a system of linearized equations for the second Kirchhoff's law 
 * with the Newton's method */

#include <iostream>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class Newton {

	public:

		// Solves linearized system 2*B*S*X(N)*B^T*dX_k(N) = -dH for dX_k
		Matrix<double, Dynamic, 1> solveLin(Matrix<int, Dynamic, Dynamic> LoopMat,
			   DiagonalMatrix<double, Dynamic> ResMat,
			   DiagonalMatrix<double, Dynamic> FlowRateMat,
			   Matrix<double, Dynamic, 1> ResVec) const {

			// Matrices with integers must be casted to double or float
			Matrix<double, Dynamic, Dynamic> KirchhoffMat = 2*LoopMat.cast<double>()*
				ResMat*FlowRateMat*LoopMat.cast<double>().transpose();

			return KirchhoffMat.colPivHouseholderQr().solve(-ResVec);

		}


		// Calculates one iteration for X(N+1)=X(N)+dX
		Matrix<double, Dynamic, 1> iteration(Matrix<int, Dynamic, Dynamic> LoopMat,
                           DiagonalMatrix<double, Dynamic> ResMat,
                           Matrix<double, Dynamic, 1> FlowRateVec,
                           Matrix<double, Dynamic, 1> ResVec) {

			// Convert flow rate vectror X into a diagonal matrix
			DiagonalMatrix<double, Dynamic> mFlowRateMat(FlowRateVec.asDiagonal()); 

			// Solve to get a correction for flow rate vector in chords dX_k
			Matrix<double, Dynamic, 1> mCorFlowRateChordVec = solveLin(LoopMat, ResMat,
				mFlowRateMat, ResVec);

			// Transform flow rate correction vector dX_k to correction vector for all branches dX
			Matrix<double, Dynamic, 1> mCorFlowRateVec = LoopMat.cast<double>().transpose()*
				mCorFlowRateChordVec; 

			return FlowRateVec + mCorFlowRateVec;
		}
				
};

int main() {

	// Loop matrix
	Matrix<int, 3, 6> B;
	B << 1, 0, 0, 1, -1, 0,
	     0, 1, 0, 0,  1, 1,
	     0, 0, 0, 1,  0, 1;

	// Resistance coefficient matrix
	DiagonalMatrix<double, 6> S;
	S.diagonal() << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;

	// Initial flow rate matrix
	Matrix<double, 6, 1> X_pre;
	X_pre << 2.0, 3.0, 4.0, 5.0, 6.0, 7.0;

	// Instantate Newton class
	Newton sol;

	// Residual vector
	Matrix<double, 3, 1> dH;
	dH << 10, 20, 30;

	cout << X_pre << endl;
	cout << endl;

	Matrix<double, Dynamic, 1> X_next;

	for(int iIter = 0; iIter < 100; iIter++) {

		X_next = sol.iteration(B, S, X_pre, dH);

		X_pre = X_next;

		cout << X_next << endl;

		cout << endl;
	};

}
