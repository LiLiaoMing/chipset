#include "stdafx.h"
#include "Cardlist.h"

const char *cardList[] = {
_T("A000000003"),         // VISA
_T("A0000000031010"),     // VISA Debit/Credit
_T("A000000003101001"),   // VISA Credit
_T("A000000003101002"),   // VISA Debit
_T("A0000000032010"),     // VISA Electron
_T("A0000000033010"),     // VISA Interlink
_T("A0000000038010"),     // VISA Plus
_T("A000000003999910"),   // VISA ATM

_T("A0000000041010"),     // Master card
_T("A0000000048010"),     // Cirrus
_T("A0000000043060"),     // Maestro
_T("A0000000050001"),     // Maestro UK
_T("A00000002401"),       // Self Service
_T("A000000025"),         // American Express
_T("A000000025010104"),   // American Express
_T("A000000025010701"),   // ExpressPay
_T("A0000000291010"),     // Link
//_T("A000000333010101"),	  // UnionPay Debit (China Unionpay Co.Ltd)

_T("B012345678"),         // Maestro TEST

_T("A0000000651010"),     // JCB
0

};


const char *cardType[] = {
	_T("VISA"),         
	_T("MASTER"),     
	_T("AMEX"),     

	0
};