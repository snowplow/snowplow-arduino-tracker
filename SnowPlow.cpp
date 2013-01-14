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
#include "SnowPlow.h"
#include <Ethernet.h>
#include <EthernetClient.h>

#define trackerVersion "arduino-0.1.0"

public:

/*==============================================================================
 * SnowPlow
 *
 * Constructor for SnowPlow class.
 *=============================================================================*/
SnowPlow::SnowPlow(EthernetClass *ethernet, byte* mac, String appId)
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
void SnowPlow::initCf(String cfSubdomain)
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
void SnowPlow::initUrl(String domain)
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
void SnowPlow::setUserId(String userId)
{
  this->userId = userId;
}

private:

/*==============================================================================
 * init 
 *
 * Common initialization, called by both initCf and initUrl.
 *=============================================================================*/
void SnowPlow::init(String domain)
{
  // Set trackerUrl and userId
  this->trackerUrl = domain;
  this->userId = bytes2String(this->mac, 6)

  this->ethernet->begin(this->mac);
  delay(1000);
  this->client = new EthernetClient();
}

/*==============================================================================
 * bytes2String 
 *
 * Helper to convert a byte array into a String.
 * Generated String is of the format: "00:01:0A:..."
 *=============================================================================*/
String bytes2String(byte* bytes, int numBytes)
{
  String buffer = String();
  for (int i = 0; i < numBytes; i++)
  {
    buffer += String(bytes[i], HEX);
    if (i < numBytes - 1) {
      buffer += ":";
    }
  }
  return buffer;
}