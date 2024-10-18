# Usar una imagen base de Ubuntu
FROM ubuntu:latest

# Instalar gcc y otras herramientas necesarias
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    valgrind \
    vim \
    && apt-get clean

# Crear un directorio de trabajo
WORKDIR /usr/src/app

# Copiar todo el contenido de tu proyecto al contenedor
COPY . .

# Compilar el programa usando el Makefile
RUN make

# Comando por defecto para ejecutar el programa con valgrind
CMD ["valgrind", "./osrms", "mem.bin"]
