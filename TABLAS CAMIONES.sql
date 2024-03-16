CREATE DATABASE IF NOT EXISTS transporte;
USE transporte;

CREATE TABLE IF NOT EXISTS camiones (
    matricula VARCHAR(20) PRIMARY KEY,
    capacidad DOUBLE,
    consumo DOUBLE,
    cargaActual DOUBLE
);

CREATE TABLE IF NOT EXISTS cargas (
    id INT AUTO_INCREMENT PRIMARY KEY,
    descripcion VARCHAR(100),
    peso DOUBLE,
    camion_matricula VARCHAR(20),
    FOREIGN KEY (camion_matricula) REFERENCES camiones(matricula),
    fragilidad boolean,
    entregado boolean,
    direccion VARCHAR(100)
);

ALTER TABLE cargas AUTO_INCREMENT = 1;
