#include <expat.h>
#include <string>
#include <map>
#include <list>

#include "XMLOper.h"

void XML_Start (void *userData, const XML_Char *name, const XML_Char **atts);
void XML_End (void *userData, const XML_Char *name);
void XML_CharacterData (void *userData, const XML_Char *s, int len);

struct SParsingData {
	SXMLDoc *m_psoXMLDoc;
	SXMLNode *m_psoCurrentNode;
};

int ParseDocument (const char *p_pmccXMLDoc, size_t p_stDocSize, SXMLDoc *p_psoXMLDoc) {
	int iRetVal = 0;
	XML_Parser psoXMLParser;
	XML_Status eResult;
	SParsingData soParsingData;

	psoXMLParser = XML_ParserCreate (NULL);
	XML_SetElementHandler (psoXMLParser, XML_Start, XML_End);
	XML_SetCharacterDataHandler (psoXMLParser, XML_CharacterData);
	soParsingData.m_psoXMLDoc = p_psoXMLDoc;
	soParsingData.m_psoCurrentNode = NULL;
	XML_SetUserData (psoXMLParser, &soParsingData);

	eResult = XML_Parse (psoXMLParser, p_pmccXMLDoc, p_stDocSize, 1);

	XML_ParserFree (psoXMLParser);

	return iRetVal;
}

void XML_Start (void *userData, const XML_Char *name, const XML_Char **atts) {
	SParsingData *psoParsingData;
	SXMLNode *psoXMLNode;
	const XML_Char **ppszAttrList;
	const XML_Char *pszAttrName, *pszAttrValue;

	psoXMLNode = new SXMLNode;
	psoParsingData = (SParsingData*)userData;
	if (psoParsingData->m_psoCurrentNode) {
		psoParsingData->m_psoCurrentNode->SetChild (psoXMLNode);
	} else {
		psoParsingData->m_psoXMLDoc->m_listRootNodeList.push_back (psoXMLNode);
	}
	psoParsingData->m_psoCurrentNode = psoXMLNode;
	psoXMLNode->m_strName = name;
	ppszAttrList = atts;
	while (*ppszAttrList) {
		pszAttrName = *ppszAttrList++;
		pszAttrValue = *ppszAttrList++;
		psoXMLNode->m_mapAttrList.insert (std::make_pair (pszAttrName, pszAttrValue));
	}
}

void XML_End (void *userData, const XML_Char *name) {
	SParsingData *psoParsingData;

	psoParsingData = (SParsingData*)userData;
	if (psoParsingData->m_psoCurrentNode) {
		if (psoParsingData->m_psoCurrentNode->m_strName == name) {
			psoParsingData->m_psoCurrentNode = psoParsingData->m_psoCurrentNode->m_psoParentNode;
		}
	}
}

void XML_CharacterData (void *userData, const XML_Char *s, int len) {
	SParsingData *psoParsingData;

	psoParsingData = (SParsingData*)userData;
	if (psoParsingData->m_psoCurrentNode) {
		psoParsingData->m_psoCurrentNode->SetValue (s, len);
	}
}

SXMLNode::SXMLNode() {
	m_psoParentNode = NULL;
	m_psoLastChildNode = NULL;
}

SXMLNode::~SXMLNode() {
	m_mapAttrList.clear ();
	std::list<SXMLNode*>::iterator iterChildList;
	for (iterChildList = m_listChildList.begin (); iterChildList != m_listChildList.end (); ++iterChildList) {
		delete *iterChildList;
	}
	m_listChildList.clear ();
	m_psoLastChildNode = NULL;
}

void SXMLNode::SetChild (SXMLNode *p_psoChild) {
	this->m_psoLastChildNode = p_psoChild;
	this->m_listChildList.push_back (p_psoChild);
	p_psoChild->m_psoParentNode = this;
}

void SXMLNode::SetValue (const char *p_pszValue, int p_iValueLen) {
	std::string strValue;

	strValue.assign (p_pszValue, p_iValueLen);
	ReplEscapeSeq (strValue, "&quot;", "\"");
	ReplEscapeSeq (strValue, "&apos;", "'");
	ReplEscapeSeq (strValue, "&lt;", "<");
	ReplEscapeSeq (strValue, "&gt;", ">");
	ReplEscapeSeq (strValue, "&amp;", "&");
	m_strValue = strValue;
}

int SXMLNode::GetValue (std::string &p_strValue) {
	p_strValue = m_strValue;
	return m_strValue.length ();
}

int SXMLNode::GetXMLValue (std::string &p_strValue) {
	p_strValue = m_strValue;
	SetEscapeSeq (p_strValue);
	return p_strValue.length ();
}

void SXMLNode::ReplEscapeSeq (std::string &p_strValue, char *p_pszA, char *p_pszB) {
	size_t stPos, stPos2;

	stPos = 0;
	stPos2 = p_strValue.find (p_pszA, stPos);
	while (stPos2 >= stPos && stPos2 != -1) {
		p_strValue.replace (stPos2, 1, p_pszB);
		++stPos2;
		stPos = stPos2;
		stPos2 = p_strValue.find (p_pszA, stPos);
	}
}

void SXMLNode::SetEscapeSeq (std::string &p_strValue) {
	size_t stPos;
	char *pszNewStr;

	for (stPos = 0; stPos < p_strValue.length (); ) {
		if (p_strValue[stPos] == '"') {
			pszNewStr = "&quot;";
			p_strValue.replace (stPos, 1, pszNewStr);
			stPos += strlen (pszNewStr);
		} else if (p_strValue[stPos] == '\'') {
			pszNewStr = "&apos;";
			p_strValue.replace (stPos, 1, pszNewStr);
			stPos += strlen (pszNewStr);
		} else if (p_strValue[stPos] == '<') {
			pszNewStr = "&lt;";
			p_strValue.replace (stPos, 1, pszNewStr);
			stPos += strlen (pszNewStr);
		} else if (p_strValue[stPos] == '>') {
			pszNewStr = "&gt;";
			p_strValue.replace (stPos, 1, pszNewStr);
			stPos += strlen (pszNewStr);
		} else if (p_strValue[stPos] == '&') {
			pszNewStr = "&amp;";
			p_strValue.replace (stPos, 1, pszNewStr);
			stPos += strlen (pszNewStr);
		} else {
			++stPos;
		}
	}
}
