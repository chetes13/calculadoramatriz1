#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <Eigen/Dense>
#include <fstream>
#include <sstream> // Para std::stringstream

// Utiliza Eigen para todas las operaciones matriciales
using Eigen::MatrixXd;

struct HistorialOperacion {
  Eigen::MatrixXd matriz1;
  Eigen::MatrixXd matriz2;
  Eigen::MatrixXd resultado;
  int operacion;
};


// Función para pedir números al usuario
double pedir_numero() {
  double numero{ 0 };
  do {
    if (std::cin >> numero && numero != 0 && std::cin.peek() == '\n') {
      return numero;
    }
    else {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Ingresa solamente un numero valido por favor." << std::endl;
    }
  } while (true);
}

// Funcion para declaracion de matrices
Eigen::MatrixXd declarar_matriz(const std::string& nombre_matriz) {
  // Definir dimensiones
  int filas, columnas;
  std::cout << "Cuantas filas tiene la matriz " << nombre_matriz << "?" << std::endl;
  filas = static_cast<int>(pedir_numero());

  std::cout << "Cuantas columnas tiene la matriz " << nombre_matriz << "?" << std::endl;
  columnas = static_cast<int>(pedir_numero());

  // Crear una matriz de Eigen
  Eigen::MatrixXd matriz(filas, columnas);

  // Declarar valores a la matriz
  std::cout << "\n";
  for (int i = 0; i < filas; i++) {
    for (int j = 0; j < columnas; j++) {
      std::cout << "Ingresa el valor [" << i + 1 << "][" << j + 1 << "]:" << std::endl;
      matriz(i, j) = pedir_numero();
    }
  }

  // Mostrar los valores con los que se declaró
  std::cout << "\nLa matriz " << nombre_matriz << " se ha declarado con los valores: " << std::endl;
  std::cout << matriz << std::endl;

  return matriz;
}

// validar dimensiones para realizar operaciones
bool validar_dimensiones(const Eigen::MatrixXd& matriz1, const Eigen::MatrixXd& matriz2, int operacion) {
  int filas1 = matriz1.rows();
  int columnas1 = matriz1.cols();

  int filas2 = matriz2.rows();
  int columnas2 = matriz2.cols();

  switch (operacion) {
  case 1: // Suma
  case 2: // Resta
    // Deben tener exactamente las mismas dimensiones
    return filas1 == filas2 && columnas1 == columnas2;

  case 3: // Multiplicación
    // El número de columnas de la primera debe ser igual al número de filas de la segunda
    return columnas1 == filas2;

  case 4: // División
    // Ambas matrices deben ser cuadradas y del mismo tamaño
    return filas1 == columnas1 && filas2 == columnas2 && filas1 == filas2;

  default:
    return false;
  }
}

// Funcion de guardado en el archivo la matriz1, matriz2, operacion y matriz resultado
void guardar_operacion(const Eigen::MatrixXd& matriz1, const Eigen::MatrixXd& matriz2, const Eigen::MatrixXd& resultado, const int& operacion) {
  std::ofstream archivo("db_matrices.txt", std::ios::app); // Abre el archivo en modo de anexar

  if (!archivo) {
    std::cerr << "Error al abrir el archivo." << std::endl;
    return;
  }

  archivo << "Operación: " << operacion << "\n";
  archivo << "Matriz 1:\n" << matriz1 << "\n\n";
  archivo << "Matriz 2:\n" << matriz2 << "\n\n";
  archivo << "Resultado:\n" << resultado << "\n";
  archivo << "-----------------------------------\n";

  archivo.close();
}


// Suponiendo que la estructura HistorialOperacion ya está definida
Eigen::MatrixXd leer_matriz(std::ifstream& archivo) {
  std::string linea;
  std::vector<std::vector<double>> valores;

  while (std::getline(archivo, linea) && !linea.empty() && linea != "-----------------------------------") {
    std::stringstream ss(linea);
    std::vector<double> fila;
    double valor;
    while (ss >> valor) {
      fila.push_back(valor);
    }
    valores.push_back(fila);
  }

  Eigen::MatrixXd matriz(valores.size(), valores[0].size());
  for (size_t i = 0; i < valores.size(); ++i) {
    for (size_t j = 0; j < valores[i].size(); ++j) {
      matriz(i, j) = valores[i][j];
    }
  }

  return matriz;
}

void mostrar_historial() {
  std::ifstream archivo("db_matrices.txt");
  if (!archivo) {
    std::cerr << "Error al abrir el archivo de historial." << std::endl;
    return;
  }

  std::string linea;
  while (std::getline(archivo, linea)) {
    if (linea.find("Operación:") != std::string::npos) {
      int operacion = std::stoi(linea.substr(linea.find(":") + 1));

      std::getline(archivo, linea); // Leer "Matriz 1:"
      Eigen::MatrixXd matriz1 = leer_matriz(archivo);

      std::getline(archivo, linea); // Leer "Matriz 2:"
      Eigen::MatrixXd matriz2 = leer_matriz(archivo);

      std::getline(archivo, linea); // Leer "Resultado:"
      Eigen::MatrixXd resultado = leer_matriz(archivo);

      // Aquí puedes almacenar la operación en una lista de HistorialOperacion
      // O simplemente imprimir la información
      std::cout << "Operación: " << operacion << "\n";
      std::cout << "Matriz 1:\n" << matriz1 << "\n\n";
      std::cout << "Matriz 2:\n" << matriz2 << "\n\n";
      std::cout << "Resultado:\n" << resultado << "\n";
      std::cout << "-----------------------------------\n\n";
    }
  }

  archivo.close();
}


// ~~~~~ Operaciones usando Eigen
Eigen::MatrixXd suma_matrices(const Eigen::MatrixXd& matriz1, const Eigen::MatrixXd& matriz2) {
  std::cout << "\nProceso de suma:" << std::endl;
  Eigen::MatrixXd resultado = matriz1 + matriz2;
  std::cout << resultado << std::endl;
  return resultado;
}

Eigen::MatrixXd resta_matrices(const Eigen::MatrixXd& matriz1, const Eigen::MatrixXd& matriz2) {
  std::cout << "\nProceso de resta:" << std::endl;
  Eigen::MatrixXd resultado = matriz1 - matriz2;
  std::cout << resultado << std::endl;
  return resultado;
}

Eigen::MatrixXd multiplicacion_matrices(const Eigen::MatrixXd& matriz1, const Eigen::MatrixXd& matriz2) {
  std::cout << "\nProceso de multiplicacion:" << std::endl;
  Eigen::MatrixXd resultado = matriz1 * matriz2;
  std::cout << resultado << std::endl;
  return resultado;
}

Eigen::MatrixXd division_matrices(const Eigen::MatrixXd& matriz1, const Eigen::MatrixXd& matriz2) {
  if (matriz2.determinant() == 0) {
    std::cerr << "Error: La segunda matriz no es invertible. La operación de división no puede ser concretada." << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << "\nProceso de division:" << std::endl;
  Eigen::MatrixXd resultado = matriz1 * matriz2.inverse();
  std::cout << resultado << std::endl;
  return resultado;
}


int main() {
  // Definicion de variables generales
  int operacion_usuario;

  // Establecer la precisión por defecto a 2 dígitos después del punto decimal
  std::cout << std::fixed << std::setprecision(2);

  // Definir las matrices con las que vamos a trabajar
  // Nota: Aquí podrías necesitar adaptar la forma en la que pides las matrices al usuario
  // para convertirlas directamente en Eigen::MatrixXd en lugar de std::vector<std::vector<double>>
  Eigen::MatrixXd matriz1 = declarar_matriz("primera");
  Eigen::MatrixXd matriz2 = declarar_matriz("segunda");

  Eigen::MatrixXd resultado;
  bool valido;

  do {
    std::cout << "\nSeleccione la operacion a realizar:\n[1] Suma\n[2] Resta\n[3] Multiplicación\n[4] División\n[5] Salir de la aplicación" << std::endl;

    operacion_usuario = pedir_numero();
    switch (operacion_usuario) {
    case 1: { // SUMA

      valido = validar_dimensiones(matriz1, matriz2, 1);

      if (!valido) {
        std::cout << "La operación no es valida, selecciona otra por favor" << std::endl;
        break;
      }

      resultado = suma_matrices(matriz1, matriz2);
      guardar_operacion(matriz1, matriz2, resultado, operacion_usuario);
      break;
    }
    case 2: { // RESTA
      valido = validar_dimensiones(matriz1, matriz2, 2);
      if (!valido) {
        std::cout << "La operación no es valida, selecciona otra por favor" << std::endl;
        break;
      }
      resultado = resta_matrices(matriz1, matriz2);
      guardar_operacion(matriz1, matriz2, resultado, operacion_usuario);
      break;
    }
    case 3: { // MULTIPLICACION
      valido = validar_dimensiones(matriz1, matriz2, 3);
      if (!valido) {
        std::cout << "La operación no es valida, selecciona otra por favor" << std::endl;
        break;
      }
      resultado = multiplicacion_matrices(matriz1, matriz2);
      guardar_operacion(matriz1, matriz2, resultado, operacion_usuario);
      break;
    }
    case 4: { // Division
      valido = validar_dimensiones(matriz1, matriz2, 4);
      if (!valido) {
        std::cout << "La operación no es valida, selecciona otra por favor" << std::endl;
        break;
      }
      resultado = division_matrices(matriz1, matriz2);
      guardar_operacion(matriz1, matriz2, resultado, operacion_usuario);
      break;
    }
    case 5: {
      std::cout << "Nos vemos pronto :)" << std::endl;
      return 0;
    }
    case 99: {
      mostrar_historial();
      break;}
    default:
      std::cout << "Selecciona una de las operaciones disponibles por favor" << std::endl;
    }

    if (operacion_usuario >= 1 && operacion_usuario <= 4) {
      std::cout << "\nMatriz resultado:\n" << std::endl;
      std::cout << resultado << std::endl;
    }

  } while (true);

  return 0;
}

