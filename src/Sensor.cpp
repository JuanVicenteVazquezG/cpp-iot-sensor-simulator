#include "Sensor.h"
#include <cstdlib> // Para rand
#include <ctime> // Para time

using namespace nlohmann;

Sensor::Sensor() {
    std::srand(std::time(nullptr));
};

nlohmann::json Sensor::leerDatos(){
// 1. Simulamos datos aleatorios
    double temperatura = 20.0 + (std::rand() % 100) / 10.0; // Entre 20.0 y 30.0
    double humedad = 40.0 + (std::rand() % 200) / 10.0;     // Entre 40.0 y 60.0

   // 2. Construimos el JSON
   json datos;
   datos["sensor_id"] = "vama_temp_01";
   datos["temperatura"] = temperatura;
   datos["humedad"] = humedad;
   datos["timestamp"] = std::time(nullptr);
    // Hora actual UNIX
    return datos;
}