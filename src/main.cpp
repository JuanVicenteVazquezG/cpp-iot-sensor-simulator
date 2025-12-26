#include <iostream>
#include <vector>
#include <memory>
#include <string>  // Para stoi
#include <cstdlib> // Para getenv
#include "Sensor.h"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

int main()
{
    //1. Configuración
    const char *id_env = std::getenv("SENSOR_ID");
    int sensor_id = id_env ? std::stoi(id_env) : 999;
    
    spdlog::info("--- INICIANDO SISTEMA DE SENSORES IOT ---");
    spdlog::info("Configuración detectada: ID {}\n", sensor_id);


    // 2. Creamos el Sensor
    auto sensor = std::make_unique<Sensor>(sensor_id, 20.5);


    // 3. Lectura de Datos Estructurados
    DatosTelemetria datos = sensor->leerDatos();

    // 4. Conversión Enum -> String (Seguridad)
    std::string estado_str;

    switch(datos.estado) {
        case EstadoSensor::OK: estado_str = "OK"; break;
        case EstadoSensor::ADVERTENCIA: estado_str = "WARNING"; break;
        case EstadoSensor::ERROR_CRITICO: estado_str = "CRITICAL"; break;
        default: estado_str = "UNKNOWN"; break;
    }

    // 5. Generación de JSON
    spdlog::info("Generando telemetría...");
    json j;
    j["id_sensor"] = datos.id;
    j["valor_temp"] = datos.temperatura;
    j["status"] = estado_str;

    std::cout << j.dump(4) << std::endl;

    return 0;

}