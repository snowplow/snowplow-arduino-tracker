/* 
 * SnowPlow Arduino Tracker: Ping Example
 *
 * @description Simple ping example for SnowPlow Arduino Tracker
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
#include <Ethernet.h>
#include <SnowPlowTracker.h>

#define LOGGING

// MAC address of this Arduino. Update with your shield's MAC address.
const byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xF8, 0xA0 };

// SnowPlow CloudFront collector subdomain. Update with your collector.
const char *snowplowCfSubdomain = "d3rkrsqld9gmqf";

// SnowPlow app name
const char *snowplowAppName = "arduino-ping-example";

// SnowPlow Tracker
SnowPlowTracker snowplow(&Ethernet, mac, snowplowAppName);

/*
 * setup() runs once when you turn your
 * Arduino on: use it to initialize and
 * set any initial values.
 *
 * We just initialize the serial
 * connection (for debugging) and
 * the SnowPlow tracker.
 */
void setup()
{
  // Serial connection lets us debug on the computer
  Serial.begin(9600);

  // Setup SnowPlow Arduino tracker
  snowplow.initCf(snowplowCfSubdomain);
  snowplow.setUserId("my-arduino");

  // Seed our random number generator
  randomSeed(analogRead(0));
}

/*
 * loop() runs over and over again.
 * An empty loop() takes just a few
 * clock cycles to complete.
 *
 * Every 15 seconds, send a 'ping'
 * event to SnowPlow.
 */
void loop()
{
  // When did we run last? 
  static unsigned long prevTime = 0;

  if (millis() - prevTime >= (15000))
  {
    // Between 0 and 3
    switch (random(4)) {

    case 0:
      // Basic ping: label, property, value all NULL
      snowplow.trackStructEvent("example", "basic ping");
      break;

    case 1:
      // String ping: all fields set
      snowplow.trackStructEvent("example", "string ping", "regular", "ascii", "a string");
      break;

    case 2:
      // Int ping: all fields but property set
      snowplow.trackStructEvent("example", "int ping", "age", NULL, 22);
      break;

    default: // Aka case 3
      // Float ping: all fields but label set
      snowplow.trackStructEvent("example", "float ping", NULL, "celsius", 15.3, 1);
      break;
    }

    prevTime = millis();
  }

  delay(500); // Running loop twice a sec is fine
}
