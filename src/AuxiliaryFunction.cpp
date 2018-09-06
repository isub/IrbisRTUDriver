#include "DriverRTU.h"
#include "AuxiliaryFunction.h"
#include <curl/curl.h>
#include "Utils/Log/Log.h"
#include "DataOper.h"

extern SCommandRule g_msoCommandRule[];
extern size_t g_stCmdRuleSize;
extern char g_mcRTUUrl[0x2000];
extern CLog *g_pcoLog;
extern int g_iDebug;

int InitialyzeCurl (CURL *pCurl, std::stringstream *p_pstrsIn, SReadData *p_psoOutData);

/* запись полученных данных */
static size_t my_fwrite (void *buffer, size_t size, size_t nmemb, void *stream) {
	((std::stringstream*)stream)->write ((char*)buffer, size * nmemb);
	return size * nmemb;
}

/* чтение передаваемых данных */
static size_t my_fread (void *buffer, size_t size, size_t nmemb, void *stream) {
	SReadData *psoReadData = (SReadData*)stream;
	size_t stDataSize;

	if (psoReadData->m_stRead < psoReadData->m_pstrsData->str().length()) {
		stDataSize = psoReadData->m_pstrsData->str().length() - psoReadData->m_stRead;
		psoReadData->m_pstrsData->read ((char*)buffer, stDataSize);
		psoReadData->m_stRead += stDataSize;
	} else {
		stDataSize = 0;
	}

	return stDataSize;
}

int InitialyzeCurl (CURL *pCurl, std::stringstream *p_pstrsIn, SReadData *p_psoOutData) {
	int iRetVal = 0;
	std::string strMsg;

	do {
		curl_slist *psoCurlList = NULL;
		CURLcode eCurlRes;

		//eCurlRes = curl_easy_setopt (pCurl, CURLOPT_URL, g_strRTUURL.c_str());
		eCurlRes = curl_easy_setopt (pCurl, CURLOPT_URL, g_mcRTUUrl);
		if (CURLE_OK != eCurlRes) {
			iRetVal = eCurlRes;
			if (strMsg.length()) strMsg += "; ";
			strMsg += "Couldn't set option 'CURLOPT_URL'";
			break;
		}

		eCurlRes = curl_easy_setopt (pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
		if (CURLE_OK != eCurlRes) {
			iRetVal = eCurlRes;
			if (strMsg.length()) strMsg += "; ";
			strMsg += "Couldn't set option 'CURLOPT_SSL_VERIFYPEER'";
			break;
		}

		eCurlRes = curl_easy_setopt (pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
		if (CURLE_OK != eCurlRes) {
			iRetVal = eCurlRes;
			if (strMsg.length()) strMsg += "; ";
			strMsg += "Couldn't set option 'CURLOPT_SSL_VERIFYHOST'";
			break;
		}

		eCurlRes = curl_easy_setopt (pCurl, CURLOPT_WRITEFUNCTION, my_fwrite);
		if (CURLE_OK != eCurlRes) {
			iRetVal = eCurlRes;
			if (strMsg.length()) strMsg += "; ";
			strMsg += "Couldn't set option 'CURLOPT_WRITEFUNCTION'";
			break;
		}

		eCurlRes = curl_easy_setopt (pCurl, CURLOPT_WRITEDATA, p_pstrsIn);
		if (CURLE_OK != eCurlRes) {
			iRetVal = eCurlRes;
			if (strMsg.length()) strMsg += "; ";
			strMsg += "Couldn't set option 'CURLOPT_WRITEDATA'";
			break;
		}

		eCurlRes = curl_easy_setopt (pCurl, CURLOPT_POST, 1);
		if (CURLE_OK != eCurlRes) {
			iRetVal = eCurlRes;
			if (strMsg.length()) strMsg += "; ";
			strMsg += "Couldn't set option 'CURLOPT_WRITEFUNCTION'";
			break;
		}

		eCurlRes = curl_easy_setopt (pCurl, CURLOPT_READFUNCTION, my_fread);
		if (CURLE_OK != eCurlRes) {
			iRetVal = eCurlRes;
			if (strMsg.length()) strMsg += "; ";
			strMsg += "Couldn't set option 'CURLOPT_WRITEFUNCTION'";
			break;
		}

		eCurlRes = curl_easy_setopt (pCurl, CURLOPT_READDATA, p_psoOutData);
		if (CURLE_OK != eCurlRes) {
			iRetVal = eCurlRes;
			if (strMsg.length()) strMsg += "; ";
			strMsg += "Couldn't set option 'CURLOPT_WRITEDATA'";
			break;
		}

		std::stringstream strsOpt;

		psoCurlList = curl_slist_append (psoCurlList, "User-Agent: TTC RTU Driver email:isi@ues.tattelecom.ru");
		psoCurlList = curl_slist_append (psoCurlList, "Accept-Encoding: q=1.0, identity");
		psoCurlList = curl_slist_append (psoCurlList, "Content-Type: text/html; charset=utf-8");
		strsOpt << "Content-Length: " << p_psoOutData->m_pstrsData->str().length();
		psoCurlList = curl_slist_append (psoCurlList, strsOpt.rdbuf()->str().c_str());

		eCurlRes = curl_easy_setopt (pCurl, CURLOPT_HTTPHEADER, psoCurlList);
		if (CURLE_OK != eCurlRes) {
			iRetVal = eCurlRes;
			if (strMsg.length()) strMsg += "; ";
			strMsg += "Couldn't set option 'CURLOPT_HTTPHEADER' 'Content-Type: text/html; charset=utf-8'";
			break;
		}
	} while (0);

	return iRetVal;
}

int SetNodeAttr (SXMLNode *p_psoXMLNode, char *p_pszAttrList) {
	int iRetVal = 0;
	char *pszAttrList, *pszAttrName, *pszAttrValue;

	pszAttrList = p_pszAttrList;
	while (*pszAttrList) {
		pszAttrName = pszAttrList;
		pszAttrValue = pszAttrName + strlen (pszAttrName);
		++pszAttrValue;
		p_psoXMLNode->m_mapAttrList.insert (std::make_pair (pszAttrName, pszAttrValue));
		pszAttrList = pszAttrValue + strlen (pszAttrValue);
		++pszAttrList;
	}

	return iRetVal;
}

bool CheckNodeAttr (SXMLNode *p_psoXMLNode, const char *p_pcszAttrList) {
	bool bRetVal = true;
	const char *pszAttrList, *pszAttrName, *pszAttrValue;
	std::map<std::string,std::string>::iterator iterAttrList;

	pszAttrList = p_pcszAttrList;
	while (*pszAttrList) {
		pszAttrName = pszAttrList;
		pszAttrValue = pszAttrName + strlen (pszAttrName);
		++pszAttrValue;
		iterAttrList = p_psoXMLNode->m_mapAttrList.find (pszAttrName);
		/* если атрибут найден */
		if (iterAttrList != p_psoXMLNode->m_mapAttrList.end()) {
			if (0 != iterAttrList->second.compare (pszAttrValue)) {
				bRetVal = false;
				break;
			}
		} else {
			bRetVal = false;
			break;
		}
		pszAttrList = pszAttrValue + strlen (pszAttrValue);
		++pszAttrList;
	}

	return bRetVal;
}

int MakeDocTemplate (const char *p_pcszCommandName, std::list<SXMLNode*> &p_listNodeList, void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult) {
	int iRetVal = 0;
	SCommandRule *psoCommandRule;
	SCommandParam *psoCommandParam;
	SXMLNode *psoXMLNode;
	const char *pcszNodeVal;

	do {
		psoXMLNode = NULL;
		psoCommandRule = GetCommandDescription (p_pcszCommandName);
		if (NULL == psoCommandRule) {
			iRetVal = -1;
			sprintf_s (p_pszResult, 4000, "RTU Driver: command '%s' rules not found", p_pcszCommandName);
			if (g_pcoLog) {
				g_pcoLog->WriteLog (p_pszResult);
			}
			break;
		}
		psoCommandParam = psoCommandRule->m_msoParamList;
		while (psoCommandParam->m_mcNodePath[0]) {
			if (psoCommandParam->m_iParamNumber < p_iParamQuantity) {
				psoXMLNode = GetNode (&p_listNodeList, psoCommandParam->m_mcNodePath);
				if (psoXMLNode) {
					switch (psoCommandParam->m_iParamNumber) {
					case -1:
						iRetVal = SetNodeAttr (psoXMLNode, psoCommandParam->m_mcAttrList);
						break;
					case -2:
						psoXMLNode->SetValue (psoCommandParam->m_mcValue, strlen (psoCommandParam->m_mcValue));
						break;
					case -3:
						iRetVal = SetNodeAttr (psoXMLNode, psoCommandParam->m_mcAttrList);
						psoXMLNode->SetValue (psoCommandParam->m_mcValue, strlen (psoCommandParam->m_mcValue));
						break;
					default:
						pcszNodeVal = reinterpret_cast<const char*>(*p_ppvParameters) + 256 * psoCommandParam->m_iParamNumber;
						psoXMLNode->SetValue (pcszNodeVal, strlen (pcszNodeVal));
						break;
					}
				} else {
					iRetVal = -1;
				}
				if (iRetVal) {
					break;
				}
			}
			++psoCommandParam;
		}
		if (iRetVal) {
			break;
		}
	} while (0);

	return iRetVal;
}

int MakeNode (SXMLNode *p_psoXMLNode, std::stringstream &p_strsOutXML) {
	int iRetVal = 0;
	std::list<SXMLNode*>::iterator iterChildList;
	std::map<std::string,std::string>::iterator iterAttrList;

	p_strsOutXML << "<" << p_psoXMLNode->m_strName;
	for (iterAttrList = p_psoXMLNode->m_mapAttrList.begin(); iterAttrList != p_psoXMLNode->m_mapAttrList.end(); ++iterAttrList) {
		p_strsOutXML << " " << iterAttrList->first << "=" << iterAttrList->second;
	}
	if (p_psoXMLNode->m_mapAttrList.size()) {
		p_strsOutXML << " ";
	}
	p_strsOutXML << ">";
	for (iterChildList = p_psoXMLNode->m_listChildList.begin(); iterChildList != p_psoXMLNode->m_listChildList.end(); ++iterChildList) {
		iRetVal = MakeNode (*iterChildList, p_strsOutXML);
		if (iRetVal) { break; }
	}
	std::string strValue;
	p_psoXMLNode->GetXMLValue (strValue);
	if (strValue.length ()) {
		p_strsOutXML.write (strValue.c_str(), strValue.length());
	}
	p_strsOutXML << "</" << p_psoXMLNode->m_strName << ">";

	return iRetVal;
}

int MakeDocument (std::list<SXMLNode*> *p_listNodeList, std::stringstream &p_strsOutXML) {
	int iRetVal = 0;
	std::list<SXMLNode*>::iterator iterRootList;

	p_strsOutXML << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
	for (iterRootList = p_listNodeList->begin(); iterRootList != p_listNodeList->end(); ++iterRootList) {
		iRetVal = MakeNode (*iterRootList, p_strsOutXML);
		if (iRetVal) { break; }
	}

	return iRetVal;
}

int SendDocument (std::stringstream *p_pstrsOutXML, SXMLDoc *p_psoXMLDoc, char *p_pszResult) {
	int iRetVal = 0;
	CURL *pCurl;
	CURLcode eCurlRes;
	std::string strMsg;
	std::stringstream strsInXML;

	pCurl = curl_easy_init();
	if (pCurl) {
		do {
			SReadData soReadData;

			soReadData.m_pstrsData = p_pstrsOutXML;
			soReadData.m_stRead = 0;
			iRetVal = InitialyzeCurl (pCurl, &strsInXML, &soReadData);
			if (iRetVal) { break; }
			if (g_iDebug) {
				if (g_pcoLog) {
					g_pcoLog->Dump ("Request", p_pstrsOutXML->rdbuf()->str().c_str());
				}
				p_pstrsOutXML->rdbuf()->pubseekpos (0);
			}
			eCurlRes = curl_easy_perform (pCurl);
			if (CURLE_OK != eCurlRes) {
				iRetVal = -1;
				if (strMsg.length()) strMsg += "; ";
				strMsg += "An error occurred while execution of 'curl_easy_perform'";
				break;
			}
			iRetVal = AnalyseResponse (strsInXML.rdbuf()->str().c_str(), strsInXML.rdbuf()->str().length(), p_psoXMLDoc);
			if (g_iDebug) {
				if (g_pcoLog) {
					g_pcoLog->Dump ("Response", strsInXML.rdbuf()->str().c_str());
				}
			}
		} while (0);
		curl_easy_cleanup (pCurl);
	} else {
		if (strMsg.length()) strMsg += "; ";
		strMsg += "Couldn't initialize pCurl";
	}
	if (strMsg.length()) {
		strncpy_s (p_pszResult, 4000, strMsg.c_str(), 4000 - 1);
		if (g_pcoLog) {
			g_pcoLog->WriteLog (p_pszResult);
		}
	}

	if (g_pcoLog) {
		g_pcoLog->Flush ();
	}

	return iRetVal;
}

int GetRespData (const char *p_pcszCommandName, SXMLDoc *p_psoXMLDoc, void **p_ppvParameters, int p_iParamQuantity, char *p_pszValue, int p_iBufSize, SXMLNode **p_ppsoXMLNode) {
	int iRetVal = 0;
	SCommandRule *psoCommandRule;
	SResult *psoResult;

	do {
		psoCommandRule = GetCommandDescription (p_pcszCommandName);
		/* если правило для команды не найдено */
		if (NULL == psoCommandRule) {
			iRetVal = -1;
			if (g_pcoLog) {
				g_pcoLog->WriteLog ("GetRespData: RTU Driver: command '%s' rules not found", p_pcszCommandName);
			}
			break;
		}

		/* обработка результата выполнения команды */
		psoResult = &(psoCommandRule->m_soResult);
		iRetVal = GetResult (psoResult, p_psoXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszValue, p_iBufSize, p_ppsoXMLNode);
		/* если правило успешного завершения сработало */
		if (0 == iRetVal) { break; }

		/* обработка ошибки*/
		psoResult = &(psoCommandRule->m_soError);
		iRetVal = GetResult (psoResult, p_psoXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszValue, p_iBufSize);
		/* если правило ошибочного завершения сработало */
		if (0 == iRetVal) {
			iRetVal = -1;
			break;
		}

		/* обработка критической ошибки */
		psoResult = &(psoCommandRule->m_soFail);
		iRetVal = GetResult (psoResult, p_psoXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszValue, p_iBufSize);
		/* если правило критической ошибки сработало */
		if (0 == iRetVal) {
			iRetVal = -2;
			break;
		}

		/* получили неожиданный результат */
		iRetVal = -1;
		strncpy_s (p_pszValue, p_iBufSize, "Expected value is not found", p_iBufSize);
	} while (0);

	return iRetVal;
}

int GetResult (SResult *p_psoResult, SXMLDoc *p_psoXMLDoc, void **p_ppvParameters, int p_iParamQuantity, char *p_pszValue, int p_iBufSize, SXMLNode **p_ppsoXMLNode) {
	int iRetVal = 0;
	SCommandParam *psoCommandParam;
	SXMLNode *psoBaseXMLNode = NULL, *psoXMLNode = NULL;
	const char *pcszValue, *pcszAttrList;
	std::string strAttList, strResult;
	std::string strValue;
	int iValueLen;
	std::map<std::string,std::string>::iterator iterAttrList;

	do {
		/* если правило задано */
		if (*(p_psoResult->m_mcBaseNodePath)) {
			psoBaseXMLNode = GetNode (&(p_psoXMLDoc->m_listRootNodeList), p_psoResult->m_mcBaseNodePath, NULL, false);
			/* базовый нод результата найден */
			if (psoBaseXMLNode) {
				/* выбираем нужную ноду */
				psoCommandParam = &(p_psoResult->m_soCriteria);
				/* если критерий выбора задан */
				if (*(psoCommandParam->m_mcNodePath)) {
					pcszAttrList = psoCommandParam->m_mcAttrList;
					if (0 > psoCommandParam->m_iParamNumber) {
						pcszValue = psoCommandParam->m_mcValue;
					} else {
						pcszValue = reinterpret_cast<const char*>(*p_ppvParameters) + 256 * psoCommandParam->m_iParamNumber;
					}
					psoXMLNode = CheckNode (&(psoBaseXMLNode->m_listChildList), psoCommandParam, pcszValue, pcszAttrList);
					/* если необходимое значение не найдено */
					if (NULL == psoXMLNode) {
						iRetVal = -1;
						break;
					}
				} else {
					/* если дочерняя нода не задана */
					psoXMLNode = psoBaseXMLNode;
				}
				/* если вызывающую функцию интересует искомая нода */
				if (p_ppsoXMLNode) {
					*p_ppsoXMLNode = psoXMLNode;
				}
				/* выбираем нужное значение */
				psoCommandParam = &(p_psoResult->m_soSelect);
				/* проверяем, нужна ли выборка значений */
				/* в случае выборки можно выбирать лишь атрибуты (-1) или значение узла (-2) */
				/* выбирать значение входного параметра (>= 0) бессмысленно */
				if (0 == psoCommandParam->m_iParamNumber) {
					break;
				}
				/* если задано выбираемое значение */
				if (*(psoCommandParam->m_mcNodePath)) {
					psoXMLNode = GetNode (&(psoXMLNode->m_listChildList), psoCommandParam->m_mcNodePath, NULL, false);
				/* если искомая нода не найдена */
					if (NULL == psoXMLNode) {
						iRetVal = -1;
						break;
					}
				}
				/* собираем список атрибутов */
				if (-1 == psoCommandParam->m_iParamNumber
					|| -3 == psoCommandParam->m_iParamNumber) {
					for (iterAttrList = psoXMLNode->m_mapAttrList.begin(); iterAttrList != psoXMLNode->m_mapAttrList.end(); ++iterAttrList) {
						strAttList += iterAttrList->first.c_str();
						strAttList += "=";
						strAttList += iterAttrList->second.c_str();
						strAttList += ";";
					}
				}
				if (psoXMLNode) {
					switch (psoCommandParam->m_iParamNumber) {
					case -1:
						strncpy_s (p_pszValue, p_iBufSize, strAttList.c_str(), p_iBufSize);
						break;
					case -2:
						iValueLen = psoXMLNode->GetValue (strValue);
						strValue.copy (p_pszValue, p_iBufSize);
						p_pszValue[iValueLen] = '\0';
						break;
					case -3:
						iValueLen = psoXMLNode->GetValue (strValue);
						strValue.copy (p_pszValue, p_iBufSize);
						p_pszValue[iValueLen] = '\0';
						strResult += "Value: ";
						strResult += p_pszValue;
						strResult += ";";
						strResult += "Attributes: ";
						strResult += strAttList;
						strncpy_s (p_pszValue, p_iBufSize, strResult.c_str(), p_iBufSize);
						break;
					default:
						/* недопустимое значение в данном контексте */
						iRetVal = -1;
						break;
					}
				} else {
					iRetVal = -1;
					break;
				}
			} else {
				/* если базовый нод не найден */
				iRetVal = -1;
				break;
			}
		}
	} while (0);

	return iRetVal;
}

SXMLNode * GetNode (std::list<SXMLNode*> *p_plistNodeList, const char *p_pszNodePath, SXMLNode *p_psoParentNode, bool p_bCreateNodes) {
	const char *pszNodeName, *pszSubNodeName;
	SXMLNode *psoXMLNode;
	std::list<SXMLNode*> *plistNodeList;
	std::list<SXMLNode*>::iterator iterNodeList;

	psoXMLNode = NULL;
	plistNodeList = p_plistNodeList;
	pszNodeName = p_pszNodePath;
	for (iterNodeList = plistNodeList->begin(); iterNodeList != plistNodeList->end(); ++iterNodeList) {
		/* если найдена нужная нода */
		if ((*iterNodeList)->m_strName == pszNodeName) {
			pszSubNodeName = pszNodeName + strlen (pszNodeName);
			++pszSubNodeName;
			if (*pszSubNodeName) {
				/* если обход не завершен */
				psoXMLNode = GetNode (&((*iterNodeList)->m_listChildList), pszSubNodeName, *iterNodeList, p_bCreateNodes);
				break;
			} else {
				/* завершаем обход */
				psoXMLNode = *iterNodeList;
				break;
			}
		}
	}
	/* если при обходе всех нод нужная не найдена */
	if (iterNodeList == plistNodeList->end()) {
		/* если необходимо создавать недостающие ноды */
		if (p_bCreateNodes) {
			psoXMLNode = new SXMLNode;
			if (p_psoParentNode) {
				p_psoParentNode->SetChild (psoXMLNode);
			} else {
				plistNodeList->push_back (psoXMLNode);
			}
			psoXMLNode->m_strName = pszNodeName;
			pszSubNodeName = pszNodeName + strlen (pszNodeName);
			++pszSubNodeName;
			if (*pszSubNodeName) {
				psoXMLNode = GetNode (&(psoXMLNode->m_listChildList), pszSubNodeName, psoXMLNode, p_bCreateNodes);
			}
		} else {
			psoXMLNode = NULL;
		}
	}

	return psoXMLNode;
}

SXMLNode * CheckNode (std::list<SXMLNode*> *p_plistNodeList, SCommandParam *p_psoCmdParam, const char *p_pcszValue, const char *p_pcszAttrList) {
	SXMLNode *psoRetVal = NULL;
	const char *pcszNodeName, *pcszSubNodeName;
	SXMLNode *psoXMLNode;
	std::list<SXMLNode*> *plistNodeList;
	std::list<SXMLNode*>::iterator iterNodeList;
	std::string strValue;
	bool bResult = false;

	psoXMLNode = NULL;
	plistNodeList = p_plistNodeList;
	pcszNodeName = p_psoCmdParam->m_mcNodePath;
	for (iterNodeList = plistNodeList->begin(); iterNodeList != plistNodeList->end(); ++iterNodeList) {
		/* если найдена нужная нода */
		if ((*iterNodeList)->m_strName == pcszNodeName) {
			pcszSubNodeName = pcszNodeName + strlen (pcszNodeName);
			++pcszSubNodeName;
			if (*pcszSubNodeName) {
				/* если обход не завершен */
				psoXMLNode = GetNode (&((*iterNodeList)->m_listChildList), pcszSubNodeName, NULL, false);
				if (NULL == psoXMLNode) { break; }
				switch (p_psoCmdParam->m_iParamNumber) {
				case -1:
					if (CheckNodeAttr (psoXMLNode, p_pcszAttrList)) { bResult = true; }
					break;
				case -3:
					if (CheckNodeAttr (psoXMLNode, p_pcszAttrList)) { bResult = true; }
					if (! bResult) { break; }
				case -2:
				default:
					psoXMLNode->GetValue (strValue);
					if (0 == strValue.compare (p_pcszValue)) { bResult = true; }
					break;
				}
				if (bResult) {
					psoRetVal = *iterNodeList;
					break;
				}
			} else {
				psoXMLNode = *iterNodeList;
				switch (p_psoCmdParam->m_iParamNumber) {
				case -1:
					if (CheckNodeAttr (psoXMLNode, p_pcszAttrList)) { bResult = true; }
					break;
				case -3:
					if (CheckNodeAttr (psoXMLNode, p_pcszAttrList)) { bResult = true; }
					if (! bResult) { break; }
				case -2:
				default:
					psoXMLNode->GetValue (strValue);
					if (0 == strValue.compare (p_pcszValue)) { bResult = true; }
					break;
				}
				if (bResult) {
					psoRetVal = *iterNodeList;
					break;
				}
			}
		}
	}

	return psoRetVal;
}

SCommandRule * GetCommandDescription (const char *p_pcszCommandName) {
	SCommandRule *psoRetValue;
	int iCmdInd;
	std::string strCommandName;
	/* ищем описание операции */
	strCommandName = p_pcszCommandName;
	for (iCmdInd = 0; iCmdInd < g_stCmdRuleSize/sizeof(*g_msoCommandRule); ++iCmdInd) {
		psoRetValue = &(g_msoCommandRule[iCmdInd]);
		if (strCommandName == psoRetValue->m_mcCommandName) {
			break;
		}
	}
	if (iCmdInd == g_stCmdRuleSize/sizeof(*g_msoCommandRule)) {
		psoRetValue = NULL;
	}
	return psoRetValue;
}
