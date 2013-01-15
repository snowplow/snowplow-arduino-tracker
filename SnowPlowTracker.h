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

class SnowPlowTracker
{
  public:

    // Constants
    static const char* kUserAgent;
    static const char* kVersion;

    // Constructor
    SnowPlow(EthernetClass *ethernet, const byte* mac, const String appId);
    
    // Initialisation options for the HTTP connection
    void initCf(const String cfSubdomain);
    void initUrl(const String domain);

    // Manually set the 'user' ID
    void setUserId(const String userId);
    
    // Track SnowPlow events
    int trackEvent(const String category, const String action, const String label, const String property, const float value) const;
    int trackEvent(const String category, const String action, const String label, const String property, const int value) const;

  private:
    class EthernetClient* client;
    class EthernetClass* ethernet;

    byte *mac;
    String appId;
    String collectorUrl;
    String userId;

    void init(String domain);
    void mac2String(byte* mac);
};

#endif