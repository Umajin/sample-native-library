//
//  UmajinFns.m
//  UmajinFns
//
//  Created by Carl Knight on 2/06/21.
//

#import <Foundation/Foundation.h>
#import "UmajinFns.h"
#include "umajin_fns.hpp"
#include <mutex>
#include <string>

/////////////// IMPORTANT //////////////
// Code below should not be taken seriously. It is simply a mixture of
// Objective C and C++ to demonstrate their use is possible in one translation unit.
// Also note, if you mix ObjC and C++, the file extension needs to be *.mm or else
// the C compiler will be used and that will not work.
namespace
{
   std::once_flag versNumOnce, versStrOnce;

   double UmajinFnsVersionNumber;
   //! Project version string for UmajinFns.
   std::string UmajinFnsVersionString;
}

@implementation UmajinFns

// Demonstrate doing some C++ in ObjC functions...
+ (double) getVersionNumber
{
   std::call_once(versNumOnce, []{ UmajinFnsVersionNumber = 1.0; });
   return UmajinFnsVersionNumber;
}

+ (const char*) getVersionString
{
   std::call_once(versStrOnce, [] {
      UmajinFnsVersionString = [[NSString stringWithFormat:@"UmajinFns version %lf", [UmajinFns getVersionNumber]] cStringUsingEncoding:NSUTF8StringEncoding];});
   return UmajinFnsVersionString.c_str();
}

@end

int replyToPoll = 0;

// Demonstrate doing some ObjC in C/C++ functions.
const char* umajin_getIdentifier(void)
{
   return static_cast< const char* >([UmajinFns getVersionString]);
}

const char* umajin_process(long long tag, const char* payload)
{
   if (strcmp(payload, "action") == 0)
   {
      replyToPoll = 1;
   }
   else if(strcmp(payload, "id") == 0)
   {
      return umajin_getIdentifier();
   }
   return "UmajinTestLib process says: maybe";
}

const char* umajin_poll(long long tag, long long timestamp)
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

