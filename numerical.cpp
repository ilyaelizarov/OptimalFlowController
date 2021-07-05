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
		Matrix<double, Dynamic, 1> solveLoop(Matrix<int, Dynamic, Dynamic> LoopMat,
			   DiagonalMatrix<double, Dynamic> ResMat,
			   DiagonalMatrix<double, Dynamic> FlowRateMat,
			   Matrix<double, Dynamic, 1> DisVec) const {

			// Matrices with integers must be casted to double or float
			Matrix<double, Dynamic, Dynamic> KirchhoffMat = 2*LoopMat.cast<double>()*
				ResMat*FlowRateMat*LoopMat.cast<double>().transpose();

			return KirchhoffMat.colPivHouseholderQr().solve(-DisVec);

		}

		// Solves system A*X = Q for X
		Matrix<double, Dynamic, 1> solveAdj(Matrix<int, Dynamic, Dynamic> AdjMat,
				Matrix<double, Dynamic, 1> FlowRateNodesVec) const {

                        return AdjMat.cast<double>().colPivHouseholderQr().solve(FlowRateNodesVec);

		}


		// Calculates one iteration for X(N+1)=X(N)+dX
		Matrix<double, Dynamic, 1> iterationLoop(Matrix<int, Dynamic, Dynamic> LoopMat,
                           DiagonalMatrix<double, Dynamic> ResMat,
                           Matrix<double, Dynamic, 1> FlowRateVec,
                           Matrix<double, Dynamic, 1> ResVec) {

			// Convert flow rate vectror X into a diagonal matrix
			DiagonalMatrix<double, Dynamic> mFlowRateMat(FlowRateVec.asDiagonal()); 

			// Solve to get a correction for flow rate vector in chords dX_k
			Matrix<double, Dynamic, 1> mCorFlowRateChordVec = solveLoop(LoopMat, ResMat,
				mFlowRateMat, ResVec);

			// Transform flow rate correction vector dX_k to correction vector for all branches dX
			Matrix<double, Dynamic, 1> mCorFlowRateVec = LoopMat.cast<double>().transpose()*
				mCorFlowRateChordVec; 

			return FlowRateVec + mCorFlowRateVec;
		}
				
};

int main() {

	/* Validating a solver for loops
	 *
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

	*/

	Newton sol;

	/*
	// Adjacency matrix
	// Matrix is rank deficient, solution is not physical
        Matrix<int, 5, 5> A;
        A << -1, 0, 0, 0, 0,
	      1,-1, 0, 0, 1,
	      0, 1, 1, 0, 0,
	      0, 0,-1, 1, 0,
	      0, 0, 0, -1,-1;


	Matrix<double, 5, 1> Q;
	Q << 2, 0, 3, -3, -4;

	*/

	        // Adjacency matrix
        // Solution is not physical, need to take in to account B-matrix
        Matrix<int, 4, 5> A;
        A << -1, 0, 0, 0, 0,
              1,-1, 0, 0, 1,
              0, 1, 1, 0, 0,
              0, 0,-1, 1, 0;

	// Matrix for CGAL must be storen in column-major order
	// i.e. A[column][row]
	int Array[5][4];
	typedef Matrix<int, Dynamic, Dynamic> ColumnMatrixXi;

	// Map (convert) A on Array
	Map<ColumnMatrixXi>(&Array[0][0], 4, 5) = A;

	cout << A << endl;
	cout << "Array [0][0]: " << Array[0][0] << endl;
	cout << "Array [0][1]: " << Array[0][1] << endl;
        cout << "Array [0][2]: " << Array[0][2] << endl;
        cout << "Array [0][3]: " << Array[0][3] << endl;
        cout << "Array [1][0]: " << Array[1][0] << endl;
        cout << "Array [1][1]: " << Array[1][1] << endl;
        cout << "Array [1][2]: " << Array[1][2] << endl;
        cout << "Array [1][3]: " << Array[1][3] << endl;

        Matrix<double, 4, 1> Q;
        Q << 2, 2, 3, -4;


	cout << sol.solveAdj(A, Q) << endl;




}
