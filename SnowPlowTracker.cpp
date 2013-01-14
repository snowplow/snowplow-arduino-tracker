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

#define trackerVersion "arduino-0.1.0"

public:

/*==============================================================================
 * SnowPlow
 *
 * Constructor for SnowPlow class.
 *=============================================================================*/
SnowPlowTracker::SnowPlowTracker(EthernetClass *ethernet, byte* mac, String appId)
{  
  this->ethernet = ethernet;
  this->mac = mac;
  this->appId = appId;
}

/*==============================================================================
 * initCf 
 *
 * Initializes the SnowPlow tracker to talk to a collector
 * hosted on CloudFront.
 *
 * Constructs CloudFront collector domain then calls the
 * private init() method.
 *=============================================================================*/
void SnowPlowTracker::initCf(String cfSubdomain)
{
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
void SnowPlowTracker::initUrl(String domain)
{
  this->init(domain);
}

/*==============================================================================
 * setUserId 
 *
 * Sets the User Id for this Arduino.
 * Overrides the default User Id, which
 * is the Arduino's MAC address.
 *=============================================================================*/
void SnowPlowTracker::setUserId(String userId)
{
  this->userId = userId;
}

private:

/*==============================================================================
 * init 
 *
 * Common initialization, called by both initCf and initUrl.
 *=============================================================================*/
void SnowPlowTracker::init(String domain)
{
  // Set trackerUrl and userId
  this->trackerUrl = domain;
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
String SnowPlowTracker::mac2String(byte* mac)
{
  const int macLength = 6;
  String buffer = String();
  for (int i = 0; i < macLength; i++)
  {
    buffer += String(mac[i], HEX);
    if (i < macLength - 1) {
      buffer += ":";
    }
  }
  return buffer;
}