/* Solves a system of linearized equations for the second Kirchhoff's law 
 * with the Newton's method */

#include "numerical.h"
#include <iostream>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

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
