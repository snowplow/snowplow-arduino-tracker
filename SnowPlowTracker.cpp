/* 
 * SnowPlow Arduino Tracker
 *
 * @description Arduino tracker for SnowPlow
 * @version     0.1.0
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

#include <stdlib.h>
#include <SPI.h>
#include "SnowPlowTracker.h"
#include <Ethernet.h>
#include <EthernetClient.h>

#define LOGGING // Switch off before release

// Initialize constants
const String SnowPlowTracker::kUserAgent = "Arduino/2.0";
const String SnowPlowTracker::kTrackerPlatform = "iot"; // Internet of things
const String SnowPlowTracker::kTrackerVersion = "arduino-0.1.0";

/**
 * Constructor for the SnowPlowTracker
 * class.
 *
 * @param aEthernet Pointer to the
 *        EthernetClass (initialised
 *        outside of this library)
 * @param aMac The MAC address of the
 *        Arduino's WiFi or Ethernet
 *        Shield
 * @param aAppId The SnowPlow application
 *        ID
 **/
SnowPlowTracker::SnowPlowTracker(EthernetClass *aEthernet, const byte* aMac, const String aAppId) {  
  this->ethernet = aEthernet;
  this->mac = (byte*)aMac;
  this->appId = aAppId;
}

/**
 * Initializes the SnowPlow tracker to
 * talk to a collector hosted on
 * CloudFront.
 *
 * @param aCfSubdomain The subdomain
 *        of the CloudFront collector
 *        e.g. "d3rkrsqgmqf"
 */
void SnowPlowTracker::initCf(const String aCfSubdomain) {
  const String host = aCfSubdomain + String(".cloudfront.net");
  this->init(host);
}

/**
 * Initializes the SnowPlow tracker
 * to speak to a URL-based (self-
 * hosted) collector.
 *
 * @param aHost The hostname of the
 *        URL hosting the collector
 *        e.g. tracking.mysite.com
 */
void SnowPlowTracker::initUrl(const String aHost) {
  this->init(aHost);
}

/**
 * Sets the User Id for this Arduino.
 * Overrides the default User Id, which
 * is the Arduino's MAC address.
 *
 * @param @aUserId The new User Id
 */
void SnowPlowTracker::setUserId(String aUserId) {
  this->userId = aUserId;

#ifdef LOGGING
  Serial.print("SnowPlow User Id updated to: ");
  Serial.println(this->userId);
#endif
}

/**
 * Tracks a structured event to a
 * SnowPlow collector: version
 * where the value field is an int.
 *
 * @param aCategory The name you supply for
 *        the group of objects you want to track
 * @param aAction A string that is uniquely
 *        paired with each category, and commonly
 *        used to define the type of user
 *        interaction for the web object
 * @param aLabel An optional string
 *        to provide additional dimensions to the
 *        event data
 * @param aProperty An optional string
 *        describing the object or the action
 *        performed on it. This might be the
 *        quantity of an item added to basket
 * @param aValue An integer value that
 *        you can use to provide numerical data
 *        about the user event
 * @return An integer indicating the success/failure
 *         of logging the event to SnowPlow
 */ 
int SnowPlowTracker::trackStructEvent(
  const String aCategory,
  const String aAction,
  const String aLabel,
  const String aProperty,
  const int aValue) const {

  // Cast aValue to String and call appropriate trackEvent
  return this->trackStructEvent(aCategory, aAction, aLabel, aProperty, String(aValue));
}

/**
 * Tracks a structured event to a
 * SnowPlow collector: version
 * where the value field is a float.
 *
 * @param aCategory The name you supply for
 *        the group of objects you want to track
 * @param aAction A string that is uniquely
 *        paired with each category, and commonly
 *        used to define the type of user
 *        interaction for the web object
 * @param aLabel An optional string
 *        to provide additional dimensions to the
 *        event data
 * @param aProperty An optional string
 *        describing the object or the action
 *        performed on it. This might be the
 *        quantity of an item added to basket
 * @param aValue A double value that
 *        you can use to provide numerical data
 *        about the user event
 * @param aValuePrecision How many digits to keep
 *        after the decimal sign
 * @return An integer indicating the success/failure
 *         of logging the event to SnowPlow
 */
int SnowPlowTracker::trackStructEvent(
  const String aCategory,
  const String aAction,
  const String aLabel,
  const String aProperty,
  const double aValue,
  const int aValuePrecision) const {

  // Cast aValue to String and call appropriate trackEvent
  return this->trackStructEvent(aCategory, aAction, aLabel, aProperty, double2String(aValue, aValuePrecision));
}

/**
 * Tracks a structured event to a
 * SnowPlow collector: version
 * where the value field is a float.
 *
 * @param aCategory The name you supply for
 *        the group of objects you want to track
 * @param aAction A string that is uniquely
 *        paired with each category, and commonly
 *        used to define the type of user
 *        interaction for the web object
 * @param aLabel An optional string
 *        to provide additional dimensions to the
 *        event data
 * @param aProperty An optional string
 *        describing the object or the action
 *        performed on it. This might be the
 *        quantity of an item added to basket
 * @param aValue A float value that
 *        you can use to provide numerical data
 *        about the user event
 * @param aValuePrecision How many digits to keep
 *        after the decimal sign
 * @return An integer indicating the success/failure
 *         of logging the event to SnowPlow
 */
int SnowPlowTracker::trackStructEvent(
  const String aCategory,
  const String aAction,
  const String aLabel,
  const String aProperty,
  const float aValue,
  const int aValuePrecision) const {

  // Cast aValue to String and call appropriate trackEvent
  return this->trackStructEvent(aCategory, aAction, aLabel, aProperty, double2String(aValue, aValuePrecision));
}

/**
 * Tracks a structured event to a
 * SnowPlow collector: version
 * where the value field is a String.
 *
 * @param aCategory The name you supply for
 *        the group of objects you want to track
 * @param aAction A string that is uniquely
 *        paired with each category, and commonly
 *        used to define the type of user
 *        interaction for the web object
 * @param aLabel An optional string
 *        to provide additional dimensions to the
 *        event data
 * @param aProperty An optional string
 *        describing the object or the action
 *        performed on it. This might be the
 *        quantity of an item added to basket
 * @param aValue A String value that
 *        you can use to provide non-numerical data
 *        about the user event
 * @return An integer indicating the success/failure
 *         of logging the event to SnowPlow
 */ 
int SnowPlowTracker::trackStructEvent(
  const String aCategory,
  const String aAction,
  const String aLabel,
  const String aProperty,
  const String aValue) const {

#ifdef LOGGING
  Serial.print("Tracking structured event ");
  Serial.print(aCategory);
  Serial.print(" ");
  Serial.println(aAction);
#endif

  const QuerystringPair eventPairs[] = {
    { "e", "c" },
    { "ev_ca", aCategory },
    { "ev_ac", aAction },
    { "ev_la", aLabel },
    { "ev_pr", aProperty },
    { "ev_va", aValue },
    { NULL, NULL } // Signals end of array
  };

  return this->track(eventPairs);
}

/**
 * Common initialization, called by
 * both initCf and initUrl.
 *
 * @param aHost The hostname of the
 *        URL hosting the collector
 *        e.g. tracking.mysite.com
 *        or d3rkrsqgmqf.cloudfront.net
 */
void SnowPlowTracker::init(const String aHost) {

  // Set collectorHost and userId
  this->collectorHost = aHost;
  this->userId = mac2String(this->mac);

  // Boot the Ethernet connection
  this->ethernet->begin((byte*)this->mac);
  delay(1000); // Wait 1 sec
  this->client = new EthernetClient();

#ifdef LOGGING
  Serial.print("SnowPlowTracker initialized with collector host: ");
  Serial.println(this->collectorHost);
#endif
}

/**
 * A wrapper around getUri to send
 * an event to the SnowPlow collector
 * via a GET.
 *
 * @param aEventPairs the name-value
 *        pairs specific to this event
 *        to add to our GET
 * @return An integer indicating the
 *         success/failure of logging
 *         the event to SnowPlow
 */
int SnowPlowTracker::track(const QuerystringPair aEventPairs[]) {

  // First combine the two sets of events
  const int eventPairCount = countPairs(aEventPairs);

  QuerystringPair qsPairs[4 + eventPairCount];
  qsPairs[0] = { "p", this->kTrackerPlatform };
  qsPairs[1] = { "uid", this->userId };
  qsPairs[2] = { "aid", this->appId };
  qsPairs[3] = { "tv", this->kTrackerVersion };

  for (i = 0; i < eventPairCount; i++) {
    qsPairs[i + 4] = &aEventPairs[i]
  }

  return this->getUri(this->collectorHost, this->kCollectorPort, "/i", qsPairs);
}

/**
 * Converts a MAC address byte array
 * into a String. Generated String is
 * of the format: "00:01:0A:2E:05:0B"
 *
 * @param aMac The MAC address, in bytes,
 *             to convert
 * @return the MAC address as a String
 */
String SnowPlowTracker::mac2String(const byte* aMac) {
  const int macLength = 6;
  String buffer = String();
  for (int i = 0; i < macLength; i++) {
    buffer += String(aMac[i], HEX);
    if (i < macLength - 1) {
      buffer += ":";
    }
  }
  return buffer;
}

/**
 * Returns the length of an array
 * of QuerystringPairs. Assumes
 * the array ends with a sentinel
 * value of NULL.
 *
 * @param aPairs The QuerystringPairs
 *        to count
 * @return the number of pairs in the
 *         array
 */
int SnowPlowTracker::countPairs(const QuerystringPair aPairs[]) {
  char* p = aPairs;
  for (; *p != NULL; ++p) {}
  return p - aPairs;
}

/**
 * Converts a double (or a float)
 * into a String. Generated String is
 * 1 or more characters long, with the
 * number of digits after the decimal
 * point specified by `aPrecision`.
 *
 * @param aDbl The double (or float) to
 *        convert into a String
 * @return the converted String
 */
String SnowPlowTracker::double2String(const double aDouble, const int aPrecision) {
  char buffer[25];
  dtostrf(aDouble, 1, aPrecision, buffer);
  return String(buffer);
}

/**
 * URL-encodes a string. Using code
 * taken from:
 *
 * http://hardwarefun.com/tutorials/url-encoding-in-arduino
 *
 * @param aMsg The characters to
 *        URL-encode.
 * @return the encoded String
 */
String SnowPlowTracker::urlEncode(const char* aMsg)
{
  const char *hex = "0123456789abcdef";
  String encodedMsg = "";

  while (*aMsg != '\0') {
    if (   ('a' <= *aMsg && *aMsg <= 'z')
        || ('A' <= *aMsg && *aMsg <= 'Z')
        || ('0' <= *aMsg && *aMsg <= '9')) {
      encodedMsg += *aMsg;
    } else {
      encodedMsg += '%';
      encodedMsg += hex[*aMsg >> 4];
      encodedMsg += hex[*aMsg & 15];
    }
    aMsg++;
  }
  return encodedMsg;
}

/**
 * Performs a GET against the
 * specified URI, passing in
 * the given parameters and
 * headers.
 *
 * @param aHost The hostname of
 *        the URI to GET
 * @param aPort The port of the
 *        URI to GET
 * @param aPath The path of the
 *        URI to GET
 * @param aParameters The name-
 *        value pairs to append
 *        on the querystring
 * @return An integer indicating the
 *         success/failure of logging
 *         the event to SnowPlow
 */
int SnowPlowTracker::getUri(const String aHost, const String aPort, const String aPath, const QuerystringPair aPairs[]) {

  // Connect to the host
  if (this->client->connect(aHost, aPort)) {
    if (this->client->connected()) {

      // Build our GET line from:
      // 1. The URI path... 
      this->client->print("GET ");
      this->client->print(aPath);

      // 2. The querystring name-value pairs
      // TODO

      // 3. Finish the GET definition
      this->client->println(" HTTP/1.1");

      // Headers
      this->client->print("Host: ");
      this->client->println(aHost);

      this->client->print("User-Agent: ");
      this->client->println(this->kUserAgent);

      this->client->println();
      // End of headers

      // TODO: check return value
      // https://github.com/exosite-garage/arduino_exosite_library/blob/master/Exosite.cpp
    }
  }

  // Return updated status code
  return SnowPlowTracker::SUCCESS;
}