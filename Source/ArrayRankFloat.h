//
//  ArrayRankFloat.h
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//

#ifndef ARRAYRANKFLOAT_H
#define ARRAYRANKFLOAT_H

#include <vector>

using std::vector;

class ArrayRankFloat {

public:

	ArrayRankFloat();
	ArrayRankFloat(const vector<float> &array);

	float getNthOrderedValue(int n, bool ascending);

private:

	vector<float> array;

	float getOrderedValue(int index);

	// sort the partitions by quick sort, and locate the target index
	void locate(int left, int right, int index);
	void swap(int i, int j);

};

#endif /* ARRAYRANKFLOAT_H */
