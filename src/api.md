## API Documentation

### Description

This API provides endpoints for controlling a device.

### Endpoints




## GET /info

#### Example Request:

```bash
GET http://{{host}}/info HTTP/1.1
Content-Type: "application/json"

{}
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Length: 325
Content-Type: application/json
Access-Control-Allow-Origin: *
Connection: close
Accept-Ranges: none

{
  "code": 200,
  "message": "OK",
  "endpoint": [
    "192.168.18.62:3000/wifi",
    "192.168.18.62:3000/color",
    "192.168.18.62:3000/mode",
    "192.168.18.62:3000/time",
    "192.168.18.62:3000/info"
  ],
  "method": 1,
  "url": "/info",
  "args": 0,
  "params": 0,
  "CODE_VERSION": "v3.6.6",
  "uptime": 286,
  "isInternetConnection": true,
  "flashChipSize": 4194304,
  "SketchSize": 895488
}

```

---
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
Content-Length: 258
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
    "ip": "192.168.18.62"
  },
  "info": {
    "Gateway": "192.168.18.1",
    "Subnet": "255.255.255.0",
    "DNS1": "192.168.18.1"
  }
}
```

---

## POST /color

#### Example Request:

```bash
POST http://{{host}}/color HTTP/1.1
Content-Type: application/json

{
}
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Length: 110
Content-Type: application/json
Access-Control-Allow-Origin: *
Connection: close
Accept-Ranges: none

{
  "code": 200,
  "message": "OK",
  "payload": {
    "red": 255,
    "green": 0,
    "blue": 0,
    "brightness": 100,
  },
  "info": {}
}
```

---

## POST /mode

#### Example Request:

```bash
POST http://{{host}}/mode HTTP/1.1
Content-Type: application/json

{
}
```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Length: 170
Content-Type: application/json
Access-Control-Allow-Origin: *
Connection: close
Accept-Ranges: none

{
  "code": 200,
  "message": "OK",
  "payload": {
    "mode": "clock",
    "segment1": "hour",
    "segment2": "min",
    "scors1": 0,
    "scors2": 0,
    "limit": 120
  },
  "info": {
    "counterCount": 0,
    "countDownCount": 100
  }
}

```

---

## POST /time

curl -X POST -H "Content-Type: application/json" -d '{"timestamp":"'"$(date +%s)"'"}' http://10.10.10.50:3000/time

#### Example Request:

```bash
POST http://{{host}}/time HTTP/1.1
Content-Type: application/json

{
}

```

#### Example Response:

```bash
HTTP/1.1 200 OK
Content-Length: 453
Content-Type: application/json
Access-Control-Allow-Origin: *
Connection: close
Accept-Ranges: none

{
  "code": 200,
  "message": "OK",
  "payload": {
    "format": 24,
    "gmtOffset_sec": 25200,
    "timestamp": 1687335491,
    "alarms": [
      {
        "hour": 0,
        "minute": 0,
        "alertIndex": 0,
        "days": [
          0,
          0,
          0,
          0,
          0,
          0,
          0
        ]
      },
      {
        "hour": 0,
        "minute": 0,
        "alertIndex": 0,
        "days": [
          0,
          0,
          0,
          0,
          0,
          0,
          0
        ]
      },
      {
        "hour": 0,
        "minute": 0,
        "alertIndex": 0,
        "days": [
          0,
          0,
          0,
          0,
          0,
          0,
          0
        ]
      },
      {
        "hour": 0,
        "minute": 0,
        "alertIndex": 0,
        "days": [
          0,
          0,
          0,
          0,
          0,
          0,
          0
        ]
      },
      {
        "hour": 0,
        "minute": 0,
        "alertIndex": 0,
        "days": []
      }
    ]
  },
  "info": {
    "ntpServer": "pool.ntp.org",
    "daylightOffset_sec": 0
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
