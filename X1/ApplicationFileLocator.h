#pragma once

class CApplicationFileLocator
{
public:
	CApplicationFileLocator(void);
	CApplicationFileLocator(BYTE* pdata);
	~CApplicationFileLocator(void);

	void SetValue(BYTE* pdata);

	BYTE SFI() { return (BYTE)(data[0]>>3); }
	BYTE FirstRecord() { return data[1];}
	BYTE LastRecord() { return data[2];}
	BYTE OfflineRecords() { return data[3];}

private:
	BYTE	data[5];
};
