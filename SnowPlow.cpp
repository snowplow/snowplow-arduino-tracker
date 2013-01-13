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

#define collectorScheme "http://"
#define cloudfrontDomain ".cloudfront.net"
#define icePixel = ""

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
 * Constructs collector domain then calls the private
 * init() method.
 *=============================================================================*/
void SnowPlow::initCf(String cfSubdomain)
{
	String domain = cfSubdomain + String(".cloudfront.net")
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

private:

/*==============================================================================
 * init 
 *
 * Common initialization, called by both initCf and initUrl.
 *=============================================================================*/
void SnowPlow::init(String domain)
{
  this->trackerUrl = domain + String("/i")
  this->ethernet->begin(mac);
  delay(1000);
  this->client = new EthernetClient();
}