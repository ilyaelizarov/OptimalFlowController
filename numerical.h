/* Solves a system of linearized equations for the second Kirchhoff's law 
 * with the Newton's method */

#ifndef NUMERICAL_H
#define NUMERICAL_H

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class Newton {

		// Solves linearized system 2*B*S*X(N)*B^T*dX_k(N) = -dH for dX_k
		Matrix<double, Dynamic, 1> SolveFlowChords(Matrix<int, Dynamic, Dynamic> LoopMat,
                           DiagonalMatrix<double, Dynamic> ResMat,
                           DiagonalMatrix<double, Dynamic> FlowRateMat,
                           Matrix<double, Dynamic, 1> DisVec) const;

	public:
		// Calculates one iteration for X(N+1)=X(N)+dX
		Matrix<double, Dynamic, 1> SolveFlowBranches(Matrix<int, Dynamic, Dynamic> LoopMat,
                           DiagonalMatrix<double, Dynamic> ResMat,
                           Matrix<double, Dynamic, 1> FlowRateVec,
                           Matrix<double, Dynamic, 1> ResVec);
};

#endif
