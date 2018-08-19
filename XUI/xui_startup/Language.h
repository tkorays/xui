#pragma once
#include <string>
#include <map>


namespace xui {
    class ILanguageLoader;

    class Language {
    public:
        Language();
        ~Language();

        void AddItem(const std::string& id, const std::string& content);
        std::string GetItem(const std::string& id);
        void SetName(const std::string& name);
        std::string GetName() const;
    private:
        std::string m_name;
        std::map<std::string, std::string> m_strMap;
    };

    class ILanguageLoader {
    public:
        virtual bool LoadAll(Language& lang, const std::string& name, const std::string& path) = 0;
    };

    class LanguageMgr {
    public:
        LanguageMgr();
        ~LanguageMgr();

        template <class T> bool InstallLanguage(const std::string& name, const std::string& path);
        void SetCurLanguage(const std::string& name);
        void SetDefLanguage(const std::string& name);
        std::string tr(const std::string& id);
        std::string operator[](const std::string& id);
    private:
        std::map<std::string, Language> m_langs;
        std::string m_sDefLang;
        std::string m_sCurLang;
    };

    template<class T>
    inline bool LanguageMgr::InstallLanguage(const std::string & name, const std::string & path)
    {
        Language lang;
        T loader;
        bool ret = loader.LoadAll(lang, name, path);
        if (ret)
        {
            m_langs[name] = lang;
        }

        if (m_sDefLang.empty()) m_sDefLang = name;
        return ret;
    }

}
