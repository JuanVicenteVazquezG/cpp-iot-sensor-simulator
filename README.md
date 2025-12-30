# 📡 IoT Sensor Simulator (C++20)

Simulador de sensor IoT modular desarrollado en C++ moderno, diseñado para entornos de alto rendimiento y desplegable mediante contenedores.

## 🚀 Características
* **Modern C++ (C++20):** Uso de `std::unique_ptr`, `enum class` y Designated Initializers.
* **Arquitectura Limpia:** Separación estricta entre Interfaz (`include/`) e Implementación (`src/`).
* **Dockerized:** Despliegue agnóstico del entorno mediante Docker (Multi-stage build).
* **Build System:** Gestión de compilación profesional con CMake.
* **Logging:** Integración con `spdlog` para trazabilidad estructurada.

## 🛠️ Tecnologías
* **Lenguaje:** C++20
* **Compilador:** GCC 13 (Alpine Linux)
* **Infraestructura:** Docker
* **Librerías:** nlohmann/json, spdlog

## 📋 Cómo ejecutar

### Prerrequisitos
* Docker instalado.

### Compilación y Ejecución
El proyecto está totalmente contenerizado. No necesitas instalar herramientas de C++ en tu host.

```bash
# 1. Construir la imagen
docker build -t sensor-iot:v1 .

# 2. Ejecutar el sensor (inyectando ID por variable de entorno)
docker run --rm -e SENSOR_ID=500 sensor-iot:v1