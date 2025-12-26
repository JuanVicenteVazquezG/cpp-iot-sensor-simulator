# 1. IMAGEN BASE: Empezamos con un Ubuntu 22.04 limpio
FROM ubuntu:22.04 AS Builder

ENV DEBIAN_FRONTEND=noninteractive

# Instalamos compilador (g++), CMake y Git (necesario para bajar la librería JSON)
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Creamos una carpeta dentro del contenedor y nos metemos en ella
WORKDIR /app


# Copiamos todo lo de tu carpeta actual (.) a la carpeta del contenedor (.)
COPY . .

# Compilamos (Aquí se descarga JSON y SPDLOG, y se genera el ejecutable)
# RUN mkdir build && cd build && cmake .. && cmake --build .
RUN mkdir -p build && cd build && cmake .. && cmake --build .

# --- ETAPA 2: EL EJECUTOR (El Producto Final) ---
# Empezamos de cero con un Ubuntu limpio.
FROM ubuntu:22.04
# Solo limpiamos caches (no instalamos compiladores)
RUN apt-get update && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# --- LA MAGIA ESTÁ AQUÍ ---
# Copiamos SOLO el archivo ejecutable DESDE la etapa "builder"
# Todo lo demás (código fuente, git, librerías intermedias) se destruye.
COPY --from=builder /app/build/sistema_modular .

# Comando de arranque
CMD ["./sistema_modular"]