#include "getFiles.h"

std::list<std::string> getFiles(const std::string &file) {
  return getFiles(&file, 1);
}
