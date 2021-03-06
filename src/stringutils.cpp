#include "stringutils.h"

#include <sstream>
#include <algorithm>

namespace stringutils
{
	std::vector<std::string> split(const std::string& s, char delim)
	{
			std::vector<std::string> elems;
			std::stringstream ss(s);
			std::string item;
			while (std::getline(ss, item, delim)) 
			{
					elems.push_back(item);
			}
			return elems;
	}

  std::string trim(const std::string& s, char c)
  {
    auto b = s.begin();
    auto e = s.end();
    e--;
    while(*b == c) b++;
    while(*e == c) e--;
    if(e++ >= b) return std::string(b, e);
    return "";
  }

  std::string lower(const std::string& s)
  {
    std::string result(s);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
  }

  std::string upper(const std::string& s)
  {
    std::string result(s);
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
  }

  void splitPath(const std::string& path, std::string& dirname, std::string& filename)
  {
    auto delimiter = path.rfind("/");
    if(delimiter == path.npos) {
      delimiter = path.rfind("\\");
    }
    if(delimiter != path.npos) {
      delimiter++;
      dirname = path.substr(0, delimiter);
      filename = path.substr(delimiter);
    } else {
      dirname = "./";
      filename = path;
    }
  }
}
