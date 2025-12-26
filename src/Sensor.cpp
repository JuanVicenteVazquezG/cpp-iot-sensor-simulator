#include "Sensor.h"
#include <spdlog/spdlog.h> // Si quieres loguear aquí, usa esto, no cout

// CORRECCIÓN 1: Usamos {} para inicializar (Uniform Initialization)
// CORRECCIÓN 2: Quitamos los guiones bajos '_id'. Usamos el mismo nombre y la lista lo resuelve.
Sensor::Sensor(int id, double temp) 
    : id{id}, temperatura{temp} 
{
    // Constructor vacío
}

// CORRECCIÓN 3: Logging consistente (opcional, o dejar vacío)
Sensor::~Sensor()
{
    // spdlog::debug("Sensor {} eliminado", id); 
}

// CORRECCIÓN 4: Eliminamos to_json() y leer() con cout. 
// El sensor no debe saber imprimir, solo medir.

// CORRECCIÓN 5: Designated Initializers (C++20) para seguridad máxima
DatosTelemetria Sensor::leerDatos() const {
    return DatosTelemetria{
        .id = this->id,
        .temperatura = this->temperatura, // Asegúrate que en Sensor.h la variable se llame 'temperatura' y no 'valor'
        .estado = EstadoSensor::OK
    };
}