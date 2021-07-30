//
//  FingerprintSimilarityComputer.h
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//

#ifndef FINGERPRINTSIMILARITYCOMPUTER_H
#define FINGERPRINTSIMILARITYCOMPUTER_H

#include <map>
#include <vector>

using std::map;
using std::vector;

struct FingerprintSimilarity;

class FingerprintSimilarityComputer {

public:

	FingerprintSimilarityComputer(const vector<uint8_t> &fingerprint1, const vector<uint8_t> &fingerprint2);
	FingerprintSimilarity getMatchResults();

private:

	vector<uint8_t> fingerprint1;
	vector<uint8_t> fingerprint2;

};

#endif /* FINGERPRINTSIMILARITYCOMPUTER_H */
