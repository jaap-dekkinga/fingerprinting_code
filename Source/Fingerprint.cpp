//
//  Fingerprint.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include "Fingerprint.h"
#include "FingerprintManager.h"
#include "FingerprintSimilarityComputer.h"


FingerprintSimilarity CompareFingerprints(const Fingerprint *fingerprint1, const Fingerprint *fingerprint2)
{
	if (fingerprint1->dataSize != fingerprint2->dataSize) {
		FingerprintSimilarity results;
		results.score = -1.0f;
		results.similarity = -1.0f;
		results.mostSimilarFramePosition = 0;
		results.mostSimilarStartTime = 0.0f;
		return results;
	}

	vector<uint8_t> data1(fingerprint1->dataSize);
	memcpy(data1.data(), fingerprint1->data, fingerprint1->dataSize);
	vector<uint8_t> data2(fingerprint2->dataSize);
	memcpy(data2.data(), fingerprint2->data, fingerprint2->dataSize);

	FingerprintSimilarityComputer computer(data1, data2);
	return computer.getMatchResults();
}

Fingerprint *ExtractFingerprint(const int16_t *wave, int waveLength)
{
	// extract the fingerprint
	FingerprintManager fingerprinter;
	vector<uint8_t> *fingerprintData = fingerprinter.extractFingerprint(wave, waveLength);
	if (fingerprintData == NULL) {
		return NULL;
	}

	// create the fingerprint
	Fingerprint *fingerprint = (Fingerprint*)malloc(sizeof(Fingerprint));
	fingerprint->dataSize = (int)fingerprintData->size();
	fingerprint->data = (uint8_t*)malloc(fingerprint->dataSize);
	memcpy(fingerprint->data, fingerprintData->data(), fingerprint->dataSize);

	return fingerprint;
}

Fingerprint *ExtractFingerprintFromRawFile(const char *filePath)
{
	// open the file
	FILE *file = fopen(filePath, "r");
	if (file == NULL) {
		return NULL;
	}

	// get the file size
	fseek(file, 0L, SEEK_END);
	int fileSize = (int)ftell(file);
	rewind(file);
	if ((fileSize <= 0) || ((fileSize & 0x1) != 0)) {
		return NULL;
	}

	// allocate the buffer
	void *fileBuffer = malloc(fileSize);
	if (fileBuffer == NULL) {
		return NULL;
	}

	// read the file
	if (fread(fileBuffer, 1, fileSize, file) != fileSize) {
		free(fileBuffer);
		return NULL;
	}

	// generate the fingerprint
	Fingerprint *fingerprint = ExtractFingerprint((const int16_t*)fileBuffer, (fileSize >> 2));
	if (fingerprint == NULL) {
		free(fileBuffer);
		return NULL;
	}

	// cleanup
	free(fileBuffer);

	return fingerprint;
}

void FingerprintFree(Fingerprint *fingerprint)
{
	// release the fingerprint
	if (fingerprint != NULL) {
		if (fingerprint->data != NULL) {
			free(fingerprint->data);
		}
		free(fingerprint);
	}
}
