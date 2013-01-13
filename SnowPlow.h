/* 
 * SnowPlow Arduino Tracker
 *
 * @description Arduino tracker for SnowPlow
 * @version     0.0.1
 * @author      Alex Dean
 * @copyright   SnowPlow Analytics Ltd
 * @license     Apache License Version 2.0
 *
 * Adapted from:
 *
 * * https://github.com/exosite-garage/arduino_exosite_library/blob/master/SnowPlow.cpp
 * * https://github.com/amcewen/HttpClient/blob/master/HttpClient.cpp
 * * https://github.com/interactive-matter/HTTPClient/blob/master/HTTPClient.cpp
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

#ifndef SnowPlow_h
#define SnowPlow_h

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>

class SnowPlow
{
  private:
    class EthernetClient* client; // Port 80 is default for HTTP
    class EthernetClass* ethernet;
    
    // From constructor
    byte *mac;
    String appId;
    
    // From setUserId
    String userId;

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

  public:
    // Constructor
    SnowPlow(EthernetClass *eth, byte* mac, String _cik); // Constructor
    
    // Alternatives to initialize the HTTP connection
    void initCf(String cloudfrontSubdomain);
    void initUrl(String collectorDomain);

    // Manually set the 'user' ID
    void setUserId(String userId);
    
    // Track SnowPlow events
    int trackEvent(String category, String action, String label, String property, float value);
};

#endif