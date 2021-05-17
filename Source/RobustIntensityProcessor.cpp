//
//  RobustIntensityProcessor.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include "ArrayRankFloat.h"
#include "RobustIntensityProcessor.h"


RobustIntensityProcessor::RobustIntensityProcessor(const vector<vector<float>> &intensities, int numPointsPerFrame) : intensities(intensities), numPointsPerFrame(numPointsPerFrame)
{
}

void RobustIntensityProcessor::execute()
{
	int numX = (int)intensities.size();
	int numY = (int)intensities[0].size();
	vector<vector<float>> processedIntensities;

	for (int i = 0; i < numX; i++) {
		processedIntensities.push_back(vector<float>(numY));
	}

	for (int i = 0; i < numX; i++) {

		// TODO: Optimization: Using a sorted array is overkill.
		// Instead find the smallest value and use that as the pass value.

		// pass value is the last some elements in sorted array
		ArrayRankFloat arrayRankFloat(intensities[i]);
		float passValue = arrayRankFloat.getNthOrderedValue(numPointsPerFrame, false);

		// only passed elements will be assigned a value
		for (int j = 0; j < numY; j++) {
			if (intensities[i][j] >= passValue) {
				processedIntensities[i][j] = intensities[i][j];
			}
		}
	}

	intensities = processedIntensities;
}
