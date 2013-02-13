/* 
 * SnowPlow Arduino Tracker
 *
 * @description Arduino tracker for SnowPlow
 * @version     0.0.1
 * @author      Alex Dean
 * @copyright   SnowPlow Analytics Ltd
 * @license     Apache License Version 2.0
 *
 * Copyright (c) 2012-2013 SnowPlow Analytics Ltd. All rights reserved.
 *
 * This program is licensed to you under the Apache License Version 2.0,
 * and you may not use this file except in compliance with the Apache License Version 2.0.
 * You may obtain a copy of the Apache License Version 2.0 at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the Apache License Version 2.0 is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the Apache License Version 2.0 for the specific language governing permissions and limitations there under.
 */

#ifndef SnowPlowTracker_h
#define SnowPlowTracker_h

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>

/**
 * SnowPlowTracker encapsulates our Arduino
 * tracking code for SnowPlow.
 */
class SnowPlowTracker
{
 public:

  // Our return values from tracking an event
  enum { 
    // No issues
    SUCCESS = 0,
    // Could not connect to the server
    ERROR_CONNECTION_FAILED = -1,
    // SnowPlowTracker wasn't expecting this call to trackEvent
    // Indicates code is using this library incorrectly
    ERROR_UNEXPECTED_CALL = -2,
    // Spent too long waiting for a reply
    ERROR_TIMED_OUT = -3,
    // The response from the server is invalid, is it definitely an HTTP
    // server?
    ERROR_INVALID_RESPONSE = -4
  };

  // Constructor
  SnowPlowTracker(EthernetClass *aEthernet, const byte* aMac, const char *aAppId);

  // Initialisation options for the HTTP connection
  void initCf(const char *aCfSubdomain);
  void initUrl(const char *aHost);

  // Manually set the 'user' ID
  void setUserId(char *userId);

  // Track structured SnowPlow events
  int trackStructEvent(const char *aCategory, const char *aAction, const char *aLabel, const char *aProperty, const int aValue) const;
  int trackStructEvent(const char *aCategory, const char *aAction, const char *aLabel, const char *aProperty, const double aValue, const int aValuePrecision = 2) const;
  int trackStructEvent(const char *aCategory, const char *aAction, const char *aLabel, const char *aProperty, const float aValue, const int aValuePrecision = 2) const;
  int trackStructEvent(const char *aCategory, const char *aAction, const char *aLabel = NULL, const char *aProperty = NULL, const char *aValue = NULL) const;

 private:
  static const char *kUserAgent;
  static const char *kTrackerPlatform;
  static const char *kTrackerVersion;
  static const int kCollectorPort = 80;
  static const int kMaxEventPairs = 5; // Total number of fields in a SnowPlow event

  // Struct to hold a querychar *name-value pair
  typedef struct
  {
    char* name;
    char* value;
  } QuerystringPair;

  class EthernetClass* ethernet;
  class EthernetClient* client;

  byte* mac;
  char *appId;
  char *collectorHost;
  char *macAddress;
  char *userId;

  void init(const char *aHost);
  int track(const QuerystringPair aEventPairs[]) const;
  int getUri(const char *aHost, const int aPort, const char *aPath, const QuerystringPair aPairs[]) const;

  static char *mac2Chars(const byte* aMac);
  static char *int2Chars(const int aInt);
  static char *double2Chars(const double aDbl, const int aPrecision);
  static int countPairs(const QuerystringPair aPairs[]);
  static char *urlEncode(const char* aMsg);
};

#endif
