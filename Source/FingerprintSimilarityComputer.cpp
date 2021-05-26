//
//  FingerprintSimilarityComputer.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include <limits.h>
#include "Fingerprint.h"
#include "FingerprintManager.h"
#include "FingerprintSimilarityComputer.h"
#include "MapRankInteger.h"
#include "PairManager.h"


FingerprintSimilarityComputer::FingerprintSimilarityComputer(const vector<uint8_t> &fingerprint1, const vector<uint8_t> &fingerprint2) : fingerprint1(fingerprint1), fingerprint2(fingerprint2)
{
}

FingerprintSimilarity FingerprintSimilarityComputer::getMatchResults()
{
	FingerprintSimilarity results;
	map<int, int> offsetScoreTable;
	int numFrames = 0;

	// reset the results
	results.score = 0.0f;
	results.similarity = 0.0f;
	results.mostSimilarFramePosition = INT_MIN;
	results.mostSimilarStartTime = 1.0f;

	// one frame may contain several points, use the shorter one be the denominator
	if (fingerprint1.size() > fingerprint2.size()) {
		numFrames = FingerprintManager::getNumFrames(fingerprint2);
	} else {
		numFrames = FingerprintManager::getNumFrames(fingerprint1);
	}

	// get the pairs
	PairManager pairManager;
	map<int, vector<int>> this_Pair_PositionList_Table = pairManager.getPair_PositionList_Table(fingerprint1);
	map<int, vector<int>> compareWave_Pair_PositionList_Table = pairManager.getPair_PositionList_Table(fingerprint2);

	for (auto& it : compareWave_Pair_PositionList_Table) {
		int compareWaveHashNumber = it.first;

		// for each compare hash number, get the positions
		// if the compareWaveHashNumber in either table, no need to compare
		if ((this_Pair_PositionList_Table.find(compareWaveHashNumber) == this_Pair_PositionList_Table.end()) ||
			(compareWave_Pair_PositionList_Table.find(compareWaveHashNumber) == compareWave_Pair_PositionList_Table.end())) {
			continue;
		}

		vector<int>& wavePositionList = this_Pair_PositionList_Table[compareWaveHashNumber];
		vector<int>& compareWavePositionList = compareWave_Pair_PositionList_Table[compareWaveHashNumber];

		for (auto& thisPosition : wavePositionList) {
			for (auto& compareWavePosition : compareWavePositionList) {
				int offset = (thisPosition - compareWavePosition);

				if (offsetScoreTable.find(offset) != offsetScoreTable.end()) {
					offsetScoreTable[offset] += 1;
				} else {
					offsetScoreTable[offset] = 1;
				}
			}
		}
	}

	// map rank
	MapRankInteger mapRank = MapRankInteger(offsetScoreTable, false);

	// get the most similar positions and scores
	vector<int> orderedKeyList = mapRank.getOrderedKeyList(100, true);
	if (orderedKeyList.size() > 0) {
		int key = orderedKeyList[0];

		// get the highest score position
		if (results.mostSimilarFramePosition == INT_MIN) {
			results.mostSimilarFramePosition = key;
			results.score = (float)offsetScoreTable[key];

			// accumulate the scores from neighbors
//			if int offsetScore = offsetScoreTable[(key - 1)] {
			if (offsetScoreTable.find(key - 1) != offsetScoreTable.end()) {
				int offsetScore = offsetScoreTable[key - 1];
				results.score += (float)(offsetScore / 2);
			}
//			if int offsetScore = offsetScoreTable[(key + 1)] {
			if (offsetScoreTable.find(key + 1) != offsetScoreTable.end()) {
				int offsetScore = offsetScoreTable[key + 1];
				results.score += (float)(offsetScore / 2);
			}
		}
	}

	results.score /= (float)numFrames;
	results.similarity = results.score;
	if (results.similarity > 1.0f) {
		// similarity > 1.0 means in average there is at least one match in every frame
		results.similarity = 1.0f;
	}

	// calculate the most similar start time
	results.mostSimilarStartTime = ((float)results.mostSimilarFramePosition / (float)FingerprintProperties::numRobustPointsPerFrame / (float)FingerprintProperties::fps);

	return results;
}
