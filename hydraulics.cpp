/* Calculates hydraulic properties */

#include "hydraulics.h"
#include <iostream>
#include <math.h>

#define GRAVITY 9.81

using namespace std;

double Hydraulics::pressure_loss (double Q, double D, double L) const {

	const double mA = mArea(D);
	const double mV = mVelocity(Q, mA); 
	const double mRe = mReynolds(Q, D, mA, cRho, cMu);
	const double mFan = mFanning(mRe, D, cEps);

	// Pressure loss per unit meter
	const double mDpPerMeter = mDarcy_weisbach(mFan, mV, D, cRho);

	return mDpPerMeter * L;
	
}

double Hydraulics::mFanning (double Re, double D, double Eps) const {
	return 0.25 / ( pow( log10( (Eps / D) / 3.7 + 5.74 / pow(Re, 0.9) ), 2)  );
}

double Hydraulics::mDarcy_weisbach(double Fan, double V, double D, double Rho) const {
       return Fan * pow(V, 2) * Rho  / (2 * D) ;
}

double Hydraulics::mReynolds (double Q, double D, double A, double Rho, double Mu) const {
       return Rho * Q * D / (Mu * A);
}

double Hydraulics::mArea (double D) const {
       return M_PI * pow(D, 2)/4;
}

double Hydraulics::mVelocity(double Q, double A) const {
       return Q / A;
}
