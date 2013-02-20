/* 
 * SnowPlow Arduino Tracker
 *
 * @description Arduino tracker for SnowPlow
 * @version     0.1.0
 * @author      Alex Dean
 * @copyright   SnowPlow Analytics Ltd
 * @license     Apache License Version 2.0
 *
 * Copyright (c) 2012-2013 SnowPlow Analytics Ltd. All rights reserved.
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
#include <limits.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include "SnowPlowTracker.h"

// Initialize constants
const char *SnowPlowTracker::kUserAgent = "Arduino/2.0";
const char *SnowPlowTracker::kTrackerPlatform = "iot"; // Internet of things
const char *SnowPlowTracker::kTrackerVersion = "arduino-0.1.0";

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
SnowPlowTracker::SnowPlowTracker(EthernetClass *aEthernet, const byte* aMac, const char *aAppId) {  
  this->ethernet = aEthernet;
  this->mac = (byte*)aMac;
  this->appId = (char*)aAppId;
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
void SnowPlowTracker::initCf(const char *aCfSubdomain) {

  const size_t hostLength = sizeof(aCfSubdomain) + 16; // .cloudfront.net\0 = 16
  char *host = (char*)malloc(hostLength);

  snprintf(host, hostLength, "%s.cloudfront.net", aCfSubdomain);
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
void SnowPlowTracker::initUrl(const char *aHost) {
  this->init(aHost);
}

/**
 * Sets the User Id.
 *
 * @param @aUserId The new User Id
 */
void SnowPlowTracker::setUserId(char *aUserId) {
  this->userId = aUserId;

  LOG_INFO("SnowPlow user id updated to [");
  LOG_INFO(this->userId);
  LOGLN_INFO("]");
}

/**
 * Tracks a structured event to a
 * SnowPlow collector: version
 * where the value field is an int.
 *
 * @param aCategory The name you supply for
 *        the group of objects you want to track
 * @param aAction A char *that is uniquely
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
  const char *aCategory,
  const char *aAction,
  const char *aLabel,
  const char *aProperty,
  const int aValue) const {

  char *value = int2Chars(aValue);
  const int status = this->trackStructEvent(aCategory, aAction, aLabel, aProperty, value);
  free(value);
  return status;
}

/**
 * Tracks a structured event to a
 * SnowPlow collector: version
 * where the value field is a float.
 *
 * @param aCategory The name you supply for
 *        the group of objects you want to track
 * @param aAction A char *that is uniquely
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
  const char *aCategory,
  const char *aAction,
  const char *aLabel,
  const char *aProperty,
  const double aValue,
  const int aValuePrecision) const {

  char *value = double2Chars(aValue, aValuePrecision);
  const int status = this->trackStructEvent(aCategory, aAction, aLabel, aProperty, value);
  free(value);
  return status;
}

/**
 * Tracks a structured event to a
 * SnowPlow collector: version
 * where the value field is a float.
 *
 * @param aCategory The name you supply for
 *        the group of objects you want to track
 * @param aAction A char *that is uniquely
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
  const char *aCategory,
  const char *aAction,
  const char *aLabel,
  const char *aProperty,
  const float aValue,
  const int aValuePrecision) const {

  char *value = double2Chars(aValue, aValuePrecision);
  const int status = this->trackStructEvent(aCategory, aAction, aLabel, aProperty, value);
  free(value);
  return status;
}

/**
 * Sends a structured event to a SnowPlow
 * collector. Builds the set of event
 * name=value pairs and then passes them
 * to the general-purpose track() to track
 * the event.
 *
 * @param aCategory The name you supply for
 *        the group of objects you want to track
 * @param aAction A char *that is uniquely
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
 * @param aValue A char *value that
 *        you can use to provide non-numerical data
 *        about the user event
 * @return An integer indicating the success/failure
 *         of logging the event to SnowPlow
 */ 
int SnowPlowTracker::trackStructEvent(
  const char *aCategory,
  const char *aAction,
  const char *aLabel,
  const char *aProperty,
  const char *aValue) const {

  LOG_INFO("Tracking structured event: category [");
  LOG_INFO(aCategory);
  LOG_INFO("], action [");
  LOG_INFO(aAction);
  LOG_INFO("], label [");
  LOG_INFO(aLabel);
  LOG_INFO("], property [");
  LOG_INFO(aProperty);
  LOG_INFO("], value [");
  LOG_INFO(aValue);  
  LOGLN_INFO("]");

  const QuerystringPair eventPairs[] = {
    { "e", "se" }, // Structured event
    { "ev_ca", (char*)aCategory },
    { "ev_ac", (char*)aAction },
    { "ev_la", (char*)aLabel },
    { "ev_pr", (char*)aProperty },
    { "ev_va", (char*)aValue }, 
    { NULL, NULL } // Signals end of array
  };

  const int status = this->track(eventPairs);
  switch (status) {
  case SUCCESS:
    LOGLN_INFO("Tracking returned SUCCESS");
    break;
  // TODO: add individual error codes
  default:
    LOGLN_ERROR("Tracking returned unknown error (should never happen)");
    break;
  }

  return status;
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
void SnowPlowTracker::init(const char *aHost) {

  // Set collectorHost and userId
  this->collectorHost = (char*)aHost;
  this->macAddress = mac2Chars(this->mac);

  // Boot the Ethernet connection
  this->ethernet->begin((byte*)this->mac);
  delay(1000); // Wait 1 sec
  this->client = new EthernetClient();

  LOG_INFO("Ethernet booted with MAC address [");
  LOG_INFO(this->macAddress);
  LOG_INFO("], local IP address [");
  LOG_INFO(this->ethernet->localIP());
  LOGLN_INFO("]");
  
  LOG_INFO("SnowPlowTracker initialized with collector host [");
  LOG_INFO(this->collectorHost);
  LOGLN_INFO("]");
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
int SnowPlowTracker::track(const QuerystringPair aEventPairs[]) const {

  char *txnId = this->getTransactionId();

  const int fixedPairCount = 6; // Update this if more pairs added below.
  QuerystringPair qsPairs[fixedPairCount + this->kMaxEventPairs] = {
    { "tid", (char*)txnId },
    { "p",   (char*)this->kTrackerPlatform },
    { "mac", (char*)this->macAddress },
    { "uid", (char*)this->userId },
    { "aid", (char*)this->appId },
    { "tv",  (char*)this->kTrackerVersion }
  };

  const int eventPairCount = countPairs(aEventPairs);
  for (int i = 0; i < eventPairCount; i++) {
    qsPairs[fixedPairCount + i].name = aEventPairs[i].name;
    qsPairs[fixedPairCount + i].value = aEventPairs[i].value;
  }

  const int status = this->getUri(this->collectorHost, this->kCollectorPort, "/i", qsPairs);
  free(txnId);
  return status;
}

/**
 * Returns the transaction ID for this
 * track event. Uses random().
 *
 * IMPORTANT: be sure to free() the returned
 * string after use
 *
 * @return the transaction ID, effectively
 *         millis() cast to an integer
 */
char *SnowPlowTracker::getTransactionId() {
  const int rndm = (int)random(INT_MAX); // Restrict to int range
  return int2Chars(rndm);
}

/**
 * Converts a MAC address byte array
 * into a String. Generated char *is
 * of the format: "00:01:0A:2E:05:0B"
 *
 * IMPORTANT: be sure to free() the returned
 * string after use
 *
 * @param aMac The MAC address, in bytes,
 *             to convert
 * @return the MAC address as a String
 */
char *SnowPlowTracker::mac2Chars(const byte* aMac) {
  const size_t bufferLength = 18; // 17 chars plus \0
  char *buffer = (char*)malloc(bufferLength);
  snprintf(buffer, bufferLength, "%02X:%02X:%02X:%02X:%02X:%02X",
          aMac[0],
          aMac[1],
          aMac[2],
          aMac[3],
          aMac[4],
          aMac[5]);
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
  int i = 1;
  while (aPairs[i].name != NULL) {
    i++;
  }
  return i;
}

/**
 * Converts an int into a char *.
 *
 * IMPORTANT: be sure to free() the returned
 * string after use
 *
 * @param aInt The integer to
 *        convert into a char *
 * @return the converted String
 */
char *SnowPlowTracker::int2Chars(const int aInt) {
  const size_t bufferLength = 12; // "-2147483648\0"
  char *buffer = (char*)malloc(bufferLength);
  snprintf(buffer, bufferLength, "%d", aInt);
  return buffer;
}

/**
 * Converts a double (or a float)
 * into a String. Generated char *is
 * 1 or more characters long, with the
 * number of digits after the decimal
 * point specified by `aPrecision`.
 *
 * IMPORTANT: be sure to free() the returned
 * string after use
 *
 * @param aDbl The double (or float) to
 *        convert into a String
 * @return the converted String
 */
char *SnowPlowTracker::double2Chars(const double aDouble, const int aPrecision) {
  const size_t bufferLength = 25;
  char *buffer = (char*)malloc(bufferLength);
  dtostrf(aDouble, 1, aPrecision, buffer);
  return buffer;
}

/**
 * Converts an integer value to its
 * hex character
 *
 * @param aChar The character to hexify
 * @return the character in hex form
 */
char SnowPlowTracker::char2Hex(const char aChar) {
  static char hex[] = "0123456789abcdef";
  return hex[aChar & 15];
}

/**
 * URL-encodes a string. Using code
 * adapted from:
 *
 * http://www.geekhideout.com/urlcode.shtml
 * http://hardwarefun.com/tutorials/url-encoding-in-arduino
 *
 * IMPORTANT: be sure to free() the returned
 * string after use
 * 
 * @param aStr The characters to URL-encode.
 * @return the encoded String
 */
char *SnowPlowTracker::urlEncode(const char* aStr)
{
  const size_t bufferLength = strlen(aStr) * 3 + 1;
  char *pstr = (char*)aStr, *encodedStr = (char*)malloc(bufferLength), *pbuf = encodedStr;
  
  while (*pstr) {
    if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
      *pbuf++ = *pstr;
    else 
      *pbuf++ = '%', *pbuf++ = char2Hex(*pstr >> 4), *pbuf++ = char2Hex(*pstr & 15);
    pstr++;
  }
  *pbuf = '\0'; // Tail it
  
  return encodedStr;
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
int SnowPlowTracker::getUri(
  const char *aHost,
  const int aPort,
  const char *aPath,
  const QuerystringPair aPairs[]) const {

  // Connect to the host
  if (this->client->connect(aHost, aPort)) {
    // Build our GET line from:
    // 1. The URI path... 
    this->client->print("GET ");
    LOG_DEBUG("GET ");
    this->client->print(aPath);
    LOG_DEBUG(aPath);

    // 2. The querychar *name-value pairs
    if (aPairs != NULL) {
      this->client->print("?");
      LOG_DEBUG("?");
      char idx = 0;
      QuerystringPair* pair = (QuerystringPair*)&aPairs[0];
      // Loop for all pairs
      while (pair->name != NULL) {
        // Only add if value is not null
        if (pair->value != NULL) {
          if (idx > 0) {
            this->client->print("&");
            LOG_DEBUG("&");
          }
          
          this->client->print(pair->name);
          LOG_DEBUG(pair->name);

          this->client->print("=");
          LOG_DEBUG("=");
          
          char *encoded = urlEncode(pair->value);
          this->client->print(encoded);
          LOG_DEBUG(encoded);
          free(encoded);
        }
        pair = (QuerystringPair*)&aPairs[++idx];
      }
    }

    // 3. Finish the GET definition
    this->client->println(" HTTP/1.1");
    LOGLN_DEBUG(" HTTP/1.1");

    // Headers
    this->client->print("Host: ");
    this->client->println(aHost);

    this->client->print("User-Agent: ");
    this->client->println(this->kUserAgent);

    this->client->println("Connection: close");

    this->client->println();
    // End of headers

    // TODO: check return value
    // https://github.com/amcewen/HttpClient/blob/master/HttpClient.cpp
    // https://github.com/exosite-garage/arduino_exosite_library/blob/master/Exosite.cpp
  } else {
    // Connection didn't work
    return SnowPlowTracker::ERROR_CONNECTION_FAILED;
  }

  // Return updated status code
  return SnowPlowTracker::SUCCESS;
}
