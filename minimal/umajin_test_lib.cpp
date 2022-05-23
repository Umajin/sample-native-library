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
#include <memory>

const int POLL_RETURN_STRING = 1;
const int POLL_RETURN_PROCESSED_IMAGE = 2;

int replyToPoll = 0;

namespace
{
	// Avoid buffer destruction if we can. Keep the last buffer
	// and don't reallocate unless we can't fit the data into
	// lastBuffer.
	std::unique_ptr<uint8_t[]> lastBuffer;
	uint64_t lastBufferSize = 0;
}

const char* umajinGetIdentifier(void)
{
	return "UmajinTestLib";
}

const char* umajinProcessV2(uint64_t tag, uint64_t timestamp, const char* payload)
{
	if (strcmp(payload, "ask-for-string") == 0)
	{
		replyToPoll = POLL_RETURN_STRING;
		return "UmajinTestLib:umajinProcessV2 - ask for string";
	}
	else
	{
		return "UmajinTestLib:umajinProcessV2 - unknown request";
	}
}

const char* umajinProcessBinary(uint64_t tag, uint64_t timestamp, const char* payload, uint64_t sizeData, const uint8_t* data)
{
	if (strcmp(payload, "send-image") == 0)
	{
		if (sizeData != lastBufferSize)
		{
			lastBufferSize = sizeData;
			lastBuffer.reset(new uint8_t[lastBufferSize]);
		}
		memcpy(lastBuffer.get(), data, sizeData);

		// assume: RGB, stride = width*depth, so we can just treat it all as one stream of pixels
		for (int i = 0; i < sizeData; i += 3)
		{
			lastBuffer[i + 0] = lastBuffer[i + 0] ^ 0xFF;
			lastBuffer[i + 1] = lastBuffer[i + 1] ^ 0xFF; 
			lastBuffer[i + 2] = lastBuffer[i + 2] ^ 0xFF;
		}
		 
		replyToPoll = POLL_RETURN_PROCESSED_IMAGE;
		return "image accepted, will respond on next poll";
	}
	else
	{
		return umajinProcessV2(tag, timestamp, payload);
	}
}

const char* umajinPoll(uint64_t tag, uint64_t timestamp)
{
	if (replyToPoll == POLL_RETURN_STRING)
	{
		replyToPoll = 0;
		return "UmajinTestLib:umajinPoll - returning string";
	}
	else
	{
		return "";
	}
}

const char* umajinPollBinary(uint64_t tag, uint64_t timestamp, uint64_t* sizeOut, uint8_t** bufOut)
{
	if (replyToPoll == POLL_RETURN_PROCESSED_IMAGE)
	{
		replyToPoll = 0;
		// return "Would have returned processed image";
		*bufOut = lastBuffer.get();
		*sizeOut = lastBufferSize;
		return "UmajinTestLib:umajinPollBinary - returning processed image";;
	}
	else
	{
		return "";
	}
}

void umajinDestroy(	uint64_t size, // The size of the buffer being destroyed.
							uint8_t* buf /* Address of the buffer the framework previously allocated for the caller in umajinPollBinary */) {
		// Not used. We keep the last buffer allocated and reuse it to avoid
		// constant allocations.
	return;
}