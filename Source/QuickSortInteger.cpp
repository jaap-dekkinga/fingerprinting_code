//
//  QuickSortInteger.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include "QuickSortInteger.h"


QuickSortInteger::QuickSortInteger(const vector<int> &array) : array(array)
{
	for (int i = 0; i < (int)array.size(); i++) {
		indexes.push_back(i);
	}
}

vector<int> QuickSortInteger::getSortIndexes()
{
	quicksort(0, ((int)indexes.size() - 1));
	return indexes;
}

// MARK: -
// MARK: Private

void QuickSortInteger::quicksort(int left, int right)
{
	if (right <= left) {
		return;
	}

	int i = partition(left, right);
	quicksort(left, (i - 1));
	quicksort((i + 1), right);
}

int QuickSortInteger::partition(int left, int right)
{
	int i = (left - 1);
	int j = right;

	while (true) {
		// find item on left to swap, a[right] acts as sentinel

		do {
			i += 1;
		} while (array[indexes[i]] < array[indexes[right]]);

		// find item on right to swap
		while (true) {
			j -= 1;

			if (!(array[indexes[right]] < array[indexes[j]])) {
				break;
			}

			if (j == left) {
				// don't go out-of-bounds
				break;
			}
		}

		// check if pointers cross
		if (i >= j) {
			break;
		}

		// swap two elements into place
		swap(i, j);
	}

	// swap with partition element
	swap(i, right);

	return i;
}

void QuickSortInteger::swap(int i, int j)
{
	int swap = indexes[i];
	indexes[i] = indexes[j];
	indexes[j] = swap;
}
