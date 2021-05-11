//
//  umajin_test_lib.h
//  UmajinTestLib
//
//  Created by Aaron Lawrence on 29/04/21.
//

#ifndef umajin_test_lib_h
#define umajin_test_lib_h

#ifdef _MSC_VER 
#define EXPORT __declspec(dllexport)
#endif
#ifdef __APPLE__
#define EXPORT 
#endif

// TBD: conflict between function "poll" and standard includes!
// #include <stdio.h>

EXPORT const char* getIdentifier(void);

EXPORT const char* process(long long tag, const char* payload);

EXPORT const char* poll(long long tag, long long timestamp);

#endif /* umajin_test_lib_h */
