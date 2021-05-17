//
//  Fingerprint.h
//  TuneURL
//
//  Created by Gerrit on 5/4/21.
//  Copyright © 2021 TuneURL Inc. All rights reserved.
//

#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct Fingerprint {

	uint8_t		*data;
	int			dataSize;

} Fingerprint;

float CompareFingerprints(const Fingerprint *fingerprint1, const Fingerprint *fingerprint2);
Fingerprint *ExtractFingerprint(const int16_t *wave, int waveLength);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* FINGERPRINT_H */
