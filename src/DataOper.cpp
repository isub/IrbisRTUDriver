#include <map>
#include <list>

#include "XMLOper.h"
#include "DataOper.h"

int AnalyseResponse (const char *p_pmccXMLDoc, size_t p_stDocSize, SXMLDoc *p_psoXMLDoc) {
	int iRetVal = 0;

	iRetVal = ParseDocument (p_pmccXMLDoc, p_stDocSize, p_psoXMLDoc);

	return iRetVal;
}
