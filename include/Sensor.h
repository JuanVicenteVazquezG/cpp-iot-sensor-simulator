// ESTILO VIEJO (Lo verás en código legacy) no usar pero si entender
// #ifndef SENSOR_H
// #define SENSOR_H

#pragma once
#include <iostream>
#include "nlohmann/json.hpp" // <--- 1. Incluimos la librería externa

// --- NUEVO: DEFINICIONES DE TIPOS (Stroustrup Cap. 2) ---

// 1. Enum Class: Evita que uses números o strings sueltos.
// Solo existen estos tres estados. Nada más.

enum class EstadoSensor{
    OK,
    ADVERTENCIA,
    ERROR_CRITICO
};

// 2. Struct: Un paquete de datos puro (sin funciones complejas).
// Esto es lo que enviaremos a la nube.

struct DatosTelemetria{
    int id;
    double temperatura; 
    EstadoSensor estado;
};
// --- FIN NUEVO ---

// 2. Creamos un alias para no escribir nlohmann::json todo el rato
using json = nlohmann::json;

class Sensor
{
public:
    int id;
    double temperatura;
    // Solo declaramos. No hay código lógico aquí.
    Sensor(int _id, double _temp);
    ~Sensor();

    void leer() const;

    json to_json()const;

    DatosTelemetria leerDatos() const;
};