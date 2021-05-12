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

// All functions are guaranteed to be called within the same thread, so no multithread safety is
// required in the library.

// The returned identifier has to be GLOBALLY unique between all external libraries and also it
// has to be an immutable value - i.e. the value cannot change between calls.
EXPORT const char* getIdentifier();

EXPORT const char* process(
    uint64_t tag /* Unique identifier for the customer. It can be used as a key to store state. */,
    const char* payload);

EXPORT const char* poll(uint64_t tag /* The same as in process(). */,
    uint64_t timestamp /* Microseconds since app launch. */);

#endif // UMAJIN_TEST_LIB_H
