# Snowplow Analytics for Arduino 

[![unsupported]][tracker-classificiation] 

**DEPRECATED:** This project is no longer maintained.

## Overview

Add analytics to sketches on IP-connected Arduino boards with our event tracker for [Arduino][arduino], our first event tracker for the [Internet of Things][iot].

With this tracker you can collect sensor- and eventstream data from your Arduino-based projects and send it back to your [Snowplow][snowplow] collector. Some project ideas:

* Track the movement of products around your shop/warehouse/factory using Arduino, [RFID readers][arduino-rfid] and Snowplow
* Deploy a set of Snowplow-connected Arduinos to monitor the environment (temperature, humidity, light levels etc) in your home
* Send vehicle fleet information (locations, speeds, fuel levels etc) back to Snowplow using Arduino's [3G and GPS][3g-gps] shields 

For a real project using this tracker to log temperature sensor data to SnowPlow, see [arduino-temp-tracker][arduino-temp-tracker].

## Find out more

| Technical Docs                  | Setup Guide               | Roadmap & Contributing               |         
|---------------------------------|---------------------------|--------------------------------------|
| ![i1][techdocs-image]          | ![i2][setup-image]       | ![i3][roadmap-image]                |
| **[Technical Docs][techdocs]** | **[Setup Guide][setup]** | _coming soon_                        |

## Copyright and license

The SnowPlow Arduino Tracker is copyright 2012-2013 Snowplow Analytics Ltd.

Licensed under the **[Apache License, Version 2.0][license]** (the "License");
you may not use this software except in compliance with the License.

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

[arduino]: http://arduino.cc/
[iot]: http://www.forbes.com/sites/ericsavitz/2013/01/14/ces-2013-the-break-out-year-for-the-internet-of-things/
[snowplow]: https://github.com/snowplow/snowplow
[arduino-rfid]: http://arduino.cc/blog/category/wireless/rfid/
[3g-gps]: http://www.cooking-hacks.com/index.php/documentation/tutorials/arduino-3g-gprs-gsm-gps

[arduino-temp-tracker]: https://github.com/alexanderdean/arduino-temp-tracker

[techdocs-image]: https://d3i6fms1cm1j0i.cloudfront.net/github/images/techdocs.png
[setup-image]: https://d3i6fms1cm1j0i.cloudfront.net/github/images/setup.png
[roadmap-image]: https://d3i6fms1cm1j0i.cloudfront.net/github/images/roadmap.png
[techdocs]: https://github.com/snowplow/snowplow/wiki/Arduino-Tracker
[setup]: https://github.com/snowplow/snowplow/wiki/Arduino-Tracker-Setup

[license]: http://www.apache.org/licenses/LICENSE-2.0

[tracker-classificiation]: https://github.com/snowplow/snowplow/wiki/Tracker-Maintenance-Classification
[unsupported]: https://img.shields.io/static/v1?style=flat&label=Snowplow&message=Unsupported&color=24292e&labelColor=lightgrey&logo=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAAeFBMVEVMaXGXANeYANeXANZbAJmXANeUANSQAM+XANeMAMpaAJhZAJeZANiXANaXANaOAM2WANVnAKWXANZ9ALtmAKVaAJmXANZaAJlXAJZdAJxaAJlZAJdbAJlbAJmQAM+UANKZANhhAJ+EAL+BAL9oAKZnAKVjAKF1ALNBd8J1AAAAKHRSTlMAa1hWXyteBTQJIEwRgUh2JjJon21wcBgNfmc+JlOBQjwezWF2l5dXzkW3/wAAAHpJREFUeNokhQOCA1EAxTL85hi7dXv/E5YPCYBq5DeN4pcqV1XbtW/xTVMIMAZE0cBHEaZhBmIQwCFofeprPUHqjmD/+7peztd62dWQRkvrQayXkn01f/gWp2CrxfjY7rcZ5V7DEMDQgmEozFpZqLUYDsNwOqbnMLwPAJEwCopZxKttAAAAAElFTkSuQmCC 

