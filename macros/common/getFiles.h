#include <boost/filesystem.hpp>
#include <iterator>
#include <list>
#include <string>
#include <type_traits>

/// Takes a iterator over something which can be converted into a const
/// std::string& and pushes the filename onto a list.
/// if a directory name is given it also adds all the files included in said
/// directory.
// TODO: add recursive flag
template <class I> std::list<std::string> getFiles(I begin, I end) {
  static_assert(
      std::is_convertible<typename std::iterator_traits<I>::value_type,
                          const std::string &>::value,
      "getFiles requires an iterator that can be converted to const char*");
  std::list<std::string> chain;
  for (auto it = begin; it != end; it++) {
    boost::filesystem::path p(*it);
    if (is_directory(p)) {
      for (auto i = boost::filesystem::directory_iterator(p);
           i != boost::filesystem::directory_iterator(); i++) {
        if (!is_directory(i->path())) // we eliminate directories
        {
          std::string new_name = p.string();
          chain.push_back(new_name.append(i->path().filename().string()));
        } else
          continue;
      }
    } else {
      chain.push_back(*it);
    }
  }
  return chain;
}

template <class I> std::list<std::string> getFiles(I structure, size_t size) {
  return getFiles(structure, structure + size);
}

std::list<std::string> getFiles(const std::string &file);
