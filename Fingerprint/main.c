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

			// open the file
			FILE *file = fopen(filePath, "r");
			if (file == NULL) {
				printf("Error opening file.\n");
				continue;
			}

			// get the file size
			fseek(file, 0L, SEEK_END);
			int fileSize = (int)ftell(file);
			rewind(file);
			if ((fileSize <= 0) || ((fileSize & 0x1) != 0)) {
				printf("Error getting file size.\n");
				continue;
			}

			// allocate the buffer
			void *fileBuffer = malloc(fileSize);
			if (fileBuffer == NULL) {
				printf("Error allocating file buffer.\n");
				continue;
			}

			// read the file
			if (fread(fileBuffer, 1, fileSize, file) != fileSize) {
				printf("Error reading file.\n");
				free(fileBuffer);
				continue;
			}

			// generate the fingerprint
			Fingerprint *fingerprint = ExtractFingerprint(fileBuffer, (fileSize >> 2));
			if (fingerprint == NULL) {
				printf("Error generating fingerprint.\n");
				free(fileBuffer);
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
			free(fileBuffer);
		}
	}

	return 0;
}
