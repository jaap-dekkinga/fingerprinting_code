//
//  WindowFunction.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include <math.h>
#include "WindowFunction.h"


vector<float> WindowFunction::generate(WindowFunctionType windowType, int sampleCount)
{
	// generate window function values
	// for index values 0 .. (sampleCount - 1)
	int mInt = (sampleCount / 2);
	float m = (float)mInt;
//	const float pi = M_PI;
	const float pi = 3.1415925f;	// Swift float pi
	vector<float> window = vector<float>(sampleCount);

	switch (windowType) {

		// Bartlett (triangular) window
		case WindowFunctionType::bartlett:
			for (int n = 0; n < sampleCount; n++) {
				window[n] = 1.0f - fabsf((float)n - m) / m;
			}
			break;

		// Hanning window
		case WindowFunctionType::hanning:
		{
			float r = (pi / (m + 1.0f));
			int n = -mInt;
			while (n < mInt) {
				window[mInt + n] = 0.5f + 0.5f * cosf((float)n * r);
				n += 1;
			}
			break;
		}

		// Hamming window
		case WindowFunctionType::hamming:
		{
			float r = (pi / m);
			int n = -mInt;
			while (n < mInt) {
				window[mInt + n] = 0.54f + 0.46f * cosf((float)n * r);
				n += 1;
			}
			break;
		}

		// Blackman window
		case WindowFunctionType::blackman:
		{
			float r = (pi / m);
			int n = -mInt;
			while (n < mInt) {
				window[mInt + n] = 0.42f + 0.5f * cosf((float)n * r) + 0.08f * cosf(2.0f * (float)n * r);
				n += 1;
			}
			break;
		}

		// Rectangular window function
		default:
			for (int n = 0; n < sampleCount; n++) {
				window[n] = 1.0f;
			}
			break;

	}

	return window;
}
