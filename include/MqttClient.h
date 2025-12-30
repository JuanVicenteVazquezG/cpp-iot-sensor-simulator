#pragma once

#include <string>
#include <mqtt/client.h> 

class MqttClient {
private:
    mqtt::client cli;
    mqtt::connect_options connOpts;
    const int QOS = 1;

public:
    MqttClient(const std::string& address, const std::string& clientId);
    ~MqttClient();

    bool conectar();
    void desconectar();
    bool publicar(const std::string& topic, const std::string& payload);
}; // <--- ¡AQUÍ FALTABA ESTE PUNTO Y COMA!