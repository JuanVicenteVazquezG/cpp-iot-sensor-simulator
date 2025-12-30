# --- ETAPA 1: BUILDER (El Taller) ---
FROM ubuntu:22.04 AS Builder

ENV DEBIAN_FRONTEND=noninteractive

# 1. Instalamos herramientas básicas (SIN librerías paho de apt)
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp

# 2. COMPILAR PAHO MQTT C (El Motor) - Desde Fuente
# Necesitamos compilarla con flags específicos para que genere los targets estáticos
RUN git clone https://github.com/eclipse/paho.mqtt.c.git && \
    cd paho.mqtt.c && \
    git checkout v1.3.13 && \
    cmake -Bbuild -H. \
        -DPAHO_ENABLE_TESTING=FALSE \
        -DPAHO_BUILD_STATIC=TRUE \
        -DPAHO_BUILD_SHARED=TRUE \
        -DPAHO_WITH_SSL=FALSE && \
    cmake --build build/ --target install && \
    ldconfig

# 3. COMPILAR PAHO MQTT C++ (El Wrapper) - Desde Fuente
# Ahora sí encontrará la librería C estática que acabamos de compilar
RUN git clone https://github.com/eclipse/paho.mqtt.cpp.git && \
    cd paho.mqtt.cpp && \
    git checkout v1.3.2 && \
    cmake -Bbuild -H. \
        -DPAHO_BUILD_DOCUMENTATION=FALSE \
        -DPAHO_BUILD_SAMPLES=FALSE \
        -DPAHO_BUILD_STATIC=TRUE \
        -DPAHO_BUILD_SHARED=TRUE \
        -DPAHO_WITH_SSL=FALSE && \
    cmake --build build/ --target install && \
    ldconfig

# --- AHORA VAMOS CON TU PROYECTO ---
WORKDIR /app
COPY . .

# 4. Compilamos tu proyecto
RUN mkdir -p build && cd build && cmake .. && cmake --build .


# --- ETAPA 2: RUNNER (El Producto Final) ---
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Necesitamos copiar las librerías que compilamos manualmente
# desde el Builder al Runner, porque no existen en el apt de Ubuntu por defecto.
COPY --from=Builder /usr/local/lib/libpaho* /usr/lib/

# Actualizamos la caché de librerías del runner
RUN ldconfig

WORKDIR /app

# Copiamos el ejecutable
COPY --from=Builder /app/build/sistema_modular .

CMD ["./sistema_modular"]