//
//  FingerprintProperties.cpp
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//


#include "FingerprintProperties.h"


// the number of points in each frame (i.e. top 4 intensities in fingerprint)
int FingerprintProperties::numRobustPointsPerFrame = 4;

// the number of audio samples in a frame (it is suggested to be the FFT Size)
int FingerprintProperties::sampleSizePerFrame = 2048;

// overlapFactor: 8 means each move 1/8 nSample length. 1 means no overlap, better 1, 2, 4, 8 ... 32
int FingerprintProperties::overlapFactor = 4;

int FingerprintProperties::numFilterBanks = 4;

// low pass
int FingerprintProperties::upperBoundedFrequency = 1500;

// high pass
int FingerprintProperties::lowerBoundedFrequency = 400;

// in order to have 5fps with 2048 sampleSizePerFrame, wave's sample rate need to be 10240 (sampleSizePerFrame * fps)
int FingerprintProperties::fps = 5;

// the audio's sample rate needed to resample to this in order to fit the sampleSizePerFrame and fps
float FingerprintProperties::sampleRate = (float)(sampleSizePerFrame * fps);

// max active pairs per anchor point for reference songs
int FingerprintProperties::refMaxActivePairs = 1;

// max active pairs per anchor point for sample clip
int FingerprintProperties::sampleMaxActivePairs = 10;

int FingerprintProperties::numAnchorPointsPerInterval = 10;

// in frames (5fps, 4 overlap per second)
int FingerprintProperties::anchorPointsIntervalLength = 4;

// in frame (5fps, 4 overlap per second)
int FingerprintProperties::maxTargetZoneDistance = 4;

// num frequency units
int FingerprintProperties::numFrequencyUnits = (upperBoundedFrequency - lowerBoundedFrequency + 1) / fps + 1;
