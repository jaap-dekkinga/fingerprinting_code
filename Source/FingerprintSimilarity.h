//
//  FingerprintSimilarity.h
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//

#ifndef FINGERPRINTSIMILARITY_H
#define FINGERPRINTSIMILARITY_H

#include <limits.h>

class FingerprintSimilarity {

public:

	// the frame number that was most similar
	int mostSimilarFramePosition { INT_MIN };

	// the number of features matched per frame.
	float score { -1.0f };

	// similarity ranges from 0.0 - 1.0, where 0.0 means no similar features were found
	// and 1.0 means on average there is at least one match in every frame.
	float similarity { -1.0f };


	float mostSimilarStartTime();

};

#endif /* FINGERPRINTSIMILARITY_H */
