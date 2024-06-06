#ifndef __MQTTCOMMS_H__
#define __MQTTCOMMS_H__
#include <string>
#include "mqtt_client.h"
#include <functional>
#include <map>

typedef void(*MqttEventCallBack_t)(void *, esp_event_base_t , int32_t , void *);

struct MqttConfig{
    std::string host;
    uint32_t port;
    bool enableTLS;
    std::string caCert;
    std::string clientCert;
    std::string clientKey;
    MqttEventCallBack_t callback;
};

enum class CommsError{
    OK,
    FAIL,
};

enum class MqttQOS{
    QOS_ZERO = 0,
    QOS_ONE,
    QOS_TWO
};
class MqttComms
{
    public:
        MqttComms();
        MqttComms(MqttConfig config);
        CommsError Connect();
        CommsError Publish(std::string topic, std::string message,MqttQOS qos);
        CommsError Subscribe(std::string topic,MqttQOS qos );

    private:
        MqttConfig config;
        esp_mqtt_client_handle_t client;
};
#endif