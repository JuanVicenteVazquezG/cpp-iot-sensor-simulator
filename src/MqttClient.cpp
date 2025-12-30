#include "MqttClient.h"
#include <spdlog/spdlog.h> // Usamos spdlog para logs profesionales

// --- CONSTRUCTOR ---
MqttClient::MqttClient(const std::string& address, const std::string& clientId) 
    : cli(address, clientId) { // Inicializamos el objeto cliente de Paho
    
    // Configuración de la conexión
    // Keep Alive 20s: Envía un "ping" cada 20s para que el broker sepa que seguimos vivos
    connOpts.set_keep_alive_interval(20);
    
    // Clean Session true: Borra mensajes viejos al conectar. 
    // Para un sensor en tiempo real, no queremos mensajes de ayer.
    connOpts.set_clean_session(true);
    
    spdlog::info("Cliente MQTT inicializado. Broker: {}", address);
}

// --- DESTRUCTOR ---
MqttClient::~MqttClient() {
    // Si el objeto se destruye, nos aseguramos de cerrar la conexión
    desconectar();
}

// --- CONECTAR ---
bool MqttClient::conectar() {
    try {
        spdlog::info("Intentando conectar al broker...");
        
        // Esta línea es la que realmente lanza la conexión de red
        cli.connect(connOpts);
        
        spdlog::info("¡Conexión MQTT Exitosa!");
        return true;
    }
    catch (const mqtt::exception& exc) {
        // Si falla (ej: no hay internet), capturamos el error para que el programa no explote
        spdlog::error("Error fatal al conectar: {} [Código: {}]", exc.what(), exc.get_reason_code());
        return false;
    }
}

// --- DESCONECTAR ---
void MqttClient::desconectar() {
    // Solo desconectamos si estamos conectados
    if (cli.is_connected()) {
        try {
            cli.disconnect();
            spdlog::info("Desconectado correctamente del broker.");
        }
        catch (const mqtt::exception& exc) {
            spdlog::error("Error al desconectar: {}", exc.what());
        }
    }
}

// --- PUBLICAR ---
bool MqttClient::publicar(const std::string& topic, const std::string& payload) {
    // 1. Verificación de seguridad
    if (!cli.is_connected()) {
        spdlog::warn("⚠️ Intento de publicar fallido: No hay conexión.");
        return false;
    }

    try {
        // 2. Creamos el mensaje Paho
        // Convertimos el string std::string a un mensaje mqtt
        auto msg = mqtt::make_message(topic, payload);
        
        // 3. QoS (Calidad de Servicio)
        // 0 = Fire and forget (rápido, inseguro)
        // 1 = At least once (seguro, el estándar industrial)
        // 2 = Exactly once (lento, muy pesado)
        msg->set_qos(QOS);
        
        // 4. Enviar
        cli.publish(msg);
        
        // Usamos 'debug' en vez de 'info' para no saturar la consola con cada envío
        spdlog::debug("Mensaje enviado a topic '{}': {}", topic, payload);
        return true;
    }
    catch (const mqtt::exception& exc) {
        spdlog::error("Error al publicar mensaje: {}", exc.what());
        return false;
    }
}