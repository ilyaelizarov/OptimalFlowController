/* Formulas to calculate hydraulic properties */

#include <iostream>
#include <math.h>

#define GRAVITY 9.81

using namespace std;

class Hydraulics {

	public:

	/* Returns pressure loss in Pa. 
	 * Arguments are: volumetric flow rate Q [m^3/s], pipe's diameter D [m], pipe's length L [m]
	 * */
	double pressure_loss (double Q, double D, double L) const {

		const double mA = mArea(D);
		const double mV = mVelocity(Q, mA); 
		const double mRe = mReynolds(Q, D, mA, cRho, cMu);
		const double mFan = mFanning(mRe, D, cEps);

		// Pressure loss per unit meter
		const double mDpPerMeter = mDarcy_weisbach(mFan, mV, D, cRho);

		return mDpPerMeter * L;
	}

	private:

	       // Pipe roughness
	       const double cEps = 0.0011e-3;

	       // Water density
	       const double cRho = 1000;

	       // Water viscosity
	       const double cMu = 1e-3;

	       // Fanning friction factor (the Swamee-Jain equation)
	       double mFanning (double Re, double D, double Eps) const {
			return 0.25 / ( pow( log10( (Eps / D) / 3.7 + 5.74 / pow(Re, 0.9) ), 2)  );
		}

	       // Pressure loss per unit meter (the Darcy-Weisbach formula)
	       double mDarcy_weisbach(double Fan, double V, double D, double Rho) const {
		       return Fan * pow(V, 2) * Rho  / (2 * D) ;
		}

	       // Reynolds number
	       double mReynolds (double Q, double D, double A, double Rho, double Mu) const {
		       return Rho * Q * D / (Mu * A);
		}

	       // Cross-sectional area
	       double mArea (double D) const {
		       return M_PI * pow(D, 2)/4;
		}

	       // Velocity of the flow
	       double mVelocity(double Q, double A) const {
		       return Q / A;
		}
	       	       
};

int main() {
	
	Hydraulics hyd;

	/* Validating pressure loss results
	 * Input values:
	 * - Volumetric flow rate Q = 0.0277 m^3/s
	 * - Diameter of pipe D = 0.1 m
	 * - Length of pipe L = 10 m
	 * Result:
	 * - Pressure loss 8850 Pa
	 */

	cout << "Pressure loss for the given input values: " << hyd.pressure_loss(0.0277, 0.1, 10) << " Pa" << endl;
	cout << "The return value must be 8850 Pa" << endl;

}
