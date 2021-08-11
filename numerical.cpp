/* Solves a system of linearized equations for the second Kirchhoff's law 
 * with the Newton's method */

#include "numerical.h"
#include "hydraulics.h"
#include "graph.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

Matrix<double, Dynamic, 1> Newton::GetFlowBranches(Matrix<int, Dynamic, Dynamic> AdjTreeMat,
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
                           Matrix<double, Dynamic, 1> FlowRateMat,
                           Matrix<double, Dynamic, 1> DisVec) const {

			// Conversion to diagonal matrix
			DiagonalMatrix<double, Dynamic> FlowRateMatDiag = FlowRateMat.asDiagonal();

                        // Matrices with integers must be casted to double or float
                        Matrix<double, Dynamic, Dynamic> KirchhoffMat = 2*LoopMat.cast<double>()*
                                ResMat*FlowRateMatDiag*LoopMat.cast<double>().transpose();

                        return KirchhoffMat.colPivHouseholderQr().solve(-DisVec);
}

Matrix<double, Dynamic, 1> Newton::Solve(Matrix<int, Dynamic, Dynamic> AdjTreeMat,
					 Matrix<int, Dynamic, Dynamic> AdjChordMat,
					 Matrix<int, Dynamic, Dynamic> LoopMat,
//					 DiagonalMatrix<double, Dynamic> ResMat,

					 Matrix<double, Dynamic, 1> NodesFlowVec,
					 Matrix<double, Dynamic, 1> InitialChordsFlowVec,

					 vector<double> * BranchesDiameterVec,
				         vector<double> * BranchesLengthVec) {

	Matrix<double, Dynamic, 1> X_c = InitialChordsFlowVec;

	// S-diagonal matrix
	DiagonalMatrix<double, Dynamic> ResMat = GetResMat(AdjTreeMat,
				AdjChordMat,
				InitialChordsFlowVec,
				NodesFlowVec,
				BranchesDiameterVec,
				BranchesLengthVec);


	// X_0 = A_t^-1 * ( Q - A_c * X_c_0)
	Matrix<double, Dynamic, 1> X = GetFlowBranches(AdjTreeMat,
					AdjChordMat,
					NodesFlowVec,
					X_c);
	// Define dH, dX_c
	Matrix<double, Dynamic, 1> dH = Matrix<double, Dynamic, 1>::Zero(LoopMat.rows(), 1);
	Matrix<double, Dynamic, 1> dX_c = Matrix<double, Dynamic, 1>::Zero(AdjChordMat.cols(), 1);

	for (unsigned int i_iteration = 0; i_iteration != 20; i_iteration++) {

		// dH = B * sign(X) * h(X) [Pa]
		dH = GetResVec(LoopMat,
			X,
			BranchesDiameterVec,
			BranchesLengthVec);

		cout << "Iteration: " << i_iteration << endl;

		cout << "dH:" << endl;
		cout << dH << endl;

		// dX_c = -dH / (2 * B * S * X * B^T) [m3/s]
		dX_c = SolveFlowChords(LoopMat,
			ResMat,
			X,
			dH);

		cout << "dX_c:" << endl;
		cout << dX_c << endl;

		// X_c_plus_1 = X_c + dX_c [m3/s]
		X_c = X_c + dX_c;

		cout << "X_c:" << endl;
		cout << X_c << endl;
				
		// X = A_t^-1 (Q - A_c * X_c)
		X = GetFlowBranches(AdjTreeMat,
			AdjChordMat,
			NodesFlowVec,
			X_c);

		cout << "X:" << endl;
		cout << X << endl;
	}

	return X;

}

DiagonalMatrix<double, Dynamic> Newton::GetResMat(Matrix<int, Dynamic, Dynamic> AdjTreeMat,
                                         Matrix<int, Dynamic, Dynamic> AdjChordMat,

					 Matrix<double, Dynamic, 1> InitialChordsFlowVec,
					 Matrix<double, Dynamic, 1> NodesFlowVec,

                                         vector<double> * BranchesDiameterVec,
                                         vector<double> * BranchesLengthVec) {

	Matrix<double, Dynamic, 1> X_0 = GetFlowBranches(AdjTreeMat,
						AdjChordMat,
				                NodesFlowVec,
       					        InitialChordsFlowVec);

	// Declare S-vector
        Matrix<double, Dynamic, 1> ResMatVec = Matrix<double, Dynamic, 1>::Zero((AdjTreeMat.cols() + AdjChordMat.cols()), 1);

        Hydraulics HydraulicMethods;

	for (unsigned int i_branch = 0; i_branch != (AdjTreeMat.cols() + AdjChordMat.cols());
			i_branch++ ) {

		ResMatVec(i_branch, 0) = HydraulicMethods.pressure_loss(abs(X_0(i_branch, 0)),
                                (*BranchesDiameterVec).at(i_branch),
                                (*BranchesLengthVec).at(i_branch)) / pow (X_0(i_branch, 0), 2);

	};

	return ResMatVec.asDiagonal();
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

Matrix<double, Dynamic, 1> Newton::GetResVec(Matrix<int, Dynamic, Dynamic> LoopMat,
		Matrix<double, Dynamic, 1> FlowRateVec,
		vector<double> * BranchesDiameter,
		vector<double> * BranchesLength) {

		// Get a sign: sgn(X)
		Matrix<double, Dynamic, 1> SignVec = Matrix<double, Dynamic, 1>::Zero(FlowRateVec.rows(), 1);

//		cout << "Sign vector for the flow rates in the branches:" << endl;

		for (unsigned int i_branch = 0; i_branch != FlowRateVec.rows();
		   i_branch++ ) {

			// What to do if FlowRate is 0?
			SignVec(i_branch, 0) = (FlowRateVec(i_branch, 0) >= 0) ? 1 : -1;
//			cout << SignVec(i_branch, 0) << " ";

		};

//		cout << endl;

		// Pressure loss vector
                Matrix<double, Dynamic, 1> PressureLossVec = Matrix<double, Dynamic, 1>::Zero(FlowRateVec.rows(), 1);

		Hydraulics HydraulicMethods;

//              cout << "Pressure loss vector:" << endl;

                for (unsigned int i_branch = 0; i_branch != FlowRateVec.rows();
                   i_branch++ ) {

			PressureLossVec(i_branch, 0) = HydraulicMethods.pressure_loss(abs(FlowRateVec(i_branch, 0)),
				(*BranchesDiameter).at(i_branch),
			       	(*BranchesLength).at(i_branch));

//			cout << PressureLossVec(i_branch, 0) << " ";


                };

//		cout << endl;

		return LoopMat.cast<double>()*PressureLossVec.cwiseProduct(SignVec);

}
