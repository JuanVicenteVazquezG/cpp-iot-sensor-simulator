# 1. Version minima de cmake
cmake_minimum_required(VERSION 3.15)
# 2. Nombre del programa
project(SensorIoT VERSION 1.0.0)
# 3. Especificar version de c++ que usaremos (Crucial para tu "auto" y "concepts")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# BUSCAR: Encuentra dónde guardó Docker los archivos

# 4. --- ZONA DE DESCARTA DE LIBRERÍAS (DEPENDENCIES) ---

# A) MQTT: ¡NO USAR FETCHCONTENT! Usamos la del sistema que compiló Docker.
find_package(PahoMqttCpp REQUIRED)

include(FetchContent)  # Cargamos el módulo de descargas

# 5. Declaramos qué queremos bajar (nlohmann/json) Descargar y hacer disponible ahora mismo
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3  # Fijamos una versión estable
)

FetchContent_MakeAvailable(json)

# 6. Declaramos qué queremos bajar (gabime/spdlog) Descargar y hacer disponible ahora mismo
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.12.0            
)

FetchContent_MakeAvailable(spdlog)

# --- FIN DE DEPENDENCIAS ---

# 7. El Objetivo (Target): ¿Qué queremos fabricar?
#    Nombre del ejecutable: sistema_modular
#    Ingredientes: main.cpp y Sensor.cpp
add_executable(sistema_modular 
    src/main.cpp 
    src/Sensor.cpp      # <--- Faltaba esto o no se estaba leyendo bien
    src/MqttClient.cpp  # <--- Faltaba esto (crítico para tus errores)
)

# 2. Le decimos dónde buscar los .h (Header Search Path)
# Esto permite que en tu código pongas #include "Sensor.h" 
# sin tener que poner rutas feas como "../include/Sensor.h"
target_include_directories(sistema_modular PRIVATE 
    "${PROJECT_SOURCE_DIR}/include"
)

# 8. ENLAZADO (LINKING)
# Le decimos al ejecutable: "Vas a usar la librería nlohmann_json"
target_link_libraries(sistema_modular 
    PRIVATE 
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    PahoMqttCpp::paho-mqttpp3-static # O la versión que detecte tu sistema
)
