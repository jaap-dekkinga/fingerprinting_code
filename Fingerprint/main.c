//
//  main.c
//  Fingerprint
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 6/16/21.
//  Copyright (c) 2021 TuneURL Inc. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "Fingerprint.h"

int main(int argc, const char* argv[])
{
	for (int index = 0; index < argc; index++) {
		if ((strcmp("fingerprint", argv[index]) == 0) && ((index + 1) <= argc)) {
			const char *filePath = argv[++index];

			printf("Extracting fingerprint: '%s'\n", filePath);

			// generate the fingerprint
			Fingerprint *fingerprint = ExtractFingerprintFromRawFile(filePath);
			if (fingerprint == NULL) {
				printf("Error generating fingerprint.\n");
				continue;
			}

			// dump the fingerprint
			printf("Fingerprint: [ %d", fingerprint->data[0]);
			for (int c = 1; c < fingerprint->dataSize; c++) {
				printf(", %d", fingerprint->data[c]);
			}
			printf(" ]\n");

			// cleanup
			FingerprintFree(fingerprint);
		}
	}

	return 0;
}
