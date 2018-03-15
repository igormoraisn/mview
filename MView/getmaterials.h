#ifndef GETMATERIALS_H
#define GETMATERIALS_H

#include <string>
#include <vector>

class GetMaterials
{
public:
    GetMaterials(std::string n_path);
    void getFiles();
    std::vector<std::string> getMaterials();
    std::string basename(std::string x);
private:
    std::string path;
    std::vector<std::string> materials;
};

#endif // GETMATERIALS_H
