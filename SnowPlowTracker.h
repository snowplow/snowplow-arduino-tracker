/* 
 * SnowPlow Arduino Tracker
 *
 * @description Arduino tracker for SnowPlow
 * @version     0.1.0
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

// Logging - adapted from https://github.com/dmcrodrigues/macro-logger
#define NO_LOG          0x00
#define ERROR_LEVEL     0x01
#define INFO_LEVEL      0x02
#define DEBUG_LEVEL     0x03

#ifndef LOG_LEVEL
#define LOG_LEVEL   NO_LOG
#endif

#define SERIALPRINT(...) Serial.print(__VA_ARGS__)
#define SERIALPRINTLN(...) Serial.println(__VA_ARGS__)

#if LOG_LEVEL >= DEBUG_LEVEL
#define LOG_DEBUG(...)     SERIALPRINT(__VA_ARGS__)
#define LOGLN_DEBUG(...)   SERIALPRINTLN(__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#define LOGLN_DEBUG(...)
#endif

#if LOG_LEVEL >= INFO_LEVEL
#define LOG_INFO(...)      SERIALPRINT(__VA_ARGS__)
#define LOGLN_INFO(...)    SERIALPRINTLN(__VA_ARGS__)
#else
#define LOG_INFO(...)
#define LOGLN_INFO(...)
#endif

#if LOG_LEVEL >= ERROR_LEVEL
#define LOG_ERROR(...)     SERIALPRINT(__VA_ARGS__)
#define LOGLN_ERROR(...)   SERIALPRINTLN(__VA_ARGS__)
#else
#define LOG_ERROR(...)
#define LOGLN_ERROR(...)
#endif

/**
 * SnowPlowTracker encapsulates our Arduino
 * tracking code for SnowPlow.
 */
class SnowPlowTracker
{
 public:

  // Our return values from tracking an event
  // Could not connect to the server
  static const int ERROR_CONNECTION_FAILED = -1;
  // Spent too long waiting for a reply
  static const int ERROR_TIMED_OUT = -2;
  // The response from the server is invalid
  static const int ERROR_INVALID_RESPONSE = -3;
  // Required fields (category & action) missing
  static const int ERROR_MISSING_ARGUMENT = -4;
  // We had a client or server HTTP error
  static const int ERROR_HTTP_STATUS = -5;  

  // Constructor
  SnowPlowTracker(EthernetClass *aEthernet, const byte* aMac, const char *aAppId);

  // Initialisation options for the HTTP connection
  void initCf(const char *aCfSubdomain);
  void initUrl(const char *aHost);

  // Manually set the 'user' ID
  void setUserId(char *userId);

  // Track structured SnowPlow events
  int trackStructEvent(const char *aCategory, const char *aAction, const char *aLabel = NULL, const char *aProperty = NULL) const;
  int trackStructEvent(const char *aCategory, const char *aAction, const char *aLabel, const char *aProperty, const int aValue) const;
  int trackStructEvent(const char *aCategory, const char *aAction, const char *aLabel, const char *aProperty, const double aValue, const int aValuePrecision = 2) const;
  int trackStructEvent(const char *aCategory, const char *aAction, const char *aLabel, const char *aProperty, const float aValue, const int aValuePrecision = 2) const;

 private:
  static const char *kUserAgent;
  static const char *kTrackerPlatform;
  static const char *kTrackerVersion;
  static const int kCollectorPort = 80; // Default port
  static const int kMaxEventPairs = 7; // 6 fields plus trailing NULL indicator
  static const int kHttpResponseTimeout = 15*1000; // ms to wait before sending timeout
  static const int kHttpWaitForDataDelay = 750; // ms to wait each time there's no data available

  // Not possible to call trackStructEvent with a string directly
  int trackStructEvent(const char *aCategory, const char *aAction, const char *aLabel, const char *aProperty, const char *aValue) const;

  // Struct to hold a querychar *name-value pair
  typedef struct
  {
    char* name;
    char* value;
  } QuerystringPair;

  // To track different HTTP statuses
  typedef enum {
    eIdle,
    eRequestStarted,
    eRequestSent,
    eReadingStatusCode,
    eStatusCodeRead,
    eReadingContentLength,
    eSkipToEndOfHeader,
    eLineStartingCRFound,
    eReadingBody
  } HttpState;

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
  int getResponseCode() const;

  static char *getTransactionId();
  static char *mac2Chars(const byte* aMac);
  static char *int2Chars(const int aInt);
  static char *double2Chars(const double aDbl, const int aPrecision);
  static char char2Hex(const char aChar);
  static int countPairs(const QuerystringPair aPairs[]);
  static char *urlEncode(const char* aStr);
};

#endif
