/* Solves a system of linearized equations for the second Kirchhoff's law 
 * with the Newton's method */

#include "numerical.h"
#include "hydraulics.h"
#include "graph.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <Eigen/Dense>

#define CONVERGENCE 1e-6

using namespace std;
using namespace Eigen;

// Calculates flow rate in the tree branches and adds it to a flow rate in the branches vector
// along with the flow rate in the chords
Matrix<double, Dynamic, 1> Newton::GetFlowBranches(Matrix<int, Dynamic, Dynamic> AdjTreeMat,
		Matrix<int, Dynamic, Dynamic> AdjChordMat,
		Matrix<double, Dynamic, 1>  NodesFlowVec,
		Matrix<double, Dynamic, 1> ChordsFlowVec) {

	unsigned int num_rows = AdjTreeMat.rows()-1;
	unsigned int num_cols = AdjTreeMat.cols();

        // The last row of the adjacency matrix A_tree for the tree must be thrown away
	// to avoid singularity
	AdjTreeMat.block(0, 0, num_rows, num_cols) = 
		AdjTreeMat.block(0, 0, num_rows, num_cols);
	AdjTreeMat.conservativeResize(num_rows, num_cols);
	
	// As well as for the adjacency matrix for the branches A_branch
        num_rows = AdjChordMat.rows()-1;
        num_cols = AdjChordMat.cols();
        AdjChordMat.block(0, 0, num_rows, num_cols) =
                AdjChordMat.block(0, 0, num_rows, num_cols);
        AdjChordMat.conservativeResize(num_rows, num_cols);

	// And for the vector that stores flow rates in the nodes Q
	num_rows = NodesFlowVec.rows()-1;
	num_cols = NodesFlowVec.cols();
        NodesFlowVec.block(0, 0, num_rows, num_cols) =
        NodesFlowVec.block(0, 0, num_rows, num_cols);
        NodesFlowVec.conservativeResize(num_rows, num_cols);

	// X_tree = A_tree^-1 (Q - A_chord * X_chord)
	Matrix<double, Dynamic, 1> X_tree_unsorted = AdjTreeMat.cast<double>().inverse()*(NodesFlowVec - AdjChordMat.cast<double>()*ChordsFlowVec);

	unsigned int tree_num = AdjTreeMat.cols();
	unsigned int chords_num = AdjChordMat.cols();

	Matrix<double, Dynamic, 1> X = Matrix<double, Dynamic, 1>::Zero(tree_num+chords_num, 1);

	// Vector with the flow rates in the branches needs to be sorted according to the id numbers
	// given to branches when the network graph was populated
	for (unsigned int i_branch = 0; i_branch != tree_num; i_branch++) {

		X(GraphNetwork::tree_column_to_id[i_branch],0) = X_tree_unsorted(i_branch, 0);
	
	};

	for (unsigned int i_branch = 0; i_branch != chords_num; i_branch++) {

		X(GraphNetwork::chords_column_to_id[i_branch],0) = ChordsFlowVec(i_branch, 0);

	}

        return X;

}

// Calculates the difference for flow rate in the chords dX_chord to perform the next iteration step
Matrix<double, Dynamic, 1> Newton::SolveFlowChords(Matrix<int, Dynamic, Dynamic> LoopMat,
                           DiagonalMatrix<double, Dynamic> ResMatDiag,
                           DiagonalMatrix<double, Dynamic> FlowRateMatDiag,
                           Matrix<double, Dynamic, 1> PressureLossResidualsVec) const {

                        // Matrices with integers must be casted to double or float!
                        Matrix<double, Dynamic, Dynamic> KirchhoffMat = 2*LoopMat.cast<double>()*
                                ResMatDiag*FlowRateMatDiag*LoopMat.cast<double>().transpose();

                        // Solves linearized system 2 * B * S * X *B^T * dX_chord = -dH for dX_chord
                        return KirchhoffMat.colPivHouseholderQr().solve(-PressureLossResidualsVec);
}

// Iterates to find a solution for flow rates in all branches until a convergence criteria is met
Matrix<double, Dynamic, 1> Newton::Solve(Matrix<double, Dynamic, 1> InitialChordsFlowVec,

					 vector<double> * BranchesDiameterVec,
				         vector<double> * BranchesLengthVec) {

	// Initial flow rate in the chords X_chord_0
	Matrix<double, Dynamic, 1> X_chord = InitialChordsFlowVec;

	// Calculates initial flow rate in all branches
	// as X_0 = A_t^-1 * ( Q - A_c * X_c_0)
	// Gets adjacency matrices A_tree and A_chord from the GraphNetwork class
	// as well as flow rates in the nodes as static types
	Matrix<double, Dynamic, 1> X = GetFlowBranches(GraphNetwork::A_tree_st,
                                                GraphNetwork::A_chord_st,
                                                GraphNetwork::Q_st,
                                                X_chord);

	// Declares resistance vector (S-vector)
	Matrix<double, Dynamic, 1> S = Matrix<double, Dynamic, 1>::Zero(GraphNetwork::B_st.cols(), 1);


	Matrix<double, Dynamic, 1> A11 = Matrix<double, Dynamic, 1>::Zero(GraphNetwork::B_st.cols(), 1);

	Matrix<int, Dynamic, Dynamic> A21 = GraphNetwork::A_st;

	A21.block(0, 0, A21.rows()-1, A21.cols()) = A21.block(0, 0, A21.rows()-1, A21.cols());
        A21.conservativeResize(A21.rows()-1, A21.cols());

	Matrix<double, Dynamic, 1> Q_cut = GraphNetwork::Q_st;
	Q_cut.block(0, 0, Q_cut.rows() - 1 , Q_cut.cols()) = Q_cut.block(0, 0, Q_cut.rows() - 1 , Q_cut.cols());
        Q_cut.conservativeResize(Q_cut.rows() - 1 , Q_cut.cols());


/*	// Declares dH and dX_chord vectors
	Matrix<double, Dynamic, 1> dH = Matrix<double, Dynamic, 1>::Zero(GraphNetwork::B_st.rows(), 1);
*/
	Matrix<double, Dynamic, 1> dX = Matrix<double, Dynamic, 1>::Zero(GraphNetwork::A_chord_st.cols(), 1);

	// Declares M1 matrix: number of rows = rows from B-matrix + rows from A-matrix (left-out)
	Matrix<double, Dynamic, Dynamic> M1 = Matrix<double, Dynamic, Dynamic>::Zero( (GraphNetwork::B_st.rows() + A21.rows()), GraphNetwork::A_st.cols() );

	// Declares M2 matrix
	Matrix<double, Dynamic, Dynamic> M2 = Matrix<double, Dynamic, Dynamic>::Zero( (GraphNetwork::B_st.rows() + A21.rows()), 1);


	// Uses an algorithm from the article 'A Gradient Algorithm for the Analysis of Pipe Networks' E. Todini, S. Pilati, 1988
	//
	do {

		// S = |dP(X, D, L)| / X^2
		S = GetResVec(X, BranchesDiameterVec, BranchesLengthVec);
		
		// A11 matrix: S .* |X|
		A11 = S.cwiseProduct(X.cwiseAbs());

		// M1 matrix: | M31*A11| where M31 = B, A21 = A (with one row left-out)
		//            |   A21  |
		 M1 << GraphNetwork::B_st.cast<double>()*A11.asDiagonal(), A21.cast<double>();

		
		// M2 matrix: | M31 * A11 * X |  Q with one row left-out
		// 	      |   A21*X - Q   |
		 M2 << GraphNetwork::B_st.cast<double>()*A11.asDiagonal()*X, A21.cast<double>()*X - Q_cut; 

		dX = M1.colPivHouseholderQr().solve(M2);

		X = X - dX;

		/* cout << "X:" << endl;
		cout << X << endl; */

	}
	while (dX.cwiseAbs().maxCoeff() > CONVERGENCE);

	return X;

}

Matrix<double, Dynamic, 1> Newton::GetResVec(Matrix<double, Dynamic, 1> FlowRateVec,

                                         vector<double> * BranchesDiameterVec,
                                         vector<double> * BranchesLengthVec) {

        // Declare S-vector
        Matrix<double, Dynamic, 1> ResVec = Matrix<double, Dynamic, 1>::Zero(FlowRateVec.rows(), 1);

        Hydraulics HydraulicMethods;

        for (unsigned int i_branch = 0; i_branch != FlowRateVec.rows(); i_branch++ ) {
		
			ResVec(i_branch, 0) = HydraulicMethods.pressure_loss(abs(FlowRateVec(i_branch, 0)),
                                (*BranchesDiameterVec).at(i_branch),
                                (*BranchesLengthVec).at(i_branch)) / pow (FlowRateVec(i_branch, 0), 2);

        };

        return ResVec;
}

Matrix<double, Dynamic, 1> Newton::GetPressureLossResidualsVec(Matrix<double, Dynamic, 1> FlowRateVec,
		Matrix<double, Dynamic, 1> ResVec) {

		// A sign vector i.e. sgn(X)
		Matrix<double, Dynamic, 1> SignVec = Matrix<double, Dynamic, 1>::Zero(FlowRateVec.rows(), 1);

		for (unsigned int i_branch = 0; i_branch != FlowRateVec.rows();
		   i_branch++ ) {

			// What to do if FlowRate is 0?
			SignVec(i_branch, 0) = (FlowRateVec(i_branch, 0) >= 0) ? 1 : -1;

		};

		// Pressure loss residuals vector
                Matrix<double, Dynamic, 1> PressureLossResidualsVec = Matrix<double, Dynamic, 1>::Zero(GraphNetwork::B_st.rows(), 1);

		// Flow rates squared vector
		Matrix<double, Dynamic, 1> FlowRateVecSquared = FlowRateVec.array().pow(2);

		 PressureLossResidualsVec = GraphNetwork::B_st.cast<double>()*( FlowRateVecSquared.cwiseProduct(ResVec).cwiseProduct(SignVec) );


		return PressureLossResidualsVec;

}
