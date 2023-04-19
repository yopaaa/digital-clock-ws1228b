## API Documentation

### Description

This API provides endpoints for controlling a device.

### Endpoints

## GET /ping

#### Example Request:

```bash
GET /ping HTTP/1.1
Host: <target ip>:3000
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "message": "OK",
    "CodeVersion": "v3",
    "CycleCount": 172085510,
    "SketchSize": 870976,
    "version": 1,
    "method": 1,
    "url": "/ping",
    "host": "<target ip>:3000",
    "contentType": "",
    "contentLength": 0,
    "multipart": false
}
```

---
## GET /variable

#### Example Request:

```bash
GET /variable HTTP/1.1
Host: <target ip>:3000
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "payload": {
        "CodeVersion": "v3",
        "ssid": "ssid",
        "APssid": "digital_clock",
        "APpassword": "12345678zxcvbnm",
        "RED": 255,
        "GREEN": 255,
        "BLUE": 0,
        "BRIGHTNESS": 100,
        "timeFormat": 12,
        "displayMode": "clock",
        "ColorMode": "",
        "isStaticIP": true,
        "IP": "1.1.0.1"
    },
    "code": 200,
    "message": "OK",
    "method": 1,
    "url": "/variable",
    "host": "<target ip>:3000"
}
```

---

## POST /wifi/set

#### Example Request:

```bash
POST /wifi/set HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
    "ssid": "your_ssid" ,
    "password": "your_password"
}
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "payload": {
        "ssid": "your_ssid"
    },
    "code": 200,
    "message": "OK",
    "method": 2,
    "url": "/wifi/set",
    "host": "<target ip>:3000"
}
```

---

## POST /restart

#### Example Request:

```bash
POST /restart HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{}
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "payload": {},
    "code": 200,
    "message": "OK",
    "method": 2,
    "url": "/restart",
    "host": "<target ip>:3000
}
```

---

## POST /mode

#### Example Request:

```bash
POST /mode HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
    "mode": "counter",
    "limit": 100
}
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "payload": {
        "mode": "counter",
        "limit": 100
    },
    "code": 200,
    "message": "OK",
    "method": 2,
    "url": "/mode",
    "host": "<target ip>:3000
}
```

---

## POST /timeformat

#### Example Request:

```bash
POST /timeformat HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
    "format": 12
}
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "payload": {
        "format": 12
    },
    "code": 200,
    "message": "OK",
    "method": 2,
    "url": "/timeformat",
    "host": "<target ip>:3000
}
```

---

## POST /brightness

#### Example Request:

```bash
POST /brightness HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
    "brightness": 20
}
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "payload": {
        "brightness": 20
    },
    "code": 200,
    "message": "OK",
    "method": 2,
    "url": "/brightness",
    "host": "<target ip>:3000
}
```

---

## POST /color/change

#### Example Request:

```bash
POST /color/change HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
    "red": 0,
    "green": 110,
    "blue": 0,
    "brightness": 10
}
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "payload": {
        "red": 0,
        "green": 110,
        "blue": 0,
    },
    "code": 200,
    "message": "OK",
    "method": 2,
    "url": "/color/change",
    "host": "<target ip>:3000
}
```

---

## POST /color/mode

#### Example Request:

```bash
POST /color/mode HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
    "mode": "random"
}
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "payload": {
        "mode": "random"
    },
    "code": 200,
    "message": "OK",
    "method": 2,
    "url": "/color/mode",
    "host": "<target ip>:3000
}
```

---

## POST /staticIp

#### Example Request:

```bash
POST /staticIp HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
    "isStaticIP": true,
    "ip1": 192,
    "ip2": 168,
    "ip3": 0,
    "ip4": 1
}
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "payload": {
        "ip": "192.168.0.1",
        "isStaticIP": true
    },
    "code": 200,
    "message": "OK",
    "method": 2,
    "url": "/staticIp",
    "host": "<target ip>:3000"
```

---