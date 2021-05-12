//
//  umajin_test_lib.c
//  UmajinTestLib
//
//  Created by Aaron Lawrence on 29/04/21.
//

#include "umajin_test_lib.h"
#include <string.h>

int replyToPoll = 0;

const char* getIdentifier()
{
   return "UmajinTestLib";
}

const char* process(long long tag, const char* payload)
{
   if ( strcmp( payload, "action" ) == 0 )
   {
      replyToPoll = 1;
   }
   return "UmajinTestLib process says: maybe";
}

const char* poll(long long tag, long long timestamp)
{
   if ( replyToPoll )
   {
      replyToPoll = 0;
      return "UmajinTestLib says hello in response";
   }
   else
   {
      return "";
   }
}
