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

#define cloudfrontDomain ".cloudfront.net"

/*==============================================================================
* SnowPlow
*
* Constructor for Exosite class
*=============================================================================*/
SnowPlow::SnowPlow(EthernetClass *ethernet, byte* mac, String appId)
{  
  this->ethernet = ethernet;
  this->mac = mac;
  this->appId = appId;
}

/*==============================================================================
* init 
*
* initialization function for SnowPlow class. 
*=============================================================================*/
void SnowPlow::initCf(String cfSubdomain)
{
  ethernet->begin(mac);
  delay(500);
  client = new EthernetClient();
}