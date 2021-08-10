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

		/* Calculates flow rates in the tree
		 * X_tree = A_tree^-1 (Q - A_chords * X_chords)
		 * and adds flow rates in the chords from the arguments:
		 * X_tree + X_chords
		 * Arguments are adjacency matrices for a tree, chords, the flow rate in nodes
		 * The function will through the last row itself
		 */ 
		Matrix<double, Dynamic, 1> GetFlowTree(Matrix<int, Dynamic, Dynamic> AdjTreeMat,
                Matrix<int, Dynamic, Dynamic> AdjChordMat,
                Matrix<double, Dynamic, 1>  NodesFlowVec,
		Matrix<double, Dynamic, 1> ChordsFlowVec);
};

#endif
