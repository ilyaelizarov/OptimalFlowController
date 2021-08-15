/* Solves a system of linearized equations for the second Kirchhoff's law 
 * with the Newton's method */

#ifndef NUMERICAL_H
#define NUMERICAL_H

#include <Eigen/Dense>
#include <vector>

using namespace std;
using namespace Eigen;

class Newton {

		// Solves linearized system 2 * B * S * X *B^T * dX_chord = -dH for dX_chord
		// Arguments: a loop matrix B, a resistance vector (matrix) S in the diagonal form,
		// flow rate in the branches vector (matrix) X in the diagonal form,
		// pressure loss residuals vector
		Matrix<double, Dynamic, 1> SolveFlowChords(Matrix<int, Dynamic, Dynamic> LoopMat,
                           DiagonalMatrix<double, Dynamic> ResMatDiag,
                           DiagonalMatrix<double, Dynamic> FlowRateMatDiag,
                           Matrix<double, Dynamic, 1> PressureLossResidualsVec) const;

                // Calculates pressure drop residuals vector dH = B * sgn(X) * X^2 * S
                // Arguments: flow rate in the branches vector, a resistance vector S
                Matrix<double, Dynamic, 1> GetPressureLossResidualsVec(Matrix<double, Dynamic, 1> FlowRateVec,
                Matrix<double, Dynamic, 1> ResVec);

                // Calculates flow rates in the tree branches X_tree = A_tree^-1 (Q - A_chord * X_chord)
		// and sums them up with the chords X = X_tree + X_chord
		// Arguments: a tree adjacency matrix A_tree, an adjacency matrix for chords A_chord,
		// vector of flow rates in the nodes Q, vector with flow rates in the chords X_chord
                Matrix<double, Dynamic, 1> GetFlowBranches(Matrix<int, Dynamic, Dynamic> AdjTreeMat,
                Matrix<int, Dynamic, Dynamic> AdjChordMat,

                Matrix<double, Dynamic, 1>  NodesFlowVec,
                Matrix<double, Dynamic, 1> ChordsFlowVec);

                // Calculates resistance (S-vector) S = |dP(X, D, L)| / X^2
		// Arguments: flow rate in the branches vector X, pointer to a vector with branches diameter
		// D, pointer to a vector with lengths of the branches L
                Matrix<double, Dynamic, 1> GetResVec(Matrix<double, Dynamic, 1> FlowRateVec,

                                         vector<double> * BranchesDiameterVec,
                                         vector<double> * BranchesLengthVec);


	public:

		// Iterates to find a solution for flow rates in all branches
		// Arguments: initial flow rates in the chords X_chord_0, pointer to a vector with
		// branches diameter D, pointer to a vector with lengths of the branches L
		Matrix<double, Dynamic, 1> Solve(Matrix<double, Dynamic, 1> InitialChordsFlowVec,

                                         vector<double> * BranchesDiameterVec,
                                         vector<double> * BranchesLengthVec);

};


#endif
