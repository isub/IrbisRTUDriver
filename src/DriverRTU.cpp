#include <Windows.h>
#include <sstream>
#include <time.h>
#include <list>
#include <map>
#include <errno.h>

#include "DriverRTU.h"
#include "AuxiliaryFunction.h"
#include "XMLOper.h"
#include "DataOper.h"
#include "Utils/Log/Log.h"

char g_mcRTUUrl[0x2000];
int g_iDebug;
CLog *g_pcoLog = NULL;

int __stdcall InitService (char *p_pszInitParameters, char *p_pszResult)
{
	int iRetVal = 0;
	int iFnRes;
	bool bRTUURLInitialized = false;
	char mcDup[256];
	char *pszName;
	char *pszNext;
	char *pszValue;

	do {
		p_pszResult[0] = '\0';

		if (NULL == g_pcoLog) {
			g_pcoLog = new CLog;
		}
		if (NULL == p_pszInitParameters) {
			break;
		}
		/* разбор строки инициализации */
		/* копируем строку параметров, т.к. в процессе обработки данные будут изменяться */
		iFnRes = strncpy_s (mcDup, p_pszInitParameters, _TRUNCATE);
		if (iFnRes) {
			iRetVal = errno;
			break;
		} else {
			pszName = mcDup;
		}
		while (pszName) {
			pszNext = strstr (pszName, ";");
			if (pszNext) {
				*pszNext = '\0';
				++pszNext;
			}
			pszValue = strstr (pszName, "=");
			if (pszValue) {
				*pszValue = '\0';
				++pszValue;
				if (0 == _stricmp (pszName, "log_file_mask")) {
					if (g_pcoLog) {
						iFnRes = g_pcoLog->Init (pszValue, "\r\n");
						if (iFnRes) {
							iRetVal = iFnRes;
							strncat_s (p_pszResult, 4000, "can not initialyze 'CLog'", _TRUNCATE);
							break;
						}
					}
				} else if (0 == _stricmp (pszName, "rtu_url")) {
					iFnRes = strncpy_s (g_mcRTUUrl, pszValue, _TRUNCATE);
					if (iFnRes) {
						iRetVal = errno;
						break;
					} else {
						bRTUURLInitialized = true;
					}
				} else if (0 == _stricmp (pszName, "debug")) {
					g_iDebug = atoi (pszValue);
				}
			}
			pszName = pszNext;
		}
		if (iRetVal) { break; }
		if (! bRTUURLInitialized) {
			iRetVal = -1;
			if (p_pszResult[0]) {
				strncat_s (p_pszResult, 4000, "; ", _TRUNCATE);
			}
			strncat_s (p_pszResult, 4000, "RTU URL is not defined", _TRUNCATE);
		}
		if (iRetVal) {
			break;
		}
	} while (0);
	if (0 == iRetVal) {
		if (p_pszResult[0]) {
			strncat_s (p_pszResult, 4000, "; ", _TRUNCATE);
		}
		strncat_s (p_pszResult, 4000, "Service initialized successfully", _TRUNCATE);
	}
	if (g_pcoLog) {
		g_pcoLog->WriteLog (p_pszResult);
	}

	return iRetVal;
}

int __stdcall DeInitService (char *p_pszResult)
{
	int iRetVal = 0;

	if (p_pszResult) {
		strcpy_s (p_pszResult, 4000, "Service deinitialized successfully");
	}
	if (g_pcoLog) {
		g_pcoLog->WriteLog (p_pszResult);
		delete g_pcoLog;
		g_pcoLog = NULL;
	}

	return iRetVal;
}

int __stdcall GetUserTerminalGUId (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult)
{
	int iRetVal = 0;
	std::list<SXMLNode*> listNodeList;
	std::stringstream strsOutXML;
	SXMLDoc soXMLDoc;
	const char *pcszCommanName;

	do {
		pcszCommanName = "GetUserTerminalGUId";
		iRetVal = MakeDocTemplate (pcszCommanName, listNodeList, p_ppvParameters, p_iParamQuantity, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = MakeDocument (&listNodeList, strsOutXML);
		if (iRetVal) { break; }
		iRetVal = SendDocument (&strsOutXML, &soXMLDoc, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = GetRespData (pcszCommanName, &soXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszResult, 4000);
		if (iRetVal) { break; }
	} while (0);

	return iRetVal;
}

int __stdcall UpdateUserTerminal (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult)
{
	int iRetVal = 0;
	std::list<SXMLNode*> listNodeList;
	std::stringstream strsOutXML;
	SXMLDoc soXMLDoc;
	const char *pcszCommanName;

	do {
		pcszCommanName = "UpdateUserTerminal";
		iRetVal = MakeDocTemplate (pcszCommanName, listNodeList, p_ppvParameters, p_iParamQuantity, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = MakeDocument (&listNodeList, strsOutXML);
		if (iRetVal) { break; }
		iRetVal = SendDocument (&strsOutXML, &soXMLDoc, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = GetRespData (pcszCommanName, &soXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszResult, 4000);
		if (iRetVal) { break; }
	} while (0);

	return iRetVal;
}

int __stdcall DeleteUserTerminal (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult)
{
#if SIP_RTU_API_VERSION >= 18120
	if (g_pcoLog) {
		g_pcoLog->WriteLog (
			"DeleteUserTerminal: this function is not implemented in driver version '%d'",
			SIP_RTU_API_VERSION);
	}
	return -1;
#else
	int iRetVal = 0;
	std::list<SXMLNode*> listNodeList;
	std::stringstream strsOutXML;
	SXMLDoc soXMLDoc;
	const char *pcszCommanName;

	do {
		pcszCommanName = "DeleteUserTerminal";
		iRetVal = MakeDocTemplate (pcszCommanName, listNodeList, p_ppvParameters, p_iParamQuantity, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = MakeDocument (&listNodeList, strsOutXML);
		if (iRetVal) { break; }
		iRetVal = SendDocument (&strsOutXML, &soXMLDoc, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = GetRespData (pcszCommanName, &soXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszResult, 4000);
		if (iRetVal) { break; }
	} while (0);

	return iRetVal;
#endif
}

int __stdcall GetUserInfo (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult)
{
	int iRetVal = 0;
	std::list<SXMLNode*> listNodeList;
	std::stringstream strsOutXML;
	SXMLDoc soXMLDoc;
	const char *pcszCommandName;

	do {
		pcszCommandName = "GetUserInfo";
		iRetVal = MakeDocTemplate (pcszCommandName, listNodeList, p_ppvParameters, p_iParamQuantity, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = MakeDocument (&listNodeList, strsOutXML);
		if (iRetVal) { break; }
		iRetVal = SendDocument (&strsOutXML, &soXMLDoc, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = GetRespData (pcszCommandName, &soXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszResult, 4000);
		if (iRetVal) { break; }
	} while (0);

	return iRetVal;
}

int __stdcall CreateUser (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult)
{
	int iRetVal = 0;
	std::list<SXMLNode*> listNodeList;
	std::stringstream strsOutXML;
	SXMLDoc soXMLDoc;
	const char *pcszCommandName;

	do {
		pcszCommandName = "CreateUser";
		iRetVal = MakeDocTemplate (pcszCommandName, listNodeList, p_ppvParameters, p_iParamQuantity, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = MakeDocument (&listNodeList, strsOutXML);
		if (iRetVal) { break; }
		iRetVal = SendDocument (&strsOutXML, &soXMLDoc, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = GetRespData (pcszCommandName, &soXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszResult, 4000);
		if (iRetVal) { break; }
	} while (0);

	return iRetVal;
}

int __stdcall ChangePhoneNumber (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult)
{
	int iRetVal = 0;
	std::list<SXMLNode*> listNodeList;
	std::stringstream strsOutXML;
	SXMLDoc soXMLDoc;
	const char *pcszCommandName;

	do {
		pcszCommandName = "ChangePhoneNumber";
		iRetVal = MakeDocTemplate (pcszCommandName, listNodeList, p_ppvParameters, p_iParamQuantity, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = MakeDocument (&listNodeList, strsOutXML);
		if (iRetVal) { break; }
		iRetVal = SendDocument (&strsOutXML, &soXMLDoc, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = GetRespData (pcszCommandName, &soXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszResult, 4000);
		if (iRetVal) { break; }
	} while (0);

	return iRetVal;
}

int __stdcall UpdateUserIdentity (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult)
{
	int iRetVal = 0;
	std::list<SXMLNode*> listNodeList;
	std::stringstream strsOutXML;
	SXMLDoc soXMLDoc;
	const char *pcszCommandName;

	do {
		pcszCommandName = "UpdateUserIdentity";
		iRetVal = MakeDocTemplate (pcszCommandName, listNodeList, p_ppvParameters, p_iParamQuantity, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = MakeDocument (&listNodeList, strsOutXML);
		if (iRetVal) { break; }
		iRetVal = SendDocument (&strsOutXML, &soXMLDoc, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = GetRespData (pcszCommandName, &soXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszResult, 4000);
		if (iRetVal) { break; }
	} while (0);

	return iRetVal;
}

int __stdcall UpdateUserPolicy (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult)
{
	int iRetVal = 0;
	char *pszGrList = NULL, *pszAttrList = NULL, *pszPackageList = NULL;
	char *pszName, *pszNext, *pszValue;

	do {
		switch (p_iParamQuantity) {
		case 8:
			pszPackageList = _strdup (((const char*)*p_ppvParameters) + 256 * 7);
		case 7:
			pszAttrList = _strdup (((const char*)*p_ppvParameters) + 256 * 6);
		case 6:
			pszGrList = _strdup (((const char*)*p_ppvParameters) + 256 * 5);
			break;
		}

		std::list<SXMLNode*> listNodeList;
		std::stringstream strsOutXML;
		SXMLDoc soXMLDoc;
		const char *pcszCommandName;

		pcszCommandName = "UpdateUser";
		iRetVal = MakeDocTemplate (pcszCommandName, listNodeList, p_ppvParameters, 5 <= p_iParamQuantity ? 5 : p_iParamQuantity, p_pszResult);
		if (iRetVal) { break; }
		SXMLNode *psoXMLNode;
		SXMLNode *psoSubNode, *psoSubValue;
		/* обрабатываем группы учетной записи абонента */
		psoXMLNode = GetNode (&listNodeList, "commands\0command\0item\0groups");
		if (NULL == psoXMLNode) {
			iRetVal = -1;
			break;
		}
		pszValue = pszGrList;
		while (pszValue) {
			pszNext = strstr (pszValue, ";");
			if (pszNext) {
				*pszNext = '\0';
				++pszNext;
			}
			if (strlen (pszValue)) {
				/* создаем ноду group */
				psoSubNode = new SXMLNode;
				psoSubNode->m_strName = "group";
				/* создаем ноду name */
				psoSubValue = new SXMLNode;
				psoSubValue->m_strName = "name";
				psoSubValue->SetValue (pszValue, strlen (pszValue));
				/* привязываем ноду name к ноде group */
				psoSubNode->SetChild (psoSubValue);
				/* создаем ноду enabled */
				psoSubValue = new SXMLNode;
				psoSubValue->m_strName = "enabled";
				psoSubValue->SetValue ("true", 4);
				/* привязываем ноду enabled к ноде group */
				psoSubNode->SetChild (psoSubValue);
				/* привязываем ноду group к ноде groups */
				psoXMLNode->SetChild (psoSubNode);
			}
			pszValue = pszNext;
		}
		/* обрабатываем атрибуты учетной записи абонента */
		psoXMLNode = GetNode (&listNodeList, "commands\0command\0item");
		if (NULL == psoXMLNode) {
			iRetVal = -2;
			break;
		}
		pszName = pszAttrList;
		while (pszName) {
			pszNext = strstr (pszName, ";");
			if (pszNext) {
				*pszNext = '\0';
				++pszNext;
			}
			pszValue = strstr (pszName, "=");
			if (pszValue) {
				*pszValue = '\0';
				++pszValue;
			}
			if (strlen (pszName)) {
				/* создаем дочернюю ноду */
				psoSubValue = new SXMLNode;
				psoSubValue->m_strName = pszName;
				psoSubValue->SetValue (pszValue, strlen (pszValue));
				/* привязываем дочернюю ноду к ноде item */
				psoXMLNode->SetChild (psoSubValue);
			}
			pszName = pszNext;
		}
		/* обрабатываем пакеты учетной записи абонента */
		psoXMLNode = GetNode (&listNodeList, "commands\0command\0item\0packages");
		if (NULL == psoXMLNode) {
			iRetVal = -3;
			break;
		}
		pszValue = pszPackageList;
		while (pszValue) {
			pszNext = strstr (pszValue, ";");
			if (pszNext) {
				*pszNext = '\0';
				++pszNext;
			}
			if (strlen (pszValue)) {
				/* создаем ноду package */
				psoSubValue = new SXMLNode;
				psoSubValue->m_strName = "package";
				psoSubValue->SetValue (pszValue, strlen (pszValue));
				/* привязываем ноду package к ноде packages */
				psoXMLNode->SetChild (psoSubValue);
			}
			pszValue = pszNext;
		}
		iRetVal = MakeDocument (&listNodeList, strsOutXML);
		if (iRetVal) { break; }
		iRetVal = SendDocument (&strsOutXML, &soXMLDoc, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = GetRespData (pcszCommandName, &soXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszResult, 4000);
		if (iRetVal) { break; }
	} while (0);

	if (pszGrList) {
		free (pszGrList);
		pszGrList = NULL;
	}
	if (pszAttrList) {
		free (pszAttrList);
		pszAttrList = NULL;
	}

	return iRetVal;
}

int __stdcall DeleteUser (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult)
{
	int iRetVal = 0;
	std::list<SXMLNode*> listNodeList;
	std::stringstream strsOutXML;
	SXMLDoc soXMLDoc;
	const char *pcszCommanName;

	do {
		pcszCommanName = "DeleteUser";
		iRetVal = MakeDocTemplate (pcszCommanName, listNodeList, p_ppvParameters, p_iParamQuantity, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = MakeDocument (&listNodeList, strsOutXML);
		if (iRetVal) { break; }
		iRetVal = SendDocument (&strsOutXML, &soXMLDoc, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = GetRespData (pcszCommanName, &soXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszResult, 4000);
		if (iRetVal) { break; }
	} while (0);

	return iRetVal;
}

int __stdcall CreateUserTerminal (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult)
{
	int iRetVal = 0;
	std::list<SXMLNode*> listNodeList;
	std::stringstream strsOutXML;
	SXMLDoc soXMLDoc;
	const char *pcszCommanName;

	do {
		pcszCommanName = "CreateUserTerminal";
		iRetVal = MakeDocTemplate (pcszCommanName, listNodeList, p_ppvParameters, p_iParamQuantity, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = MakeDocument (&listNodeList, strsOutXML);
		if (iRetVal) { break; }
		iRetVal = SendDocument (&strsOutXML, &soXMLDoc, p_pszResult);
		if (iRetVal) { break; }
		iRetVal = GetRespData (pcszCommanName, &soXMLDoc, p_ppvParameters, p_iParamQuantity, p_pszResult, 4000);
		if (iRetVal) { break; }
	} while (0);

	return iRetVal;
}
