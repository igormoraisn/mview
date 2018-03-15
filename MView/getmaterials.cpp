#include "getmaterials.h"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <algorithm>

GetMaterials::GetMaterials(std::string n_path) {
    this->path = n_path;
}

void GetMaterials::getFiles(){
    DIR* dirp = opendir(this->path.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        std::string aux = dp->d_name;
        if(aux.compare(".") != 0 && aux.compare("..") != 0){
            materials.push_back(basename(aux));
        }
    }
    closedir(dirp);
    std::sort(materials.begin(), materials.end());
}

std::vector<std::string> GetMaterials::getMaterials(){
    return this->materials;
}
std::string GetMaterials::basename(std::string primary){
    primary.erase(primary.end()-7, primary.end());
    return primary;
}
