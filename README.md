# fingerprinting_code


To use the Fingerprint library, just import 'Fingerprint.h'

> #include "Fingerprint.h"


## Step 1: Use 'ExtractFingerprint' to create a 'Fingerprint'.

> Fingerprint *ExtractFingerprint(const int16_t *wave, int waveLength);

* This function expects the wave data to be 16-bit mono PCM wave data.
* The sample rate of the audio should be 10,240 Hz. (You will need to resample the audio.)
* The length of the fingerprint will be proportional to the length of the audio.

> Fingerprint *ExtractFingerprintFromRawFile(const char *filePath);

* This function will read a file containing the raw PCM wave data. It assumes there is no audio file header.
* The audio samples should be 16-bit mono PCM, with a sample rate of 10,240 Hz.

## Step 2: Use 'CompareFingerprints' to calculate the similiarity of two fingerprints.

> FingerprintSimilarity CompareFingerprints(const Fingerprint *fingerprint1, const Fingerprint *fingerprint2);

* This function requires the fingerprints to have been generated from audio clips *of the same length*.
* 'similiarity' in the FingerprintSimilarity is the similarity of the fingerprints in the range 0.0 - 1.0.


## Step 3: Cleanup.

> void FingerprintFree(Fingerprint *fingerprint);

* Use 'FingerprintFree' to release the Fingerprints created by 'ExtractFingerprint'.
