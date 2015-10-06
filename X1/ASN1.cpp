#include "StdAfx.h"
#include "ASN1.h"

//
// ASN1.cs: Abstract Syntax Notation 1 - micro-parser and generator
//
// Authors:
//	Sebastien Pouliot  <sebastien@ximian.com>
//	Jesper Pedersen  <jep@itplus.dk>
//
// (C) 2002, 2003 Motus Technologies Inc. (http://www.motus.com)
// Copyright (C) 2004 Novell, Inc (http://www.novell.com)
// (C) 2004 IT+ A/S (http://www.itplus.dk)
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Modified by Nic Bedford (http://nicbedford.co.uk) to support multiple
// length tags.
//

// References:
// a.	ITU ASN.1 standards (free download)
//	http://www.itu.int/ITU-T/studygroups/com17/languages/

CASN1::~CASN1(void)
{
	RemoveAllArray();
}

CASN1::CASN1()
{
	ZeroMemory(m_aTag, sizeof(m_aTag));
	ZeroMemory(m_aValue, sizeof(m_aValue));
	ZeroMemory(m_aTag2, sizeof(m_aTag2));
	ZeroMemory(m_aValue2, sizeof(m_aValue2));
}

void CASN1::DoProcess(BYTE* data, int dataLen, int nBaseLevel)
{
	RemoveAllArray();

	int pos = 0;
	int nTagLength = 0;
	
	if (data == NULL)
		return;

	m_nBaseLevel = nBaseLevel+1;

	// Check for multi byte tags
	if ((data[nTagLength++] & 0x1f) == 0x1f)
	{
		// Tag number is encoded in the following bytes as a sequence of seven bit bytes
		// The high bit of these bytes is used as a flag to indicate whether there's more tag available
		while ((data[nTagLength++] & 0x80) == 0x80)
		{
		}
	}

	ZeroMemory(m_aTag, sizeof(m_aTag));
	memcpy(m_aTag, data, nTagLength);

	pos = nTagLength;
	int nLenLength = 0;
	int nLength = data[pos++];

	if (nLength > 0x80)
	{
		// Composed length
		nLenLength = nLength - 0x80;
		nLength = 0;
		for (int i = 0; i < nLenLength; i++)
		{
			nLength *= 256;
			nLength += data[i + pos];
		}
	}
	else if (nLength == 0x80)
	{
		// Undefined length encoding
		//throw new NotSupportedException("Undefined length encoding.");
		return;
	}

	ZeroMemory(m_aValue, sizeof(m_aValue));
	memcpy(m_aValue, data+(pos + nLenLength), nLength);
	
	AddItemToArray(m_nBaseLevel, (char*)m_aTag, nTagLength, (char*)m_aValue, nLength, TRUE);

	if ((m_aTag[0] & 0x20) == 0x20)
	{
		int nStart = (pos + nLenLength);
		
		m_nBaseLevel = m_nBaseLevel+1;
		m_nLevel = m_nBaseLevel;
		Decode(data, nStart, dataLen);
	}
}


// Note: Recursive
void CASN1::Decode(BYTE* asn1, int& anPos, int anLength)	//protected
{
	int nTagLength = 0;
	//int nLength;
	int nValLen;
	
	// Check for multi byte tags
	if ((asn1[anPos + nTagLength++] & 0x1f) == 0x1f)
	{
		// Tag number is encoded in the following bytes as a sequence of seven bit bytes
		// The high bit of these bytes is used as a flag to indicate whether there's more tag available
		while ((asn1[anPos + nTagLength++] & 0x80) == 0x80)
		{
		}
	}

	ZeroMemory(m_aTag2, sizeof(m_aTag2));
	memcpy(m_aTag2, asn1+anPos, nTagLength);

	// Minimum is 2 bytes (tag + length of 0)
	while (anPos < anLength - 1)
	{
		nValLen = 0;
		DecodeTLV(asn1, anPos, m_aTag2, nTagLength, m_aValue2, nValLen);

		// Sometimes we get trailing 0
		if (m_aTag2[0] == 0)
			continue;

		//ASN1 elm = Add(new ASN1(aTag, aValue));
		AddItemToArray(m_nLevel, (char*)m_aTag2, nTagLength, (char*)m_aValue2, nValLen);
		
		if ((m_aTag2[0] & 0x20) == 0x20)
		{
			int nConstructedPos = anPos;
			//elm.Decode(asn1, nConstructedPos, nConstructedPos + nLength);

			m_nLevel ++;
			Decode(asn1, nConstructedPos, nConstructedPos + nValLen);

			m_nLevel = m_nBaseLevel;
		}
		anPos += nValLen; // Value length
	}
}

// TLV : Tag - Length - Value
void CASN1::DecodeTLV(BYTE* asn1, int& pos, BYTE* tag, int& tagLen, BYTE* content, int& contentLen) //protected 
{
	int nTagLength = 0;
	int length;

	// Check for multi byte tags
	if ((asn1[pos + nTagLength++] & 0x1f) == 0x1f)
	{
		// Tag number is encoded in the following bytes as a sequence of seven bit bytes
		// The high bit of these bytes is used as a flag to indicate whether there's more tag available
		while ((asn1[pos + nTagLength++] & 0x80) == 0x80)
		{
		}
	}

	ZeroMemory(tag, MAX_TAG_LEN);
	memcpy(tag, asn1+pos, nTagLength);
	tagLen = nTagLength;

	pos += nTagLength;
	length = asn1[pos++];

	// Special case where L contains the Length of the Length + 0x80
	if ((length & 0x80) == 0x80)
	{
		int nLengthLen = length & 0x7F;
		length = 0;
		for (int i = 0; i < nLengthLen; i++)
			length = length * 256 + asn1[pos++];
	}

	ZeroMemory(content, MAX_BUF_LEN);
	memcpy(content, asn1+pos, length);
	contentLen = length;
}

void CASN1::DecodeTLV(BYTE* asn1, int& pos, unsigned short tag, int& tagLen, BYTE* content, int& contentLen) //protected 
{
	int length;

	tag = asn1[pos++];

	// Check for 2 byte tags
	switch (tag)
	{
	case 0x5f:
	case 0x9f:
	case 0xbf:
		tag <<= 8;
		tag |= asn1[pos++];
		break;
	}

	length = asn1[pos++];
	
	tagLen = 2;

	// Special case where L contains the Length of the Length + 0x80
	if ((length & 0x80) == 0x80)
	{
		int nLengthLen = length & 0x7F;
		length = 0;

		for (int i = 0; i < nLengthLen; i++)
			length = length * 256 + asn1[pos++];
	}

	ZeroMemory(content, MAX_BUF_LEN);
	memcpy(content, asn1+pos, length);
	contentLen = length;
}

void CASN1::AddItemToArray(int level, char* pszTag, int tagLen, char* szValue, int valueLen, BOOL bIstItem)
{
	LPCARD_INFO p = new CARD_INFO;

	memset(p, 0, sizeof(CARD_INFO));

	p->level = level;
	p->bTag = TRUE;
	p->bIst = bIstItem;
	
	CString strTag = ConvertBytesToHexStringEx(tagLen, (BYTE*)pszTag);
	
	memcpy(p->szTag, (LPCTSTR)strTag, tagLen*2);
	
	CString strVal = ConvertBytesToHexStringEx(valueLen, (BYTE*)szValue);
	memcpy(p->szValue, (LPCTSTR)strVal, valueLen*2);

	m_infoArray.AddTail(p);
}

void CASN1::RemoveAllArray()
{
	POSITION nPos = m_infoArray.GetHeadPosition();
	while(nPos) {
		LPCARD_INFO p = m_infoArray.GetNext(nPos);
		delete p;
	}
	m_infoArray.RemoveAll();
}

LPCARD_INFO CASN1::GetItemFromArray(int idx)
{
	int nIndex = 0;

	POSITION nPos = m_infoArray.GetHeadPosition();
	while(nPos) {
		LPCARD_INFO p = m_infoArray.GetNext(nPos);
		
		if(nIndex == idx) return p;
		nIndex ++;
	}
	return NULL;
}

LPCARD_INFO CASN1::FindItemInArray(char* chTag)
{
	POSITION nPos = m_infoArray.GetHeadPosition();

	while(nPos) {
		LPCARD_INFO p = m_infoArray.GetNext(nPos);

		if( !strcmp(p->szTag, chTag))
			return p;
	}
	return NULL;
}

LPCARD_INFO CASN1::FindNextItemInArray(char* chTag, POSITION& nPos, BOOL bStart)
{
	if(bStart) {
		nPos = m_infoArray.GetHeadPosition();

		while(nPos) {
			LPCARD_INFO p = m_infoArray.GetNext(nPos);

			if( !strcmp(p->szTag, chTag))
				return p;
		}
		return NULL;
	}
	else {
		while(nPos) {
			LPCARD_INFO p = m_infoArray.GetNext(nPos);

			if( !strcmp(p->szTag, chTag))
				return p;
		}
		return NULL;
	}
}