#pragma once

#include <nlohmann/json.hpp> 
using namespace nlohmann;

class Sensor {
public:
    // Constructor vacío (sin argumentos)
    Sensor(); 
    
    // Método que devuelve JSON
    nlohmann::json leerDatos();
};