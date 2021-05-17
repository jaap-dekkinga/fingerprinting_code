//
//  FingerprintSimilarity.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include "FingerprintProperties.h"
#include "FingerprintSimilarity.h"


float FingerprintSimilarity::mostSimilarStartTime()
{
	int fps = FingerprintProperties::fps;
	int numRobustPointsPerFrame = FingerprintProperties::numRobustPointsPerFrame;
	return ((float)mostSimilarFramePosition / (float)numRobustPointsPerFrame / (float)fps);
}
