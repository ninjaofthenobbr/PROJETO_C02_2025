#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

string obterData() {
    time_t agora = time(0);
    struct tm* tempo = localtime(&agora);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", tempo);
    return string(buffer);
}

int separar(string linha, char delim, string partes[], int maxPartes) {
    string parte = "";
    int count = 0;
    for (size_t i = 0; i < linha.length(); i++) {
        if (linha[i] == delim && count < maxPartes - 1) {
            partes[count++] = parte;
            parte = "";
        } else {
            parte += linha[i];
        }
    }
    partes[count++] = parte;
    return count;
}

double stringParaDouble(string s) {
    double resultado = 0.0;
    double multiplicador = 0.1;
    bool aposVirgula = false;

    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '.' || s[i] == ',') {
            aposVirgula = true;
            multiplicador = 0.1;
        } else if (s[i] >= '0' && s[i] <= '9') {
            int digito = s[i] - '0';
            if (!aposVirgula) {
                resultado = resultado * 10.0 + digito;
            } else {
                resultado = resultado + (digito * multiplicador);
                multiplicador *= 0.1;
            }
        }
    }
    return resultado;
}

int stringParaInt(string s) {
    int resultado = 0;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            resultado = resultado * 10 + (s[i] - '0');
        }
    }
    return resultado;
}

void adicionarGasto() {
    double valor;
    string descricao;

    cout << endl << "=====================================" << endl;
    cout << "          ADICIONAR GASTO" << endl;
    cout << "=====================================" << endl << endl;

    cout << "Valor do gasto (R$): ";
    cin >> valor;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "[ERRO] Valor invalido!" << endl;
        return;
    }

    cin.ignore(10000, '\n');

    cout << "Descricao: ";
    getline(cin, descricao);

    if (descricao.empty()) {
        cout << "[ERRO] Descricao nao pode ser vazia!" << endl;
        return;
    }

    ofstream arquivo("gastos.txt", ios::app);
    arquivo << obterData() << ":" << valor << ":" << descricao << endl;
    arquivo.close();

    cout << endl << "[SUCESSO] Gasto registrado!" << endl;
}

void definirGanho() {
    double ganho;
    int dia;

    cout << endl << "=====================================" << endl;
    cout << "        DEFINIR GANHO MENSAL" << endl;
    cout << "=====================================" << endl << endl;

    cout << "Ganho mensal (R$): ";
    cin >> ganho;

    if (cin.fail() || ganho < 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "[ERRO] Ganho invalido!" << endl;
        return;
    }

    cout << "Dia do recebimento (1-31): ";
    cin >> dia;

    if (cin.fail() || dia < 1 || dia > 31) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "[ERRO] Dia invalido!" << endl;
        return;
    }

    cin.ignore(10000, '\n');

    ofstream arquivo("ganho.txt");
    arquivo << ganho << ":" << dia;
    arquivo.close();

    cout << endl << "[SUCESSO] Ganho salvo!" << endl;
}

void verGastos() {
    cout << endl << "===================================================" << endl;
    cout << "               RESUMO FINANCEIRO" << endl;
    cout << "===================================================" << endl << endl;

    ifstream arquivo("gastos.txt");

    if (!arquivo.is_open()) {
        cout << "[INFO] Nenhum gasto registrado." << endl;
        return;
    }

    string linha;
    double total = 0;
    int contador = 0;

    cout << "Data          Valor(R$)    Descricao" << endl;
    cout << "----------------------------------------" << endl;

    string partes[10];

    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        int n = separar(linha, ':', partes, 10);

        if (n >= 3) {
            double valor = stringParaDouble(partes[1]);
            string desc = partes[2];

            if (desc.length() > 18)
                desc = desc.substr(0, 18);

        
            cout << partes[0] << "    " << fixed << setprecision(2) << valor << "    " << desc << endl;

            total += valor;
            contador++;
        }
    }

    cout << endl;
    arquivo.close();

    ifstream ganhoArq("ganho.txt");
    double ganho = 0;

    if (ganhoArq.is_open()) {
        string gLinha;
        getline(ganhoArq, gLinha);

        int n = separar(gLinha, ':', partes, 10);

        if (n >= 1) {
            ganho = stringParaDouble(partes[0]);
        }
        ganhoArq.close();
    }

    double saldo = ganho - total;

    cout << "Ganho Mensal:    R$ " << fixed << setprecision(2) << ganho << endl;
    cout << "Total de Gastos: R$ " << fixed << setprecision(2) << total << endl;
    cout << "Qtd de Gastos:   " << contador << endl;

    if (saldo >= 0)
        cout << "[SALDO POSITIVO] R$ " << fixed << setprecision(2) << saldo << endl << endl;
    else
        cout << "[SALDO NEGATIVO] -R$ " << fixed << setprecision(2) << -saldo << endl << endl;
}

void menu() {
    int opcao = 0;

    while (opcao != 4) {
        cout << endl << "===================================================" << endl;
        cout << "           ORGANIZADOR FINANCEIRO" << endl;
        cout << "===================================================" << endl;
        cout << "  1. Definir Ganho Mensal" << endl;
        cout << "  2. Adicionar Gasto" << endl;
        cout << "  3. Ver Gastos e Saldo" << endl;
        cout << "  4. Sair" << endl;
        cout << "===================================================" << endl;
        cout << "Escolha uma opcao (1-4): ";

        cin >> opcao;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[ERRO] Digite um numero valido!" << endl;
            continue;
        }

        cin.ignore(10000, '\n');

        switch (opcao) {
            case 1: definirGanho(); break;
            case 2: adicionarGasto(); break;
            case 3: verGastos(); break;
            case 4: cout << "[SAIDA] Ate logo!" << endl; break;
            default: cout << "[ERRO] Opcao invalida." << endl;
        }
    }
}

int main() {
    menu();
    return 0;
}
