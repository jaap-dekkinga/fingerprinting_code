//
//  Spectrogram.h
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//

#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <vector>
#include "FingerprintProperties.h"

using std::vector;

class Spectrogram {

public:

	Spectrogram(vector<int16_t> wave, int fftSampleSize, int overlapFactor);

	inline const vector<vector<float>>& getNormalizedSpectrogramData()
	{
		return spectrogram;
	}

private:

	float sampleRate = FingerprintProperties::sampleRate;

	vector<vector<float>> absoluteSpectrogram;
	vector<vector<float>> spectrogram;	// relative spectrogram
	vector<int16_t> waveData;
	float waveDuration { 0.0f };

	int fftSampleSize;	// number of samples in fft, the value needed to be a number to power of 2
	int overlapFactor;	// 1 / overlapFactor overlapping, e.g. 1 / 4 = 25% overlapping


	void buildSpectrogram();

};

#endif /* SPECTROGRAM_H */
