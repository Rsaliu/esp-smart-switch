#include <MqttComms.hpp>
#include <esp_event_base.h>
#include <esp32-hal-log.h>
static void defaultCallback(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        if (strncmp(event->data, "send binary please", event->data_len) == 0) {
            ESP_LOGI(TAG, "Sending the binary");
            //send_binary(client);
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
            ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            ESP_LOGI(TAG, "Last captured errno : %d (%s)",  event->error_handle->esp_transport_sock_errno,
                     strerror(event->error_handle->esp_transport_sock_errno));
        } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
            ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
        } else {
            ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}
static const MqttConfig DEFAULT_CONFIG = {
    .host = "192.168.43.142",
    .port = 1883,
    .enableTLS = false,
    .caCert = "",
    .clientCert = "",
    .clientKey = "",
    .callback = defaultCallback,
};

MqttComms::MqttComms():MqttComms(DEFAULT_CONFIG)
{

}
MqttComms::MqttComms(MqttConfig config):config(config)
{

}
CommsError MqttComms::Connect()
{
    esp_err_t err = ESP_OK;
    esp_mqtt_client_config_t mqtt_cfg ={
        .host = config.host.c_str(),
        .port = config.port,
    };
    if(config.enableTLS){
        mqtt_cfg.cert_pem = (const char *)config.caCert.c_str();
        mqtt_cfg.cert_len = config.caCert.length();
        mqtt_cfg.client_cert_pem = (const char *)config.clientCert.c_str();
        mqtt_cfg.client_cert_len =config.clientCert.length();
        mqtt_cfg.client_key_pem = (const char *)config.clientKey.c_str();
        mqtt_cfg.client_key_len = config.clientKey.length();
        mqtt_cfg.transport = MQTT_TRANSPORT_OVER_SSL;
    }

    client = esp_mqtt_client_init(&mqtt_cfg);
    err = esp_mqtt_client_register_event(client, MQTT_EVENT_ANY, config.callback, this);
    if(err != ESP_OK){
        return CommsError::FAIL;
    }
    err = esp_mqtt_client_start(client);
    if(err != ESP_OK){
        return CommsError::FAIL;
    }
    return CommsError::OK;
}
CommsError MqttComms::Publish(std::string topic, std::string message,MqttQOS qos)
{
    int result;
    result = esp_mqtt_client_publish(client,topic.c_str(),message.c_str(),message.size(),(int)qos,0);
    if(result == -1){
        return CommsError::FAIL;
    }
    return CommsError::OK;
}
CommsError MqttComms::Subscribe(std::string topic,MqttQOS qos)
{
    int result;
    result = esp_mqtt_client_subscribe(client,topic.c_str(),(int)qos);
    if(result == -1){
        return CommsError::FAIL;
    }
    return CommsError::OK;
}