/*
 *
 *  Project:      Umajin External Library Example
 *  Author:       Aaron Lawrence <Aaron.Lawrence@umajin.com>
 *                Kuzma Shapran <Kuzma.Shapran@umajin.com>
 *  License:      CC0
 *
 * To the extent possible under law, Umajin has waived all copyright and related or neighboring
 * rights to Umajin External Library Example.
 *
 */

#ifndef UMAJIN_TEST_LIB_H
#define UMAJIN_TEST_LIB_H


#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64) // Windows:
	#define EXPORT __declspec(dllexport)
#else // *nix:
	#define EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif

// All functions are guaranteed to be called within the same thread, so no multithread safety is
// required in the library.

// The returned identifier has to be GLOBALLY unique between all external libraries and also it
// has to be an immutable value - i.e. the value cannot change between calls.
EXPORT const char* umajinGetIdentifier(void);

EXPORT const char* umajinProcessV2(
   uint64_t tag /* Unique identifier for the customer. It can be used as a key to store state. */,
   uint64_t timestamp,
   const char* payload);

EXPORT const char* umajinProcessBinary(
   uint64_t tag /* Unique identifier for the customer. It can be used as a key to store state. */,
   uint64_t timestamp, 
   const char* payload, 
   uint64_t sizeData, 
   const uint8_t* data);

EXPORT const char* umajinPoll(uint64_t tag /* The same as in process(). */,
    uint64_t timestamp /* Microseconds since app launch. */);

EXPORT const char* umajinPollBinary(uint64_t tag /* The same as in process(). */,
   uint64_t timestamp, /* Microseconds since app launch. */
   uint64_t* sizeOut, uint8_t** bufOut);

EXPORT void umajinDestroy(
   uint64_t size, // The size of the buffer being destroyed.
   uint8_t* buf /* Address of the buffer the framework previously allocated for the caller in umajinPollBinary */);

#ifdef __cplusplus
}
#endif

#endif // UMAJIN_TEST_LIB_H

