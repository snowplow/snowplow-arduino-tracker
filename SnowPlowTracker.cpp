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

#include <SPI.h>
#include "SnowPlowTracker.h"
#include <Ethernet.h>
#include <EthernetClient.h>

private:

// Initialize constants
const char* SnowPlowTracker::kUserAgent = "Arduino/2.0";
const char* SnowPlowTracker::kVersion = "arduino-0.1.0"

public:

/**
 * Constructor for SnowPlow class.
 *
 * 
 **/
SnowPlowTracker::SnowPlowTracker(EthernetClass *ethernet, const byte* mac, const String appId) {  
  this->ethernet = ethernet;
  this->mac = mac;
  this->appId = appId;
}

/**
 * Initializes the SnowPlow tracker to talk to a collector
 * hosted on CloudFront.
 *
 * @param cfSubdomain the subdomain of the CloudFront
 *                    collector e.g. "d3rkrsqld9gmqf"
 */
void SnowPlowTracker::initCf(const String cfSubdomain) {
  String domain = cfSubdomain + String(".cloudfront.net");
  this->init(domain);
}

/*==============================================================================
 * initUrl
 *
 * Initializes the SnowPlow tracker to talk to a self-hosted
 * collector on a dedicated domain.
 *
 * Alias for private init() method.
 *=============================================================================*/
void SnowPlowTracker::initUrl(const String domain) {
  this->init(domain);
}

/*==============================================================================
 * setUserId 
 *
 * Sets the User Id for this Arduino.
 * Overrides the default User Id, which
 * is the Arduino's MAC address.
 *=============================================================================*/
void SnowPlowTracker::setUserId(const String userId) {
  this->userId = userId;
}

private:

/*==============================================================================
 * init 
 *
 * Common initialization, called by both initCf and initUrl.
 *=============================================================================*/
void SnowPlowTracker::init(const String domain) {
  // Set trackerUrl and userId
  this->collectorUrl = domain;
  this->userId = mac2String(this->mac);

  // Boot the Ethernet connection
  this->ethernet->begin(this->mac);
  delay(1000);
  this->client = new EthernetClient();
}

/*==============================================================================
 * mac2String 
 *
 * Helper to convert a MAC address byte array into a String.
 * Generated String is of the format: "00:01:0A:2E:05:0B"
 *=============================================================================*/
String SnowPlowTracker::mac2String(const byte* mac) const {
  const int macLength = 6;
  String buffer = String();
  for (int i = 0; i < macLength; i++) {
    buffer += String(mac[i], HEX);
    if (i < macLength - 1) {
      buffer += ":";
    }
  }
  return buffer;
}


int SnowPlowTracker::trackEvent(const String category, const String action, const String label, const String property, const float value) const {
  // TODO: fix this crap.
  char rxdata[150];
  int ret = 0;
  int stringPos = 0;
  boolean DataRx = false;
  boolean RxLoop = true;
  char c;
  unsigned long timeout_time = 0;
  unsigned long time_now = 0;
  unsigned long timeout = 3000; // 3 seconds
  String myDataString = ""; // Allocate for actual data sent

  if (client->connect(serverName,80)) {
    if (client->connected()) {
      // Send request to SnowPlow collector
      client->println("GET /i HTTP/1.1");
      client->print("Host: ");
      client->println(this->collectorUrl);
      client->print("User-Agent: ");
      client->println(kUserAgent);
      // TODO: check if we need more headers.

      // Read from the nic
      //
      timeout_time = millis()+ timeout; 
      while ((timeout_time > time_now) && RxLoop) { 
        if (client->available()) {
          if (!DataRx)
            DataRx= true;          
          c = client->read();
          rxdata[stringPos] = c;          
          stringPos += 1;
        } else {
          rxdata[stringPos] = 0;

          if (DataRx) {
            DataRx= false;
            RxLoop = false;

            ret=1;
          }
        }//else
        time_now = millis();
      }// while ((timeout_time > time_now) && RxLoop) {

      client->stop();
    }
  }// if (client->connect(serverName,80)) {
  
  // Return updated status code
  return ret;
}