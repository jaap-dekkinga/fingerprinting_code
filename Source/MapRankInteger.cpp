//
//  MapRankInteger.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include "MapRankInteger.h"


MapRankInteger::MapRankInteger(const map<int, int> &_map, bool ascending) : _map(_map), ascending(ascending), array(_map.size())
{
}

vector<int> MapRankInteger::getOrderedKeyList(int numKeys, bool sharpLimit)
{
	// if sharp limited, will return sharp numKeys, otherwise will return until the values not equals the exact key's value

	vector<int> keyList;

	// if the numKeys is larger than map size, limit it
	if (numKeys > (int)_map.size()) {
		numKeys = (int)_map.size();
	}

	if (_map.size() > 0) {
//		var array = [Int](repeating: 0, count: map.count);
		int count = 0;

		// get the pass values
		for (auto it : _map) {
			array[count] = it.second;
			count += 1;
		}

		int targetindex;
		if (ascending) {
			targetindex = numKeys;
		} else {
			targetindex = ((int)array.size() - numKeys);
		}

		// get the passed keys and values
		int passValue = getOrderedValue(targetindex);	// this value is the value of the numKey-th element
		map<int, int> passedMap;
		vector<int> valueList;

		for (auto it : _map) {
			if ((ascending && (it.second <= passValue)) || (!ascending && (it.second >= passValue))) {
				passedMap[it.first] = it.second;
				valueList.push_back(it.second);
			}
		}

		// sort the value list
		sort(valueList.begin(), valueList.end());

		// get the list of keys
		int resultCount = 0;
		int index;

		if (ascending) {
			index = 0;
		} else {
			index = ((int)valueList.size() - 1);
		}

		if (!sharpLimit) {
			numKeys = (int)valueList.size();
		}

		while (true) {
			int targetValue = valueList[index];

			for (auto it : passedMap) {
				if (it.second == targetValue) {
					keyList.push_back(it.first);
					// TODO: not sure this leaves the loop iterator in a correct state
//					passedMap.removeValue(forKey: it.first);
					passedMap.erase(it.first);
					resultCount += 1;
					break;
				}
			}

			if (ascending) {
				index += 1;
			} else {
				index -= 1;
			}

			if (resultCount >= numKeys) {
				break;
			}
		}
	}

	return keyList;
}

// MARK: -
// MARK: Private

int MapRankInteger::getOrderedValue(int index)
{
	locate(0, ((int)array.size() - 1), index);
	return array[index];
}

// sort the partitions by quick sort, and locate the target index
void MapRankInteger::locate(int left, int right, int index)
{
	int mid = ((left + right) / 2);

	if (right == left) {
		return;
	}

	if (left < right) {
		int s = array[mid];
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

void MapRankInteger::swap(int i, int j)
{
	int t = array[i];
	array[i] = array[j];
	array[j] = t;
}
