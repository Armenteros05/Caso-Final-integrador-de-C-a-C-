#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct ColorConsole
{
    static constexpr auto fg_blue = "\033[34m";
    static constexpr auto bg_white = "\033[47m";
};

struct ConsoleBox
{
    void new_text() {/*...*/}
    void set_text(const string &text) { cout << text << endl; }
};

ConsoleBox *consoleBox = new ConsoleBox; // suponemos que ya está inicializado

void load_script(const string &filename, bool show_script = false)
{
    string script;
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cerr << "Error de apertura de " << filename << endl;
        return;
    }

    char buf[4001];
    while (file.read(buf, sizeof(buf) - 1))
    {
        buf[file.gcount()] = '\0';
        script.append(buf);
    }

    file.close();

    if (show_script)
    {
        cout << ColorConsole::fg_blue << ColorConsole::bg_white;
        cout << script << endl;
    }
    consoleBox->new_text();
    consoleBox->set_text(script);
}

void add_script()
{
    string filename, content;
    cout << "Nombre del nuevo archivo: ";
    cin >> filename;

    cout << "Contenido del nuevo archivo (termina con EOF/CTRL+D en Unix, CTRL+Z en Windows):" << endl;
    cin.ignore(); // Ignora el salto de línea pendiente del input anterior
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
        cerr << "Error al crear el archivo '" << filename << "'" << endl;
    }
}

int main()
{
    cout << "Introduce el nombre del archivo a cargar: ";
    string filename;
    cin >> filename;
    load_script(filename, true);
    return 0;
}
