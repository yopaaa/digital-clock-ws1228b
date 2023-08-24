# API Documentation

### Description

This API provides endpoints for controlling a device.

### POST /wifi

#### Example Request:

```bash
POST http://{{host}}/wifi HTTP/1.1
Content-Type: application/json

{
}

```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Length: 259
Content-Type: application/json
Access-Control-Allow-Origin: *
Connection: close
Accept-Ranges: none

{
  "code": 200,
  "message": "OK",
  "payload": {
    "STAssid": "Biznet_Store",
    "STApassword": "",
    "APssid": "digital_clock",
    "APpassword": "12345678zxcvbnm",
    "isStaticIP": false,
    "ip": "192.168.18.136",
    "CODE_VERSION": "v1.1.1",
    "DEVICES_ID": "Digital_clock-24433FSFDG32",
    "uptime": 12345
  },
  "info": {
    "Gateway": "192.168.18.1",
    "Subnet": "255.255.255.0",
    "DNS1": "192.168.18.1"
  }
}
```

---

### DELETE /restart

#### Example Request:

```bash
DELETE  http://{{host}}/restart HTTP/1.1
Content-Type: application/json

{
}
```

#### Example Response:

```bash
{}
```

---

### DELETE /reset

#### Example Request:

```bash
DELETE  http://{{host}}/reset HTTP/1.1
Content-Type: application/json

{
}
```

#### Example Response:

```bash
{}
```

---

# MQTT API Documentation

### Description

This API provides MQTT topics and their corresponding data format for controlling a device.

### Topics and Payload Format

#### DEVICES_NAME + "-" + DEVICES_ID + "-color"
Topic Description:
When a message is published to this topic, the ESP32 will send a JSON payload to the topic ___DEVICES_NAME + "-" + DEVICES_ID + "-info-color"__. The payload contains color and brightness data.

Example Payload:

```json
{
  "red": 255,
  "green": 2,
  "blue": 23,
  "brightness": 130
}
```
>This payload can be used when sending data to the topics above.

#### DEVICES_NAME + "-" + DEVICES_ID + "-time"
Topic Description:
When a message is published to this topic, the ESP32 will send a JSON payload to the topic __DEVICES_NAME + "-" + DEVICES_ID + "-info-time"__. The payload contains timestamp, NTP server information, time format, and timezone data.

Example Payload:
```json
{
"timestamp": 1690549472,
"ntpServer": "pool.ntp.org",
"daylightOffset_sec": 0,
"format": 12,
"gmtOffset_sec": 25200
}
```
>This payload can be used when sending data to the topics above, except for properties ntpServer and daylightOffset_sec.

#### DEVICES_NAME + "-" + DEVICES_ID + "-alarms"
Topic Description:
When a message is published to this topic, the ESP32 will send data to the topic __DEVICES_NAME + "-" + DEVICES_ID + "-info-alarms"__ in the following format:

```json
{
"index": 0,
"hour": 0,
"min": 0,
"alertIndex": 0,
"days": 127
}
```
This data will be sent five times consecutively, according to the maximum alarms.
>This payload can be used when sending data to the topics above.

#### DEVICES_NAME + "-" + DEVICES_ID + "-wifi"
Topic Description:
When a message is published to this topic, the ESP32 will send a JSON payload to the topic __DEVICES_NAME + "-" + DEVICES_ID + "-info-wifi"__. The payload contains Wi-Fi connection information.

Example Payload:
```json
{
"STAssid": "vivo1904",
"STApassword": "rWiXDQZpD7dqguBC5ShXr3h2F8lUsY",
"APssid": "digital_clock",
"APpassword": "12345678zxcvbnm",
"isStaticIP": false,
"ip": "10.10.10.11"
}
```
>This payload can be used when sending data to the topics above.

#### DEVICES_NAME + "-" + DEVICES_ID + "-mode"
Topic Description:
When a message is published to this topic, the ESP32 will send a JSON payload to the topic __DEVICES_NAME + "-" + DEVICES_ID + "-info-mode"__. The payload contains mode and color mode information.

Example Payload:
```json
{
"mode": "clock",
"colorMode": "solid",
"segment1": "hour",
"segment2": "min",
"scors1": 0,
"scors2": 0,
"isPause": true,
"limit": 120,
"counterCount": 0,
"countDownCount": 100
}
```
> This payload can be used when sending data to the topics above

#### DEVICES_NAME + "-" + DEVICES_ID + "-restart"
Topic Description:
When a message is published to this topic, the ESP32 will restart the device.

#### DEVICES_NAME + "-" + DEVICES_ID + "-restart"
Topic Description:
When a message is published to this topic, the ESP32 will bepp the buzzer.

---
