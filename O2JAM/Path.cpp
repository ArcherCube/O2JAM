#include "Path.h"
#include "CommonType.h"

const String Path::currentPath = std::filesystem::current_path();
const String Path::modelPath = Path::currentPath + TEXT("\\src\\data\\");
const String Path::musicPath = Path::currentPath + TEXT("\\src\\music\\");
const String Path::melodyPath = Path::currentPath + TEXT("\\src\\melody\\");