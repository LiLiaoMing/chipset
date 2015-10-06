#include "StdAfx.h"
#include "Reader.h"
#include "ASN1.h"
#include "ApplicationFileLocator.h"


#define APDU_PREFIX		_T("send_apdu -sc 0 -APDU ")


CReader::CReader(void)
{
}

CReader::~CReader(void)
{
	RemoveAllArray();
}

void CReader::AddItemToArray(int level, const char* pszValue, BOOL bIstItem, BYTE sfi)
{
	LPCARD_INFO p = new CARD_INFO;
	
	memset(p, 0, sizeof(CARD_INFO));

	p->level = level;
	p->bIst = bIstItem;
	p->sfi = sfi;
	memcpy(p->szLabal, pszValue, strlen(pszValue));
	m_infoArray.AddTail(p);
}

LPCARD_INFO CReader::FindItemInArray(char* chTag, int nTagIdx)
{
	int nIdx = 0;
	POSITION nPos = m_infoArray.GetHeadPosition();

	while(nPos) {
		LPCARD_INFO p = m_infoArray.GetNext(nPos);
		
		if( !strcmp(p->szTag, chTag) && nTagIdx == nIdx)
			return p;

		if(p->bTag)
			nIdx ++;
	}
	return NULL;
}

void CReader::RemoveAllArray()
{
	POSITION nPos = m_infoArray.GetHeadPosition();
	while(nPos) {
		LPCARD_INFO p = m_infoArray.GetNext(nPos);
		if(p)
			delete p;
	}
	m_infoArray.RemoveAll();
}

void CReader::AddSubArray(CARDINFOARRAY& array)
{
	POSITION nPos = array.GetHeadPosition();
	while(nPos) {
		LPCARD_INFO p = array.GetNext(nPos);
		
		// Must deep copy !!!
		LPCARD_INFO pp = new CARD_INFO;
		ZeroMemory(pp, sizeof(CARD_INFO));
		memcpy(pp, p, sizeof(CARD_INFO));
		m_infoArray.AddTail(pp);
	}
}

int CReader::GetCardType(char* szAID)
{
	int idx = 0;
	char *card = 0;
	BOOL bFound = FALSE;
	CString strOrig, strGiven = szAID;

	CStringArray pseIdentifiers;
	CString pse;

	pseIdentifiers.Add(_T("1PAY.SYS.DDF01"));
	pseIdentifiers.Add(_T("2PAY.SYS.DDF01"));

	for(int m=0;m<pseIdentifiers.GetSize();m++) 
	{
		pse = pseIdentifiers.GetAt(m);
		pse = ConvertBytesToHexStringEx(pse.GetLength(), (BYTE*)(LPCTSTR)pse);

		if( !pse.CompareNoCase(strGiven)){
			bFound = TRUE;
			break;
		}
	}

	if(bFound)
		return VISA;

	while(1) {
		card = (char *)cardList[idx++];
		if(card == 0) break;
		
		strOrig = card;
		if( !strOrig.CompareNoCase(strGiven)) {
			idx --;
			bFound = TRUE;
			break;
		}
	}

	if( !bFound) return -1;

	if(idx>=0 && idx<=7)
		return VISA;
	else if(idx==8)
		return MASTER;
	else if(idx==9)
		return CIRRUS;
	else if(idx==10)
	    return MAESTRO;
	else
		return -1;
}

int CReader::GetCardTypeEx()
{
	int cardType = VISA;
	int cardLastType = VISA;
	POSITION nPos = m_infoArray.GetHeadPosition();

	while(nPos) {
		LPCARD_INFO p = m_infoArray.GetNext(nPos);

		if(p->bIst) {	// This is element of ist file.

			if(!p->bTag) {		//Application

				GetAID(p->szLabal);
				cardType = GetCardType((char*)(LPCTSTR)m_strAID);	//last card type is valid !!!
				
				if(cardType>=0)
					cardLastType = cardType;

				if(cardType==MASTER)
					m_strMasterAID = m_strAID;
			}
		}
	}

	//return cardType;

	if( !m_strMasterAID.IsEmpty())
		return MASTER;

	return cardLastType;
}

int CReader::GetAIDNum()
{
	int nAIDNum = 0;
	POSITION nPos = m_infoArray.GetHeadPosition();

	while(nPos) {
		LPCARD_INFO p = m_infoArray.GetNext(nPos);

		if(p->bIst) {	// This is element of ist file.

			if(!p->bTag) {		//Application

				nAIDNum ++;
			}
		}
	}
	return nAIDNum;
}

CString CReader::FindValue(char* szArray, char* szVal)
{
	CString strResult = _T("");
	CString strTmp = szArray;
	CString strVal = szVal;
	int pos;
	BYTE len;
	char chLen[3];

	pos = strTmp.Find(strVal);
	if(pos>=0) {
		
		if(pos>0)
			strTmp.Delete(0, pos-1);
		
		chLen[0] = strTmp[2];
		chLen[1] = strTmp[3];
		chLen[2] = 0;
		ConvertHexCharToByte(chLen, &len);

		strResult = strTmp.Mid(4, len*2);

	}
	else {
		strVal.MakeLower();
		
		pos = strTmp.Find(strVal);
		if(pos>=0) {

			if(pos>0)
				strTmp.Delete(0, pos-1);

			chLen[0] = strTmp[2];
			chLen[1] = strTmp[3];
			chLen[2] = 0;
			ConvertHexCharToByte(chLen, &len);

			strResult = strTmp.Mid(4, len*2);
		}
	}
	
	return strResult;
}

BOOL CReader::CardConnect()
{
	CStringArray arr;

	arr.Add(_T("establish_context"));
	arr.Add(_T("enable_trace"));
	arr.Add(_T("enable_timer"));
	arr.Add(_T("card_connect"));

	if( handleCommands(arr) == EXIT_SUCCESS) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void CReader::CardDisConnect()
{
	CStringArray arr;

	arr.Add(_T("card_disconnect"));
	arr.Add(_T("release_context"));

	handleCommands(arr);
}

BOOL CReader::TransmitAPDU(CString& strAPDU)
{
	CStringArray arr;

	arr.Add(strAPDU);
	
	if( handleCommands(arr) == EXIT_SUCCESS) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CReader::Run()
{
	RemoveAllArray();

	int m, cardType;
	CStringArray pseIdentifiers;
	CStringArray applicationIdentifiers;

	CPtrList AppFileList;
	POSITION pos;

	CString	strAPDU;
	CString strAID;
	CString	pse;
	CString strNode;
	BYTE SW1, SW2;
	BOOL pseFound = FALSE;
	BOOL bRetVal;

	CASN1 ASN1;

	pseIdentifiers.Add(_T("1PAY.SYS.DDF01"));
	pseIdentifiers.Add(_T("2PAY.SYS.DDF01"));

	if( !CardConnect()) return FALSE;

	for(m=0;m<pseIdentifiers.GetSize();m++) 
	{
		
		pse = pseIdentifiers.GetAt(m);
		pse = ConvertBytesToHexStringEx(pse.GetLength(), (BYTE*)(LPCTSTR)pse);
		strAPDU.Format(_T("%s00A40400%02x%s"),APDU_PREFIX,pse.GetLength()/2, pse);
		bRetVal = TransmitAPDU(strAPDU);
		if( !bRetVal) continue;

		SW1 = g_recvAPDU[g_recvAPDULen-2];
		SW2 = g_recvAPDU[g_recvAPDULen-1];

		// Get response nescesary
		// Process in GPShell internally !!!
// 		if(SW1==0x61) {
// 			strAPDU.Format(_T("%s00C00000%02x"),APDU_PREFIX,SW2);
// 			bRetVal = TransmitAPDU(strAPDU);
// 			if( !bRetVal) continue;
// 
// 			// PSE application read found ok
// 			SW1 = g_recvAPDU[g_recvAPDULen-2];
// 			SW2 = g_recvAPDU[g_recvAPDULen-1];
// 		}

		if (SW1 == 0x90)
		{
			pseFound = true;
#if 1
// 			pseNode = new TreeNode(String.Format("Application {0}", encoding.GetString(pse)));
// 			pseNode.ImageIndex = 1;
// 			pseNode.SelectedImageIndex = 1;
// 			pseNode.Tag = pse;
// 			cardNode.Nodes.Add(pseNode);

			strNode.Format(_T("Application %s"), pseIdentifiers.GetAt(m));						//LEVEL ---->	1
			AddItemToArray(1, (LPCTSTR)strNode, TRUE);

// 			fciNode = new TreeNode("File Control Information");
// 			fciNode.ImageIndex = 3;
// 			fciNode.SelectedImageIndex = 3;
// 			fciNode.Tag = "fci";
// 			pseNode.Nodes.Add(fciNode);

			strNode.Format(_T("File Control Information"));										//LEVEL ---->	2 
			AddItemToArray(2, (LPCTSTR)strNode);

// 			fci = new ASN1(response.Data);
// 			AddRecordNodes(fci, fciNode);

			g_recvAPDULen -= 2;
			g_recvAPDU[g_recvAPDULen] = 0;
			if(g_recvAPDULen<=0) continue;

			ASN1.DoProcess(g_recvAPDU, g_recvAPDULen, 2);										//[SUBLEVEL] -->2,
			AddSubArray(ASN1.GetArrayInfo());
			

			//byte sfi = new ASN1(response.Data).Find(0x88).Value[0];
			byte sfi=0;
			byte recordNumber = 0x01;
			byte p2 = 0;

			char chVal[3]={0,};
			CARD_INFO *paip = ASN1.FindItemInArray(_T("88"));
			if(paip) {
				
				memcpy(chVal, paip->szValue, 2);
				ConvertHexCharToByte(chVal, &sfi);

				p2 = (byte)((sfi << 3) | 4);
			}
			else
				continue;

// 			TreeNode efDirNode = new TreeNode(String.Format("EF Directory - {0:X2}", sfi));
// 			efDirNode.ImageIndex = 2;
// 			efDirNode.SelectedImageIndex = 2;
// 			efDirNode.Tag = sfi;
// 			pseNode.Nodes.Add(efDirNode);
			
			strNode.Format(_T("EF Directory - %02x"), sfi);										//LEVEL ---->	2 
			AddItemToArray(2, (LPCTSTR)strNode);

			while (SW1 != 0x6A && SW2 != 0x83)
			{
// 				apdu = new APDUCommand(0x00, 0xB2, recordNumber, p2, null, 0x00);
// 				response = cardReader.Transmit(apdu);
				strAPDU.Format(_T("%s00B2%02x%02x%02x"),APDU_PREFIX, recordNumber, p2, 0);
				bRetVal = TransmitAPDU(strAPDU);

				SW1 = g_recvAPDU[g_recvAPDULen-2];
				SW2 = g_recvAPDU[g_recvAPDULen-1];

				// Retry with correct length
				// Process in GPShell internally !!! ???
// 				if (SW1 == 0x6C)
// 				{
// 					apdu = new APDUCommand(0x00, 0xB2, recordNumber, p2, null, response.SW2);
// 					response = cardReader.Transmit(apdu);
// 				}
// 
// 				if (SW1 == 0x61)
// 				{
// 					apdu = new APDUCommand(0x00, 0xC0, 0x00, 0x00, null, response.SW2);
// 					response = cardReader.Transmit(apdu);
// 				}

				//if (response.Data != null)
				if (SW1 == 0x90)
				{
					g_recvAPDULen -= 2;
					g_recvAPDU[g_recvAPDULen] = 0;
					if(g_recvAPDULen<=0) break;

// 					TreeNode recordNode = new TreeNode(String.Format("Record - {0:X2}", recordNumber));
// 					recordNode.ImageIndex = 4;
// 					recordNode.SelectedImageIndex = 4;
// 					recordNode.Tag = recordNumber;
// 					efDirNode.Nodes.Add(recordNode);
					
					strNode.Format(_T("Record - %02x"), recordNumber);								//LEVEL ---->	3 
					AddItemToArray(3, (LPCTSTR)strNode, FALSE, p2);

// 					ASN1 aef = new ASN1(response.Data);
// 					AddRecordNodes(aef, recordNode);

					ASN1.DoProcess(g_recvAPDU, g_recvAPDULen, 3);									//[SUBLEVEL] -->3,
					AddSubArray(ASN1.GetArrayInfo());

// 					foreach (ASN1 appTemplate in aef)
// 					{
// 						// Check we really have an Application Template
// 						if (appTemplate.Tag[0] == 0x61)
// 						{
// 							applicationIdentifiers.Add(appTemplate.Find(0x4f).Value);
// 						}
// 					}

					BOOL bStart = TRUE;
					POSITION nSubPos;
					while(1) 
					{
						LPCARD_INFO p = ASN1.FindNextItemInArray(_T("61"), nSubPos, bStart);
						if(p == NULL) break;
						
						bStart = FALSE;

						CString strVal = FindValue(p->szValue, _T("4F"));
						if(strVal.GetLength()>0) {
							applicationIdentifiers.Add(strVal);
						}
						
// 						paip = ASN1.FindItemInArray(_T("4F"));
// 						if(paip) {
// 							applicationIdentifiers.Add(paip->szValue);
// 						}
// 						else {
// 							paip = ASN1.FindItemInArray(_T("4f"));
// 							if(paip) {
// 								applicationIdentifiers.Add(paip->szValue);
// 							}
// 						}
					}
				}

				recordNumber++;
			}//end while
#endif
		}//end if (SW1 == 0x90)

		if (pseFound)
			break;
	} //end for
	
	CardDisConnect();
	ASN1.RemoveAllArray();

	// We couldn't read the AID's from the PSE, so we'll just try querying all ADI's we know about
	//if (!pseFound)
	if(applicationIdentifiers.GetCount()==0)
	{
		// From http://www.darkc0de.com/others/ChAP.py
		// Load card list
		int idx = 0;
		char* pCardList = 0;

		while(1){
		
			pCardList = (char*)cardList[idx++];
			if(pCardList==0) break;
			applicationIdentifiers.Add(pCardList);
		}
	}

	// Now lets process all of the AID's we found

	for(m=0;m<applicationIdentifiers.GetSize();m++) 
	{
		strAID = applicationIdentifiers.GetAt(m);

		cardType = GetCardType((char*)(LPCTSTR)strAID);

		CardConnect();

		// Select AID
		strAPDU.Format(_T("%s00A40400%02x%s"),APDU_PREFIX, strAID.GetLength()/2, strAID);
		bRetVal = TransmitAPDU(strAPDU);
		if( !bRetVal) continue;

		SW1 = g_recvAPDU[g_recvAPDULen-2];
		SW2 = g_recvAPDU[g_recvAPDULen-1];

		// Get response nescesary
		// Process in GPShell internally !!!
// 		if (SW1 == 0x61)
// 		{
// 			strAPDU.Format(_T("%s00C00000%02x"),APDU_PREFIX,SW2);
// 			bRetVal = TransmitAPDU(strAPDU);
// 			if( !bRetVal) continue;	
// 			
// 			SW1 = g_recvAPDU[g_recvAPDULen-2];
// 			SW2 = g_recvAPDU[g_recvAPDULen-1];
//  	}
		
		// Application not found
		if (SW1 == 0x6A && SW2 == 0x82)
			continue;

		if (SW1 == 0x90)
		{
			strNode.Format(_T("Application %s"), strAID);
			AddItemToArray(1, (LPCTSTR)strNode, TRUE);												//LEVEL ---->	1

			strNode.Format(_T("File Control Information"));											//LEVEL ---->	2
			AddItemToArray(2, (LPCTSTR)strNode);

			g_recvAPDULen -= 2;
			g_recvAPDU[g_recvAPDULen] = 0;
			if(g_recvAPDULen<=0) continue;

			ASN1.DoProcess(g_recvAPDU, g_recvAPDULen, 2);											//[SUBLEVEL] -->2,
			AddSubArray(ASN1.GetArrayInfo());

// 			fci = new ASN1(response.Data);
// 			AddRecordNodes(fci, fciNode);

			// Get processing options (with empty PDOL)
			//strAPDU.Format(_T("%s80A80000%02x8300"),APDU_PREFIX,2);	//MASTER card
			strAPDU.Format(_T("%s80A8000002830000"),APDU_PREFIX);	//MASTER card
			bRetVal = TransmitAPDU(strAPDU);
			if( !bRetVal) continue;	

			SW1 = g_recvAPDU[g_recvAPDULen-2];
			SW2 = g_recvAPDU[g_recvAPDULen-1];

			if(SW1 != 0x90) {
				strAPDU.Format(_T("%s80A80000048302025000"),APDU_PREFIX);	//VISA card
				bRetVal = TransmitAPDU(strAPDU);

				SW1 = g_recvAPDU[g_recvAPDULen-2];
				SW2 = g_recvAPDU[g_recvAPDULen-1];
			}
						
			// Get response nescesary
			// Process in GPShell internally !!!
// 			if (SW1 == 0x61)
// 			{
// 				strAPDU.Format(_T("%s00C00000%02x"),APDU_PREFIX,SW2);
// 				bRetVal = TransmitAPDU(strAPDU);
// 				if( !bRetVal) continue;
// 
// 				SW1 = g_recvAPDU[g_recvAPDULen-2];
// 				SW2 = g_recvAPDU[g_recvAPDULen-1];
// 			}

			if (SW1 == 0x90)
			{
				strNode.Format(_T("Application Interchange Profile - Application File Locator"));
				AddItemToArray(2, (LPCTSTR)strNode);												//LEVEL ---->	2

				g_recvAPDULen -= 2;
				g_recvAPDU[g_recvAPDULen] = 0;
				if(g_recvAPDULen<=0) continue;
				
				ASN1.DoProcess(g_recvAPDU, g_recvAPDULen, 2);										//[SUBLEVEL] -->2,
				AddSubArray(ASN1.GetArrayInfo());
				
// 				ASN1 template = new ASN1(response.Data);
// 				ASN1 aip = null;
// 				ASN1 afl = null;
				
				CARD_INFO	aip, *paip;
				CARD_INFO	afl, *pafl;
				CString		strVal;

				ZeroMemory(&aip, sizeof(CARD_INFO));
				ZeroMemory(&afl, sizeof(CARD_INFO));

				// Primative response (Template Format 1)
				//if (template.Tag[0] == 0x80)
				if(g_recvAPDU[0] == 0x80)
				{
// 					byte[] tempAIP = new byte[2];
// 					Buffer.BlockCopy(template.Value, 0, tempAIP, 0, 2);
// 					aip = new ASN1(0x82, tempAIP);
					
					aip.szTag[0] = '8';
					aip.szTag[1] = '2';
					//memcpy(aip.szValue, (char*)ASN1.m_aValue, 2);
					strVal = ConvertBytesToHexStringEx(2, ASN1.m_aValue);
					memcpy(aip.szValue, (LPCTSTR)strVal, strVal.GetLength());
 
// 					byte[] tempAFL = new byte[template.Length - 2];
// 					Buffer.BlockCopy(template.Value, 2, tempAFL, 0, template.Length - 2);
// 					afl = new ASN1(0x94, tempAFL);

					afl.szTag[0] = '9';
					afl.szTag[1] = '4';
					//memcpy(afl.szValue, (char*)ASN1.m_aValue+2, g_recvAPDULen-2);
					strVal = ConvertBytesToHexStringEx(g_recvAPDULen-4, ASN1.m_aValue+2);
					memcpy(afl.szValue, (LPCTSTR)strVal, strVal.GetLength());
				}

				// constructed data object response (Template Format 2)
				//if (template.Tag[0] == 0x77)
				if(g_recvAPDU[0] == 0x77)
				{
// 					aip = template.Find(0x82);
// 					afl = template.Find(0x94);
					paip = ASN1.FindItemInArray(_T("82"));
					if(paip)
						memcpy(&aip, paip, sizeof(aip));

					pafl = ASN1.FindItemInArray(_T("94"));
					//if(paip)
					if(pafl)
						memcpy(&afl, pafl, sizeof(afl));
				}

				// Chop up AFL's
				//for (int i = 0; i < afl.Length; i += 4)
				int aflLen = strlen((char*)afl.szValue)/2;
				//int aflLen = g_recvAPDULen-4;
				for (int i = 0; i < aflLen; i += 4)
				{
					BYTE AFL[5] = {0,};
					char szVal[9] = {0,};

					//Buffer.BlockCopy(afl.Value, i, AFL, 0, 4);
					//memcpy(AFL, afl.szValue+i, 4);
					memcpy(szVal, afl.szValue+i*2, 8);	//4B *2
					ConvertHexStringToBytes(szVal, AFL);

					//ApplicationFileLocator fileLocator = new ApplicationFileLocator(AFL);
					//applicationFileLocators.Add(fileLocator);
					CApplicationFileLocator* fileLocator = new CApplicationFileLocator(AFL);
					AppFileList.AddTail(fileLocator);

				}

// 				strNode.Format(_T("Application Interchange Profile - Application File Locator"));
// 				AddItemToArray(2, (LPCTSTR)strNode);												//LEVEL ---->	2

// 				ASN1 aipafl = new ASN1(response.Data);
// 				AddRecordNodes(aipafl, aipaflNode);

//				foreach (ApplicationFileLocator file in applicationFileLocators)
				pos = AppFileList.GetHeadPosition();
				int nPrevSFI=-1;	//[+JYC]
				int lr2 = 100;
				while(pos)
				{
					CApplicationFileLocator* file = (CApplicationFileLocator*)AppFileList.GetNext(pos);

					int r = file->FirstRecord();// +afl.OfflineRecords;     // We'll read SDA records too
					int lr = file->LastRecord();
					BYTE p2 = (BYTE)((file->SFI() << 3) | 4);

					if(nPrevSFI != file->SFI()) {
						strNode.Format(_T("Elementary File - %02x "), file->SFI());
						AddItemToArray(2, (LPCTSTR)strNode);										//LEVEL ---->	2
					}

					//to escape 1~3, 6~7 issue
					if(r>lr2+1)
						r = lr2+1;
					lr2 = lr;

					while (r <= lr)
					{
						strAPDU.Format(_T("%s00B2%02x%02x%02x"),APDU_PREFIX,(BYTE)r, p2,0);
						bRetVal = TransmitAPDU(strAPDU);
						if( !bRetVal) break;	
						
						SW1 = g_recvAPDU[g_recvAPDULen-2];
						SW2 = g_recvAPDU[g_recvAPDULen-1];

// 						apdu = new APDUCommand(0x00, 0xB2, (byte)r, p2, null, 0x00);
// 						response = cardReader.Transmit(apdu);

						// Retry with correct length
						// Process in GPShell internally !!!
// 						if (SW1 == 0x6C)
// 						{
// 							strAPDU.Format(_T("%s00B2%02x%02x%02x"),APDU_PREFIX,(BYTE)r, p2,SW2);
// 							bRetVal = TransmitAPDU(strAPDU);
// 							//if( !bRetVal) continue;
// 						}

						strNode.Format(_T("Record - %02x"), r);
						AddItemToArray(3, (LPCTSTR)strNode, FALSE, p2);							//LEVEL ---->	3

// 						TreeNode recordNode = new TreeNode(String.Format(" Record - {0:X2}", r));
// 
// 						if (r <= file.OfflineRecords)
// 						{
// 							recordNode.ImageIndex = 5;
// 							recordNode.SelectedImageIndex = 5;
// 						}
// 						else
// 						{
// 							recordNode.ImageIndex = 4;
// 							recordNode.SelectedImageIndex = 4;
// 						}
// 
// 						recordNode.Tag = r;
// 						efNode.Nodes.Add(recordNode);

						g_recvAPDULen -= 2;
						g_recvAPDU[g_recvAPDULen] = 0;
						//if(g_recvAPDULen<=0) break;
						
						ASN1.DoProcess(g_recvAPDU, g_recvAPDULen, 3);								//[SUBLEVEL] -->3,
						AddSubArray(ASN1.GetArrayInfo());
						ASN1.RemoveAllArray();

// 						ASN1 record = new ASN1(response.Data);
// 						AddRecordNodes(record, recordNode);

						r++;
					}
					nPrevSFI = file->SFI();
				}

				pos = AppFileList.GetHeadPosition();
				while(pos) {
					CApplicationFileLocator* p = (CApplicationFileLocator*)AppFileList.GetNext(pos);
					delete p;
				}
				AppFileList.RemoveAll();

				//IEnumerable<XElement> tags = tagsDocument.Descendants().Where(el => el.Name == "Tag");
				//foreach (XElement element in tags)
				//{
				//    string tag = element.Attribute("Tag").Value;

				//    // Only try GET_DATA on two byte tags
				//    if (tag.Length == 4)
				//    {
				//        byte p1 = byte.Parse(tag.Substring(0, 2), NumberStyles.HexNumber);
				//        byte p2 = byte.Parse(tag.Substring(2, 2), NumberStyles.HexNumber);

				//        apdu = new APDUCommand(0x80, 0xCA, p1, p2, null, 0);
				//        response = cardReader.Transmit(apdu);

				//        if (response.SW1 == 0x90)
				//        {
				//            Debug.WriteLine(response.ToString());
				//        }
				//    }
				//}

				// Referenced to 'superemv.lua'
				strAPDU.Format(_T("%s80CA9F13%02x"),APDU_PREFIX,0);		//Last Online ATC Register
				bRetVal = TransmitAPDU(strAPDU);
				if( !bRetVal) continue;

				SW1 = g_recvAPDU[g_recvAPDULen-2];
				SW2 = g_recvAPDU[g_recvAPDULen-1];

				if (SW1 == 0x90)
				{
					g_recvAPDULen -= 2;
					g_recvAPDU[g_recvAPDULen] = 0;
					
					if(g_recvAPDULen>0) {
						strNode.Format(_T("Last Online ATC Register"));									//LEVEL ---->	2
						AddItemToArray(2, (LPCTSTR)strNode);

						ASN1.DoProcess(g_recvAPDU, g_recvAPDULen, 2);									//[SUBLEVEL] -->2,
						AddSubArray(ASN1.GetArrayInfo());
					}
				}


				strAPDU.Format(_T("%s80CA9F17%02x"),APDU_PREFIX,0);		//PIN Try counter
				bRetVal = TransmitAPDU(strAPDU);
				if( !bRetVal) continue;

				SW1 = g_recvAPDU[g_recvAPDULen-2];
				SW2 = g_recvAPDU[g_recvAPDULen-1];

				if (SW1 == 0x90)
				{
					g_recvAPDULen -= 2;
					g_recvAPDU[g_recvAPDULen] = 0;

					if(g_recvAPDULen>0) {
						strNode.Format(_T("PIN Try counter"));											//LEVEL ---->	2
						AddItemToArray(2, (LPCTSTR)strNode);

						ASN1.DoProcess(g_recvAPDU, g_recvAPDULen, 2);									//[SUBLEVEL] -->2,
						AddSubArray(ASN1.GetArrayInfo());
					}
				}

				strAPDU.Format(_T("%s80CA9F36%02x"),APDU_PREFIX,0);		//Application Transaction Counter (ATC)
				bRetVal = TransmitAPDU(strAPDU);
				if( !bRetVal) continue;
				
				SW1 = g_recvAPDU[g_recvAPDULen-2];
				SW2 = g_recvAPDU[g_recvAPDULen-1];

				if (SW1 == 0x90)
				{
					g_recvAPDULen -= 2;
					g_recvAPDU[g_recvAPDULen] = 0;

					if(g_recvAPDULen>0) {
						strNode.Format(_T("Application Transaction Counter (ATC)"));
						AddItemToArray(2, (LPCTSTR)strNode);											//LEVEL ---->	2

						ASN1.DoProcess(g_recvAPDU, g_recvAPDULen, 2);									//[SUBLEVEL] -->2,
						AddSubArray(ASN1.GetArrayInfo());
					}
				}

				strAPDU.Format(_T("%s80CA9F4F%02x"),APDU_PREFIX,0);		//Log Format
				bRetVal = TransmitAPDU(strAPDU);
				if( !bRetVal) continue;

				SW1 = g_recvAPDU[g_recvAPDULen-2];
				SW2 = g_recvAPDU[g_recvAPDULen-1];

				if (SW1 == 0x90)
				{
					g_recvAPDULen -= 2;
					g_recvAPDU[g_recvAPDULen] = 0;

					if(g_recvAPDULen>0) {
						strNode.Format(_T("Log Format"));
						AddItemToArray(2, (LPCTSTR)strNode);											//LEVEL ---->	2

						ASN1.DoProcess(g_recvAPDU, g_recvAPDULen, 2);									//[SUBLEVEL] -->2,
						AddSubArray(ASN1.GetArrayInfo());
					}
				}

				strAPDU.Format(_T("%s80CA9F7F%02x"),APDU_PREFIX,0);		//CPCL Data
				bRetVal = TransmitAPDU(strAPDU);
				if( !bRetVal) continue;

				SW1 = g_recvAPDU[g_recvAPDULen-2];
				SW2 = g_recvAPDU[g_recvAPDULen-1];

				if (SW1 == 0x90)
				{
					g_recvAPDULen -= 2;
					g_recvAPDU[g_recvAPDULen] = 0;

					if(g_recvAPDULen>0) {
						strNode.Format(_T("cpcl data"));
						AddItemToArray(2, (LPCTSTR)strNode);											//LEVEL ---->	2

						ASN1.DoProcess(g_recvAPDU, g_recvAPDULen, 2);									//[SUBLEVEL] -->2,
						AddSubArray(ASN1.GetArrayInfo());
					}
				}
			}
			else
			{
				// Unexpected status word
				//UpdateStatusLabel(String.Format("Unexpected response from GET PROCESSING OPTIONS command: 0x{0:X2}{1:X2}", response.SW1, response.SW2));
			}
		}
		else
		{
			// Unexpected status word
			//UpdateStatusLabel(String.Format("Unexpected response from SELECT command: 0x{0:X2}{1:X2}", response.SW1, response.SW2));
		}

		CardDisConnect();
		ASN1.RemoveAllArray();
	}
		
	
	return TRUE;
}

void CReader::GetAID(char* szLabal)
{
	CString strTmp, strTmp2;
	CString strAIDPrefix = _T("Application");
	int	 pos;

	strTmp = szLabal;
	pos = strTmp.Find(strAIDPrefix);
	if(pos>=0) {	//AID
		strTmp2 = strTmp.Mid(strAIDPrefix.GetLength()+1);

		if( !strTmp2.CompareNoCase(_T("1PAY.SYS.DDF01")) ||
			!strTmp2.CompareNoCase(_T("2PAY.SYS.DDF01")))
		{
			strTmp2 = ConvertBytesToHexStringEx(strTmp2.GetLength(), (BYTE*)(LPCTSTR)strTmp2);
			m_strAID = strTmp2;
		}
		else 
		{
			m_strAID = strTmp2;	//For eg, "A000000003101001"
		}
	}
}

void CReader::GetRecordNum(POSITION nNextPos)
{
	CString strLabel;
	CString	strFind = _T("Record - ");
	POSITION nPos = nNextPos;
	int  pos;

	while(nPos) {
		LPCARD_INFO p = m_infoArray.GetPrev(nPos);
		p = m_infoArray.GetPrev(nPos);
		p = m_infoArray.GetPrev(nPos);
		
		strLabel = p->szLabal;
		pos = strLabel.Find(_T("Record"));
		if(pos>=0) {
			strLabel = strLabel.Mid(strFind.GetLength());
			ConvertHexCharToInt((char*)(LPCTSTR)strLabel, &m_nRecord);
			m_nSFI = p->sfi;
		}
		break;
	}
}

void CReader::MakeAPDULine(char* szTag, char* szValue, POSITION nPos, CString& strAPDU)
{
	CString strSub;
	CString strTmp, strTmp2;
	CString strAIDPrefix = _T("Application");

	int	 len=0, len2=0;
	int  tagVal;	

	strAPDU = _T("");

	ConvertHexCharToInt(szTag, &tagVal);

	switch(tagVal){
		case 0x6F:		//File Control Information(FCI)
			len2 = strlen(szValue)/2;
			strTmp.Format(_T("%s%02X%s"), szTag, len2, szValue);

			strTmp2.Format(_T("00A40400%02X%s%02X"), m_strAID.GetLength()/2, m_strAID, strTmp.GetLength()/2);

			strSub = strTmp2 + strTmp;
			len = strSub.GetLength()/2+1;
			break;
		case 0x70:		//Application Elementary File(AEF)
			len2 = strlen(szValue)/2;

			if(len2>=100)
				strTmp.Format(_T("%s81%02X%s"), szTag, len2, szValue);
			else
				strTmp.Format(_T("%s%02X%s"), szTag, len2, szValue);
			
			GetRecordNum(nPos);
			strTmp2.Format(_T("00B2%02X%02X00%02X"), m_nRecord, m_nSFI, strTmp.GetLength()/2);	// [70][size]{...data} : 2B
			
			strSub = strTmp2 + strTmp;
			len = strSub.GetLength()/2+1;
			break;
		case 0x77:		//TLV (Application-specific)
			len2 = strlen(szValue)/2;

			strTmp.Format(_T("%s%02X%s"), szTag, len2, szValue);
			strTmp2.Format(_T("80A8000000%02X"), strTmp.GetLength()/2);	// [70][size]{...data} : 2B

			strSub = strTmp2 + strTmp;
			len = strSub.GetLength()/2+1;
			break;
		case 0x80:		//application File Locator(AFL) : Get Processing Options
			len2 = strlen(szValue)/2;
			strTmp.Format(_T("%s%02X%s"), szTag, len2, szValue);

			strTmp2.Format(_T("80A8000000%02X"), strTmp.GetLength()/2);	// [80][size]{...data} : 2B

			strSub = strTmp2 + strTmp;
			len = strSub.GetLength()/2+1;
			break;
		case 0x9F13:	//ATC value of the last transaction that went online
			len2 = strlen(szValue)/2;
			strTmp.Format(_T("%s%02X%s"), szTag, len2, szValue);

			strTmp2.Format(_T("80CA9F1300%02X"), strTmp.GetLength()/2);	// 

			strSub = strTmp2 + strTmp;
			len = strSub.GetLength()/2+1;
			break;
		case 0x9F17:	//PIN Try Counter
			len2 = strlen(szValue)/2;
			strTmp.Format(_T("%s%02X%s"), szTag, len2, szValue);

			strTmp2.Format(_T("80CA9F1700%02X"), strTmp.GetLength()/2);	// 

			strSub = strTmp2 + strTmp;
			len = strSub.GetLength()/2+1;
			break;
		case 0x9F36:	//Counter maintained by the application in the card. 
						//Initial value is zero. It is incremented by 1 each time a transaction is performed.
			len2 = strlen(szValue)/2;
			strTmp.Format(_T("%s%02X%s"), szTag, len2, szValue);

			strTmp2.Format(_T("80CA9F3600%02X"), strTmp.GetLength()/2);	// 

			strSub = strTmp2 + strTmp;
			len = strSub.GetLength()/2+1;
			break;
// 		case 0x9F12:	//Application Preferred Name (Added last script)
// 			break;
		case 0x9F4F:	//Log format
			len2 = strlen(szValue)/2;
			strTmp.Format(_T("%s%02X%s"), szTag, len2, szValue);

			strTmp2.Format(_T("80CA9F4F00%02X"), strTmp.GetLength()/2);	// 

			strSub = strTmp2 + strTmp;
			len = strSub.GetLength()/2+1;
			break;
		case 0x9F7F:	//CPCL Data
			len2 = strlen(szValue)/2;
			strTmp.Format(_T("%s%02X%s"), szTag, len2, szValue);

			strTmp2.Format(_T("80CA9F7F00%02X"), strTmp.GetLength()/2);	// 

			strSub = strTmp2 + strTmp;
			len = strSub.GetLength()/2+1;
			break;
		default:
			//return _T("");
			return;
	}

	if(len>0)
		strAPDU.Format(_T("%s00A40102%02X%02X%s"),APDU_PREFIX, len, len, strSub);

//	return strAPDU;
}

void CReader::MakeVISA_IST(CStringArray& array)
{
	CString strAPDU = _T("");
	BOOL bFirst = TRUE;
	POSITION nPos = m_infoArray.GetHeadPosition();

	while(nPos) {
		LPCARD_INFO p = m_infoArray.GetNext(nPos);

		if(p->bIst) {	// This is element of ist file.

			if(!p->bTag) {		//Application

				GetAID(p->szLabal);

				if( bFirst) {
					strAPDU.Format(_T("%s00A40400%02X%s"),APDU_PREFIX,m_strAID.GetLength()/2, m_strAID);
					array.Add(strAPDU);

					strAPDU.Format(_T("%s00a401060100"),APDU_PREFIX);
					array.Add(strAPDU);
					strAPDU.Format(_T("%s00a40101"),APDU_PREFIX);
					array.Add(strAPDU);
					strAPDU.Format(_T("%s00a40105"),APDU_PREFIX);
					array.Add(strAPDU);

					bFirst = FALSE;
					continue;
				}//end if( bFirst)
				if(!bFirst) {
					strAPDU.Format(_T("%s00A40102090900B2%02X0C00026A83"),APDU_PREFIX, m_nRecord+1);  //The mark of record end
					array.Add(strAPDU);
				}
			}
			else if(p->bTag) {	//TAG

				MakeAPDULine(p->szTag, p->szValue, nPos, strAPDU);
				array.Add(strAPDU);
			}
		}
	}

	// {- LAST -}
//	strAPDU.Format(_T("%s00a401020B0B80CA9F1700049F170103"),APDU_PREFIX);	//Add new
//	array.Add(strAPDU);
	strAPDU.Format(_T("%s00a4010209090020008000029000"),APDU_PREFIX);
	array.Add(strAPDU);
	strAPDU.Format(_T("%s00a401021B1B80AE8000001480128001EF4980B68436958E5D06020A03A42000"),APDU_PREFIX);
	array.Add(strAPDU);
	strAPDU.Format(_T("%s00a4010209090082000000029000"),APDU_PREFIX);
	array.Add(strAPDU);
	strAPDU.Format(_T("%s00a401021B1B80AE4000001480124001EFFB42F466022D9E1906020A03642000"),APDU_PREFIX);
	array.Add(strAPDU);
	strAPDU.Format(_T("%s00a401021B1B80AE0000001400128001EF4980B68436958E5D06020A03A42000"),APDU_PREFIX);
	array.Add(strAPDU);
//	strAPDU.Format(_T("%s00a401020b0b80CA9F1700049f180103"),APDU_PREFIX);
//	array.Add(strAPDU);
	strAPDU.Format(_T("%s00a401060100"),APDU_PREFIX);
	array.Add(strAPDU);
}

void CReader::MakeMASTER_IST(CStringArray& array)
{
	CString strAPDU;
	BOOL bFirst = TRUE;
	int nAIDNum = GetAIDNum();
	int nCurAIDCount = 0;
	POSITION nPos = m_infoArray.GetHeadPosition();

	while(nPos) {
		LPCARD_INFO p = m_infoArray.GetNext(nPos);

		if(p->bIst) {	// This is element of ist file.

			if(!p->bTag) {		//Application

				GetAID(p->szLabal);
				nCurAIDCount ++;

				if( bFirst) {
					//strAPDU.Format(_T("%s00A40400%02X%s"),APDU_PREFIX,m_strAID.GetLength()/2, m_strAID);
					strAPDU.Format(_T("%s00A40400%02X%s"),APDU_PREFIX,m_strMasterAID.GetLength()/2, m_strMasterAID);
					array.Add(strAPDU);

					strAPDU.Format(_T("%s00a401060100"),APDU_PREFIX);
					array.Add(strAPDU);
					strAPDU.Format(_T("%s00a40101"),APDU_PREFIX);
					array.Add(strAPDU);
					strAPDU.Format(_T("%s00a40105"),APDU_PREFIX);
					array.Add(strAPDU);

					bFirst = FALSE;
					continue;
				}//end if( bFirst)
			}
			else if(p->bTag) {	//TAG
				
				if(nCurAIDCount == nAIDNum) {//last AID items are valid !!!
					MakeAPDULine(p->szTag, p->szValue, nPos, strAPDU);
					array.Add(strAPDU);
				}
			}
		}
	}

	// {- LAST -}
//	strAPDU.Format(_T("%s00a401020b0b80ca9f1700049f170103"),APDU_PREFIX);
//	array.Add(strAPDU);
	strAPDU.Format(_T("%s00a40102323280ae8000002B77299F2701809F360200499F26088A3470B2406A11489F10120114A500030200003B0F00000000000000FF"),APDU_PREFIX);
	array.Add(strAPDU);
	strAPDU.Format(_T("%s00a40102323280ae4000002B77299F2701409F360200499F2608BA9A9D755485B3EC9F101201146500030200003B0F00000000000000FF"),APDU_PREFIX);
	array.Add(strAPDU);
	strAPDU.Format(_T("%s00a40102323280ae0000002B77299F2701009F360200499F26088A3470B2406A11489F10120114A500030200003B0F00000000000000FF"),APDU_PREFIX);
	array.Add(strAPDU);
	strAPDU.Format(_T("%s00a4010209090020008000029000"),APDU_PREFIX);
	array.Add(strAPDU);
//	strAPDU.Format(_T("%s00a401020b0b80ca9f1700049f170103"),APDU_PREFIX);
//	array.Add(strAPDU);
	strAPDU.Format(_T("%s00a401060100"),APDU_PREFIX);
	array.Add(strAPDU);
}

void CReader::GenerateIst(CStringArray& array)
{
	int cardType= GetCardTypeEx();

	if(cardType<0) 
	{
		return;
	}

	array.Add(_T("#NAME: ZOOLANDER X#"));
	array.Add(_T(""));

	// {- HEADER -}

	if(cardType==MASTER)
		array.Add(_T("mode_211"));

	array.Add(_T("establish_context"));
	array.Add(_T("enable_trace"));
	array.Add(_T("enable_timer"));
	array.Add(_T("card_connect"));

	if(cardType==VISA)
		MakeVISA_IST(array);
	else
		MakeMASTER_IST(array);

	// {- TAIL -}
	array.Add(_T("card_disconnect"));
	array.Add(_T("release_context"));
}
