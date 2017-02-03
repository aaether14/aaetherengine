#include "aae_ctype.h"
#define _XA 0x200 /** extra alpha **/
#define _XS 0x100 /** extra space **/
#define _BB 0x80 /** BEL, BS etc. **/
#define _CN 0x40 /** CR, FF, HT, NL, VT **/
#define _DI 0x20 /* '0' - '9' **/
#define _LO 0x10 /* 'a' - 'z' **/
#define _PU 0x8 /** punctuation **/
#define _SP 0x4 /** space **/
#define _UP 0x2 /** 'A' - 'Z' **/
#define _XD 0x1 /** '0' - '9', 'a' - 'f', 'A' - 'F' **/
#define _XDI (_DI|_XD)
#define _XLO (_LO|_XD)
#define _XUP (_UP|_XD)
static const i16 m_ctype_tab[257] = {
	0, /** AAE_EOF **/
	_BB, _BB, _BB, _BB, _BB, _BB, _BB, _BB,
	_BB, _CN, _CN, _CN, _CN, _CN, _BB, _BB,
	_BB, _BB, _BB, _BB, _BB, _BB, _BB, _BB,
	_BB, _BB, _BB, _BB, _BB, _BB, _BB, _BB,
	_SP, _PU, _PU, _PU, _PU, _PU, _PU, _PU,
	_PU, _PU, _PU, _PU, _PU, _PU, _PU, _PU,
	_XDI, _XDI, _XDI, _XDI, _XDI, _XDI, _XDI, _XDI,
	_XDI, _XDI, _PU, _PU, _PU, _PU, _PU, _PU,
	_PU, _XUP, _XUP, _XUP, _XUP, _XUP, _XUP, _UP,
	_UP, _UP, _UP, _UP, _UP, _UP, _UP, _UP,
	_UP, _UP, _UP, _UP, _UP, _UP, _UP, _UP,
	_UP, _UP, _UP, _PU, _PU, _PU, _PU, _PU,
	_UP, _XLO, _XLO, _XLO, _XLO, _XLO, _XLO, _LO,
	_LO, _LO, _LO, _LO, _LO, _LO, _LO, _LO,
	_LO, _LO, _LO, _LO, _LO, _LO, _LO, _LO,
	_LO, _LO, _LO, _PU, _PU, _PU, _PU, _BB, 
};
static const aae_thread_local i16* m_ctype = &m_ctype_tab[1];


bool aae_isdigit(byte c)
{
	return (*(m_ctype + c)) & _DI; 
}
bool aae_isalpha(byte c)
{
	return (*(m_ctype + c) & (_LO | _UP | _XA));
}
bool aae_islower(byte c)
{
	return (*(m_ctype + c) & _LO);
}
bool aae_isupper(byte c)
{
	return (*(m_ctype + c) & _UP);
}