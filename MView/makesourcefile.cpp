#include "makesourcefile.h"
#include <iostream>
#include <fstream>

MakeSourceFile::MakeSourceFile(std::string material) {
   text = "LookAt 0 5 10   0 -1 0 0 1 0\n";
   text += "Camera \"perspective\" \"float fov\" [11]\n";
   text += "PixelFilter \"mitchell\" \"float xwidth\" [4] \"float ywidth\" [4]\n";
   text += "Sampler \"bestcandidate\"\n";
   text += "Film \"image\" \"string filename\" [\"simple.exr\"]\n";
   text +=     "\"integer xresolution\" [440] \"integer yresolution\" [400]\n";

   text += "WorldBegin\n";
   text += "AttributeBegin\n";
   text += "CoordSysTransform \"camera\"\n";
   text += "LightSource \"distant\"\n";
   text +=               "\"point from\" [0 0 0] \"point to\"   [0 0 1]\n";
   text +=               "\"rgb L\"    [10 10 10]\n";
   text += "LightSource \"distant\"\n";
   text +=               "\"point from\" [1 4 0] \"point to\"   [-1 2 1]\n";
   text +=               "\"rgb L\"    [10 10 10]\n";
   text += "LightSource \"distant\"\n";
   text +=               "\"point from\" [-4 -2 0] \"point to\"   [2 4 -16]\n";
   text +=               "\"rgb L\"    [10 10 10]\n";
   text += "AttributeEnd\n";
   text += "AttributeBegin\n";
   text += "Translate 0 -1 0\n";
   text += "Material \"measured\" \"string filename\" ";
   text += "\"/home/igor/MView/New/" + material + ".binary\"\n";
   text += "Shape \"sphere\"\n";
   text += "AttributeEnd\n";
   text += "WorldEnd\n";
}

void MakeSourceFile::createFile(){
    std::ofstream pbrtFile;
    pbrtFile.open("sample.pbrt", std::ios::trunc);
    if (pbrtFile.is_open()) {
        pbrtFile << text;
    }
    else
       std::cout << "Erro ao abrir arquivo de texto.";
    pbrtFile.close();
}
