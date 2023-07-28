# API Documentation

### Description

This API provides endpoints for controlling a device.

### Endpoints

## POST /wifi

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

## DELETE /restart

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

## DELETE /reset

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
Additionally, ESP32 will also send data to the topic DEVICES_NAME + "-" + DEVICES_ID + "-info-alarms" in the following format:

```json
{
"index": 0,
"hour": 0,
"min": 0,
"alertIndex": 0,
"days": [0, 0, 0, 0, 0, 0, 0]
}
```
This data will be sent five times consecutively, according to the maximum alarms.

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
Additionally, the ESP32 will send data to the topic DEVICES_NAME + "-" + DEVICES_ID + "-info-alarms" in the same format as mentioned in the DEVICES_NAME + "-" + DEVICES_ID + "-time" topic.

#### DEVICES_NAME + "-" + DEVICES_ID + "-restart"
Topic Description:
When a message is published to this topic, the ESP32 will restart the device.

## Note
- The properties from the -info-color, -info-time, -info-wifi, and -info-mode topics can be used when sending data to the corresponding -color, -time, -wifi, and -mode topics.
- The properties ntpServer and daylightOffset_sec are not used when sending data to the -color and -wifi topics, respectively.
- The properties counterCount and countDownCount are not used when sending data to the -color topic.
- The properties ntpServer and daylightOffset_sec in the -info-time topic and counterCount and countDownCount in the -info-mode topic are not used when sending data to the corresponding -time and -mode topics.
