//
//  main.swift
//  TuneURL
//
//  Created by Gerrit Goossen <developer@gerrit.email> on 5/4/21.
//  Copyright © 2021 TuneURL Inc. All rights reserved.
//


import AVFoundation
import Foundation


fileprivate func extractFingerprint(audioData: [Int16]) -> [UInt8]?
{
	// generate the fingerprint
	guard let fingerprint = ExtractFingerprint(audioData, Int32(audioData.count)) else {
		return nil
	}

	var array = [UInt8]()
	let pointer = fingerprint.pointee.data!
	for x in 0 ..< Int(fingerprint.pointee.dataSize) {
		array.append(pointer[x])
	}

	// cleanup
	FingerprintFree(fingerprint)

	return array
}

fileprivate func loadAudio(from fileURL: URL, resample: Bool) -> [Int16]?
{
	var result: OSStatus = noErr
	var audioFile: AudioFileID?
	var propertyDataSize: UInt32 = 8
	var dataSize: UInt64 = 0

	result = AudioFileOpenURL(fileURL as CFURL, .readPermission, kAudioFileAIFFType, &audioFile)
	if (result != noErr) {
		print("AudioMatcher: Error opening audio file. \(result)")
		return nil
	}

	result = AudioFileGetProperty(audioFile!, kAudioFilePropertyAudioDataByteCount, &propertyDataSize, &dataSize)
	if (result != noErr) {
		print("AudioMatcher: Error getting audio file property. \(result)")
		return nil
	}

	let frameCount = UInt32(dataSize >> 1)	// 16-bit audio
	var dataBuffer = [Int16](repeating: 0, count: Int(frameCount))
	dataBuffer.withUnsafeMutableBytes {
		bufferPointer in

		var packetCount = frameCount
		var dataRead = UInt32(dataSize)

		result = AudioFileReadPacketData(audioFile!, false, &dataRead, nil, 0, &packetCount, bufferPointer.baseAddress)
	}

	// check the result of the read
	if (result != noErr) {
		print("AudioMatcher: Error reading audio file packet data. \(result)")
		return nil
	}

	result = AudioFileClose(audioFile!)
	if (result != noErr) {
		print("AudioMatcher: Error closing audio file. \(result)")
	}

	return dataBuffer
}

// MARK: -

let arguments = CommandLine.arguments
var index = 0;

while index < arguments.count {

	if (arguments[index].lowercased() == "fingerprint") {
		index += 1
		if (index < arguments.count) {
			let filePath = arguments[index]
			print("Extracting fingerprint: '\(filePath)'")

			// load the audio file
			if let audioData = loadAudio(from: URL(fileURLWithPath: filePath), resample: false) {
				if let fingerprint = extractFingerprint(audioData: audioData) {
					print("Fingerprint:\n\(fingerprint)")
				} else {
					print("Error creating fingerprint.")
				}
			} else {
				print("Error loading audio file. ('\(filePath)')")
			}
		}
	}

	index += 1
}
