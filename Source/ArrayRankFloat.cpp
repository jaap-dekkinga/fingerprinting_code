//
//  ArrayRankFloat.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include "ArrayRankFloat.h"


ArrayRankFloat::ArrayRankFloat()
{
}

ArrayRankFloat::ArrayRankFloat(const vector<float> &array) : array(array)
{
}

// MARK: -
// MARK: Public

float ArrayRankFloat::getNthOrderedValue(int n, bool ascending)
{
	int targetIndex = n;

	if (targetIndex > array.size()) {
		targetIndex = (int)array.size();
	}

	if (ascending == false) {
		targetIndex = ((int)array.size() - targetIndex);
	}

	// this value is the value of the numKey-th element
	float passValue = getOrderedValue(targetIndex);

	return passValue;
}

// MARK: -
// MARK: Private

float ArrayRankFloat::getOrderedValue(int index)
{
	locate(0, ((int)array.size() - 1), index);
	return array[index];
}

void ArrayRankFloat::locate(int left, int right, int index)
{
	// sort the partitions by quick sort, and locate the target index
	if (right == left) {
		return;
	}

	if (left < right) {
		int mid = ((left + right) / 2);
		float s = array[mid];
		int i = (left - 1);
		int j = (right + 1);

		while (true) {
			do {
				i += 1;
			} while (array[i] < s);

			do {
				j -= 1;
			} while (array[j] > s);

			if (i >= j) {
				break;
			}

			swap(i, j);
		}

		if (i > index) {
			// the target index in the left partition
			locate(left, (i - 1), index);
		} else {
			// the target index in the right partition
			locate((j + 1), right, index);
		}
	}
}

void ArrayRankFloat::swap(int i, int j)
{
	float t = array[i];
	array[i] = array[j];
	array[j] = t;
}
