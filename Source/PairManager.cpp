//
//  PairManager.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include "FingerprintManager.h"
#include "PairManager.h"
#include "QuickSortInteger.h"


PairManager::PairManager()
{
	bandwidthPerBank = (numFrequencyUnits / numFilterBanks);
	maxPairs = refMaxActivePairs;
}

PairManager::PairManager(bool isReferencePairing) : isReferencePairing(isReferencePairing)
{
	// Constructor, number of pairs of robust points depends on the parameter isReferencePairing
	// no. of pairs of reference and sample can be different due to environmental influence of source

	bandwidthPerBank = (numFrequencyUnits / numFilterBanks);
	if (isReferencePairing) {
		maxPairs = refMaxActivePairs;
	} else {
		maxPairs = sampleMaxActivePairs;
	}
}

/**
* Get a pair-positionList table
* It's a hash map which the key is the hashed pair, and the value is list of positions
* That means the table stores the positions which have the same hashed pair
*/

map<int, vector<int>> PairManager::getPair_PositionList_Table(const vector<uint8_t> &fingerprint)
{
	vector<PairPosition> pairPositionList = getPairPositionList(fingerprint);

	// table to store pair: pos, pos, pos, ...; pair2: pos, pos, pos, ...
	map<int, vector<int>> pair_positionList_table;

	// get all pair positions from list, use a table to collect the data group by pair hashcode
	for (auto& pairPosition : pairPositionList) {
		// group by pair-hashcode, i.e.: <pair, List<position>>
		vector<int> &array = pair_positionList_table[pairPosition.hashcode];
		array.push_back(pairPosition.position);
	}

	return pair_positionList_table;
}

// MARK: -
// MARK: Private

// this return list contains: int[0] = pair_hashcode, int[1] = position
vector<PairPosition> PairManager::getPairPositionList(const vector<uint8_t> &fingerprint)
{
	int numFrames = FingerprintManager::getNumFrames(fingerprint);

	// table for paired frames
	vector<uint8_t> pairedFrameTable((numFrames / anchorPointsIntervalLength + 1));

	// each second has numAnchorPointsPerSecond pairs only
	vector<PairPosition> pairList;
	vector<ArrayCoord> sortedCoordinateList = getSortedCoordinateList(fingerprint);

	for (auto& anchorPoint : sortedCoordinateList) {
		int numPairs = 0;

		for (auto& targetPoint : sortedCoordinateList) {

			if (numPairs >= maxPairs) {
				break;
			}

			if (isReferencePairing && pairedFrameTable[anchorPoint.x / anchorPointsIntervalLength] >= numAnchorPointsPerInterval) {
				break;
			}

			if ((anchorPoint.x == targetPoint.x) && (anchorPoint.y == targetPoint.y)) {
				continue;
			}

			// pair up the points
			int x1;
			int y1;
			int x2;
			int y2;	// x2 always >= x1

			if (targetPoint.x >= anchorPoint.x) {
				x2 = targetPoint.x;
				y2 = targetPoint.y;
				x1 = anchorPoint.x;
				y1 = anchorPoint.y;
			} else {
				x2 = anchorPoint.x;
				y2 = anchorPoint.y;
				x1 = targetPoint.x;
				y1 = targetPoint.y;
			}

			// check target zone
			if ((x2 - x1) > maxTargetZoneDistance) {
				continue;
			}

			// check filter bank zone
			if (!((y1 / bandwidthPerBank) == (y2 / bandwidthPerBank))) {
				// same filter bank should have equal value
				continue;
			}

			int pairHashcode = (x2 - x1) * numFrequencyUnits * numFrequencyUnits + y2 * numFrequencyUnits + y1;

			// stop list applied on sample pairing only
			if (!isReferencePairing && (stopPairTable.find(pairHashcode) != stopPairTable.end())) {
				numPairs += 1;	// no reservation
				continue;	// escape this point only
			}

			// pass all rules
			pairList.push_back(PairPosition(pairHashcode, anchorPoint.x));
			pairedFrameTable[anchorPoint.x / anchorPointsIntervalLength] += 1;
			numPairs += 1;
		}
	}

	return pairList;
}

vector<ArrayCoord> PairManager::getSortedCoordinateList(const vector<uint8_t> &fingerprint)
{
	// each point data is 8 bytes
	// x: 2 byte integer
	// y: 2 byte integer
	// intensity: 4 bytes

	// get all intensities
	int numCoordinates = ((int)fingerprint.size() / 8);
	vector<int> intensities(numCoordinates);

	for (int i = 0; i < numCoordinates; i++) {
		int pointer = (i * 8 + 4);
		int intensity = (int)(fingerprint[pointer] & 0xFF) << 24 | (int)(fingerprint[pointer + 1] & 0xFF) << 16 | (int)(fingerprint[pointer + 2] & 0xFF) << 8 | (int)(fingerprint[pointer + 3] & 0xFF);
		intensities[i] = intensity;
	}

	QuickSortInteger quicksort(intensities);
	vector<int> sortIndexes = quicksort.getSortIndexes();

	vector<ArrayCoord> sortedCoordinateList;
	int i = ((int)sortIndexes.size() - 1);

	while (i >= 0) {
		int pointer = (sortIndexes[i] * 8);
		int x = (((int)fingerprint[pointer + 0] << 8) | (int)fingerprint[pointer + 1]);
		int y = (((int)fingerprint[pointer + 2] << 8) | (int)fingerprint[pointer + 3]);
		sortedCoordinateList.push_back(ArrayCoord(x, y));
		i -= 1;
	}

	return sortedCoordinateList;
}
