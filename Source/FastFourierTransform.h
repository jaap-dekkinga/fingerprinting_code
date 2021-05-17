//
//  FastFourierTransform.h
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//

#ifndef FASTFOURIERTRANSFORM_H
#define FASTFOURIERTRANSFORM_H

#if __APPLE__
#include <Accelerate/Accelerate.h>
#endif // __APPLE

#include <vector>

using std::vector;

class FastFourierTransform {

public:

	FastFourierTransform(int numberOfSamples);
	~FastFourierTransform();

	vector<float> getMagnitudes(const vector<float> &timeDomainData);

private:

	int fftFrameSize { 0 };
	vector<float> outFFTData;

#if __APPLE__
	// ios accelerated fft
	vector<float> complexReal;
	vector<float> complexImag;
	FFTSetup fftSetup { nil };

	vector<float> getMagnitudesAcceleratedFFT(const vector<float> &timeDomainData);
#endif // __APPLE__

	// fft
	vector<int> bitm_array;
	vector<float> w;
	int fftFrameSize2 { 0 };

	vector<float> getMagnitudesJavaFFT(const vector<float> &timeDomainData);
	void setup();
	void transform(vector<float> &data);
	vector<float> computeTwiddleFactors(int fftFrameSize);
	void calc(int fftFrameSize, vector<float> &data, vector<float> &w);
	void calcF2E(int fftFrameSize, vector<float> &data, int i, int nstep, vector<float> &w);
	void calcF4F(int fftFrameSize, vector<float> &data, int i, int nstep, vector<float> &w);
	void calcF4FE(int fftFrameSize, vector<float> &data, int i, int nstep, vector<float> &w);
	void bitreversal(vector<float> &data);

};

#endif /* FASTFOURIERTRANSFORM_H */
