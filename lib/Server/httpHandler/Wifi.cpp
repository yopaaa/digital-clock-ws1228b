#include "HttpHandle.h"

/*
when making a request, it can contain JSON:
{
    "STAssid": <value>,
    "STApassword": <value>,
    "APssid": <value>,
    "APpassword": <value>,
}
*/
void HandleWifi(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json)
{
    JsonObject payload = json.createNestedObject("payload");
    JsonObject info = json.createNestedObject("info");

    json["code"] = 200;
    json["message"] = "OK";

    if (jsonDoc.containsKey("STAssid"))
    {
        String ssid = jsonDoc["STAssid"].as<String>();
        IPAddress nullIP(0, 0, 0, 0);

        writeString(SSID_ADDRESS, ssid);
        writeStaticIp(nullIP);
    }

    if (jsonDoc.containsKey("STApassword"))
    {
        String password = jsonDoc["STApassword"].as<String>();
        writeString(PASSWORD_ADDRESS, password);
    }

    if (jsonDoc.containsKey("APssid"))
    {
        String ssid = jsonDoc["APssid"].as<String>();
        writeString(AP_SSID_ADDRESS, ssid);
    }

    if (jsonDoc.containsKey("APpassword"))
    {
        String password = jsonDoc["APpassword"].as<String>();
        writeString(AP_PASSWORD_ADDRESS, password);
    }

    if (jsonDoc.containsKey("ip"))
    {
        String ip = jsonDoc["ip"].as<String>();

        int octetIp[4];

        int dot1 = ip.indexOf(".");
        octetIp[0] = ip.substring(0, dot1).toInt();

        int dot2 = ip.indexOf(".", dot1 + 1);
        octetIp[1] = ip.substring(dot1 + 1, dot2).toInt();

        int dot3 = ip.indexOf(".", dot2 + 1);
        octetIp[2] = ip.substring(dot2 + 1, dot3).toInt();

        octetIp[3] = ip.substring(dot3 + 1).toInt();

        IPAddress combineIp(octetIp[0], octetIp[1], octetIp[2], octetIp[3]);
        writeStaticIp(combineIp);
    }

    if (jsonDoc.containsKey("isStaticIP"))
    {
        bool extractisStaticIP = jsonDoc["isStaticIP"].as<bool>();
        writeBool(IS_STATIC_IP_ADDRESS, extractisStaticIP);
    }
    
    payload["STAssid"] = ssid;
    payload["STApassword"] = password;
    payload["APssid"] = APssid;
    payload["APpassword"] = APpassword;
    payload["isStaticIP"] = isStaticIP;
    payload["ip"] = IP;

    info["Gateway"] = Gateway;
    info["Subnet"] = Subnet;
    info["DNS1"] = DNS1;
}
