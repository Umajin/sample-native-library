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

#include "umajin_test_lib.h"

#include <string.h>

int replyToPoll = 0;

const char* getIdentifier()
{
	return "UmajinTestLib";
}

const char* process(uint64_t tag, const char* payload)
{
	if (strcmp(payload, "action") == 0)
	{
		replyToPoll = 1;
	}
	return "UmajinTestLib process says: maybe";
}

const char* poll(uint64_t tag, long long timestamp)
{
	if (replyToPoll)
	{
		replyToPoll = 0;
		return "UmajinTestLib says hello in response";
	}
	else
	{
		return "";
	}
}
