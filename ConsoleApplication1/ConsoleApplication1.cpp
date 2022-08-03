// ConsoleApplication1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ConsoleApplication1.h"
#include "tinyxml2.h"

using namespace std;

void CreateBaseDocument(tinyxml2::XMLDocument &doc)
{
	tinyxml2::XMLDeclaration *decl = doc.NewDeclaration();
	doc.LinkEndChild(decl);

	tinyxml2::XMLElement *node = doc.NewElement("EIF");

	node->SetAttribute("VERSION", "2.0");
	node->SetAttribute("ID", "urok");
	node->SetAttribute("NAME", "Command name");
	doc.InsertFirstChild(node);

	// 여기서 부터 추가되는 코드
	tinyxml2::XMLElement *element = doc.NewElement("ELEMENT");
	node->InsertFirstChild(element);

	tinyxml2::XMLElement *eqpId = doc.NewElement("EQPID");
	eqpId->SetAttribute("VALUE", "EQPID");
	element->InsertFirstChild(eqpId);
	element->InsertEndChild(eqpId);

	node->InsertEndChild(element);

	doc.InsertEndChild(node);
}

CString ConvertXmlToCString(tinyxml2::XMLDocument &targetDoc)
{
	tinyxml2::XMLPrinter printer;
	targetDoc.Print(&printer);

	CString cst = (CString)printer.CStr();
	return cst;
}


void S1F2(tinyxml2::XMLDocument &doc) 
{
	CString content;
	CreateBaseDocument(doc);
	tinyxml2::XMLElement *node = doc.FirstChildElement();

	tinyxml2::XMLElement *itemElement = doc.NewElement("ITEM");
	tinyxml2::XMLElement *softRevElement = doc.NewElement("SOFTREV");
	softRevElement->SetAttribute("VALUE", "REV");
	itemElement->InsertFirstChild(softRevElement);
	itemElement->InsertEndChild(softRevElement);
	node->InsertFirstChild(itemElement);
	node->InsertEndChild(itemElement);

	content = ConvertXmlToCString(doc);

	doc.SaveFile("c:\\MESXML\\s1f2.xml");
	doc.Clear();
}

void S1F3(int svidListCount, CString svidName[], CString svidValue[])
{
	CString content;
	tinyxml2::XMLDocument doc;
	CreateBaseDocument(doc);
	tinyxml2::XMLElement *node = doc.FirstChildElement();

	tinyxml2::XMLElement *itemElement = doc.NewElement("ITEM");
	tinyxml2::XMLElement *svidElement = doc.NewElement("SVIDLIST");
	
	svidElement->SetAttribute("COUNT", CStringA(_T("%d"), svidListCount));

	for (int i = 0; i < svidListCount; i++)
	{
		tinyxml2::XMLElement *svidItem = doc.NewElement("SVID");
		svidItem->SetAttribute("NAME", CStringA(svidName[i]));
		svidItem->SetAttribute("VALUE", CStringA(svidName[i]));
		svidElement->InsertEndChild(svidItem);
	}

	itemElement->InsertFirstChild(svidElement);
	itemElement->InsertEndChild(svidElement);
	node->InsertFirstChild(itemElement);
	node->InsertEndChild(itemElement);

	doc.SaveFile("c:\\MESXML\\s1f3.xml");
	doc.Clear();
}

CString CreateSingleItemList(CString fileName, CString itemListTag, CString itemTag, int itemCount, CString itemNames[], CString itemValues[])
{
	CString content;
	tinyxml2::XMLDocument doc;
	CreateBaseDocument(doc);
	tinyxml2::XMLElement *node = doc.FirstChildElement();

	tinyxml2::XMLElement *itemElement = doc.NewElement("ITEM");
	tinyxml2::XMLElement *itemCountElement = doc.NewElement(CStringA(itemListTag));

	CString tempCount;
	tempCount.Format(_T("%d"), itemCount);
	itemCountElement->SetAttribute("COUNT", CStringA(tempCount));

	for (int i = 0; i < itemCount; i++)
	{
		tinyxml2::XMLElement *itemElem = doc.NewElement(CStringA(itemTag));
		itemElem->SetAttribute("NAME", CStringA(itemNames[i]));
		itemElem->SetAttribute("VALUE", CStringA(itemValues[i]));
		itemCountElement->InsertEndChild(itemElem);
	}

	itemElement->InsertFirstChild(itemCountElement);
	itemElement->InsertEndChild(itemCountElement);
	node->InsertFirstChild(itemElement);
	node->InsertEndChild(itemElement);

	content = ConvertXmlToCString(doc);
	
	CString filePath;
	filePath.Format(_T("c:\\MESXML\\%s.xml"), fileName);
	doc.SaveFile(CStringA(filePath));
	doc.Clear();
	return content;
}

CString CreateBasicAck(int code, CString fileName)
{
	CString content;
	tinyxml2::XMLDocument doc;
	CreateBaseDocument(doc);
	tinyxml2::XMLElement *node = doc.FirstChildElement();

	tinyxml2::XMLElement *itemElement = doc.NewElement("ITEM");
	tinyxml2::XMLElement *itemCountElement = doc.NewElement("ACKC");	

	CString codeStr;
	codeStr.Format(_T("%d"), code);

	itemCountElement->SetAttribute("VALUE", CStringA(codeStr));
	itemElement->InsertFirstChild(itemCountElement);
	itemElement->InsertEndChild(itemCountElement);
	node->InsertFirstChild(itemElement);
	node->InsertEndChild(itemElement);

	content = ConvertXmlToCString(doc);

	CString filePath;
	filePath.Format(_T("c:\\MESXML\\%s.xml"), fileName);
	doc.SaveFile(CStringA(filePath));
	doc.Clear();
	return content;
}

void BuildContent(CString fileName, CString rcmdcp_rcmdValue, int crmdcp_cpListCount, CString rcmdcp_cp_cpNameValue[], CString rcmdcp_cp_cpAckcValue[], CString rcmdcp_result_codeValue, CString rcmdcp_result_textValue, CString hcackValue)
{
	CString content;

	tinyxml2::XMLDocument doc;
	CreateBaseDocument(doc);
	tinyxml2::XMLElement *node = doc.FirstChildElement();

	tinyxml2::XMLElement *itemElement = doc.NewElement("ITEM");
	tinyxml2::XMLElement *RCMDCPElement = doc.NewElement("RCMDCP");

	tinyxml2::XMLElement *RCMDValElement = doc.NewElement("RCMD");
	RCMDValElement->SetAttribute("VALUE", CStringA(rcmdcp_rcmdValue));
	RCMDCPElement->InsertEndChild(RCMDValElement);

	//CPLIST
	tinyxml2::XMLElement *CPLISTElement = doc.NewElement("CPLIST");
	CString CPLISTCountStr;
	CPLISTCountStr.Format(_T("%d"), crmdcp_cpListCount);
	CPLISTElement->SetAttribute("COUNT", CStringA(CPLISTCountStr));

	//CP Items
	tinyxml2::XMLElement *CPElement = doc.NewElement("CP");
	for (int i = 0; i < crmdcp_cpListCount; i++)
	{
		tinyxml2::XMLElement *CPNameElement = doc.NewElement("CPNAME");
		CPNameElement->SetAttribute("VALUE", CStringA(rcmdcp_cp_cpNameValue[i]));
		tinyxml2::XMLElement *CPAckcElement = doc.NewElement("CPACKC");
		CPAckcElement->SetAttribute("VALUE", CStringA(rcmdcp_cp_cpAckcValue[i]));

		CPElement->InsertEndChild(CPNameElement);
		CPElement->InsertEndChild(CPAckcElement);
	}
	CPLISTElement->InsertFirstChild(CPElement);
	CPLISTElement->InsertEndChild(CPElement);
	RCMDCPElement->InsertEndChild(CPLISTElement);

	//Result
	tinyxml2::XMLElement *resultElement = doc.NewElement("RESULT");
	tinyxml2::XMLElement *resultCodeElement = doc.NewElement("CODE");
	resultCodeElement->SetAttribute("VALUE", CStringA(rcmdcp_result_codeValue));
	tinyxml2::XMLElement *resultTextElement = doc.NewElement("TEXT");
	resultTextElement->SetAttribute("VALUE", CStringA(rcmdcp_result_textValue));
	resultElement->InsertEndChild(resultCodeElement);
	resultElement->InsertEndChild(resultTextElement);
	RCMDCPElement->InsertEndChild(resultElement);

	itemElement->InsertEndChild(RCMDCPElement);

	//HCACK
	tinyxml2::XMLElement *hcackElement = doc.NewElement("HCACK");
	hcackElement->SetAttribute("VALUE", CStringA(hcackValue));
	itemElement->InsertEndChild(hcackElement);

	node->InsertFirstChild(itemElement);
	node->InsertEndChild(itemElement);

	CString filePath;
	filePath.Format(_T("c:\\MESXML\\%s.xml"), fileName);
	doc.SaveFile(CStringA(filePath));
	doc.Clear();
}

void BuildAlarm(CString fileName, int code, int alarmId, CString alarmText)
{
	CString content;
	tinyxml2::XMLDocument doc;
	CreateBaseDocument(doc);
	tinyxml2::XMLElement *node = doc.FirstChildElement();

	tinyxml2::XMLElement *itemElement = doc.NewElement("ITEM");
	tinyxml2::XMLElement *alCodElement = doc.NewElement("ALCD");
	CString alCodeStr;
	alCodeStr.Format(_T("%d"), code);
	alCodElement->SetAttribute("VALUE", CStringA(alCodeStr));

	tinyxml2::XMLElement *alIdElement = doc.NewElement("ALID");
	CString alIdStr;
	alCodeStr.Format(_T("%d"), alarmId);
	alIdElement->SetAttribute("VALUE", CStringA(alCodeStr));

	tinyxml2::XMLElement *alTextElement = doc.NewElement("ALTX");
	alTextElement->SetAttribute("VALUE", CStringA(alarmText));

	itemElement->InsertEndChild(alCodElement);
	itemElement->InsertEndChild(alIdElement);
	itemElement->InsertEndChild(alTextElement);

	node->InsertEndChild(itemElement);

	CString filePath;
	filePath.Format(_T("c:\\MESXML\\%s.xml"), fileName);
	doc.SaveFile(CStringA(filePath));
	doc.Clear();
}


void BuildAlarmList(CString fileName, int count, int code[], int alarmId[], CString alarmText[])
{
	CString content;
	tinyxml2::XMLDocument doc;
	CreateBaseDocument(doc);
	tinyxml2::XMLElement *node = doc.FirstChildElement();

	tinyxml2::XMLElement *itemElement = doc.NewElement("ITEM");
	
	tinyxml2::XMLElement *alListElement = doc.NewElement("ALLIST");
	CString alListCountStr;
	alListCountStr.Format(_T("%d"), count);
	alListElement->SetAttribute("COUNT", CStringA(alListCountStr));

	tinyxml2::XMLElement *alElement = doc.NewElement("AL");
	
	for (int i = 0; i < count; i++) 
	{
		tinyxml2::XMLElement *alCodElement = doc.NewElement("ALCD");
		CString alCodeStr;
		alCodeStr.Format(_T("%d"), code[i]);
		alCodElement->SetAttribute("VALUE", CStringA(alCodeStr));

		tinyxml2::XMLElement *alIdElement = doc.NewElement("ALID");
		CString alIdStr;
		alCodeStr.Format(_T("%d"), alarmId[i]);
		alIdElement->SetAttribute("VALUE", CStringA(alCodeStr));

		tinyxml2::XMLElement *alTextElement = doc.NewElement("ALTX");
		alTextElement->SetAttribute("VALUE", CStringA(alarmText[i]));

		alElement->InsertEndChild(alCodElement);
		alElement->InsertEndChild(alIdElement);
		alElement->InsertEndChild(alTextElement);
	}
	alListElement->InsertEndChild(alElement);
	itemElement->InsertEndChild(alListElement);
	node->InsertEndChild(itemElement);

	CString filePath;
	filePath.Format(_T("c:\\MESXML\\%s.xml"), fileName);
	doc.SaveFile(CStringA(filePath));
	doc.Clear();
}

CString S6F11(CString fileName, CString ceidName, CString ceidValue, CString rptidName, CString rptidValue, int dvListCount, CString dvName[], CString dvValue[]) 
{
	CString content;
	tinyxml2::XMLDocument doc;
	CreateBaseDocument(doc);
	tinyxml2::XMLElement *node = doc.FirstChildElement();

	tinyxml2::XMLElement *itemElement = doc.NewElement("ITEM");
	tinyxml2::XMLElement *ceidElement = doc.NewElement("CEID");
	ceidElement->SetAttribute("NAME", CStringA(ceidValue));
	ceidElement->SetAttribute("VALUE", CStringA(ceidValue));

	tinyxml2::XMLElement *rptIdElement = doc.NewElement("RPTID");
	rptIdElement->SetAttribute("NAME", CStringA(rptidName));
	rptIdElement->SetAttribute("VALUE", CStringA(rptidValue));
	
	tinyxml2::XMLElement *dvListElement = doc.NewElement("DVLIST");
	CString dvListCountStr;
	dvListCountStr.Format(_T("%d"), dvListCount);
	dvListElement->SetAttribute("COUNT", CStringA(dvListCountStr));
	for (int i = 0; i < dvListCount; i++) 
	{
		tinyxml2::XMLElement *dvElement = doc.NewElement("DV");
		dvElement->SetAttribute("NAME", CStringA(dvName[i]));
		dvElement->SetAttribute("VALUE", CStringA(dvValue[i]));
		dvListElement->InsertEndChild(dvElement);
	}

	itemElement->InsertEndChild(ceidElement);
	itemElement->InsertEndChild(rptIdElement);
	itemElement->InsertEndChild(dvListElement);

	node->InsertEndChild(itemElement);

	CString filePath;
	filePath.Format(_T("c:\\MESXML\\%s.xml"), fileName);
	doc.SaveFile(CStringA(filePath));

	CString str = ConvertXmlToCString(doc);
	doc.Clear();
	doc.Parse(CStringA(str));
	return str;
}

interface IMes 
{
public: 
	virtual void Test(CString text) {}
	virtual CString Get(CString text) { return _T(""); }
};
class InterfaceTester : public IMes
{
private :
	CString baseText;

public :
	InterfaceTester() {}
	virtual ~InterfaceTester() {};
	void Test(CString text) 
	{
		printf("%s", baseText);
	}
	CString Get(CString text)
	{
		baseText = text;
		return text;
	}
	static void ConvertCStrToXml(tinyxml2::XMLDocument &doc, CString xmlStr) 
	{
		doc.Parse(CStringA(xmlStr));
	}
};

class InterfaceBase
{
public:
	InterfaceBase() {}
	virtual ~InterfaceBase() {};
	IMes *mes;
};

void ParseExample() 
{
	CString xml;
	xml += _T("<?xml version=\"1.0\" encoding=\"utf-16\"?>\r\n");
	xml += _T("<EIF VERSION=\"2.0\" ID=\"S2F31\" NAME=\"Date and Time Set Request\">\r\n");
	xml += _T("<ELEMENT>\r\n");
	xml += _T("<EQPID VALUE=\"VCMLEAA015\"/>\r\n");
	xml += _T("</ELEMENT>\r\n");
	xml += _T("<ITEM>\r\n");
	xml += _T("<TIME NAME=\"TIME\" VALUE =\"20210908001157\"/>\r\n");
	xml += _T("</ITEM>\r\n");
	xml += _T("</EIF>");

	tinyxml2::XMLDocument doc;
	/*doc.LoadFile("D:\\MESXml\\S2F31.xml");*/
	doc.Parse(CStringA(xml));
	doc.SaveFile("c:\\MESXML\\parse.xml");
	tinyxml2::XMLNode *root = doc.RootElement();
	tinyxml2::XMLElement *timeElement = root->FirstChildElement("ITEM")->FirstChildElement("TIME");
	const char* data = timeElement->FindAttribute("VALUE")->Value();
	CString timeStr(data);
}


int main()
{
    int nRetCode = 0;

	/*InterfaceTester tester;
	InterfaceBase base;
	base.mes = &tester;
	CString result = base.mes->Get(_T("ok"));
	base.mes->Test(_T("Can"));*/

	//tinyxml2::XMLDocument doc;
	////S1F2(doc);

	//CString svid[5];
	//for (int i = 0; i < 5; i++) 
	//{
	//	svid[i].Format(_T("ITEM_%d"), i);
	//}	
	//CreateSingleItemList(_T("s1f3"), _T("SVIDLIST"), _T("SVID"), 5, svid, svid);
	//CreateSingleItemList(_T("s1f4"), _T("SVLIST"), _T("SV"), 5, svid, svid);
	////CString CreateSingleItemList(_T("s1f3"), _T("SVIDLIST"), _T("SVIDLIST"), 5, svid, svid);
	//CreateBasicAck(1, _T("s2f24"));

	CString cpVal[2] = { _T("cpValu1"), _T("cpValu2") };
	CString cpAckVal[2] = { _T("1"), _T("2") };
	//BuildContent(_T("S2F50"), _T("RCMDCPVALUE"), 1, cpVal, cpAckVal, _T("OK"), _T("OK"), _T("1"));
	//BuildAlarm(_T("S5F1"), 1, 1, _T("EMERGENCY"));

	int alCd[2] = {1, 2};
	int alId[2] = {1, 2};

	//BuildAlarmList(_T("S5F6"), 2, alCd, alId, cpVal);
	CString val = S6F11(_T("S6F11"), _T("test1"), _T("test2"), _T("test3"), _T("test3"), 2, cpVal, cpAckVal);
	tinyxml2::XMLDocument doc;

	InterfaceTester::ConvertCStrToXml(doc, val);

	/*CString basecon(_T("data,data,data"));
	CString testCon;
	testCon.Format(_T("%c%s%c"), (char)0x02, basecon, (char)0x31);*/

	ParseExample();

    return nRetCode;
}


