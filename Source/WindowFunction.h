//
//  WindowFunction.h
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//

#ifndef WINDOWFUNCTION_H
#define WINDOWFUNCTION_H

#include <vector>

using std::vector;

enum WindowFunctionType {
	rectangular,
	bartlett,
	hanning,
	hamming,
	blackman
};

class WindowFunction {

public:

	static vector<float> generate(WindowFunctionType windowType, int sampleCount);

};

#endif /* WINDOWFUNCTION_H */
