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
 * Define our track event return values (not to
 * be confused with HTTP response codes).
 */

// No issues
// The end of the headers has been reached.  This consumes the '\n'
static const int HTTP_SUCCESS =0;
// Could not connect to the server
static const int HTTP_ERROR_CONNECTION_FAILED =-1;
// This call was made when the HttpClient class wasn't expecting it
// to be called.  Usually indicates your code is using the class
// incorrectly
static const int HTTP_ERROR_CALL =-2;
// Spent too long waiting for a reply
static const int HTTP_ERROR_TIMED_OUT =-3;
// The response from the server is invalid, is it definitely an HTTP
// server?
static const int HTTP_ERROR_INVALID_RESPONSE =-4;

/**
 * SnowPlowTracker encapsulates our Arduino
 * tracking code for SnowPlow.
 */
class SnowPlowTracker
{
  public:

    // Constructor
    SnowPlow(EthernetClass *aEthernet, const byte* aMac, const String aAppId);
    
    // Initialisation options for the HTTP connection
    void initCf(const String aCfSubdomain);
    void initUrl(const String aHost);

    // Manually set the 'user' ID
    void setUserId(const String userId);
    
    // Track SnowPlow events
    int trackEvent(const String aCategory, const String aAction, const String aLabel, const String aProperty, const float aValue) const;
    int trackEvent(const String aCategory, const String aAction, const String aLabel, const String aProperty, const int aValue) const;
    int trackEvent(const String aCategory, const String aAction, const String aLabel, const String aProperty, const String aValue) const;

  private:
    static const char* kUserAgent;
    static const char* kVersion;

    class EthernetClass* ethernet;
    class EthernetClient* client;

    byte* mac;
    String appId;
    String collectorUrl;
    String userId;

    void init(String aHost);
    static void mac2String(byte* aMac);
};

#endif
