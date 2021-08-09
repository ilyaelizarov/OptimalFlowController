/* Formulas to calculate hydraulic properties */

#include "hydraulics.h"
#include <iostream>

using namespace std;

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
