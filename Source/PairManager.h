//
//  PairManager.h
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//

#ifndef PAIRMANAGER_H
#define PAIRMANAGER_H

#include <map>
#include <vector>
#include "ArrayCoord.h"
#include "FingerprintProperties.h"

using std::map;
using std::vector;

struct PairPosition {

	int hashcode;
	int position;

	PairPosition(int hashcode, int position) : hashcode(hashcode), position(position) { }

};

class PairManager {

public:

	PairManager();
	PairManager(bool isReferencePairing);

	map<int, vector<int>> getPair_PositionList_Table(const vector<uint8_t> &fingerprint);

private:

	int fps { FingerprintProperties::fps };
	int numFilterBanks { FingerprintProperties::numFilterBanks };
	int anchorPointsIntervalLength { FingerprintProperties::anchorPointsIntervalLength };
	int numAnchorPointsPerInterval { FingerprintProperties::numAnchorPointsPerInterval };
	int refMaxActivePairs { FingerprintProperties::refMaxActivePairs };
	int sampleMaxActivePairs { FingerprintProperties::sampleMaxActivePairs };
	int upperBoundedFrequency { FingerprintProperties::upperBoundedFrequency };
	int lowerBoundedFrequency { FingerprintProperties::lowerBoundedFrequency };
	int maxTargetZoneDistance { FingerprintProperties::maxTargetZoneDistance };
	int numFrequencyUnits { FingerprintProperties::numFrequencyUnits };

	int bandwidthPerBank;
	int maxPairs;
	bool isReferencePairing { true };
	map<int, bool> stopPairTable;


	vector<PairPosition> getPairPositionList(const vector<uint8_t> &fingerprint);
	vector<ArrayCoord> getSortedCoordinateList(const vector<uint8_t> &fingerprint);

};

#endif /* PAIRMANAGER_H */
