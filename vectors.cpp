/* Compares two vectors and shows the difference */

#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

int main() {

	vector<int> v1 {2, 3, 1, 0, 4, 6};
	vector<int> v2 {2, 6, 3, 4};
	vector<int> diff;

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
		inserter(diff, diff.begin()));

	for (vector<int>::iterator i=diff.begin(); i != diff.end(); ++i) {
		cout << *i << ' ' << endl;
	}

}

	

