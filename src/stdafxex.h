


#include <vector>
#include <set>
#include <string>
using std::string;
using std::wstring;
namespace std
{
    typedef std::basic_string<TCHAR> tstring;
}
using std::tstring;

typedef std::vector<std::tstring> vectstring;
typedef std::set<std::tstring> settstring;

#include <map>
typedef std::map<std::tstring, std::tstring> maptstring;