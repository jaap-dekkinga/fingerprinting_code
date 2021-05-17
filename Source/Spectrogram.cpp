//
//  Spectrogram.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include <float.h>
#include <math.h>
#include "FastFourierTransform.h"
#include "Spectrogram.h"
#include "WindowFunction.h"


Spectrogram::Spectrogram(vector<int16_t> wave, int fftSampleSize, int overlapFactor) : fftSampleSize(fftSampleSize), overlapFactor(overlapFactor)
{
	waveData = wave;
	waveDuration = ((float)waveData.size() / sampleRate);

	buildSpectrogram();
}


// MARK: -
// MARK: Private

void Spectrogram::buildSpectrogram()
{
	vector<int16_t> amplitudes = waveData;
	int numSamples = (int)amplitudes.size();

	// create the overlapping amplitude data
	if (overlapFactor > 1) {
		int numOverlappedSamples = (numSamples * overlapFactor);
		int backSamples = fftSampleSize * (overlapFactor - 1) / overlapFactor;
		int fftSampleSize_1 = (fftSampleSize - 1);
		vector<int16_t> overlapAmp(numOverlappedSamples);
		int pointer = 0;
		int i = 0;
		while (i < (int)amplitudes.size()) {
			overlapAmp[pointer] = amplitudes[i];
			pointer += 1;
			if ((pointer % fftSampleSize) == fftSampleSize_1) {
				// overlap
				i -= backSamples;
			}
			i += 1;
		}
		numSamples = numOverlappedSamples;
		amplitudes = overlapAmp;
	}

	// number of frames of the spectrogram
	int numFrames = (numSamples / fftSampleSize);

	// TODO: Optimization: Use vDSP for the window function.
	// TODO: Optimization: Only generate the window function once.

	// create the signals array for fft
	vector<float> window = WindowFunction::generate(WindowFunctionType::hamming, fftSampleSize);

	vector<vector<float>> signals(numFrames);
	for (int x = 0; x < numFrames; x++) {
		signals[x].resize(fftSampleSize);
	}

	for (int frameIndex = 0; frameIndex < numFrames; frameIndex++) {
		int startSample = (frameIndex * fftSampleSize);
		for (int n = 0; n < fftSampleSize; n++) {
			signals[frameIndex][n] = ((float)amplitudes[startSample + n] * window[n]);
		}
	}

	// TODO: Optimization: Move the FFT setup elsewhere (instead of setting up every time).

	absoluteSpectrogram.resize(numFrames);
	// for each frame in signals, do fft on it
	FastFourierTransform fft(fftSampleSize);
	for (int i = 0; i < numFrames; i++) {
		absoluteSpectrogram[i] = fft.getMagnitudes(signals[i]);
	}

	if (absoluteSpectrogram.size() > 0) {

		// number of y-axis unit
		int numFrequencyUnit = (int)absoluteSpectrogram[0].size();

		// get max and min amplitudes of the absoluteSpectrogram
		float maxAmplitude = FLT_MIN;
		float minAmplitude = FLT_MAX;

		for(int i = 0; i < numFrames; i++) {
			for (int j = 0; j < numFrequencyUnit; j++) {
				if (absoluteSpectrogram[i][j] > maxAmplitude) {
					maxAmplitude = absoluteSpectrogram[i][j];
				} else if (absoluteSpectrogram[i][j] < minAmplitude) {
					minAmplitude = absoluteSpectrogram[i][j];
				}
			}
		}

		// safety check the minimum amplitude to avoid divide by zero
		float minValidAmplitude = 0.00000000001f;
		if (minAmplitude == 0.0f) {
			minAmplitude = minValidAmplitude;
		}

		// normalize the absolute spectrogram
		spectrogram.resize(numFrames);
		for (int x = 0; x < numFrames; x++) {
			spectrogram[x].resize(numFrequencyUnit);
		}

		float diff = log10f(maxAmplitude / minAmplitude);	// perceptual difference
		for (int i = 0; i < numFrames; i++) {
			for (int j = 0; j < numFrequencyUnit; j++) {
				if (absoluteSpectrogram[i][j] < minValidAmplitude) {
					spectrogram[i][j] = 0.0f;
				} else {
					spectrogram[i][j] = ((log10f(absoluteSpectrogram[i][j] / minAmplitude)) / diff);
				}
			}
		}

	}
}
