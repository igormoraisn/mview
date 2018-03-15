#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string.h>
#include "makesourcefile.h"
#include <fstream>

using namespace std;

class Connection{
public:
    static std::string itos(int i) {
        stringstream s;
        s << i;
        return s.str();
    }
    static void makeConnection(std::string material){
        MakeSourceFile file(material);
        file.createFile();
        system("pbrt-v2/src/bin/pbrt sample.pbrt");
    }
    static void renderizer(int fst, int snd){
        std::ofstream trianFile;
        trianFile.open("trian.sh", std::ios::trunc);
        if (trianFile.is_open()) {
            trianFile << "cd /home/igor/MView\n";
            trianFile << "python trian.py " << itos(fst) << " " << itos(snd);
        }
        else
           std::cout << "Erro ao abrir arquivo de texto.";
        trianFile.close();
        system("chmod +x trian.sh");
        system("./trian.sh");
    }
};
