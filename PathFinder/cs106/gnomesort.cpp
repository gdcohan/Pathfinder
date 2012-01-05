/*
 *  gnomesort.cpp
 *  Assignment5
 *
 *  Created by Gregory Cohan on 12/29/11.
 *  Copyright 2011 Brown University. All rights reserved.
 *
 */

#include "gnomesort.h"
#include "vector.h"
#include <iostream>
#include "cmpfn.h"


template <typename Type>
void GnomeSort(Vector<Type> & toSort, int (cmp)(Type, Type) = OperatorCmp) {
	int pos = 1;
	while (pos < toSort.size()) {
		if (cmp(toSort[pos], toSort[pos-1]) >= 0) {
			pos += 1;
		} else {
			Type temp = toSort[pos];
			toSort[pos] = toSort[pos-1];
			toSort[pos-1] = temp;
			if (pos > 1) {
				pos -= 1;
			} else {
				pos += 1;
			}
		}
	}
}
				 
/*
int main() {
	Vector<int> v;
	v.add(1);
	v.add(4);
	v.add(3);
	v.add(9);
	v.add(0);
	v.add(-2);
	v.add(3);
	GnomeSort(v);
	for (int i = 0; i < v.size(); i++) {
		cout << v[i];
	} 
	cout << endl;
	return 0;
}
*/

