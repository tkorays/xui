#include "Language.h"
#include <iostream>
using namespace xui;
using namespace std;

class RawLanguageLoader : public ILanguageLoader {
public:
    bool LoadAll(Language& lang, const std::string& name, const std::string& path)
    {
        if (path.compare("/cn") == 0)
        {
            lang.SetName(name);
            lang.AddItem("HELLO", "hello");
            lang.AddItem("WELCOME", "welcome");
            lang.AddItem("FUCK", "fuck");
        }
        else if (path.compare("/en") == 0)
        {
            lang.SetName(name);
            lang.AddItem("HELLO", "ƒ„∫√");
            lang.AddItem("WELCOME", "ª∂”≠π‚¡Ÿ");
        }
        return true;
    }
};

int main() {
    LanguageMgr mgr;
    mgr.InstallLanguage<RawLanguageLoader>("cn", "/cn");
    mgr.InstallLanguage<RawLanguageLoader>("en", "/en");
    mgr.SetDefLanguage("cn");
    mgr.SetCurLanguage("en");
    cout << mgr["HELLO"] << endl;
    cout << mgr["WELCOME"] << endl;
    cout << mgr["FUCK"] << endl;
    return 0;
}