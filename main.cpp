#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

struct ConsoleBox
{
    void new_text() {/*...*/}
    void set_text(const string &text) { cout << text << endl; }
};

ConsoleBox *consoleBox = new ConsoleBox; // suponemos que ya está inicializado

bool file_exists(const string &filename)
{
    ifstream file(filename);
    return file.good();
}

bool load_script(const char* filename, bool show_script = false)
{
    if (!file_exists(filename))
    {
        cerr << "Error: El archivo '" << filename << "' no existe." << endl;
        return false;
    }

    string script;
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo '" << filename << "'. Comprueba que tengas permisos para leer el archivo." << endl;
        return false;
    }

    char buf[4001];
    while (file.read(buf, sizeof(buf) - 1))
    {
        buf[file.gcount()] = '\0';
        script.append(buf);
    }

    file.close();

    if (file.fail() && !file.eof())
    {
        cerr << "Error: Fallo durante la lectura del archivo '" << filename << "'. Asegúrate de que el archivo no esté dañado." << endl;
        return false;
    }

    if (show_script)
    {
        cout << "Contenido del archivo '" << filename << "':" << endl;
        cout << script << endl;
    }
    consoleBox->new_text();

    cout << "Archivo '" << filename << "' leído exitosamente." << endl;
    return true;  // Indicar éxito en la carga del archivo
}

void add_script()
{
    string filename, content;
    cout << "Nombre del nuevo archivo: ";
    cin.ignore(); // Ignora el salto de línea pendiente del input anterior
    getline(cin, filename);

    if (file_exists(filename))
    {
        cerr << "Error: El archivo '" << filename << "' ya existe. Utiliza un nombre diferente." << endl;
        return;
    }

    cout << "Contenido del nuevo archivo (termina con EOF/CTRL+D en Unix, CTRL+Z en Windows):" << endl;
    getline(cin, content, '\0');

    ofstream file(filename, ios::binary);
    if (file.is_open())
    {
        file << content;
        file.close();
        cout << "Archivo '" << filename << "' creado exitosamente." << endl;
    }
    else
    {
        cerr << "Error al crear el archivo '" << filename << "'. Comprueba que tengas permisos para escribir en la ubicación especificada." << endl;
    }
}

int main()
{
    cout << "Introduce el nombre del archivo a cargar: ";
    string filename;
    getline(cin, filename);

    // Modificado para usar la bandera de error
    load_script(filename.c_str(), true);

    return 0;
}
