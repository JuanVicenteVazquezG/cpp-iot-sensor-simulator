#include "Sensor.h"
#include "MqttClient.h"
#include <spdlog/spdlog.h> // <--- OJO: Es spdlog, no spdg
#include <thread> 
#include <chrono> 
#include <cmath> // Necesario para std::abs

using namespace std::chrono_literals;

int main() {
    // 1. Configuración de Logs
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("=== INICIANDO SISTEMA IOT V1.0 ===");

    // 2. Instanciamos los objetos
    std::string brokerUrl = "tcp://test.mosquitto.org:1883";
    std::string clientId = "sensor_vama_001"; 
    std::string topic = "vama/sensor/datos";

    // Ahora sí funcionará porque Sensor.h ya tiene el constructor vacío
    Sensor sensor;
    
    // Ahora sí funcionará porque MqttClient.h ya tiene el punto y coma
    MqttClient mqtt(brokerUrl, clientId);

    // 3. Conexión Inicial
    if (!mqtt.conectar()) {
        spdlog::critical("No se pudo conectar al broker. Abortando.");
        return 1; 
    }

    // --- MEMORIA DEL SISTEMA ---
    double lastTemp = -999.0;
    double lastHum = -999.0;
    
    const double UMBRAL_TEMP = 0.5; 
    const double UMBRAL_HUM = 1.0;

    // 4. EL BUCLE INFINITO
    while (true) {
        try {
            auto datosJson = sensor.leerDatos();
            
            double currentTemp = datosJson["temperatura"];
            double currentHum = datosJson["humedad"];

            // Delta Check
            bool cambioTemp = std::abs(currentTemp - lastTemp) >= UMBRAL_TEMP;
            bool cambioHum  = std::abs(currentHum - lastHum) >= UMBRAL_HUM;

            if (cambioTemp || cambioHum) {
                spdlog::info(">>> CAMBIO: T:{:.1f} (ant: {:.1f}) | H:{:.1f}", currentTemp, lastTemp, currentHum);
                
                std::string payload = datosJson.dump();
                if (mqtt.publicar(topic, payload)) {
                    lastTemp = currentTemp;
                    lastHum = currentHum;
                } 
            } else {
                spdlog::debug("Estable (T:{:.1f}). Ignorando.", currentTemp);
            }

        } catch (const std::exception& e) {
            spdlog::error("Error en ciclo principal: {}", e.what());
        }

        std::this_thread::sleep_for(2s);
    }

    return 0;
}