#include "Language.h"

xui::Language::Language()
{
}

xui::Language::~Language()
{
}

void xui::Language::AddItem(const std::string & id, const std::string & content)
{
    if (!id.empty())
    {
        m_strMap[id] = content;
    }
}

std::string xui::Language::GetItem(const std::string& id)
{
    auto it = m_strMap.find(id);
    if (it != m_strMap.end())
    {
        return it->second;
    }
    return "";
}

void xui::Language::SetName(const std::string& name)
{
    m_name = name;
}

std::string xui::Language::GetName() const
{
    return m_name;
}

xui::LanguageMgr::LanguageMgr()
{
}

xui::LanguageMgr::~LanguageMgr()
{
}

void xui::LanguageMgr::SetCurLanguage(const std::string & name)
{
    m_sCurLang = name;
}

void xui::LanguageMgr::SetDefLanguage(const std::string & name)
{
    m_sDefLang = name;
}

std::string xui::LanguageMgr::tr(const std::string & id)
{
    return this->operator[](id);
}

std::string xui::LanguageMgr::operator[](const std::string & id)
{
    std::string ret = "";
    auto it = m_langs.find(m_sCurLang);
    if (it != m_langs.end())
    {
        ret = it->second.GetItem(id);
    }
    if (!ret.empty()) return ret;

    it = m_langs.find(m_sDefLang);
    if (it != m_langs.end())
    {
        ret = it->second.GetItem(id);
    }
    if (!ret.empty()) return ret;
    return "";
}
