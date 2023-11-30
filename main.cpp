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
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo '" << filename << "'. Comprueba que tengas permisos para leer el archivo." << endl;
        return false;
    }

    string script;
    char buf[4001];
    while (file.read(buf, sizeof(buf) - 1))
    {
        buf[file.gcount()] = '\0';
        script.append(buf);
    }

    file.close();

    if (file.fail() && !file.eof())
    {
        cerr << "Error: Fallo durante la lectura del archivo '" << filename << "'. Asegurate de que el archivo no este dañado." << endl;
        return false;
    }

    if (show_script)
    {
        cout << "Contenido del archivo '" << filename << "':" << endl;
        cout << script << endl;
    }
    consoleBox->new_text();

    cout << "Archivo '" << filename << "' leido exitosamente." << endl;
    return true;  // Indicar éxito en la carga del archivo
}

void save_script(const char* filename, const string &content)
{
    ofstream file(filename, ios::binary);
    if (file.is_open())
    {
        file << content;
        file.close();
        cout << "Archivo '" << filename << "' guardado exitosamente." << endl;
    }
    else
    {
        cerr << "Error al guardar el archivo '" << filename << "'. Comprueba que tengas permisos para escribir en la ubicacion especificada." << endl;
    }
}

void add_script()
{
    string filename, content;
    cout << "Nombre del nuevo archivo (o 'exit' para salir): ";
    getline(cin, filename);

    if (filename == "exit")
        return;

    if (file_exists(filename))
    {
        cerr << "Error: El archivo '" << filename << "' ya existe. Utiliza un nombre diferente." << endl;
        return;
    }

    cout << "Contenido del nuevo archivo (termina con Enter para guardar y salir):" << endl;
    getline(cin, content);

    save_script(filename.c_str(), content);
}

int main()
{
    while (true)
    {
        cout << "Introduce el nombre del archivo a cargar (o 'exit' para salir o 'new' para crear uno nuevo): ";
        string filename;
        getline(cin, filename);

        if (filename == "exit")
            break;
        else if (filename == "new")
        {
            add_script();
            continue;
        }

        while (!file_exists(filename))
        {
            cerr << "Error: El archivo '" << filename << "' no existe. Intentalo de nuevo (o 'new' para crear uno nuevo): ";
            getline(cin, filename);

            if (filename == "exit")
                return 0;
            else if (filename == "new")
            {
                add_script();
                break;
            }
        }

        if (load_script(filename.c_str(), true))
        {
            // Lee el contenido del archivo y lo imprime
            ifstream file(filename);
            if (file.is_open())
            {
                string line;
                cout << "\nContenido del archivo despues de cargar:" << endl;
                while (getline(file, line))
                {
                    cout << line << endl;
                }
                file.close();
            }
        }
    }

    return 0;
}
