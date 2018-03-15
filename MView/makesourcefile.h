#ifndef MAKESOURCEFILE_H
#define MAKESOURCEFILE_H

#include <string>

class MakeSourceFile
{
public:
    MakeSourceFile(std::string material);
    void createFile();
private:
    std::string text;
};

#endif // MAKESOURCEFILE_H
