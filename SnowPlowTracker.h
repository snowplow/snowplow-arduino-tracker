/* 
 * SnowPlow Arduino Tracker
 *
 * @description Arduino tracker for SnowPlow
 * @version     0.0.1
 * @author      Alex Dean
 * @copyright   SnowPlow Analytics Ltd
 * @license     Apache License Version 2.0
 *
 * Copyright (c) 2012 SnowPlow Analytics Ltd. All rights reserved.
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
  SnowPlowTracker(EthernetClass *aEthernet, const byte* aMac, const String aAppId);

  // Initialisation options for the HTTP connection
  void initCf(const String aCfSubdomain);
  void initUrl(const String aHost);

  // Manually set the 'user' ID
  void setUserId(String userId);

  // Track structured SnowPlow events
  int trackStructEvent(const String aCategory, const String aAction, const String aLabel = NULL, const String aProperty = NULL, const int aValue = NULL) const;
  int trackStructEvent(const String aCategory, const String aAction, const String aLabel = NULL, const String aProperty = NULL, const double aValue = NULL, const int aValuePrecision = 2) const;
  int trackStructEvent(const String aCategory, const String aAction, const String aLabel = NULL, const String aProperty = NULL, const float aValue = NULL, const int aValuePrecision = 2) const;
  int trackStructEvent(const String aCategory, const String aAction, const String aLabel = NULL, const String aProperty = NULL, const String aValue = NULL) const;

 private:
  static const String kUserAgent;
  static const String kTrackerPlatform;
  static const String kTrackerVersion;
  static const int kCollectorPort = 80;

  // Struct to hold a querystring name-value pair
  typedef struct
  {
    char* name;
    char* value;
  } QuerystringPair;

  class EthernetClass* ethernet;
  class EthernetClient* client;

  byte* mac;
  String appId;
  String collectorHost;
  String userId;

  void init(String aHost);
  int SnowPlowTracker::track(const QuerystringPair aEventPairs[]);
  
  static String mac2String(const byte* aMac);
  static String double2String(const double aDbl, const int aPrecision);
  static int SnowPlowTracker::countPairs(const QuerystringPair aPairs[]);
  static String SnowPlowTracker::urlEncode(const char* aMsg);
  static int SnowPlowTracker::getUri(const String aHost, const String aPort, const String aPath, const QuerystringPair aPairs[]);
};

#endif
