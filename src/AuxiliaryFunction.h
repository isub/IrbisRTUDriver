#ifndef _AUXILIARYFUNCTION_H_
#define _AUXILIARYFUNCTION_H_

#include "XMLOper.h"
#include <sstream>
#include <list>

struct SCommandParam {
	/*	m_iParamNumber:
			-1 - использовать m_mcAttrList;
			-2 - использовать m_mcValue;
			-3 - использовать m_mcAttrList и m_mcValue
			>= 0 - использовать как индекс массива входных параметров (void **p_ppvParameters)
	*/
	int m_iParamNumber;
	char m_mcNodePath[1024];
	char m_mcAttrList[1024];
	char m_mcValue[1024];
};

struct SResult {
	char m_mcBaseNodePath[1024];
	SCommandParam m_soCriteria;
	SCommandParam m_soSelect;
};

struct SCommandRule {
	char m_mcCommandName[64];
	SCommandParam m_msoParamList[64];
	SResult m_soResult;
	SResult m_soError;
	SResult m_soFail;
};

struct SReadData {
	std::stringstream *m_pstrsData;
	size_t m_stRead;
};

int MakeDocTemplate (const char *p_pcszCommandName, std::list<SXMLNode*> &p_listNodeList, void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);
int MakeDocument (std::list<SXMLNode*> *p_listNodeList, std::stringstream &p_strsOutXML);
int SendDocument (std::stringstream *p_pstrsOutXML, SXMLDoc *p_psoXMLDoc, char *p_pszResult);
int GetRespData (const char *p_pcszCommandName, SXMLDoc *p_psoXMLDoc, void **p_ppvParameters, int p_iParamQuantity, char *p_pszValue, int p_iBufSize, SXMLNode **p_ppsoXMLNode = NULL);
int GetResult (SResult *p_psoResult, SXMLDoc *p_psoXMLDoc, void **p_ppvParameters, int p_iParamQuantity, char *p_pszValue, int p_iBufSize, SXMLNode **p_ppsoXMLNode = NULL);
SXMLNode * GetNode (std::list<SXMLNode*> *p_plistNodeList, const char *p_pszNodePath, SXMLNode *p_psoParentNode = NULL, bool p_bCreateNodes = true);
SXMLNode * CheckNode (std::list<SXMLNode*> *p_plistNodeList, SCommandParam *p_psoCmdParam, const char *p_pcszValue, const char *p_pcszAttrList);
SCommandRule * GetCommandDescription (const char *p_pcszCommandName);

#endif
