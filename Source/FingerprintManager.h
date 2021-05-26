//
//  FingerprintManager.h
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//

#ifndef FINGERPRINTMANAGER_H
#define FINGERPRINTMANAGER_H

#include <vector>
#include "FingerprintProperties.h"

using std::vector;

class FingerprintManager {

public:

	static int getNumFrames(const vector<uint8_t> &fingerprint);

	vector<uint8_t> *extractFingerprint(const int16_t *wave, int waveLength);

private:

	// robustLists[x] = y1, y2, y3, ...
	vector<vector<int>> getRobustPointList(const vector<vector<float>> &spectrogramData);

private:

	int numFilterBanks { FingerprintProperties::numFilterBanks };
	float sampleRate { FingerprintProperties::sampleRate } ;

};

#endif /* FINGERPRINTMANAGER_H */
