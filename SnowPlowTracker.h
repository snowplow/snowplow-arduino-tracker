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
    // Constructor
    SnowPlow(EthernetClass *ethernet, byte* mac, String appId);
    
    // Initialisation options for the HTTP connection
    void initCf(String cfSubdomain);
    void initUrl(String domain);

    // Manually set the 'user' ID
    void setUserId(String userId);
    
    // Track SnowPlow events
    int trackEvent(String category, String action, String label, String property, float value);
    int trackEvent(String category, String action, String label, String property, int value);

  private:
    class EthernetClient* client;
    class EthernetClass* ethernet;

    byte *mac;
    String appId;
    String trackerUrl;
    String userId;

    void init(String domain);
    void mac2String(byte* mac);

    // Misc we will probably delete
    char rxdata[150];
    int ret;
    int stringPos;
    boolean DataRx;
    boolean RxLoop;
    char c;
    unsigned long timeout_time;
    unsigned long time_now;
    unsigned long timeout;
    String myDataString;
};

#endif
