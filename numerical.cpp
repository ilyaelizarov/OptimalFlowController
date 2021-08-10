/* Solves a system of linearized equations for the second Kirchhoff's law 
 * with the Newton's method */

#include "numerical.h"
#include "graph.h"
#include <iostream>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

Matrix<double, Dynamic, 1> Newton::GetFlowTree(Matrix<int, Dynamic, Dynamic> AdjTreeMat,
		Matrix<int, Dynamic, Dynamic> AdjChordMat,
		Matrix<double, Dynamic, 1>  NodesFlowVec,
		Matrix<double, Dynamic, 1> ChordsFlowVec) {

	// The last row must be thrown away
	unsigned int num_rows = AdjTreeMat.rows()-1;
	unsigned int num_cols = AdjTreeMat.cols();

	AdjTreeMat.block(0, 0, num_rows, num_cols) = 
		AdjTreeMat.block(0, 0, num_rows, num_cols);
	
	AdjTreeMat.conservativeResize(num_rows, num_cols);

        num_rows = AdjChordMat.rows()-1;
        num_cols = AdjChordMat.cols();

        AdjChordMat.block(0, 0, num_rows, num_cols) =
                AdjChordMat.block(0, 0, num_rows, num_cols);

        AdjChordMat.conservativeResize(num_rows, num_cols);

	num_rows = NodesFlowVec.rows()-1;
	num_cols = NodesFlowVec.cols();

        NodesFlowVec.block(0, 0, num_rows, num_cols) =
        NodesFlowVec.block(0, 0, num_rows, num_cols);

        NodesFlowVec.conservativeResize(num_rows, num_cols);

	// Solve for X_tree
	Matrix<double, Dynamic, 1> X_tree_unsorted = AdjTreeMat.cast<double>().inverse()*(NodesFlowVec - AdjChordMat.cast<double>()*ChordsFlowVec);

	unsigned int tree_num = AdjTreeMat.cols();
	unsigned int chords_num = AdjChordMat.cols();

	Matrix<double, Dynamic, 1> X = Matrix<double, Dynamic, 1>::Zero(tree_num+chords_num, 1);

	for (unsigned int i_branch = 0; i_branch != tree_num; i_branch++) {

		X(GraphNetwork::tree_column_to_id[i_branch],0) = X_tree_unsorted(i_branch, 0);
	
	};

	for (unsigned int i_branch = 0; i_branch != chords_num; i_branch++) {

		X(GraphNetwork::chords_column_to_id[i_branch],0) = ChordsFlowVec(i_branch, 0);

	}

        return X;

}


Matrix<double, Dynamic, 1> Newton::SolveFlowChords(Matrix<int, Dynamic, Dynamic> LoopMat,
                           DiagonalMatrix<double, Dynamic> ResMat,
                           DiagonalMatrix<double, Dynamic> FlowRateMat,
                           Matrix<double, Dynamic, 1> DisVec) const {

                        // Matrices with integers must be casted to double or float
                        Matrix<double, Dynamic, Dynamic> KirchhoffMat = 2*LoopMat.cast<double>()*
                                ResMat*FlowRateMat*LoopMat.cast<double>().transpose();

                        return KirchhoffMat.colPivHouseholderQr().solve(-DisVec);
}

Matrix<double, Dynamic, 1> Newton::SolveFlowBranches(Matrix<int, Dynamic, Dynamic> LoopMat,
                           DiagonalMatrix<double, Dynamic> ResMat,
                           Matrix<double, Dynamic, 1> FlowRateVec,
                           Matrix<double, Dynamic, 1> ResVec) {

                        // Convert flow rate vectror X into a diagonal matrix
                        DiagonalMatrix<double, Dynamic> mFlowRateMat(FlowRateVec.asDiagonal());

                        // Solve to get a correction for flow rate vector in chords dX_k
                        Matrix<double, Dynamic, 1> mCorFlowRateChordVec = SolveFlowChords(LoopMat, ResMat,
                                mFlowRateMat, ResVec);

                        // Transform flow rate correction vector dX_k to correction vector for all branches dX
                        Matrix<double, Dynamic, 1> mCorFlowRateVec = LoopMat.cast<double>().transpose()*
                                mCorFlowRateChordVec;

                        return FlowRateVec + mCorFlowRateVec;
}
