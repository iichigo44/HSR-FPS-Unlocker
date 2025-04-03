#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

bool ReplaceFPSInRegBinary() {
    HKEY hKey;
    const wchar_t* subKey = L"SOFTWARE\\Cognosphere\\Star Rail";
    const wchar_t* valueName = L"GraphicsSettings_Model_h2986158309";

    // Abrir la clave del Registro
    if (RegOpenKeyExW(HKEY_CURRENT_USER, subKey, 0, KEY_READ | KEY_WRITE, &hKey) != ERROR_SUCCESS) {
        std::wcerr << L"Error: Couldn't open registry key.\n";
        return false;
    }

    // Obtener el tamaño del valor REG_BINARY
    DWORD dataSize = 0;
    if (RegQueryValueExW(hKey, valueName, 0, NULL, NULL, &dataSize) != ERROR_SUCCESS) {
        std::wcerr << L"Error: Couldn't get value size.\n";
        RegCloseKey(hKey);
        return false;
    }

    // Leer el valor REG_BINARY
    std::vector<BYTE> data(dataSize);
    if (RegQueryValueExW(hKey, valueName, 0, NULL, data.data(), &dataSize) != ERROR_SUCCESS) {
        std::wcerr << L"Error: Couldn't read REG_BINARY value.\n";
        RegCloseKey(hKey);
        return false;
    }

    // Convertir los datos a una cadena de texto
    std::string binaryText(reinterpret_cast<char*>(data.data()), dataSize);

    // Buscar y reemplazar "FPS":X por "FPS":120 (donde X es cualquier número)
    std::regex fpsRegex(R"("FPS":\d+)");  // Expresión regular para detectar "FPS":X
    std::string replacedText = std::regex_replace(binaryText, fpsRegex, "\"FPS\":120");

    // Verificar si hubo cambios
    if (binaryText == replacedText) {
        std::wcout << L"Error: Didn't find any value to change or the value is already set to 120FPS\n";
        RegCloseKey(hKey);
        return false;
    }

    // Convertir el texto modificado de vuelta a datos binarios
    std::vector<BYTE> newData(replacedText.begin(), replacedText.end());

    // Escribir el valor modificado en el Registro
    if (RegSetValueExW(hKey, valueName, 0, REG_BINARY, newData.data(), newData.size()) != ERROR_SUCCESS) {
        std::wcerr << L"Error: Cannot change the value in regedit...\n";
        RegCloseKey(hKey);
        return false;
    }

    // Cerrar la clave del Registro
    RegCloseKey(hKey);
    std::wcout << L"FPS Set to 120 successfully\n";
    return true;
}

int main() {
    if (ReplaceFPSInRegBinary()) {
        std::wcout << L"Done.\n";
        std::wcout << L"Press enter to close this window.\n";
    }
    else {
        std::wcerr << L"There was an error during the process...\n";
        std::wcerr << L"Press enter to close this window.\n";
    }

    std::wcin.get();


    return 0;


}
