//
//  FingerprintManager.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include "ArrayCoord.h"
#include "FingerprintManager.h"
#include "FingerprintProperties.h"
#include "RobustIntensityProcessor.h"
#include "Spectrogram.h"

// MARK: Static

int FingerprintManager::getNumFrames(const vector<uint8_t> &fingerprint)
{
	// safety check
	if (fingerprint.size() < 8) {
		return 0;
	}

	// get the last x-coordinate (length - 8 & length - 7) bytes from fingerprint
	int numFrames = (((int)(fingerprint[fingerprint.size() - 8] & 0xFF) << 8) | (int)(fingerprint[fingerprint.size() - 7] & 0xFF)) + 1;

	return numFrames;
}

// MARK: -
// MARK: Public

vector<uint8_t> *FingerprintManager::extractFingerprint(const int16_t *wave, int waveLength)
{
	// TODO: remove this copy

	// copy the wave data
	vector<int16_t> resampledWave;
	resampledWave.resize(waveLength);
	memcpy(resampledWave.data(), wave, (waveLength * sizeof(int16_t)));

	// ----

	int numRobustPointsPerFrame = FingerprintProperties::numRobustPointsPerFrame;
	int overlapFactor = FingerprintProperties::overlapFactor;
	int sampleSizePerFrame = FingerprintProperties::sampleSizePerFrame;

	// get the spectrogram data
	Spectrogram spectrogram(resampledWave, sampleSizePerFrame, overlapFactor);
	const vector<vector<float>> &spectrogramData = spectrogram.getNormalizedSpectrogramData();

	// get the robust point list
	vector<vector<int>> pointsLists = getRobustPointList(spectrogramData);
	int numFrames = (int)pointsLists.size();

	// prepare fingerprint bytes
	vector<vector<int>> coordinates(numFrames);
	for (int x = 0; x < numFrames; x++) {
		coordinates[x].resize(numRobustPointsPerFrame);
	}

	for (int x = 0; x < numFrames; x++) {
		if (pointsLists[x].size() == numRobustPointsPerFrame) {
			for (int y = 0; y < numRobustPointsPerFrame; y++) {
				coordinates[x][y] = pointsLists[x][y];
			}
		} else {
			// use -1 to fill the empty byte
			for (int y = 0; y < numRobustPointsPerFrame; y++) {
				coordinates[x][y] = -1;
			}
		}
	}

	// build the fingerprint data
	vector<uint8_t> *fingerprintData = new vector<uint8_t>;

	for (int i = 0; i < numFrames; i++) {
		for (int j = 0; j < numRobustPointsPerFrame; j++) {
			if (coordinates[i][j] != -1) {
				// x-coordinate (2 byte integer)
				int x = i;
				fingerprintData->push_back((uint8_t)((x >> 8) & 0xFF));
				fingerprintData->push_back((uint8_t)(x & 0xFF));

				// y-coordinate (2 byte integer)
				int y = coordinates[i][j];
				fingerprintData->push_back((uint8_t)((y >> 8) & 0xFF));
				fingerprintData->push_back((uint8_t)(y & 0xFF));

				// intensity (4 byte integer)
				double integerMax = (double)0x7FFFFFFF;
				float intensityFloat = spectrogramData[x][y];
				double intensityDouble = ((double)intensityFloat * integerMax);
				int intensity = (int)intensityDouble;
				fingerprintData->push_back((uint8_t)((intensity >> 24) & 0xFF));
				fingerprintData->push_back((uint8_t)((intensity >> 16) & 0xFF));
				fingerprintData->push_back((uint8_t)((intensity >> 8) & 0xFF));
				fingerprintData->push_back((uint8_t)(intensity & 0xFF));
			}
		}
	}

	return fingerprintData;
}

// MARK: -
// MARK: Private

vector<vector<int>> FingerprintManager::getRobustPointList(const vector<vector<float>> &spectrogramData)
{
	int numX = (int)spectrogramData.size();
	int numY = (int)spectrogramData[0].size();

	vector<vector<float>> allBanksIntensities(numX);
	for (int x = 0; x < numX; x++) {
		allBanksIntensities[x].resize(numY);
	}

	int bandwidthPerBank = (numY / numFilterBanks);

	for (int b = 0; b < numFilterBanks; b++) {
		vector<vector<float>> bankIntensities(numX);
		for (int x = 0; x < numX; x++) {
			bankIntensities[x].resize(bandwidthPerBank);
		}

		for (int i = 0; i < numX; i++) {
			for (int j = 0; j < bandwidthPerBank; j++) {
				bankIntensities[i][j] = spectrogramData[i][j + b * bandwidthPerBank];
			}
		}

		// get the most robust point in each filter bank
		RobustIntensityProcessor processor(bankIntensities, 1);
		processor.execute();
		const vector<vector<float>> &processedIntensities = processor.intensities;

		for (int i = 0; i < numX; i++) {
			for (int j = 0; j < bandwidthPerBank; j++) {
				allBanksIntensities[i][j + b * bandwidthPerBank] = processedIntensities[i][j];
			}
		}
	}

	vector<ArrayCoord> robustPointList;

	// find robust points
	for (int i = 0; i < (int)allBanksIntensities.size(); i++) {
		for (int j = 0; j < (int)allBanksIntensities[i].size(); j++) {
			if (allBanksIntensities[i][j] > 0.0f) {
				robustPointList.push_back(ArrayCoord(i, j));
			}
		}
	}

	// robustLists[x] = y1, y2, y3, ...
	vector<vector<int>> robustLists(spectrogramData.size());

	for (auto coord : robustPointList) {
		robustLists[coord.x].push_back(coord.y);
	}

	// return the list per frame
	return robustLists;
}
