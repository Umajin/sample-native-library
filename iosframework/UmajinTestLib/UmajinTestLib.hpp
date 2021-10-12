//
//  UmajinTestLib
//
//  Created by Carl Knight on 3/06/21.
//

#ifndef UmajinTestLib_hpp
#define UmajinTestLib_hpp

// All functions are guaranteed to be called within the same thread, so no multithread safety is
// required in the library.

// The returned identifier has to be GLOBALLY unique between all external libraries and also it
// has to be an immutable value - i.e. the value cannot change between calls.
FOUNDATION_EXPORT const char* umajinGetIdentifier(void);

FOUNDATION_EXPORT const char* umajinProcess(
    long long tag /* Unique identifier for the customer. It can be used as a key to store state. */,
    const char* payload);

FOUNDATION_EXPORT const char* umajinPoll(long long tag /* The same as in process(). */,
    long long timestamp /* Microseconds since app launch. */);

#endif /* UmajinTestLib_hpp */
