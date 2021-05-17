//
//  MapRankInteger.h
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//

#ifndef MAPRANKINTEGER_H
#define MAPRANKINTEGER_H

#include <map>
#include <vector>

using std::map;
using std::vector;

class MapRankInteger {

public:

	MapRankInteger(const map<int, int> &map, bool ascending);
	vector<int> getOrderedKeyList(int numKeys1, bool sharpLimit);

private:

	map<int, int> _map;
	vector<int> array;
	bool ascending { true };

	int getOrderedValue(int index);
	void locate(int left, int right, int index);
	void swap(int i, int j);

};

#endif /* MAPRANKINTEGER_H */
