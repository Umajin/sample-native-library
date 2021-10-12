//
//  UmajinTestLib
//
//  Created by Carl Knight on 2/06/21.
//

#import <Foundation/Foundation.h>
#import "UmajinTestLib.h"
#include "UmajinTestLib.hpp"
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

   double UmajinTestLibVersionNumber;
   //! Project version string for UmajinTestLib
   std::string UmajinTestLibVersionString;
}

@implementation UmajinTestLib

// Demonstrate doing some C++ in ObjC functions...
+ (double) getVersionNumber
{
   std::call_once(versNumOnce, []{ UmajinTestLibVersionNumber = 1.0; });
   return UmajinTestLibVersionNumber;
}

+ (const char*) getVersionString
{
   std::call_once(versStrOnce, [] {
      UmajinTestLibVersionString = [[NSString stringWithFormat:@"UmajinTestLib version %lf", [UmajinTestLib getVersionNumber]] cStringUsingEncoding:NSUTF8StringEncoding];});
   return UmajinTestLibVersionString.c_str();
}

@end

int replyToPoll = 0;

// Demonstrate doing some ObjC in C/C++ functions.
const char* umajinGetIdentifier(void)
{
   return [UmajinTestLib getVersionString];
}

const char* umajinProcess(long long tag, const char* payload)
{
   if (strcmp(payload, "action") == 0)
   {
      replyToPoll = 1;
   }
   else if(strcmp(payload, "id") == 0)
   {
      return umajinGetIdentifier();
   }
   return "UmajinTestLib process says: maybe";
}

const char* umajinPoll(long long tag, long long timestamp)
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

