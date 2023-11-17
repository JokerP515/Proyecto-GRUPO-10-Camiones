#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>

using namespace std;

class Carga {
public:
    int id;
    string descripcion;
    string direccion;
    double peso;
    string camionMatricula;  // id del camion
    bool fragilidad;
    bool entregado;

    Carga(int id, string descripcion, string direccion, double peso, string camionMatricula, bool fragilidad, bool entregado)
        : id(id), descripcion(descripcion), direccion(direccion), peso(peso), camionMatricula(camionMatricula),fragilidad(fragilidad),entregado(entregado) {}
};

class Camion {
public:
    string matricula;
    double capacidad;
    double consumo;
    double cargaActual;

    Camion(string matricula, double capacidad, double consumo, double cargaActual)
        : matricula(matricula), capacidad(capacidad), consumo(consumo), cargaActual(cargaActual) {}

    // Métodos públicos
    void registrarCamion(sql::Connection* con) {
        sql::PreparedStatement* pstmt;

        cout << "Ingrese la matrícula del camión: ";
        cin >> matricula;
        cout << "Ingrese la capacidad de carga del camión (kg): ";
        cin >> capacidad;
        cout << "Ingrese el consumo de gasolina del camión (galones/km): ";
        cin >> consumo;
        cargaActual = 0;

        pstmt = con->prepareStatement("INSERT INTO camiones (matricula, capacidad, consumo, cargaActual) VALUES (?, ?, ?, ?)");
        pstmt->setString(1, matricula);
        pstmt->setDouble(2, capacidad);
        pstmt->setDouble(3, consumo);
        pstmt->setDouble(4, cargaActual);

        pstmt->execute();
        delete pstmt;

        cout << "Camión registrado exitosamente.\n";
    }

    void consultarCamionPorID(sql::Connection* con, const string& matricula) {
        sql::PreparedStatement* pstmt;
        sql::ResultSet* res;

        // Consultar datos del camión por su matrícula
        pstmt = con->prepareStatement("SELECT * FROM camiones WHERE matricula = ?");
        pstmt->setString(1, matricula);

        res = pstmt->executeQuery();
        if (res->next()) {
            cout << "Datos del camión " << matricula << ":\n";
            cout << "Matrícula: " << res->getString("matricula") << "\n";
            cout << "Capacidad: " << res->getDouble("capacidad") << " kg\n";
            cout << "Consumo: " << res->getDouble("consumo") << " galones/km\n";
            cout << "Carga Actual: " << res->getDouble("cargaActual") << " kg\n";
            cout << "-----------------\n";

            // Mostrar las cargas del camión
            mostrarCargasC(con, matricula);
        }
        else {
            cout << "No se encontró ningún camión con la matrícula proporcionada.\n";
        }

        delete pstmt;
        delete res;
    }

    void mostrarCargas(sql::Connection* con) {
        // Solicitar la matrícula del camión al usuario
        cout << "Ingrese la matrícula del camión: ";
        cin >> matricula;

        mostrarCargasC(con, matricula);
    }

    void mostrarCargasC(sql::Connection* con, const string& matricula) { //PENDIENTE
        sql::PreparedStatement* pstmt;
        sql::ResultSet* res;

        pstmt = con->prepareStatement("SELECT * FROM cargas WHERE camion_matricula = ?");
        pstmt->setString(1, matricula);

        res = pstmt->executeQuery();

        if (res->next()) {
            cout << "Cargas del camión " << matricula << ":\n";
            do {
                cout << "ID: " << res->getInt("id") << "\n";
                cout << "Descripción: " << res->getString("descripcion") << "\n";
                cout << "Peso: " << res->getDouble("peso") << " kg\n";
                cout << "-----------------\n";
            } while (res->next());
        }
        else {
            cout << "El camión " << matricula << " no tiene cargas.\n";
        }

        delete pstmt;
        delete res;
    }

    void consultarCargaPorID(sql::Connection* con, int id) { //MOD
        sql::PreparedStatement* pstmt;
        sql::ResultSet* res;

        pstmt = con->prepareStatement("SELECT * FROM cargas WHERE id = ?");
        pstmt->setInt(1, id);

        res = pstmt->executeQuery();
        if (res->next()) {
            cout << "ID: " << res->getInt("id") << "\n";
            cout << "Descripción: " << res->getString("descripcion") << "\n";
            cout << "Peso: " << res->getDouble("peso") << " kg\n";
            cout << "Camión Matrícula: " << res->getString("camion_matricula") << "\n";
            //entregado
            if (res->getBoolean("entregado")) {
                cout << "La carga ha sido entregada." << "\n";
            }else cout << "La carga aún no ha sido entregada." << "\n";
            //fragilidad
            if (res->getBoolean("fragilidad")) {
                cout << "Carga frágil: Si" << "\n";
            }else cout << "Carga frágil: No" << "\n";
            // 
            //direccion 
            cout << "Dirección de entrega: " << res->getString("direccion") << "\n";
        }
        else {
            cout << "No se encontró ninguna carga con el ID proporcionado.\n";
        }

        delete pstmt;
        delete res;
    }

    void cargar(double carga, sql::Connection* con) { //MOD //PA
        cout << endl;
        mostrarMejorCamion(con, carga);
        cout << endl;
        mostrarCamiones(con);

        cout << "Ingrese la matrícula del camión que desea cargar: ";
        string matricula;
        cin >> matricula;

        sql::PreparedStatement* pstmt;
        sql::ResultSet* res;

        pstmt = con->prepareStatement("SELECT capacidad, cargaActual FROM camiones WHERE matricula = ?");
        pstmt->setString(1, matricula);

        res = pstmt->executeQuery();
        if (res->next()) {
            double capacidad = res->getDouble("capacidad");
            double cargaActual = res->getDouble("cargaActual");

            if (cargaActual + carga <= capacidad) {
                // Pregunta por la descripción de la carga
                string descripcion;
                cout << "Ingrese la descripción de la carga: ";
                cin.ignore(); // Limpiar el buffer de entrada
                getline(cin, descripcion);
                string direccion;
                cout << "Ingrese la dirección del destino de la carga: ";
                cin.ignore();
                getline(cin, direccion);
                bool fragilidad;
                cout << "Ingrese 1 si es frágil, 0 si no es frágil: ";
                cin >> fragilidad;

                pstmt = con->prepareStatement("UPDATE camiones SET cargaActual = ? WHERE matricula = ?");
                pstmt->setDouble(1, cargaActual + carga);
                pstmt->setString(2, matricula);
                pstmt->executeUpdate();
                cout << "Camión cargado exitosamente.\n";
                bool entregado = false;
                // Registrar la carga en la base de datos
                Carga nuevaCarga(0, descripcion, direccion, carga, matricula, fragilidad, entregado);
                registrarCarga(con, nuevaCarga);

                // Mostrar el ID de la carga registrada
                pstmt = con->prepareStatement("SELECT id FROM cargas WHERE camion_matricula = ? ORDER BY id DESC LIMIT 1");
                pstmt->setString(1, matricula);
                res = pstmt->executeQuery();
                if (res->next()) {
                    int idCarga = res->getInt("id");
                    cout << endl << "ID de la carga registrada: " << idCarga << endl;
                }
            }
            else {
                cout << "Error: La carga excede la capacidad del camión.\n";
            }
        }
        else {
            cout << "Error: No se encontró el camión con la matrícula proporcionada.\n";
        }

        delete pstmt;
        delete res;
    }

    void descargar(sql::Connection* con) { //PENDIENTE //CAMBIAR A ENTREGAR
        // Mostrar las cargas del camión
        mostrarCargas(con);

        cout << "Ingrese el ID de la carga que desea descargar: ";
        int idCarga;
        cin >> idCarga;

        // Verificar si la carga pertenece al camión actual
        if (verificarCargaPertenece(idCarga, con)) {
            // Obtener el peso de la carga a eliminar
            double pesoCarga = obtenerPesoCarga(idCarga, con);

            // Eliminar la carga de la base de datos
            eliminarCarga(idCarga, con);

            // Actualizar el peso actual del camión
            restarPesoCamion(pesoCarga, con);

            cout << "Carga descargada exitosamente.\n";
        }
        else {
            cout << "Error: La carga no pertenece al camión actual.\n";
        }
    }

    double obtenerPesoCarga(int id, sql::Connection* con) {
        sql::PreparedStatement* pstmt;
        sql::ResultSet* res;

        pstmt = con->prepareStatement("SELECT peso FROM cargas WHERE id = ?");
        pstmt->setInt(1, id);

        res = pstmt->executeQuery();

        double peso = 0.0;
        if (res->next()) {
            peso = res->getDouble("peso");
        }

        delete pstmt;
        delete res;

        return peso;
    }

    void restarPesoCamion(double pesoCarga, sql::Connection* con) {
        sql::PreparedStatement* pstmt;

        // Obtener el peso actual del camión
        double pesoActual = 0.0;
        pstmt = con->prepareStatement("SELECT cargaActual FROM camiones WHERE matricula = ?");
        pstmt->setString(1, matricula);

        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            pesoActual = res->getDouble("cargaActual");
        }

        delete pstmt;

        // Actualizar el peso actual del camión restando el peso de la carga eliminada
        pstmt = con->prepareStatement("UPDATE camiones SET cargaActual = ? WHERE matricula = ?");
        pstmt->setDouble(1, pesoActual - pesoCarga);
        pstmt->setString(2, matricula);
        pstmt->executeUpdate();

        delete pstmt;
        delete res;
    }

    void mostrarMejorCamion(sql::Connection* con, double carga) {
        sql::PreparedStatement* pstmt;
        sql::ResultSet* res;

        pstmt = con->prepareStatement("SELECT matricula, consumo, capacidad, cargaActual FROM camiones WHERE capacidad - cargaActual >= ? ORDER BY consumo LIMIT 1");
        pstmt->setDouble(1, carga);

        res = pstmt->executeQuery();
        if (res->next()) {
            string matricula = res->getString("matricula");
            double consumo = res->getDouble("consumo");

            cout << "El mejor camión disponible para transportar una carga de " << carga << " kg es:\n";
            cout << "Matrícula: " << matricula << "\n";
            cout << "Consumo de gasolina: " << consumo << " galones/kilómetro\n";
        }
        else {
            cout << "No hay ningún camión disponible para transportar la carga dada.\n";
        }

        delete pstmt;
        delete res;
    }



    void mostrarCamiones(sql::Connection* con) {
        sql::PreparedStatement* pstmt;
        sql::ResultSet* res;

        pstmt = con->prepareStatement("SELECT * FROM camiones");
        res = pstmt->executeQuery();

        cout << "Lista de camiones:\n";
        while (res->next()) {
            cout << "Matrícula: " << res->getString("matricula") << "\n";
            cout << "Capacidad: " << res->getDouble("capacidad") << " kg\n";
            cout << "Consumo: " << res->getDouble("consumo") << " galones/km\n";
            cout << "Carga Actual: " << res->getDouble("cargaActual") << " kg\n";
            cout << "-----------------\n";
        }

        delete pstmt;
        delete res;
    }

    void registrarCarga(sql::Connection* con, Carga carga) { //MOD
        sql::PreparedStatement* pstmt;

        pstmt = con->prepareStatement("INSERT INTO cargas (descripcion, direccion, peso, camion_matricula, fragilidad, entregado) VALUES (?, ?, ?, ?, ?, ?)");
        pstmt->setString(1, carga.descripcion);
        pstmt->setString(2, carga.direccion);
        pstmt->setDouble(3, carga.peso);
        pstmt->setString(4, carga.camionMatricula);
        pstmt->setBoolean(5, carga.fragilidad);
        pstmt->setBoolean(6, carga.entregado);

        pstmt->execute();
        delete pstmt;

        cout << "Carga registrada exitosamente.\n";
    }


    void eliminarCarga(int id, sql::Connection* con) {
        sql::PreparedStatement* pstmt;

        pstmt = con->prepareStatement("DELETE FROM cargas WHERE id = ?");
        pstmt->setInt(1, id);
        pstmt->execute();

        delete pstmt;
    }

    bool verificarCargaPertenece(int id, sql::Connection* con) {
        sql::PreparedStatement* pstmt;
        sql::ResultSet* res;

        pstmt = con->prepareStatement("SELECT * FROM cargas WHERE id = ? AND camion_matricula = ?");
        pstmt->setInt(1, id);
        pstmt->setString(2, matricula);

        res = pstmt->executeQuery();
        bool pertenece = res->next();

        delete pstmt;
        delete res;

        return pertenece;
    }

};

sql::Connection* conectar() {
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "HeBro065$");
    con->setSchema("transporte");

    return con;
}

int main() {
    sql::Connection* con = conectar();

    Camion camion("", 0.0, 0.0, 0.0);

    string matricula;
    double carga;

    int opcion;
    do {
        cout << "--------------Menú Principal----------------" << endl;
        cout << "1. Registrar camión" << endl;
        cout << "2. Consultar datos de un camión" << endl;
        cout << "3. Cargar camión" << endl;
        cout << "4. Descargar camión" << endl;
        cout << "5. Mostrar camiones disponibles" << endl;
        cout << "6. Mostrar todas las cargas del camión" << endl;
        cout << "7. Consultar carga por ID" << endl;
        cout << "8. Salir" << endl;
        cout << "Ingrese su opción: ";
        cin >> opcion;

        cout << endl;

        switch (opcion) {
        case 1:
            camion.registrarCamion(con);
            break;
        case 2:
            // Solicitar la matrícula del camión al usuario
            cout << "Ingrese la matrícula del camión: ";
            cin >> matricula;
            camion.consultarCamionPorID(con, matricula);
            break;
        case 3:
            cout << "Ingrese la carga a transportar (kg): ";
            cin >> carga;
            camion.cargar(carga, con);
            break;
        case 4:
            camion.descargar(con);
            break;
        case 5:
            camion.mostrarCamiones(con);
            break;
        case 6:
            camion.mostrarCargas(con);
            break;
        case 7:
            int id;
            cout << "Ingrese el ID de la carga a consultar: ";
            cin >> id;
            camion.consultarCargaPorID(con, id);
            break;
        case 8:
            break;
        default:
            cout << "Opción no válida. Intente de nuevo.\n";
            break;
        }
        cout << endl;
        if (opcion != 8) {
            system("PAUSE");
        }
        system("cls");
    } while (opcion != 8);

    delete con;
    cout << "Gracias por usar este programa, ten un excelente día" << endl;
    return 0;
}