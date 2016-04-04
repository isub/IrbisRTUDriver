#pragma once

#include <string>
#include <map>
#include <list>

struct SXMLNode {
	std::string m_strName;
	std::map<std::string,std::string> m_mapAttrList;
	std::list<SXMLNode*> m_listChildList;
	SXMLNode *m_psoParentNode;
	SXMLNode *m_psoLastChildNode;
public:
	SXMLNode();
	~SXMLNode();
public:
	void SetChild (SXMLNode *p_psoChild);
	void SetValue (const char *p_pszValue, int p_iValueLen);
	/* возвращает размер значения */
	int GetValue (std::string &p_strValue);
	int GetXMLValue (std::string &p_strValue);
private:
	/* замещает p_pszB на p_pszA */
	void ReplEscapeSeq (std::string &p_strValue, char *p_pszA, char *p_pszB);
	void SetEscapeSeq (std::string &p_strValue);
private:
	std::string m_strValue;
};

struct SXMLDoc {
	std::list<SXMLNode*> m_listRootNodeList;
};

int ParseDocument (const char *p_pmccXMLDoc, size_t p_stDocSize, SXMLDoc *p_psoXMLDoc);
