/* Calculates hydraulic properties */

class Hydraulics {

	// Pipe roughness
        const double cEps = 0.0011e-3;

        // Water density
        const double cRho = 1000;

        // Water viscosity
        const double cMu = 1e-3;

        // Fanning friction factor (the Swamee-Jain equation)
        double mFanning (double Re, double D, double Eps) const;
       
        // Pressure loss per unit meter (the Darcy-Weisbach formula)
        double mDarcy_weisbach(double Fan, double V, double D, double Rho) const;

        // Reynolds number
        double mReynolds (double Q, double D, double A, double Rho, double Mu) const;

	// Cross-sectional area
        double mArea (double D) const;
       
        // Velocity of the flow
        double mVelocity(double Q, double A) const;
       
	public:

		/* Returns pressure loss in Pa. 
         	* Arguments are: volumetric flow rate Q [m^3/s], pipe's diameter D [m], pipe's length L [m]
        	* */
        	double pressure_loss (double Q, double D, double L) const;

};	
