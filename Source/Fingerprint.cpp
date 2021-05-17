//
//  Fingerprint.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include "Fingerprint.h"
#include "FingerprintManager.h"
#include "FingerprintSimilarity.h"
#include "FingerprintSimilarityComputer.h"


float CompareFingerprints(const Fingerprint *fingerprint1, const Fingerprint *fingerprint2)
{
	if (fingerprint1->dataSize != fingerprint2->dataSize) {
		return -1.0f;
	}

	vector<uint8_t> data1(fingerprint1->dataSize);
	memcpy(data1.data(), fingerprint1->data, fingerprint1->dataSize);
	vector<uint8_t> data2(fingerprint2->dataSize);
	memcpy(data2.data(), fingerprint2->data, fingerprint2->dataSize);

	FingerprintSimilarityComputer computer(data1, data2);
	FingerprintSimilarity similarity = computer.getMatchResults();

	return similarity.similarity;
}

Fingerprint *ExtractFingerprint(const int16_t *wave, int waveLength)
{
	FingerprintManager fingerprinter;

	vector<uint8_t> *fingerprintData = fingerprinter.extractFingerprint(wave, waveLength, false);
	if (fingerprintData == NULL) {
		return NULL;
	}

	// TODO: make a copy of the fingerprint data

	Fingerprint *fingerprint = (Fingerprint*)malloc(sizeof(Fingerprint));
	fingerprint->data = fingerprintData->data();
	fingerprint->dataSize = (int)fingerprintData->size();

	return fingerprint;
}
