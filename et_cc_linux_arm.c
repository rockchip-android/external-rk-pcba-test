#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE

#ifndef __USE_FILE_OFFSET64
#define __USE_FILE_OFFSET64
#endif

#ifndef __USE_LARGEFILE64
#define __USE_LARGEFILE64
#endif

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

//#define O_LARGEFILE 32768

#include "et_cc_linux_arm.h"

#include <stdio.h>

/////////////////////////////////////////////////////////////////

int et_fn_cmp_sz_default(const char* szLeft, const char* szRight){
	return et_strcmp(szLeft,szRight);
}

int et_fn_cmp_int_default(INT64 iLeft, INT64 iRight){
	if(iLeft > iRight){
		return 1;
	}else if(iLeft < iRight){
		return -1;
	}else{
		return 0;
	}
}

////////////////////////////////////////////////////////////////

char* et_sz_by_char(char *szDest, char chSrc){
	if(szDest == null){
		return null;
	}
	szDest[0] = chSrc;
	szDest[1] = 0;
	return szDest;
}

char et_sz_to_char(const char* szSrc){
	if(szSrc == null){
		return 0;
	}
	return szSrc[0];
}

char* et_sz_by_int(char* szDest, int iNum){
	return et_sz_by_long(szDest,iNum);
}

int et_sz_to_int(const char* szSrc){
	return (int)et_sz_to_long(szSrc);
}

char* et_sz_by_long(char* szDest, INT64 iNum){
	INT64 iTemp = 0;
	INT64 iQuotient = 0;
	INT64 iRemain = 0;
	int iSize = 0;
	bool bNegative = false;

	if(szDest == null){
		return null;
	}

	szDest[0] = 0;

	if(iNum == 0){
		szDest[0] = '0';
		szDest[1] = 0;
		return szDest;
	}

	iTemp = iNum;

	if(iTemp < 0){
		iTemp *= -1;
		bNegative = true;
	}

	while(iTemp > 0){
		iRemain = iTemp % 10;
		iQuotient = iTemp / 10;
		iTemp = iQuotient;
		szDest[iSize++] = et_char_by_int(iRemain);
	}

	if(bNegative){
		szDest[iSize++] = '-';
	}

	szDest[iSize++] = 0;
	et_sz_reverse(szDest);

	return szDest;
}

INT64 et_sz_to_long(const char* szSrc){
	INT64 iResult = 0;
	int iSize = 0;
	char chTemp = 0;
	INT64 iTemp = 0;
	bool bNegative = false;
	int i = 0;
	INT64 iZeroCount = 1;

	

	if(szSrc == null){
		return 0;
	}

	iSize = et_strlen(szSrc);
	if(iSize == 0 || iSize > 20){
		return 0;
	}

	for(i = iSize - 1; i >= 0; i--){
		chTemp = szSrc[i];
		if(i == 0 && (chTemp == '-' || chTemp == '+')){
			if(chTemp == '-'){
				bNegative = true;
			}
			break;
		}

		if(!et_char_is_num(chTemp)){
			return 0;
		}

		iTemp =et_char_to_int(chTemp);
		if(i == iSize - 1){
			iResult = iTemp;
		}else{
			iZeroCount *= 10;
			iTemp *= iZeroCount;
			iResult += iTemp;
		}
	}

	
	if(bNegative){
		iResult *= -1;
	}

	return iResult;
}

char* et_sz_by_float(char *szDest, double fNum, int iAfterPoint){
	INT64 iIntValue = 0;
	char szKeep[255] = "";
	char szKeep2[255] = "";
	int iSizeInt = 0;
	int iSizeDiff = 0;
	int iSizeStr = 0;
	bool bNegative = false;
	int i = 0;

	if(szDest == null){
		return null;
	}

	if(iAfterPoint <= 0){
		return et_sz_by_long(szDest,et_math_round(fNum));
	}

	if(fNum == 0){
		szDest[0] = '0';
		szDest[1] = '.';
		szDest[2] = 0;
		et_sz_add_chars(szDest,'0',iAfterPoint);
		return szDest;
	}

	if(fNum < 0){
		bNegative = true;
		fNum *= -1;
	}

	fNum *= et_math_power(10,iAfterPoint);
	iIntValue = et_math_round(fNum);
	if(iIntValue == 0){
		szDest[0] = '0';
		szDest[1] = 0;
		return szDest;
	}

	et_sz_by_long(szKeep,iIntValue);

	iSizeInt = et_strlen(szKeep);
	iSizeDiff = iAfterPoint - iSizeInt;
	if(iSizeDiff >= 0){
		szKeep2[0] = '0';
		szKeep2[1] = '.';
		iSizeStr = 2;
		for(i = 0; i < iSizeDiff; i++){
			szKeep2[iSizeStr++] = '0';
		}
		szKeep2[iSizeStr++] = 0;
		et_strcat(szKeep2,szKeep);

	}else{
		for(i = 0; i < iSizeInt; i++){
			if(iSizeInt - i == iAfterPoint){
				szKeep2[iSizeStr++] = '.';
			}
			szKeep2[iSizeStr++] = szKeep[i];
		}
		szKeep2[iSizeStr++] = 0;
	}

	if(bNegative){
		szDest[0] = '-';
		szDest[1] = 0;
		et_strcpy(szDest + 1,szKeep2);
	}else{
		et_strcpy(szDest,szKeep2);
	}

	return szDest;
}

int et_sz_find_char(const char *szSrc, char chFind, UINT iFromPos){
	int iSrcLen = 0;
	int i = 0;

	if(szSrc == null){
		return -1;
	}
	iSrcLen = et_strlen(szSrc);

	if(iFromPos >= (UINT)iSrcLen){
		return -1;
	}

	for(i = iFromPos; i < iSrcLen; i++){
		if(szSrc[i] == chFind){
			return i;
		}
	}

	return -1;
}

int et_sz_find_not_char(const char* szSrc, char chFind, UINT iFromPos){
	int iSrcLen = 0;
	int i = 0;

	if(szSrc == null){
		return -1;
	}
	iSrcLen = et_strlen(szSrc);

	if(iFromPos >= (UINT)iSrcLen){
		return -1;
	}

	for(i = iFromPos; i < iSrcLen; i++){
		if(szSrc[i] != chFind){
			return i;
		}
	}

	return -1;
}

int et_sz_find_from(const char *szSrc, const char *szFind, UINT iFromPos){
	int bMatch = 0;
	UINT iSrcLen = 0;
	UINT iFindLen = 0;
	UINT i = 0;
	int iTempPos = 0;
	UINT iMatchCount = 0;

	if(szSrc == null || szFind == null){
		return -1;
	}

	iSrcLen = et_strlen(szSrc);
	iFindLen = et_strlen(szFind);

	if(iSrcLen - iFromPos < iFindLen
		|| iFromPos < 0
		|| iFromPos >= iSrcLen
		|| iFromPos < 0)
	{
		return -1;
	}

	for(i = iFromPos; i < iSrcLen; i++){
		if(!bMatch){
			if(szSrc[i] == szFind[0]){
				bMatch = 1;
				iMatchCount = 1;
				iTempPos = i;
			}else{
				continue;
			}
		}else{
			if(szSrc[i] == szFind[iMatchCount]){
				iMatchCount++;
			}else{
				bMatch = 0;
				iMatchCount = 0;
			}
		}

		if(iMatchCount == iFindLen){
			return iTempPos;
		}
	}
	return -1;
}

int et_sz_find(const char *szSrc, const char *szFind){
	return et_sz_find_from(szSrc,szFind,0);
}

int et_sz_find_reverse(const char* szSrc, const char* szFind){
	int iFromPos = 0;
	int bMatch = 0;
	int iSrcLen = 0;
	int iFindLen = 0;
	int i = 0;
	int iTempPos = 0;
	int iMatchCount = 0;
	iFromPos = iSrcLen - 1;

	if(szSrc == null || szFind == null){
		return 0;
	}

	iSrcLen = et_strlen(szSrc);
	iFindLen = et_strlen(szFind);
	iFromPos = iSrcLen - 1;

	if(iSrcLen < iFindLen){
		return -1;
	}

	for(i = iFromPos; i >= 0; i--){
		if(!bMatch){
			if(szSrc[i] == szFind[iFindLen - 1]){
				bMatch = 1;
				iMatchCount = 1;
				iTempPos = i;
			}else{
				continue;
			}
		}else{
			if(szSrc[i] == szFind[iFindLen - iMatchCount - 1]){
				iMatchCount++;
			}else{
				bMatch = 0;
				iMatchCount = 0;
			}
		}

		if(iMatchCount == iFindLen){
			return i;
		}
	}
	return -1;
}

int et_sz_find_safe(const char* szSrc, const char* szFind, UINT iLimit){
	UINT i = 0;
	bool bCheck = false;
	int iCheckPos = 0;
	char chTemp = 0;

	if(szSrc == null || szFind == null){
		return -1;
	}

	if(szSrc == null || szFind == null){
		return -1;
	}

	for(i = 0; i < iLimit; i++){
		chTemp = szSrc[i];
		if(bCheck){
			if(chTemp == szFind[iCheckPos + 1]){
				iCheckPos++;
			}else{
				bCheck = false;
				iCheckPos = 0;
			}
		}else{
			if(chTemp == szFind[0]){
				bCheck = true;
				iCheckPos = 0;
			}
		}

		if(bCheck && (UINT)(iCheckPos + 1) == et_strlen(szFind)){
			return i - iCheckPos;
		}
	}
	return -1;
}

int et_sz_is_end_with(const char* szSrc, const char* szFind){
	int iSrcLen = 0;
	int iFindLen = 0;
	int iStartPos = 0;

	if(szSrc == null){
		return false;
	}

	if(szSrc == szFind){
		return false;
	}

	iSrcLen = et_strlen(szSrc);
	iFindLen = et_strlen(szFind);
	if(iFindLen > iSrcLen){
		return false;
	}

	iStartPos = iSrcLen - iFindLen;

	if(et_sz_find_from(szSrc,szFind,iStartPos) >= 0){
		return true;
	}

	return false;


}
char* et_sz_repeat(char *szDest, const char* szSrc, int iCount ){
	int i = 0;
	int iLen = 0;
	int iResultLen = 0;

	if(szDest != null){
		szDest[0] = 0;
	}
	if(szSrc == null){
		return szDest;
	}

	iLen = et_strlen(szSrc);
	iResultLen = iLen * iCount;
	for(i = 0; i < iCount; i++){
		et_memcpy(szDest + (i * iLen),szSrc,iLen * sizeof(char));
	}
	szDest[iResultLen] = 0;
	return szDest;
}

char* et_sz_add_chars(char* szDest, char chAdd, UINT iAddCount){
	UINT i = 0;
	int iSrcLen = 0;

	if(szDest == null){
		return null;
	}

	if(iAddCount == 0){
		return szDest;
	}

	iSrcLen = et_strlen(szDest);
	for(i = 0; i < iAddCount; i++){
		szDest[iSrcLen + i] = chAdd;
	}
	szDest[iSrcLen + iAddCount] = 0;

	return szDest;
}

char* et_sz_reverse( char* szSrc ){
	int iSrcLen = 0;
	int i = 0;
	char chTemp = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen <= 1){
		return szSrc;
	}

	for(i = 0; i < iSrcLen / 2; i++){
		chTemp = szSrc[i];
		szSrc[i] = szSrc[iSrcLen - i - 1];
		szSrc[iSrcLen - i - 1] = chTemp;
	}

	return szSrc;
}

char* et_sz_lower( char* szSrc ){
	char *pMove = null;
	if(szSrc == null){
		return null;
	}
	pMove = szSrc;
	while(*pMove != 0){
		*pMove = et_char_lower(*pMove);
		pMove++;
	}
	return szSrc;
}

char* et_sz_upper( char* szSrc ){
	char *pMove = null;
	if(szSrc == null){
		return null;
	}
	pMove = szSrc;
	while(*pMove != 0){
		*pMove = et_char_upper(*pMove);
		pMove++;
	}
	return szSrc;
}

bool et_sz_equal( const char* szLeft, const char* szRight ){
	if(szLeft == null && szRight == null){
		return true;
	}else if( szLeft == null || szRight == null ){
		return false;
	}

	if(et_strcmp(szLeft,szRight) != 0){
		return false;
	}

	return true;
}

bool et_sz_equal_no_case( const char* szLeft, const char* szRight ){
	int iLeftLen = 0;
	int iRightLen = 0;
	int i = 0;

	if(szLeft == null && szRight == null){
		return true;
	}else if( szLeft == null || szRight == null ){
		return false;
	}

	iLeftLen = et_strlen(szLeft);
	iRightLen = et_strlen(szRight);
	if(iLeftLen != iRightLen){
		return false;
	}

	for(i = 0; i < iLeftLen; i++){
		if( !et_char_equal_no_case( szLeft[i], szRight[i])){
			return false;
		}
	}

	return true;
}

int et_sz_count_char( const char* szSrc, char chFind ){
	int iRetCount = 0;

	if(szSrc == null){
		return 0;
	}

	while(*szSrc != 0){
		if(*szSrc == chFind){
			iRetCount++;
		}
		szSrc++;
	}
	return iRetCount;
}

int et_sz_count_sz( const char* szSrc, const char* szFind ){
	UINT iRetCount = 0;
	bool bMatch = 0;
	int iFromPos = 0;
	UINT iSrcLen = 0;
	UINT iFindLen = 0;
	UINT i = 0;
	UINT iMatchCharCount = 0;

	if(szSrc == null || szFind == null){
		return 0;
	}

	iSrcLen = et_strlen(szSrc);
	iFindLen = et_strlen(szFind);

	for(i = iFromPos; i < iSrcLen; i++){
		if(!bMatch){
			if(szSrc[i] == szFind[0]){
				bMatch = true;
				iMatchCharCount = 1;
			}else{
				continue;
			}
		}else{
			if(szSrc[i] == szFind[iMatchCharCount]){
				iMatchCharCount++;
			}else{
				bMatch = false;
				iMatchCharCount = 0;
			}
		}

		if(iMatchCharCount == iFindLen){
			iRetCount++;
			iMatchCharCount = 0;
			bMatch = false;
		}
	}
	return iRetCount;
}

char* et_sz_prefix(char* szSrc, const char* szPrefix ){
	int iSrcLen = 0;
	int iPreLen = 0;
	int iResultLen = 0;

	if(szSrc == null){
		return null;
	}

	if(szPrefix == null){
		return szSrc;
	}

	iSrcLen = et_strlen(szSrc);
	iPreLen = et_strlen(szPrefix);
	iResultLen = iSrcLen + iPreLen;

	if(iPreLen == 0){
		return szSrc;
	}

	et_memmove(szSrc + iPreLen,szSrc,iSrcLen * sizeof(char));
	et_memcpy(szSrc,szPrefix,iPreLen * sizeof(char));
	szSrc[iResultLen] = 0;
	return szSrc;
}

char* et_sz_pad_left(char* szSrc, char chPad, int iTotalLen){
	int iSrcLen = 0;
	int iPadLen = 0;
	int i = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	iPadLen = iTotalLen - iSrcLen;

	if(iSrcLen >= iTotalLen){
		return szSrc;
	}

	et_memmove(szSrc + iPadLen,szSrc,iSrcLen * sizeof(char));

	for(i = 0; i < iPadLen; i++){
		szSrc[i] = chPad;
	}

	szSrc[iTotalLen] = 0;

	return szSrc;
}

char* et_sz_pad_right( char* szSrc, char chPad, int iTotalLen ){
	int iSrcLen = 0;
	int iPadLen = 0;
	int i = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	iPadLen = iTotalLen - iSrcLen;

	if(iSrcLen >= iTotalLen){
		return szSrc;
	}

	for(i = 0; i < iPadLen; i++){
		szSrc[i + iSrcLen] = chPad;
	}

	szSrc[iTotalLen] = 0;
	return szSrc;
}

char* et_sz_substr(char *szDest, const char *szSrc, UINT iStart, int iSubLen){
	UINT iSrcLen = 0;

	if(szSrc == null || szDest == null){
		return 0;
	}

	szDest[0] = 0;
	iSrcLen = et_strlen(szSrc);

	if(iSubLen == -1){
		iSubLen = iSrcLen - iStart;
	}

	if(iStart + iSubLen > iSrcLen){
		return null;
	}

	et_memcpy(szDest, szSrc + iStart, iSubLen * sizeof(char));
	szDest[iSubLen] = 0;
	return szDest;
}

char* et_sz_add_slashes( char* szSrc, char chFind ){
	int iSrcLen = 0;
	char *szCopy = null;
	int i = 0;
	int iPos = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	szCopy = (char*)et_alloc((iSrcLen * 2 + 1) * sizeof(char));
	if(szCopy == null){
		return null;
	}

	for(i = 0; i < iSrcLen; i++){
		if(szSrc[i] == chFind
			|| szSrc[i] == '\\')
		{
			szCopy[iPos++] = '\\';
			szCopy[iPos++] = szSrc[i];
		}else{
			szCopy[iPos++] = szSrc[i];
		}
	}
	szCopy[iPos++] = 0;
	et_strcpy(szSrc,szCopy);
	et_free(szCopy);
	return szSrc;
}

char* et_sz_strip_slashes( char* szSrc ){
	int iSrcLen = 0;
	char *szCopy = null;
	int i = 0;
	int iPos = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	szCopy = (char*)et_alloc((iSrcLen + 1) * sizeof(char));

	if(szCopy == null){
		return null;
	}

	for(i = 0; i < iSrcLen - 1; i++){
		if(szSrc[i] == '\\'){
			if(i == iSrcLen - 1){
				break;
			}else{
				i++;
				szCopy[iPos++] = szSrc[i];
			}
		}else{
			szCopy[iPos++] = szSrc[i];
		}
	}

	szCopy[iPos++] = 0;
	et_strcpy(szSrc,szCopy);
	et_free(szCopy);
	return szSrc;
}

char* et_sz_trim_left( char* szSrc ){
	int iSrcLen = 0;
	int iDestLen = 0;
	int iSpaceLen = 0;
	int i = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);

	if(iSrcLen == 0){
		return szSrc;
	}

	for(i = 0; i < iSrcLen; i++){
		if(et_char_is_space(szSrc[i])){
			iSpaceLen++;
			continue;
		}else{
			break;
		}
	}

	if(iSpaceLen == 0){
		return szSrc;
	}

	iDestLen = iSrcLen - iSpaceLen;
	et_memmove(szSrc,szSrc + iSpaceLen,iSrcLen - iSpaceLen + 1);
	szSrc[iDestLen] = 0;
	return szSrc;
}

char* et_sz_trim_right( char* szSrc ){
	int iSrcLen = 0;
	int i = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen == 0){
		return szSrc;
	}

	for(i = iSrcLen - 1; i >= 0; i--){
		if( et_char_is_space(szSrc[i]) ){
			szSrc[i] = 0;
		}else{
			break;
		}
	}

	return szSrc;
}

char* et_sz_trim( char* szSrc ){
	if(szSrc == null){
		return null;
	}

	et_sz_trim_left(szSrc);
	et_sz_trim_right(szSrc);
	return szSrc;
}

char* et_sz_trim_chars_left(char* szSrc, char chFind){
	int iSrcLen = 0;
	int iDestLen = 0;
	int iSpaceLen = 0;
	int i = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);

	if(iSrcLen == 0){
		return szSrc;
	}

	for(i = 0; i < iSrcLen; i++){
		if(szSrc[i] == chFind){
			iSpaceLen++;
			continue;
		}else{
			break;
		}
	}

	if(iSpaceLen == 0){
		return szSrc;
	}

	iDestLen = iSrcLen - iSpaceLen;
	et_memmove(szSrc,szSrc + iSpaceLen,iSrcLen - iSpaceLen + 1);
	szSrc[iDestLen] = 0;
	return szSrc;
}

char* et_sz_trim_chars_right( char* szSrc, char chFind){
	int iSrcLen = 0;
	int i = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen == 0){
		return szSrc;
	}


	for(i = iSrcLen - 1; i >= 0; i--){
		if( szSrc[i] == chFind){
			szSrc[i] = 0;
		}else{
			break;
		}
	}

	return szSrc;
}

char* et_sz_replace( char* szSrc, const char* szFind, const char* szReplace ){
	int iSrcLen = 0;
	int iFindLen = 0;
	int iReplaceLen = 0;
	int iTotalLen = 0;
	int iFindPos = 0;
	int iDiffLen = 0;

	if(szSrc == null){
		return null;
	}
	if(szFind == null){
		return szSrc;
	}
	if(szReplace == null){
		szReplace = ET_SZ_EMPTY;
	}

	iSrcLen = et_strlen(szSrc);
	iFindLen = et_strlen(szFind);
	iReplaceLen = et_strlen(szReplace);
	iTotalLen = iSrcLen;
	iDiffLen = iReplaceLen - iFindLen;

	while(1){
		iFindPos = et_sz_find_from(szSrc,szFind,iFindPos);
		if(iFindPos == -1){
			break;
		}else{
			et_sz_splice(szSrc,szReplace,iFindPos,iFindLen);
			iFindPos += iReplaceLen;
			iTotalLen += iDiffLen;
		}
	}

	szSrc[iTotalLen] = 0;
	return szSrc;
}

char* et_sz_cut(char* szSrc, UINT nStart, UINT nLen ){
	UINT iSrcLen = 0;
	UINT i = 0;
	UINT iDestLen = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	iDestLen = iSrcLen - nLen;
	if(nStart + nLen > iSrcLen){
		return szSrc;
	}

	for(i = nStart; i < iDestLen; i++){
		szSrc[i] = szSrc[i + nLen];
	}
	szSrc[iDestLen] = 0;
	return szSrc;
}

char* et_sz_insert( char* szSrc, const char* szInsert, UINT nStart ){
	UINT iSrcLen = 0;
	UINT iAppendLen = 0;
	UINT iDestLen = 0;
	UINT iRange = 0;
	UINT i = 0;

	if(szSrc == null){
		return null;
	}

	if(szInsert == null){
		szInsert = ET_SZ_EMPTY;
	}

	iSrcLen = et_strlen(szSrc);
	iAppendLen = et_strlen(szInsert);
	iDestLen = iSrcLen + iAppendLen;
	iRange = iSrcLen - nStart;

	if(nStart > iSrcLen){
		return szSrc;
	}

	for(i = 1; i <= iRange; i++){
		szSrc[iDestLen - i] = szSrc[iDestLen - i - iAppendLen];
	}

	for(i = 0; i < iAppendLen; i++){
		szSrc[nStart + i] = szInsert[i];
	}

	szSrc[iDestLen] = 0;
	return szSrc;
}

char* et_sz_splice( char* szSrc, const char* szInsert, UINT nStart, UINT nLen ){
	UINT iSrcLen = 0;
	if(szSrc == null){
		return null;
	}

	if(szInsert == null){
		szInsert = ET_SZ_EMPTY;
	}

	iSrcLen = et_strlen(szSrc);
	if(nStart < 0
		|| nLen < 0
		|| nStart + nLen > iSrcLen)
	{
		return szSrc;
	}
	et_sz_cut(szSrc,nStart,nLen);
	et_sz_insert(szSrc,szInsert,nStart);
	return szSrc;
}

bool et_sz_is_int( const char* szSrc ){
	int iSrcLen = 0;
	int i = 0;

	if(szSrc == null){
		return false;
	}

	iSrcLen = et_strlen(szSrc);

	if(iSrcLen == 0){
		return false;
	}

	if(szSrc[0] != '-' && szSrc[0] != '+' && !et_char_is_num(szSrc[0])){
		return false;
	}

	for(i = 1; i < iSrcLen; i++){
		if(!et_char_is_num(szSrc[i])){
			return false;
		}
	}
	return true;
}

bool et_sz_is_hex( const char* szSrc ){
	const char *szValid = "abcdefABCDEF0123456789";
	int iSrcLen = 0;
	int i = 0;

	if(szSrc == null || et_strlen(szSrc) == 0){
		return false;
	}

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen == 0){
		return false;
	}

	if( et_strcmp(szSrc,"0x")==0
		|| et_strcmp(szSrc,"0X") == 0){
		return false;
	}

	for(i = 0; i < iSrcLen; i++){
		if(et_sz_find_char(szValid,szSrc[i],0) == -1){
			if(i == 1 && (szSrc[i] == 'x' || szSrc[i] == 'X')){
				continue;
			}
			return false;
		}
	}

	return true;
}

bool et_sz_is_num( const char* szSrc ){
	UINT iSrcLen = et_strlen(szSrc);
	UINT i = 0;
	char chTemp = 0;
	int iDotCount = 0;

	if(szSrc == null){
		return false;
	}

	iSrcLen = et_strlen(szSrc);

	if(iSrcLen == 0){
		return false;
	}else if(iSrcLen > 1){
		for(i = 1; i < iSrcLen; i++){
			chTemp = *(szSrc + i);
			if(chTemp == '.'){
				iDotCount++;
				if(iDotCount > 1){
					return false;
				}
			}
			if( !et_char_is_num(chTemp) && chTemp != '.'){
				return false;
			}
		}

		if(chTemp == '.'){
			return false;
		}

		chTemp = *szSrc;
		if( !et_char_is_num(chTemp) ){
			if(chTemp != '-' && chTemp != '+'){
				return false;
			}
		}
	}else if(iSrcLen == 1){
		chTemp = *szSrc;
		if( !et_char_is_num(chTemp) ){
			return false;
		}
	}
	return true;
}

bool et_sz_is_domain( const char* szSrc ){
	int iSrcLen = et_strlen(szSrc);
	int i = 0;
	char chTemp = 0;
	int iDotPos = 0;

	if(szSrc == null){
		return false;
	}

	iSrcLen = et_strlen(szSrc);

	if(iSrcLen < 4
		|| iSrcLen > 50){
		return false;
	}

	for(i = 0; i < iSrcLen; i++){
		chTemp = szSrc[i];
		if(et_char_is_num(chTemp)
			|| et_char_is_letter(chTemp)
			|| chTemp == '-'
			|| chTemp == '.'
			)
		{
			continue;
		}else{
			return false;
		}
	}

	if(et_sz_count_sz(szSrc,".") == 0){
		return false;
	}

	iDotPos = et_sz_find(szSrc,".");

	if(iDotPos == -1){
		return false;
	}

	if(szSrc[iSrcLen - 1] == '.'
		|| szSrc[0] == '.')
	{
		return false;
	}

	return true;
}

bool et_sz_is_email( const char* szSrc ){
	int iSrcLen = 0;
	int i = 0;
	char chTemp = 0;
	int iAtPos = 0;
	int iDotPos = 0;

	if(szSrc == null){
		return false;
	}

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen < 6
		|| iSrcLen > 50){
		return false;
	}

	for(i = 0; i < iSrcLen; i++){
		chTemp = szSrc[i];
		if(et_char_is_num(chTemp)
			|| et_char_is_letter(chTemp)
			|| chTemp == '-'
			|| chTemp == '_'
			|| chTemp == '@'
			|| chTemp == '.'
			)
		{
			continue;
		}else{
			return false;
		}
	}

	if(et_sz_count_sz(szSrc,"@") != 1){
		return false;
	}

	iAtPos = et_sz_find(szSrc,"@");

	if(iAtPos == 0
		|| iAtPos > iSrcLen - 5){
		return false;
	}

	if(et_sz_count_sz(szSrc,".") == 0){
		return false;
	}

	iDotPos = et_sz_find(szSrc,".");

	if(iDotPos == -1){
		return false;
	}

	if(szSrc[iSrcLen - 1] == '.'
		|| szSrc[0] == '.')
	{
		return false;
	}

	return true;
}

bool et_sz_is_ip( const char* szSrc ){
	int iSrcLen = 0;
	int i = 0;
	char chTemp = 0;

	if(szSrc == null){
		return false;
	}

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen < 7
		|| iSrcLen > 15){
		return false;
	}

	for(i = 0; i < iSrcLen; i++){
		chTemp = szSrc[i];
		if(et_char_is_num(chTemp)
			|| chTemp == '.')
		{
			continue;
		}else{
			return false;
		}
	}


	if(et_sz_count_sz(szSrc,".") != 3){
		return false;
	}

	if(szSrc[iSrcLen - 1] == '.'
		|| szSrc[0] == '.'){
		return false;
	}

	return true;
}

bool et_sz_is_url( const char* szSrc ){
	int iSrcLen = 0;
	int i = 0;
	char chTemp = 0;
	int iDotPos = 0;
	int iProtocolPos = 0;
	const char* szValid = "~!#$%^&()_-+={[}]|:;,./\"'";

	if(szSrc == null){
		return false;
	}

	iSrcLen = et_strlen(szSrc);

	if(iSrcLen < 10
		|| iSrcLen > 255){
		return false;
	}

	for(i = 0; i < iSrcLen; i++){
		chTemp = szSrc[i];
		if(et_char_is_num(chTemp)
			|| et_char_is_letter(chTemp)
			|| et_sz_find_char(szValid,chTemp,0) != -1
			)
		{
			continue;
		}else{
			return false;
		}
	}

	if(et_sz_count_sz(szSrc,".") == 0){
		return false;
	}

	iDotPos = et_sz_find(szSrc,".");

	if(iDotPos == -1){
		return false;
	}

	if(szSrc[iSrcLen - 1] == '.'
		|| szSrc[0] == '.'){
		return false;
	}

	iProtocolPos = et_sz_find(szSrc,"://");

	if(iProtocolPos == -1
		|| iProtocolPos < 3
		|| iProtocolPos > 10)
	{
		return false;
	}

	return true;
}

bool et_sz_is_ascii( const char* szSrc ){
	int iSrcLen = 0;
	int i = 0;

	if(szSrc == null){
		return false;
	}

	iSrcLen = et_strlen(szSrc);
	for(i = 0; i < iSrcLen; i++){
		if( !et_char_is_ascii( (BYTE)szSrc[i] )){
			return false;
		}
	}
	return true;
}

bool et_sz_is_limited_in( const char* szSrc, const char* szLimited ){
	int iSrcLen = 0;
	char chTemp = 0;
	int i = 0;

	if(szSrc == null){
		return false;
	}
	if(szLimited == null){
		return false;
	}

	iSrcLen = et_strlen(szSrc);
	for(i = 0; i < iSrcLen; i++){
		chTemp = szSrc[i];
		if(et_sz_find_char(szLimited,chTemp,0) == -1){
			return false;
		}
	}
	return true;
}

bool et_sz_is_datetime( const char* szSrc ){
	int iSrcLen = 0;

	if(szSrc == null){
		return false;
	}

	iSrcLen = et_strlen(szSrc);

	if(iSrcLen != 19){
		return false;
	}
	if(szSrc[4] != '-'){
		return false;
	}
	if(szSrc[7] != '-'){
		return false;
	}
	if(szSrc[10] != ' '){
		return false;
	}
	if(szSrc[13] != ':'){
		return false;
	}
	if(szSrc[16] != ':'){
		return false;
	}
	if( !et_char_is_num(szSrc[0])
		|| !et_char_is_num(szSrc[18]) ){
		return false;
	}

	return true;
}

char* et_sz_dec2bin(char* szSrc){
	char szTemp[255] = "";
	INT64 iNum = 0;
	INT64 iTempQuotient = 0;
	INT64 iTempRemainder = 0;
	int iPos = 0;

	if(szSrc == null){
		return null;
	}

	iNum = et_sz_to_long(szSrc);
	iTempQuotient = iNum;

	if(iNum == 0){
		szSrc[0] = '0';
		szSrc[1] = 0;
		return szSrc;
	}

	while(iTempQuotient >> 1 != 0){
		iTempRemainder = iTempQuotient & 1;
		iTempQuotient = iTempQuotient >> 1;

		if(iTempRemainder == 0){
			szTemp[iPos++] = '0';
		}else{
			szTemp[iPos++] = '1';
		}
	}
	if(iTempQuotient > 0){
		szTemp[iPos++] = '1';
	}
	szTemp[iPos++] = 0;
	et_sz_reverse(szTemp);
	et_strcpy(szSrc,szTemp);
	return szSrc;
}

char* et_sz_bin2dec(char* szSrc){
	int iSrcLen = 0;
	int i = 0;
	INT64 iSum = 0;
	int iPow = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);

	if(iSrcLen <= 0){
		return szSrc;
	}

	for(i = iSrcLen - 1; i >= 0; i--){
		if(szSrc[i] == '1'){
			iSum += et_math_power(2,iPow);
		}else if(szSrc[i] != '0'){
			szSrc[0] = '0';
			szSrc[1] = 0;
			return szSrc;
		}
		iPow++;

	}
	et_sz_by_long(szSrc,iSum);

	return szSrc;
}

char* et_sz_hex2dec(char* szSrc){
	int iSrcLen = 0;
	int i = 0;
	INT64 iSum = 0;
	int iPow = 0;
	int iDigit = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	for(i = iSrcLen - 1; i >= 0; i--){
		if( et_sz_find_char(szSrc,szSrc[i],0) == -1 ){
			szSrc[0] = '0';
			szSrc[1] = 0;
			return szSrc;
		}

		iDigit = et_char_hex_to_int(szSrc[i]);
		if(szSrc[i] != '0'){
			iSum += ( iDigit * et_math_power(16,iPow) );
		}
		iPow++;
	}
	et_sz_by_long(szSrc,iSum);
	return szSrc;
}

char* et_sz_dec2hex(char* szSrc){
	char szTemp[255] = "";
	INT64 iNum = 0;
	INT64 iTempQuotient = 0;
	INT64 iTempRemainder = 0;
	int iPos = 0;

	if(szSrc == null){
		return null;
	}

	iNum = et_sz_to_long(szSrc);
	iTempQuotient = iNum;

	if(iNum == 0){
		szSrc[0] = '0';
		szSrc[1] = 0;
		return szSrc;
	}

	while(iTempQuotient >> 4 != 0){
		iTempRemainder = iTempQuotient & 0xF;
		iTempQuotient = iTempQuotient >> 4;

		if(iTempRemainder == 0){
			szTemp[iPos++] = '0';
		}else{
			szTemp[iPos++] = et_char_int_to_hex(iTempRemainder);
		}
	}
	if(iTempQuotient > 0){
		szTemp[iPos++] = et_char_int_to_hex(iTempQuotient);
	}
	szTemp[iPos++] = 0;
	et_sz_reverse(szTemp);
	et_strcpy(szSrc,szTemp);
	return szSrc;
}

char* et_sz_bin2hex(char* szSrc){
	if(szSrc == null){
		return null;
	}
	et_sz_bin2dec(szSrc);
	et_sz_dec2hex(szSrc);
	return szSrc;
}

char* et_sz_hex2bin( char* szSrc )
{
	if(szSrc == null){
		return null;
	}
	et_sz_hex2dec(szSrc);
	et_sz_dec2bin(szSrc);
	return szSrc;
}

char* et_sz_format_num( char* szSrc, UINT nLenAfterPoint){
	char chNext = '0';
	int iSrcLen = 0;
	int iDotPos = 0;
	UINT iSrcDecLen = 0;
	int iNewInt = 0;
	int iNextNum = 0;
	char szDest[255] = "";
	char szPlusZeros[255] = "";
	char szIntPart[255] = "";
	char szDecPart[255] = "";
	char szNewInt[255] = "";
	char szNewDec[255] = "";

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen == 0){
		return szSrc;
	}

	iDotPos = et_sz_find(szSrc,".");

	if(iDotPos == -1){
		et_sz_repeat(szPlusZeros,"0",nLenAfterPoint);
		et_strcpy(szDest,szSrc);
		et_strcat(szDest,".");
		et_strcat(szDest,szPlusZeros);
	}else{
		et_sz_substr(szIntPart,szSrc,0,iDotPos);
		et_sz_substr(szDecPart,szSrc,iDotPos + 1,iSrcLen - 1 - iDotPos);
		iSrcDecLen = et_strlen(szDecPart);
		if(iSrcDecLen == nLenAfterPoint){
			et_strcpy(szDest,szSrc);
		}else if(iSrcDecLen < nLenAfterPoint){
			et_sz_repeat(szPlusZeros,"0",nLenAfterPoint - iSrcDecLen);
			et_strcpy(szDest,szSrc);
			et_strcat(szDest,szPlusZeros);
		}else{
			et_strcpy(szNewInt,szIntPart);
			et_strncat(szNewInt,szDecPart,nLenAfterPoint);
			iNewInt = et_sz_to_int(szNewInt);
			chNext = szDecPart[nLenAfterPoint];
			iNextNum = et_char_to_int(chNext);

			if(iNextNum >= 5){
				iNewInt++;
			}

			et_sz_by_int(szNewInt,iNewInt);

			if(nLenAfterPoint == 0){
				et_strcpy(szDest,szNewInt);
			}else{
				if(et_strlen(szNewInt) <= nLenAfterPoint){
					et_sz_repeat(szPlusZeros,"0",nLenAfterPoint - et_strlen(szNewInt) + 1);
					et_sz_prefix(szPlusZeros,szNewInt);
				}

				et_sz_substr(szNewDec,szNewInt,et_strlen(szNewInt) - nLenAfterPoint,nLenAfterPoint);
				et_strncpy(szDest,szNewInt,et_strlen(szNewInt) - nLenAfterPoint);
				et_strcat(szDest,".");
				et_strcat(szDest,szNewDec);
			}
		}
	}

	et_strcpy(szSrc,szDest);
	return szSrc;
}

void et_sz_alloc_array( char ***szArr, int iRow, int iCol ){
	int i = 0;
	*szArr = (char**)et_alloc(sizeof(char*) * iRow);
	for(i = 0; i < iRow; i++){
		(*szArr)[i] = (char*)et_alloc(sizeof(char) * iCol);
		et_memset((*szArr)[i], 0, iCol);
	}
}

void et_sz_et_free_array( char ***szArr, int iRow ){
	int i = 0;
	if(*szArr == null){
		return;
	}
	for(i = 0; i < iRow; i++){
		if((*szArr)[i] == null){
			continue;
		}
		et_free((*szArr)[i] );
	}
	et_free(*szArr);
}

INT64 et_sz_ip_text2num( const char* szIpText ){
	INT64 iTotal = 0;
	int iPointPos0 = -1;
	int iPointPos1 = 0;
	int iPointPos2 = 0;
	int iPointPos3 = 0;
	char szPart0[5] = "";
	char szPart1[5] = "";
	char szPart2[5] = "";
	char szPart3[5] = "";

	INT64 iPart0 = 0;
	INT64 iPart1 = 0;
	INT64 iPart2 = 0;
	INT64 iPart3 = 0;

	if(szIpText == null){
		return 0;
	}

	if(!et_sz_is_ip(szIpText)){
		return 0;
	}

	iPointPos1 = et_sz_find_from(szIpText,".",0);
	if(iPointPos1 == -1){
		return 0;
	}

	iPointPos2 = et_sz_find_from(szIpText,".",iPointPos1 + 1);
	if(iPointPos2 == -1){
		return 0;
	}

	iPointPos3 = et_sz_find_from(szIpText,".",iPointPos2 + 1);
	if(iPointPos3 == -1){
		return 0;
	}

	et_sz_substr(szPart0, szIpText,iPointPos0 + 1,iPointPos1 - iPointPos0 - 1);
	et_sz_substr(szPart1, szIpText,iPointPos1 + 1,iPointPos2 - iPointPos1 - 1);
	et_sz_substr(szPart2, szIpText,iPointPos2 + 1,iPointPos3 - iPointPos2 - 1);
	et_sz_substr(szPart3, szIpText,iPointPos3 + 1, -1);

	iPart0 = et_sz_to_long(szPart0);
	iPart1 = et_sz_to_long(szPart1);
	iPart2 = et_sz_to_long(szPart2);
	iPart3 = et_sz_to_long(szPart3);

	iTotal += (iPart0 << 24);
	iTotal += (iPart1 << 16);
	iTotal += (iPart2 << 8);
	iTotal += iPart3;

	return iTotal;
}

char* et_sz_ip_num2text( char *szDest, INT64 iIpNum ){
	char szPart0[5] = "";
	char szPart1[5] = "";
	char szPart2[5] = "";
	char szPart3[5] = "";

	INT64 iTotal = 0;
	INT64 iPart0 = 0;
	INT64 iPart1 = 0;
	INT64 iPart2 = 0;
	INT64 iPart3 = 0;

	if(szDest != null){
		szDest[0] = 0;
	}

	if(szDest == null){
		return null;
	}

	iTotal = iIpNum;
	iPart0 = (iTotal >> 24);
	iPart1 = (iTotal >> 16) & 0x00FF;
	iPart2 = (iTotal >>  8) & 0x0000FF;
	iPart3 = (iTotal >>  0) & 0x000000FF;

	szDest[0] = 0;

	et_sz_by_long(szPart0,iPart0);
	et_sz_by_long(szPart1,iPart1);
	et_sz_by_long(szPart2,iPart2);
	et_sz_by_long(szPart3,iPart3);

	et_strcat(szDest,szPart0);
	et_strcat(szDest,".");
	et_strcat(szDest,szPart1);
	et_strcat(szDest,".");
	et_strcat(szDest,szPart2);
	et_strcat(szDest,".");
	et_strcat(szDest,szPart3);

	return szDest;
}

char* et_sz_file_dir( char* szDest, const char* szSrc ){
	int nSlashPos = 0;
	int iLen = 0;

	if(szSrc == null || szDest == null){
		return null;
	}

	szDest[0] = 0;
	et_strcpy(szDest,szSrc);
	et_sz_replace(szDest,"\\","/");
	iLen = et_strlen(szDest);
	if(szDest[iLen - 1] == '/'){
		szDest[iLen - 1] = 0;
	}
	nSlashPos = et_sz_find_reverse(szDest,"/");
	if(nSlashPos == -1){
		szDest[0] = 0;
		return szDest;
	}
	iLen = et_strlen(szDest);
	et_sz_cut(szDest,nSlashPos + 1,iLen - nSlashPos - 1);
	return szDest;
}

char* et_sz_file_name( char* szDest, const char* szSrc ){
	int nSlashPos = 0;
	if(szSrc == null || szDest == null){
		return null;
	}
	szDest[0] = 0;
	et_strcpy(szDest,szSrc);
	et_sz_replace(szDest,"\\","/");
	nSlashPos = et_sz_find_reverse(szDest,"/");
	if(nSlashPos == -1){
		return szDest;
	}
	et_sz_cut(szDest,0,nSlashPos + 1);
	return szDest;
}

char* et_sz_file_end( char* szDest, const char* szSrc ){
	int nSlashPos = 0;
	int iLen = 0;

	if(szSrc == null || szDest == null){
		return null;
	}
	szDest[0] = 0;
	et_strcpy(szDest,szSrc);
	et_sz_replace(szDest,"\\","/");
	iLen = et_strlen(szDest);
	if(szDest[iLen - 1] == '/'){
		szDest[iLen - 1] = 0;
	}
	nSlashPos = et_sz_find_reverse(szDest,"/");
	if(nSlashPos == -1){
		return szDest;
	}
	et_sz_cut(szDest,0,nSlashPos + 1);
	return szDest;
}

char* et_sz_file_title( char* szDest, const char* szSrc ){
	int nDotPos = 0;

	if(szSrc == null || szDest == null){
		return null;
	}

	szDest[0] = 0;
	et_sz_file_name(szDest,szSrc);
	nDotPos = et_sz_find_reverse(szDest,".");
	if(nDotPos == -1){
		//no prefix
		return szDest;
	}else if(nDotPos == 0){
		//like ".config"
		return szDest;
	}
	et_sz_cut(szDest,nDotPos,et_strlen(szDest) - nDotPos);
	return szDest;
}

char* et_sz_file_ext( char* szDest, const char* szSrc ){
	int nDotPos = 0;

	if(szSrc == null || szDest == null){
		return null;
	}

	szDest[0] = 0;
	et_strcpy(szDest,szSrc);
	nDotPos = et_sz_find_reverse(szDest,".");
	if(nDotPos == -1){
		return szDest;
	}
	et_sz_cut(szDest,0,nDotPos);
	return szDest;
}

char* et_sz_file_fix_dir( char* szDest, const char* szSrc ){
	int iLen = 0;

	if(szSrc == null || szDest == null){
		return null;
	}
	szDest[0] = 0;

	iLen = et_strlen(szSrc);
	if(iLen == 0){
		return szDest;
	}
	et_strcpy(szDest,szSrc);
	et_sz_replace(szDest,"\\","/");
	iLen = et_strlen(szDest);
	if(iLen == 1){
		return szDest;
	}
	if(szDest[iLen - 1] == '/'){
		szDest[iLen - 1] = 0;
	}

	return szDest;
}

char* et_sz_url_protocol( char *szDest, const char* szSrc ){
	int iColonPos = 0;
	int iSrcLen = 0;
	if(szSrc == null || szDest == null){
		return null;
	}
	szDest[0] = 0;

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen < 3){
		return szDest;
	}
	iColonPos = et_sz_find(szSrc,":/");
	if(iColonPos == -1){
		return szDest;
	}
	et_sz_substr(szDest,szSrc,0,iColonPos);

	return szDest;
}

char* et_sz_url_host( char *szDest, const char* szSrc ){
	int iBegin = 0;
	int iEnd = 0;
	int iFindPos = 0;
	int iSrcLen = 0;
	int iPortColonPos = 0;

	if(szSrc == null || szDest == null){
		return null;
	}
	szDest[0] = 0;

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen < 3){
		return szDest;
	}

	iFindPos = et_sz_find(szSrc,":/");
	if(iFindPos == -1){
		iBegin = 0;
	}else{
		iBegin = iFindPos + 2;
		if(szSrc[iBegin] == '/'){
			iBegin++;
		}
	}

	iFindPos = et_sz_find_from(szSrc,"/",iBegin + 1);
	if(iFindPos == -1){
		iEnd = iSrcLen - 1;
	}else{
		iEnd = iFindPos - 1;
	}

	et_sz_substr(szDest,szSrc,iBegin,iEnd - iBegin + 1);
	iPortColonPos = et_sz_find(szDest,":");
	if(iPortColonPos != -1){
		szDest[iPortColonPos] = 0;
	}
	return szDest;
}

USHORT et_sz_url_port( const char* szSrc ){
	USHORT iRetPort = 0;
	int iBegin = 0;
	int iEnd = 0;
	int iFindPos = 0;
	int iSrcLen = 0;
	char szTemp[20] = "";

	if(szSrc == null){
		return 0;
	}

	iSrcLen = et_strlen(szSrc);

	if(iSrcLen < 3){
		return 0;
	}

	iFindPos = et_sz_find(szSrc,":/");
	if(iFindPos == -1){
		iFindPos = et_sz_find(szSrc,":");
	}else{
		iFindPos = et_sz_find_from(szSrc,":",iFindPos + 1);
	}

	if(iFindPos == -1){
		return 80;
	}else{
		iBegin = iFindPos + 1;
	}

	iFindPos = et_sz_find_from(szSrc,"/",iBegin + 1);
	if(iFindPos == -1){
		iEnd = iSrcLen - 1;
	}else{
		iEnd = iFindPos - 1;
	}

	if(iEnd > iBegin)
	{
		et_sz_substr(szTemp,szSrc,iBegin,iEnd - iBegin + 1);
		iRetPort = et_sz_to_int(szTemp);
	}else{
		iRetPort = 80;
	}

	return iRetPort;
}

char* et_sz_url_path( char *szDest, const char* szSrc ){
	int iSrcLen = 0;
	int iFindPos = 0;

	if(szSrc == null || szDest == null){
		return null;
	}
	szDest[0] = 0;

	iSrcLen = et_strlen(szSrc);

	iFindPos = et_sz_find_from(szSrc,":/",0);
	if(iFindPos != -1){
		iFindPos += 3;
		if(szSrc[iFindPos] == '/'){
			iFindPos++;
		}
	}
	iFindPos = et_sz_find_from(szSrc,"/",iFindPos);
	if(iFindPos == -1){
		et_strcpy(szDest,"/");
	}else{
		et_sz_substr(szDest,szSrc,iFindPos,-1);
	}

	iFindPos = et_sz_find(szDest,"?");
	if(iFindPos != -1){
		szDest[iFindPos] = 0;
	}

	return szDest;
}

char* et_sz_url_param( char *szDest, const char* szSrc ){
	int iSrcLen = et_strlen(szSrc);
	int iFindPos = 0;

	if(szSrc == null || szDest == null){
		return null;
	}

	szDest[0] = 0;
	iSrcLen = et_strlen(szSrc);
	iFindPos = et_sz_find_from(szSrc,"?",0);
	if(iFindPos == -1){
		return szDest;
	}
	et_sz_substr(szDest,szSrc,iFindPos + 1,-1);

	return szDest;
}

char* et_sz_url_path_with_param( char *szDest, const char* szSrc ){
	int iSrcLen = 0;
	int iFindPos = 0;

	if(szSrc == null || szDest == null){
		return null;
	}

	szDest[0] = 0;
	iSrcLen = et_strlen(szSrc);
	iFindPos = et_sz_find_from(szSrc,":/",0);
	if(iFindPos != -1){
		iFindPos += 3;
		if(szSrc[iFindPos] == '/'){
			iFindPos++;
		}
	}else{
		iFindPos = 0;
	}

	iFindPos = et_sz_find_from(szSrc,"/",iFindPos);
	if(iFindPos == -1){
		return szDest;
	}

	et_sz_substr(szDest,szSrc,iFindPos,-1);

	return szDest;
}

char* et_sz_hex_increase( char *szSrc ){
	int iSrcLen = 0;
	int iDestLen = 0;
	int iDecimal = 0;
	char szTemp[50] = "";

	if(!et_sz_is_hex(szSrc)){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	et_strcpy(szTemp,szSrc);
	et_sz_trim_chars_left(szTemp,'0');
	et_sz_hex2dec(szTemp);
	iDecimal = et_sz_to_long(szTemp);
	iDecimal++;
	et_sz_by_long(szTemp,iDecimal);
	et_sz_dec2hex(szTemp);
	iDestLen = et_strlen(szTemp);
	if(iDestLen > iSrcLen){
		et_sz_repeat(szSrc,"0",iSrcLen);
		return szSrc;
	}
	et_sz_pad_left(szTemp,'0',iSrcLen);
	et_strcpy(szSrc,szTemp);
	return szSrc;
}

int et_sz_hex_to_bytes( BYTE* pByte, const char* szStr ){
	int iStrLen = 0;
	char szTemp[3] = "";
	char szDec[50] = "";
	int iTempDec = 0;
	int iByteCount = 0;
	int i = 0;

	if(szStr == null || pByte == null){
		return 0;
	}

	iStrLen = et_strlen(szStr);

	for(i = 0; i < iStrLen; i++){
		if(i == iStrLen - 1){
			break;
		}
		et_memset(szTemp,0,3 * sizeof(char));
		et_memset(szDec,0,50 * sizeof(char));
		et_strncpy(szTemp,szStr + i,2);
		et_strcpy(szDec,szTemp);
		et_sz_hex2dec(szDec);
		iTempDec = et_sz_to_int(szDec);
		pByte[iByteCount++] = (BYTE)iTempDec;
		i++;
	}
	return iByteCount;
}

char* et_sz_hex_from_bytes( char* szHex, const BYTE* pBytes, int iByteCount ){
	char szTemp[10] = "";
	int iTempInt = 0;
	int i = 0;

	if(szHex == null || pBytes == null){
		return null;
	}

	szHex[0] = 0;
	for(i = 0; i < iByteCount; i++){
		iTempInt = pBytes[i];
		et_memset(szTemp,0,10 * sizeof(char));
		et_sz_by_int(szTemp,iTempInt);
		et_sz_dec2hex(szTemp);
		et_sz_pad_left(szTemp,'0',2);
		et_strcat(szHex,szTemp);
	}
	return szHex;
}

char* et_sz_delimit( char* szSrc, int iUnitLen, char chDeimiter ){
	int iSrcLen = 0;
	char *szCopy = null;
	int iPos = 0;
	int i = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen == 0){
		return szSrc;
	}

	szCopy = (char*)et_alloc( (iSrcLen * 2 + 1) * sizeof(char) );
	et_memset(szCopy,0,(iSrcLen * 2 + 1) * sizeof(char));
	et_strcpy(szCopy,szSrc);

	for(i = 0; i < iSrcLen / iUnitLen; i++){
		if(i != 0){
			szCopy[iPos++] = chDeimiter;
		}
		szCopy[iPos++] = szSrc[i*2];
		szCopy[iPos++] = szSrc[i*2+1];
	}
	et_strcpy(szSrc,szCopy);
	et_free(szCopy);
	return szSrc;
}

char* et_sz_fix_time(char* szDest, const char* szSrc){
	int iSrcLen = 0;

	if(szSrc == null || szDest == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	et_strcpy(szDest,szSrc);
	if(iSrcLen == 4){
		et_strcat(szDest,"-00-00 00:00:00");
	}else if(iSrcLen == 7){
		et_strcat(szDest,"-00 00:00:00");
	}else if(iSrcLen == 10){
		et_strcat(szDest," 00:00:00");
	}else if(iSrcLen == 13){
		et_strcat(szDest,":00:00");
	}else if(iSrcLen == 16){
		et_strcat(szDest,":00");
	}

	return szDest;
}

char* et_sz_html_enc(char* szDest, const char* szSrc){
	if(szDest == null || szSrc == null){
		return 0;
	}

	et_strcpy(szDest,szSrc);

	et_sz_replace(szDest,"<","&lt;");
	et_sz_replace(szDest,">","&gt;");
	et_sz_replace(szDest,"&","&amp;");
	et_sz_replace(szDest,"\"","&quot;");
	et_sz_replace(szDest," ","&nbsp;");
	et_sz_replace(szDest,"<","&lt;");

	return szDest;
}
char* et_sz_html_dec(char* szDest, const char* szSrc){
	if(szDest == null || szSrc == null){
		return 0;
	}

	et_strcpy(szDest,szSrc);

	et_sz_replace(szDest,"&lt;","<");
	et_sz_replace(szDest,"&gt;",">");
	et_sz_replace(szDest,"&amp;","&");
	et_sz_replace(szDest,"&quot;","\"");
	et_sz_replace(szDest,"&nbsp;"," ");
	et_sz_replace(szDest,"&lt;","<");

	return null;
}

int et_sz_split(EtLists* pList, const char* szSrc, const char* szDelimiter){
	int iResult = 0;
	int iSrcLen = 0;
	int nStart = 0;
	int nCount = 0;
	int nPos = 0;
	int nDelimiterLen = 0;
	char szTemp[40960] = {0};

	if(pList == null || szSrc == null || szDelimiter == NULL){
		return ET_ERR_MEM_NULL;
	}

	et_lists_clear(pList);
	nDelimiterLen = et_strlen(szDelimiter);

	while(true){
		nPos = et_sz_find_from(szSrc,szDelimiter,nStart);
		if(nPos == -1){
			break;
		}
		nCount++;
		et_sz_substr(szTemp,szSrc,nStart,nPos - nStart);
		et_lists_add(pList,szTemp);
		nStart = nPos + nDelimiterLen;
	}
	nCount++;
	et_sz_substr(szTemp,szSrc,nStart,-1);
	et_lists_add(pList,szTemp);
	return nCount;
}


char* et_sz_join(EtLists* pList, char* szDest, const char* szDelimiter){
	char szTemp[40960] = {0};
	int iListSize = 0;
	int i = 0;

	if(szDelimiter == null || szDest == null || pList == null){
		return null;
	}

	szDest[0] = 0;

	iListSize = et_lists_size(pList);

	for(i = 0; i < iListSize; i++){
		if(i != 0){
			et_strcat(szDest,szDelimiter);
		}
		et_lists_get(pList,i,szTemp);
		et_strcat(szDest,szTemp);
	}

	return szDest;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ET_URLENCODE_CHARS "!#$%&'()*+,./:;=?"

static char et_url_2_to_1(const char * szCode) {
	unsigned char chDest = 0;
	int iLeft = 0;
	int iRight = 0;

	if(szCode == null){
		return -1;
	}

	iLeft = et_char_hex_to_int(szCode[0]);
	iRight = et_char_hex_to_int(szCode[1]);
	if(iLeft == -1 || iRight == -1){
		return -1;
	}
	
	chDest = (iLeft << 4) + iRight;

	return chDest;
}

char* et_url_encode(char *szDest, const char *szSrc, bool bEncodeAll){
	int i = 0;
	int j = 0;
	unsigned char chSrcTemp;
	int iSrcLen = 0;

	if(szDest == null || szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen == 0){
		szDest[0] = 0;
		return szDest;
	}

	for(i = 0; i < iSrcLen; i++){
		chSrcTemp = szSrc[i];
		if(et_sz_find_char(ET_URLENCODE_CHARS, chSrcTemp, 0) != -1){
			if(bEncodeAll){
				szDest[j++] = '%';
				szDest[j++] = et_char_int_to_hex( (chSrcTemp >> 4) & 0x0F );
				szDest[j++] = et_char_int_to_hex( chSrcTemp & 0x0F );
			}else{
				szDest[j++] = chSrcTemp;
			}
		}else if (chSrcTemp == ' '){
			szDest[j++] = '+';
		}else if (et_char_is_num(chSrcTemp) || et_char_is_letter(chSrcTemp) ){
			szDest[j++] = chSrcTemp;
		}else{
			szDest[j++] = '%';
			szDest[j++] = et_char_int_to_hex( (chSrcTemp >> 4) & 0x0F );
			szDest[j++] = et_char_int_to_hex( chSrcTemp & 0x0F );
		}
	}

	szDest[j] = 0;
	return szDest;
}

char* et_url_decode(char *szDest, const char *szSrc){
	int i = 0;
	int j = 0;
	unsigned char chSrcTemp;
	char chDestTemp;
	int iSrcLen = 0;

	if(szDest == null || szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen == 0){
		szDest[0] = 0;
		return szDest;
	}

	for(i = 0; i < iSrcLen; i++){
		chSrcTemp = szSrc[i];
		if(chSrcTemp == '+'){
			szDest[j++] = ' ';
		}else if(chSrcTemp == '%'){
			if(iSrcLen - i <= 2){
				break;
			}

			chDestTemp = et_url_2_to_1(szSrc + i + 1);
			if(chDestTemp < 0){
				szDest[j++] = '?';
			}else{
				szDest[j++] = chDestTemp;
			}

			i += 2;
		}else{
			szDest[j++] = chSrcTemp;
		}
	}

	szDest[j] = 0;
	return szDest;
}


//////////////////////////////////////////////////////////////


#ifdef ET_IN_KERNEL
#else
#include <stdlib.h>
#endif //#ifdef ET_IN_KERNEL

INT64 et_limit_between(INT64 iNum, INT64 iMin, INT64 iMax){
	if(iNum < iMin){
		return iMin;
	}else if(iNum > iMax){
		return iMax;
	}else{
		return iNum;
	}
}

void* et_alloc(unsigned int iSize){
	#ifdef ET_IN_KERNEL
		return kalloc(iSize);
	#else
		return malloc(iSize);
	#endif
}

void et_free(void* pPtr){
	#ifdef ET_IN_KERNEL
		return kfree(pPtr);
	#else
		return free(pPtr);
	#endif
}

//////////////////////////////////////////////////////////////////////

static const char *et_base64_s_sz_char_table =
     "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

static const BYTE et_base64_s_arr_map_table[256] ={
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
	  52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	 255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
	 255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
	  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
	  49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255, 255
};

char* et_base64_encode(char *szDest, const char *szSrc){
	int i = 0;
	int iSrcLen = 0;
	int iLoop = 0;
	BYTE *pLoop = (BYTE *)szDest;
	BYTE *pIn = (BYTE *)szSrc;
	BYTE chA = 0;
	BYTE chB = 0;
	BYTE chC = 0;

	if(szDest == null){
		return null;
	}

	szDest[0] = 0;
	if(szSrc == null){
		return szDest;
	}

	iSrcLen = et_strlen(szSrc);
	iLoop = 3 * (iSrcLen / 3);

	for(i = 0; i < iLoop; i += 3){
		*pLoop++ = et_base64_s_sz_char_table[pIn[0] >> 2];
		*pLoop++ = et_base64_s_sz_char_table[((pIn[0] & 3) << 4) + (pIn[1] >> 4)];
		*pLoop++ = et_base64_s_sz_char_table[((pIn[1] & 0xf) << 2) + (pIn[2] >> 6)];
		*pLoop++ = et_base64_s_sz_char_table[pIn[2] & 0x3f];
		pIn += 3;
	}

	if (i < iSrcLen){
		chA = pIn[0];
		chB = ((i + 1) < iSrcLen) ? pIn[1] : 0;
		chC = 0;

		*pLoop++ = et_base64_s_sz_char_table[chA >> 2];
		*pLoop++ = et_base64_s_sz_char_table[((chA & 3) << 4) + (chB >> 4)];
		*pLoop++ = ((i + 1) < iSrcLen) ? et_base64_s_sz_char_table[((chB & 0xf) << 2) + (chC >> 6)] : '=';
		*pLoop++ = '=';
	}

	*pLoop = 0;
	return szDest;
}

char* et_base64_decode(char *szDest, const char *szSrc){
	int iSrcLen = 0;
	int iMiddle = 0;
	int iSrcIndex = 0;
	int iCheck4 = 0;
	int iNewIndex = 0;
	int iMode3 = 3;
	BYTE bySrcTemp = 0;
	BYTE *pIn = (BYTE*)szSrc;
	BYTE *pOut = (BYTE*)szDest;

	if(szDest == null){
		return null;
	}

	szDest[0] = 0;
	if(szSrc == null){
		return szDest;
	}

	iSrcLen = et_strlen((const char*)szSrc);

	for(iSrcIndex = 0; iSrcIndex < iSrcLen; iSrcIndex++){
		bySrcTemp = et_base64_s_arr_map_table[pIn[iSrcIndex]];
		if(bySrcTemp == 255){
			continue;
		}

		if(bySrcTemp == 254){
			bySrcTemp = 0;
			iMode3--;
		}

		iMiddle = (iMiddle << 6) | bySrcTemp;
		if(++iCheck4 == 4){
			pOut[iNewIndex++] = (BYTE)((iMiddle >> 16) & 255);
			if(iMode3 > 1){
				pOut[iNewIndex++] = (BYTE)((iMiddle >> 8) & 255);
			}
			if(iMode3 > 2){
				pOut[iNewIndex++] = (BYTE)(iMiddle & 255);
			}
			iCheck4 = iMiddle = 0;
		}
	}
	pOut[iNewIndex++] = 0;
	return szDest;
}

///////////////////////////////////////////////////////////////

int et_char_to_int( char chSrc ){
	if(!et_char_is_num(chSrc)){
		return 0;
	}
	return (int)chSrc - 0x30;
}

char et_char_by_int(int iSrc){
	if(iSrc > 10 || iSrc < 0)
	{
		return '0';
	}
	return iSrc + 0x30;
}

int et_char_hex_to_int(char chHex){
	const char *szHex = ET_SZ_HEX_UP;
	int i = 0;
	
	char chUpper = et_char_upper(chHex);
	
	for(i = 0; i < 16; i++){
		if(chUpper == szHex[i]){
			return i;
		}
	}
	
	return -1;
}

char et_char_int_to_hex(int iNum){
	const char *szHex = ET_SZ_HEX_UP;
	if(iNum < 16 && iNum >= 0){
		return szHex[iNum];
	}
	return -1;
}

bool et_char_equal_no_case(char chLeft, char chRight){
	if(chLeft == chRight){
		return true;
	}else if(et_char_upper(chLeft) == et_char_upper(chRight)){
		return true;
	}

	return false;
}

bool et_char_is_space(unsigned char chSrc){
	if(chSrc > 128){
		return false;
	}

	if(chSrc == ET_CH_SPACE){
		return true;
	}else if(chSrc == ET_CH_TAB){
		return true;
	}else if(chSrc == ET_CH_R){
		return true;
	}else if(chSrc == ET_CH_N){
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

void* et_memmove(void* pDest, void* pSrc, UINT iCount){
	int i = 0;
	char* pChDest = (char*)pDest;
	char* pChSrc = (char*)pSrc;

	if(pDest == null){
		return null;
	}

	if(pSrc == null){
		return null;
	}

	if(iCount == 0){
		return pDest;
	}

	if(pDest == pSrc){
		return pDest;
	}

	if(pChDest > pChSrc){
		for(i = iCount - 1; i >= 0; i--){
			*(pChDest + i) = *(pChSrc + i);
		}
	}else{
		for(i = 0; i < (int)iCount; i++){
			*(pChDest + i) = *(pChSrc + i);
		}
	}

	return pDest;
}

void* et_memcpy(void* pDest, const void* pSrc, UINT iCount){
	int i = 0;
	char* pChDest = (char*)pDest;
	char* pChSrc = (char*)pSrc;

	if(pDest == null){
		return null;
	}

	if(pSrc == null){
		return null;
	}

	if(iCount == 0){
		return pDest;
	}

	if(pDest == pSrc){
		return pDest;
	}

	if((pChDest > pChSrc && pChDest < pChSrc + iCount)
		|| ( pChDest < pChSrc && pChDest + iCount > pChSrc)
	){
		return null;
	}

	for(i = 0; i < (int)iCount; i++){
		*(pChDest + i) = *(pChSrc + i);
	}

	return pDest;
}

void* et_memset(void* pDest, char chValue, UINT iCount){
	UINT i = 0;
	char* pChDest = (char*)pDest;

	if(pDest == null){
		return null;
	}

	if(iCount == 0){
		return pDest;
	}

	for(i = 0; i < iCount; i++){
		*(pChDest + i) = chValue;
	}

	return pDest;
}

UINT et_strlen(const char* szSrc){
	UINT iResult = 0;
	const char* szMove = szSrc;

	if(szSrc == null){
		return 0;
	}

	while(*szMove++ != 0){
		if(iResult > ET_SZ_MAX_LEN){
			break;
		}
		iResult++;
	}

	return iResult;
}

char* et_strcpy(char* szDest, const char* szSrc){
	UINT iSrcLen = 0;
	UINT i = 0;

	if(szDest == null){
		return null;
	}

	if(szSrc == null){
		return szDest;
	}

	if(szDest == szSrc){
		return szDest;
	}

	iSrcLen = et_strlen(szSrc);

	if(  (szDest > szSrc && szSrc + iSrcLen > szDest)
		|| (szSrc > szDest && szDest + iSrcLen > szSrc)
	){
		return null;
	}
	
	for(i = 0; i < iSrcLen; i++){
		szDest[i] = szSrc[i];
	}
	szDest[iSrcLen] = 0;

	return szDest;
}

char* et_strncpy(char* szDest, const char* szSrc, UINT iNum){
	UINT i = 0;

	if(szDest == null){
		return null;
	}

	if(szSrc == null){
		return szDest;
	}

	if(szDest == szSrc){
		return szDest;
	}

	if(  (szDest > szSrc && szSrc + iNum > szDest)
		|| (szSrc > szDest && szDest + iNum > szSrc)
	){
		return null;
	}

	for(i = 0; i < iNum; i++){
		szDest[i] = szSrc[i];
	}
	szDest[iNum] = 0;

	return szDest;
}

char* et_strcat(char* szDest, const char* szRight){
	UINT iDestLen = 0;
	UINT iRightLen = 0;
	UINT i = 0;

	if(szDest == null){
		return null;
	}

	if(szRight == null){
		return szDest;
	}

	if(szDest == szRight){
		return null;
	}

	iDestLen = et_strlen(szDest);
	iRightLen = et_strlen(szRight);

	if(  (szRight > szDest && szDest + iDestLen > szRight)
		|| (szDest > szRight && szRight + iRightLen > szDest)
	){
		return null;
	}

	for(i = 0; i < iRightLen; i++){
		szDest[iDestLen + i] = szRight[i];
	}
	szDest[iDestLen + i] = 0;

	return szDest;
}

char* et_strncat(char* szDest, const char* szRight, UINT iNum){
	UINT iDestLen = 0;
	UINT i = 0;

	if(szDest == null){
		return null;
	}

	if(szRight == null){
		return szDest;
	}

	if(szDest == szRight){
		return null;
	}

	iDestLen = et_strlen(szDest);

	if(  (szRight > szDest && szDest + iDestLen > szRight)
		|| (szDest > szRight && szRight + iNum > szDest)
	){
		return null;
	}

	for(i = 0; i < iNum; i++){
		szDest[iDestLen + i] = szRight[i];
	}
	szDest[iDestLen + i] = 0;

	return szDest;
}

int et_strcmp(const char* szLeft, const char* szRight){
	int iLeftLen = 0;
	int iRightLen = 0;
	int iSmallerLen = 0;
	int i = 0;

	if(szLeft == null && szRight == null){
		return 0;
	}else if(szLeft == null){
		return -1;
	}else if(szRight == null){
		return 1;
	}else if(szLeft == szRight){
		return 0;
	}

	iLeftLen = et_strlen(szLeft);
	iRightLen = et_strlen(szRight);
	if(iLeftLen == 0 && iRightLen == 0){
		return 0;
	}else if(iLeftLen == 0){
		return -1;
	}else if(iRightLen == 0){
		return 1;
	}

	iSmallerLen = et_min(iLeftLen,iRightLen);
	for(i = 0; i < iSmallerLen; i++){
		if(szLeft[i] < szRight[i]){
			return -1;
		}else if(szLeft[i] > szRight[i]){
			return 1;
		}else{
			continue;
		}
	}

	if(iLeftLen < iRightLen){
		return -1;
	}else if(iLeftLen > iRightLen){
		return 1;
	}

	return 0;
}

int et_strncmp(const char* szLeft, const char* szRight, UINT iNum){
	int iLeftLen = 0;
	int iRightLen = 0;
	int iSmallerLen = 0;
	int i = 0;

	if(szLeft == null && szRight == null){
		return 0;
	}else if(szLeft == null){
		return -1;
	}else if(szRight == null){
		return 1;
	}else if(szLeft == szRight){
		return 0;
	}

	iLeftLen = et_strlen(szLeft);
	iRightLen = et_strlen(szRight);
	if(iLeftLen == 0 && iRightLen == 0){
		return 0;
	}else if(iLeftLen == 0){
		return -1;
	}else if(iRightLen == 0){
		return 1;
	}

	iSmallerLen = et_min(iLeftLen,iRightLen);
	iSmallerLen = et_min(iSmallerLen,iNum);
	for(i = 0; i < iSmallerLen; i++){
		if(szLeft[i] < szRight[i]){
			return -1;
		}else if(szLeft[i] > szRight[i]){
			return 1;
		}else{
			continue;
		}
	}

	return 0;
}

const char* et_strchr(const char* szSrc, char chFind){
	int iFromPos = 0;
	int iSrcLen = 0;
	int i = 0;

	if(szSrc == null){
		return null;
	}

	iSrcLen = et_strlen(szSrc);
	if(iFromPos >= iSrcLen){
		return null;
	}

	for(i = iFromPos; i < iSrcLen; i++){
		if(szSrc[i] == chFind){
			return szSrc + i;
		}
	}

	return null;
}

///////////////////////////////////////////////////////////////////////////////

int et_listi_node_create(EtListiNode** ppNode, INT64 iValue){
	*ppNode = (EtListiNode*)et_alloc(sizeof(EtListiNode));
	et_memset(*ppNode,0,sizeof(EtListiNode));
	(*ppNode)->cb = sizeof(EtListiNode);
	(*ppNode)->prev = null;
	(*ppNode)->next = null;
	(*ppNode)->value = iValue;

	return 0;
}

int et_listi_node_destroy(EtListiNode** ppNode){
	if(*ppNode == null){
		return 0;
	}

	et_memset(*ppNode,0,sizeof(EtListiNode));
	et_free(*ppNode);
	*ppNode = null;

	return 0;
}

int et_listi_create(EtListi** ppList){

	*ppList = (EtListi*)et_alloc(sizeof(EtListi));
	et_memset(*ppList,0,sizeof(EtListi));
	(*ppList)->cb = sizeof(EtListi);
	(*ppList)->head = null;
	(*ppList)->foot = null;
	(*ppList)->size = 0;
	
	return 0;
}

int et_listi_destroy(EtListi** ppList){
	if(*ppList == null){
		return -1;
	}

	et_listi_clear(*ppList);
	et_memset(*ppList,0,sizeof(EtListi));
	et_free(*ppList);
	*ppList = null;

	return 0;
}

int et_listi_head(const EtListi* pList, INT64* pData){
	if(pList == null){
		return -1;
	}

	if(pList->size == 0){
		return -1;
	}
	if(pData == null){
		return -1;
	}

	*pData = pList->head->value;
	return 0;
}

int et_listi_foot(const EtListi* pList, INT64* pData){
	if(pList == null){
		return -1;
	}

	if(pList->size == 0){
		return -1;
	}
	if(pData == null){
		return -1;
	}

	*pData = pList->foot->value;
	return 0;
}

int et_listi_size(const EtListi* pList){
	if(pList == null){
		return -1;
	}

	return pList->size;
}

int et_listi_iterator_start(EtListi* pList){
	if(pList == null){
		return ET_ERR_MEM_NULL;
	}
	pList->iterator = pList->head;

	return 0;
}

bool et_listi_iterator_check(EtListi* pList){
	if(pList == null){
		return false;
	}
	if(pList->iterator != null){
		return true;
	}

	return false;
}

INT64 et_listi_iterator_value(EtListi* pList){
	if(pList == null){
		return null;
	}
	if(pList->iterator == null){
		return null;
	}

	return pList->iterator->value;
}

int et_listi_iterator_move(EtListi* pList){
	if(pList == null){
		return ET_ERR_MEM_NULL;
	}
	if(pList->iterator == null){
		return ET_ERR_MEM_NULL;
	}

	pList->iterator = pList->iterator->next;

	return 0;
}

int et_listi_add(EtListi* pList, INT64 iValue){
	int iRet = 0;
	EtListiNode* pNodeNew = null;

	if(pList == null){
		return -1;
	}


	iRet = et_listi_node_create(&pNodeNew,iValue);
	if(iRet < 0){
		return iRet;
	}

	if(pList->size == 0){
		pList->head = pNodeNew;
	}else{
		pList->foot->next = pNodeNew;
		pNodeNew->prev = pList->foot;
	}

	pList->foot = pNodeNew;
	pList->size++;

	return 0;
}

int et_listi_remove(EtListi* pList){
	EtListiNode* pFootNode = null;

	if(pList == null){
		return -1;
	}

	if(pList->size == 0){
		return 0;
	}

	pFootNode = pList->foot;
	if(pList->size == 1){
		pList->head = null;
		pList->foot = null;
	}else{
		pList->foot = pList->foot->prev;
		pList->foot->next = null;
	}
	et_listi_node_destroy(&pFootNode);
	pList->size--;

	return 0;
}

int et_listi_copy(EtListi* pListDest, const EtListi* pListSrc){
	EtListiNode* pNode = null;

	if(pListDest == null || pListSrc == null){
		return -1;
	}


	et_listi_clear(pListDest);
	pNode = pListSrc->head;
	while(pNode != null){
		et_listi_add(pListDest,pNode->value);
		pNode = pNode->next;
	}

	return 0;
}

int et_listi_clear(EtListi* pList){
	if(pList == null){
		return -1;
	}


	while(pList->size > 0){
		et_listi_remove(pList);
	}

	return 0;
}

bool et_listi_contains(const EtListi* pList, INT64 iValue){
	EtListiNode* pNode = null;

	if(pList == null){
		return -1;
	}


	pNode = pList->head;
	while(pNode != null){
		if(iValue == pNode->value){
			return true;
		}
		pNode = pNode->next;
	}

	return false;
}

bool et_listi_count(const EtListi* pList, INT64 iValue){
	int iCount = 0;
	EtListiNode* pNode = null;

	if(pList == null){
		return -1;
	}


	pNode = pList->head;
	while(pNode != null){
		if(iValue == pNode->value){
			iCount++;
		}
		pNode = pNode->next;
	}

	return iCount;
}

int et_listi_get(const EtListi* pList, int iIndex, INT64* pData){
	int iCount = 0;
	EtListiNode* pNode = null;
	bool bFound = false;
	
	if(pData == null){
		return -1;
	}
	if(pList == null){
		return -1;
	}

	if(iIndex >= pList->size){
		return -1;
	}

	pNode = pList->head;
	while(iCount <= iIndex){
		if(iCount == iIndex){
			*pData = pNode->value;
			bFound = true;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	if(!bFound){
		return ET_ERR_LOST;
	}

	return 0;
}

int et_listi_set(EtListi* pList, int iIndex, INT64 iValue){
	int iCount = 0;
	EtListiNode* pNode = null;

	if(pList == null){
		return -1;
	}

	if(iIndex >= pList->size){
		return -1;
	}

	pNode = pList->head;
	while(iCount <= iIndex){
		if(iCount == iIndex){
			pNode->value = iValue;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	return 0;
}

int et_listi_insert(EtListi* pList, int iIndex, INT64 iValue){
	int iRet = 0;
	EtListiNode* pNodePrev = null;
	int iCount = 0;
	EtListiNode* pNode = null;
	EtListiNode* pNodeNew = null;

	if(pList == null){
		return -1;
	}

	if(iIndex > pList->size){
		return -1;
	}

	pNode = pList->head;
	iRet = et_listi_node_create(&pNodeNew,iValue);
	if(iRet < 0){
		return iRet;
	}

	while(iCount <= iIndex){
		if(iCount == iIndex){
			pNodeNew->prev = pNodePrev;
			pNodeNew->next = pNode;

			if(pNodePrev != null){
				pNodePrev->next = pNodeNew;
			}else{
				pList->head = pNodeNew;
			}

			if(pNode != null){
				pNode->prev = pNodeNew;
			}else{
				pList->foot = pNodeNew;
			}

			pList->size++;
			
			break;
		}
		iCount++;
		pNodePrev = pNode;
		pNode = pNode->next;
	}

	return 0;
}

int et_listi_remove_at(EtListi* pList, int iIndex){
	EtListiNode* pNodePrev = null;
	int iCount = 0;
	EtListiNode* pNode = null;

	if(pList == null){
		return -1;
	}

	if(iIndex >= pList->size){
		return -1;
	}

	pNode = pList->head;
	while(iCount <= iIndex){
		if(iCount == iIndex){
			if(pNodePrev != null){
				pNodePrev->next = pNode->next;
			}else{
				pList->head = pNode->next;
			}

			if(pNode->next != null){
				pNode->next->prev = pNode->prev;
			}else{
				pList->foot = pNode->prev;
			}		

			pList->size--;
			break;
		}
		iCount++;
		pNodePrev = pNode;
		pNode = pNode->next;
	}
	et_listi_node_destroy(&pNode);

	return 0;
}

int et_listi_walk(EtListi* pList, et_listi_walk_fn fnWalk){
	EtListiNode* pNode = null;
	int iTempResult = 0;

	if(pList == null){
		return -1;
	}


	pNode = pList->head;
	while(pNode != null){
		iTempResult = fnWalk(&(pNode->value));
		if(iTempResult < 0){
			break;
		}
		pNode = pNode->next;
	}

	return 0;
}

int et_listi_sort(EtListi* pList, et_fn_cmp_int fnCmp, bool isDesc){
	INT64 iTemp = 0;
	EtListiNode* pNodeHead1 = null;
	EtListiNode* pNodeHead2 = null;
	int iCmpResult = 0;

	if(pList == null){
		return -1;
	}

	if(pList->size <= 1){
		return 0;
	}
	if(fnCmp == null){
		fnCmp = et_fn_cmp_int_default;
	}

	pNodeHead1 = pList->head;
	while(pNodeHead1 != null){
		if(pNodeHead1->next == null){
			break;
		}
		pNodeHead2 = pNodeHead1->next;
		while(pNodeHead2 != null){
			iCmpResult = fnCmp(pNodeHead1->value,pNodeHead2->value);
			if( 
				(iCmpResult > 0 && !isDesc)
				|| (iCmpResult < 0 && isDesc)
			){
				iTemp = pNodeHead1->value;
				pNodeHead1->value = pNodeHead2->value;
				pNodeHead2->value = iTemp;
			}
			pNodeHead2 = pNodeHead2->next;
		}
		pNodeHead1 = pNodeHead1->next;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

int et_listv_node_create(EtListvNode** ppNode, void* pData){
	*ppNode = (EtListvNode*)et_alloc(sizeof(EtListvNode));
	et_memset(*ppNode,0,sizeof(EtListvNode));
	(*ppNode)->cb = sizeof(EtListvNode);
	(*ppNode)->prev = null;
	(*ppNode)->next = null;
	(*ppNode)->value = pData;

	return 0;
}

int et_listv_node_destroy(EtListvNode** ppNode){
	if(*ppNode == null){
		return 0;
	}

	et_memset(*ppNode,0,sizeof(EtListvNode));
	et_free(*ppNode);
	*ppNode = null;

	return 0;
}

int et_listv_create(EtListv** ppList){
	*ppList = (EtListv*)et_alloc(sizeof(EtListv));
	et_memset(*ppList,0,sizeof(EtListv));
	(*ppList)->cb = sizeof(EtListv);
	(*ppList)->head = null;
	(*ppList)->foot = null;
	(*ppList)->size = 0;
	
	return 0;
}

int et_listv_destroy(EtListv** ppList){
	if(*ppList == null){
		return 0;
	}

	et_listv_clear(*ppList);
	et_memset(*ppList,0,sizeof(EtListv));
	et_free(*ppList);
	*ppList = null;

	return 0;
}

int et_listv_head(const EtListv* pList, void** pData){
	if(pList == null){
		return -1;
	}

	if(pList->size == 0){
		return -1;
	}
	if(pData == null){
		return -1;
	}

	*pData = pList->head->value;
	return 0;
}

int et_listv_foot(const EtListv* pList, void** pData){
	if(pList == null){
		return -1;
	}

	if(pList->size == 0){
		return -1;
	}
	if(pData == null){
		return -1;
	}

	*pData = pList->foot->value;
	return 0;
}

int et_listv_size(const EtListv* pList){
	if(pList == null){
		return -1;
	}

	return pList->size;
}

int et_listv_iterator_start(EtListv* pList){
	if(pList == null){
		return ET_ERR_MEM_NULL;
	}
	pList->iterator = pList->head;

	return 0;
}

bool et_listv_iterator_check(EtListv* pList){
	if(pList == null){
		return false;
	}
	if(pList->iterator != null){
		return true;
	}

	return false;
}

void* et_listv_iterator_value(EtListv* pList){
	if(pList == null){
		return null;
	}
	if(pList->iterator == null){
		return null;
	}

	return pList->iterator->value;
}

int et_listv_iterator_move(EtListv* pList){
	if(pList == null){
		return ET_ERR_MEM_NULL;
	}
	if(pList->iterator == null){
		return ET_ERR_MEM_NULL;
	}

	pList->iterator = pList->iterator->next;

	return 0;
}

int et_listv_add(EtListv* pList, void* pData){
	int iRet = 0;
	EtListvNode* pNodeNew = null;

	if(pList == null){
		return -1;
	}


	iRet = et_listv_node_create(&pNodeNew,pData);
	if(iRet < 0){
		return iRet;
	}

	if(pList->size == 0){
		pList->head = pNodeNew;
	}else{
		pList->foot->next = pNodeNew;
		pNodeNew->prev = pList->foot;
	}

	pList->foot = pNodeNew;
	pList->size++;

	return 0;
}

int et_listv_remove(EtListv* pList){
	EtListvNode* pFootNode = null;

	if(pList == null){
		return -1;
	}

	if(pList->size == 0){
		return 0;
	}

	pFootNode = pList->foot;
	if(pList->size == 1){
		pList->head = null;
		pList->foot = null;
	}else{
		pList->foot = pList->foot->prev;
		pList->foot->next = null;
	}
	et_listv_node_destroy(&pFootNode);
	pList->size--;

	return 0;
}

int et_listv_copy(EtListv* pListDest, const EtListv* pListSrc){
	EtListvNode* pNode = null;

	if(pListDest == null || pListSrc == null){
		return -1;
	}


	et_listv_clear(pListDest);
	pNode = pListSrc->head;
	while(pNode != null){
		et_listv_add(pListDest,pNode->value);
		pNode = pNode->next;
	}

	return 0;
}

int et_listv_clear(EtListv* pList){
	if(pList == null){
		return -1;
	}


	while(pList->size > 0){
		et_listv_remove(pList);
	}

	return 0;
}

bool et_listv_contains(const EtListv* pList, void* pData){
	EtListvNode* pNode = null;

	if(pList == null){
		return -1;
	}


	pNode = pList->head;
	while(pNode != null){
		if(pData == pNode->value){
			return true;
		}
		pNode = pNode->next;
	}

	return false;
}

bool et_listv_count(const EtListv* pList, void* pData){
	int iCount = 0;
	EtListvNode* pNode = null;

	if(pList == null){
		return -1;
	}


	pNode = pList->head;
	while(pNode != null){
		if(pData == pNode->value){
			iCount++;
		}
		pNode = pNode->next;
	}

	return iCount;
}

int et_listv_get(const EtListv* pList, int iIndex, void** pData){
	int iCount = 0;
	EtListvNode* pNode = null;
	bool bFound = false;
	
	if(pData == null){
		return -1;
	}
	if(pList == null){
		return -1;
	}

	if(iIndex >= pList->size){
		return -1;
	}

	pNode = pList->head;
	while(iCount <= iIndex){
		if(iCount == iIndex){
			*pData = pNode->value;
			bFound = true;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	if(!bFound){
		return ET_ERR_LOST;
	}

	return 0;
}

int et_listv_set(EtListv* pList, int iIndex, void* pData){
	int iCount = 0;
	EtListvNode* pNode = null;

	if(pList == null){
		return -1;
	}

	if(iIndex >= pList->size){
		return -1;
	}

	pNode = pList->head;
	while(iCount <= iIndex){
		if(iCount == iIndex){
			pNode->value = pData;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	return 0;
}

int et_listv_insert(EtListv* pList, int iIndex, void* pData){
	int iRet = 0;
	EtListvNode* pNodePrev = null;
	int iCount = 0;
	EtListvNode* pNode = null;
	EtListvNode* pNodeNew = null;

	if(pList == null){
		return -1;
	}

	if(iIndex > pList->size){
		return -1;
	}

	pNode = pList->head;
	iRet = et_listv_node_create(&pNodeNew,pData);
	if(iRet < 0){
		return iRet;
	}

	while(iCount <= iIndex){
		if(iCount == iIndex){
			pNodeNew->prev = pNodePrev;
			pNodeNew->next = pNode;

			if(pNodePrev != null){
				pNodePrev->next = pNodeNew;
			}else{
				pList->head = pNodeNew;
			}

			if(pNode != null){
				pNode->prev = pNodeNew;
			}else{
				pList->foot = pNodeNew;
			}

			pList->size++;
			
			break;
		}
		iCount++;
		pNodePrev = pNode;
		pNode = pNode->next;
	}

	return 0;
}

int et_listv_remove_at(EtListv* pList, int iIndex){
	EtListvNode* pNodePrev = null;
	int iCount = 0;
	EtListvNode* pNode = null;

	if(pList == null){
		return -1;
	}

	if(iIndex >= pList->size){
		return -1;
	}

	pNode = pList->head;
	while(iCount <= iIndex){
		if(iCount == iIndex){
			if(pNodePrev != null){
				pNodePrev->next = pNode->next;
			}else{
				pList->head = pNode->next;
			}

			if(pNode->next != null){
				pNode->next->prev = pNode->prev;
			}else{
				pList->foot = pNode->prev;
			}		

			pList->size--;
			break;
		}
		iCount++;
		pNodePrev = pNode;
		pNode = pNode->next;
	}
	et_listv_node_destroy(&pNode);

	return 0;
}

int et_listv_walk(EtListv* pList, et_listv_walk_fn fnWalk){
	EtListvNode* pNode = null;
	int iTempResult = 0;

	if(pList == null){
		return -1;
	}


	pNode = pList->head;
	while(pNode != null){
		iTempResult = fnWalk(&(pNode->value));
		if(iTempResult < 0){
			break;
		}
		pNode = pNode->next;
	}

	return 0;
}

int et_listv_sort(EtListv* pList, et_fn_cmp_int fnCmp, bool isDesc){
	void* iTemp = 0;
	EtListvNode* pNodeHead1 = null;
	EtListvNode* pNodeHead2 = null;
	int iCmpResult = 0;

	if(pList == null){
		return -1;
	}

	if(pList->size <= 1){
		return 0;
	}
	if(fnCmp == null){
		fnCmp = et_fn_cmp_int_default;
	}

	pNodeHead1 = pList->head;
	while(pNodeHead1 != null){
		if(pNodeHead1->next == null){
			break;
		}
		pNodeHead2 = pNodeHead1->next;
		while(pNodeHead2 != null){
			iCmpResult = fnCmp((int)pNodeHead1->value,(int)pNodeHead2->value);
			if( 
				(iCmpResult > 0 && !isDesc)
				|| (iCmpResult < 0 && isDesc)
			){
				iTemp = pNodeHead1->value;
				pNodeHead1->value = pNodeHead2->value;
				pNodeHead2->value = iTemp;
			}
			pNodeHead2 = pNodeHead2->next;
		}
		pNodeHead1 = pNodeHead1->next;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////


int et_lists_node_create(EtListsNode** ppNode, const char* szData){
	char* szNew = null;

	if(szData == null){
		return null;
	}

	*ppNode = (EtListsNode*)et_alloc(sizeof(EtListsNode));
	if(*ppNode == null){
		return null;
	}

	szNew = (char*)et_alloc(et_strlen(szData) + 1);
	if(szNew == null){
		et_free(*ppNode);
		return null;
	}
	et_strcpy(szNew,szData);

	et_memset((*ppNode),0,sizeof(EtListsNode));
	(*ppNode)->cb = sizeof(EtListsNode);
	(*ppNode)->prev = null;
	(*ppNode)->next = null;
	(*ppNode)->value = szNew;
	return 0;
}

int et_lists_node_destroy(EtListsNode** ppNode){
	if(*ppNode == null){
		return -1;
	}

	et_free((*ppNode)->value);
	et_memset(*ppNode,0,sizeof(EtListsNode));
	et_free(*ppNode);
	*ppNode = null;

	return 0;
}

int et_lists_create(EtLists** ppList){
	*ppList = (EtLists*)et_alloc(sizeof(EtLists));
	et_memset(*ppList,0,sizeof(EtLists));
	(*ppList)->cb = sizeof(EtLists);
	(*ppList)->head = null;
	(*ppList)->foot = null;
	(*ppList)->size = 0;
	
	return 0;
}

int et_lists_destroy(EtLists** ppList){
	if(*ppList == null){
		return -1;
	}

	et_lists_clear(*ppList);
	et_memset(*ppList,0,sizeof(EtLists));
	et_free(*ppList);
	*ppList = null;

	return 0;
}

int et_lists_head(const EtLists* pList, char* szData){
	if(pList == null){
		return -1;
	}
	if(szData == null){
		return -1;
	}

	if(pList->size == 0){
		return -1;
	}

	et_strcpy(szData,pList->head->value);

	return 0;
}

int et_lists_head_len(const EtLists* pList){
	if(pList == null){
		return -1;
	}

	if(pList->size == 0){
		return -1;
	}

	return et_strlen(pList->head->value);
}

int et_lists_foot(const EtLists* pList, char* szData){
	if(pList == null){
		return -1;
	}
	if(szData == null){
		return -1;
	}

	if(pList->size == 0){
		return -1;
	}

	et_strcpy(szData,pList->foot->value);

	return 0;
}

int et_lists_foot_len(const EtLists* pList){
	if(pList == null){
		return -1;
	}

	if(pList->size == 0){
		return -1;
	}

	return et_strlen(pList->foot->value);
}

int et_lists_size(const EtLists* pList){
	if(pList == null){
		return -1;
	}


	return pList->size;
}

int et_lists_iterator_start(EtLists* pList){
	if(pList == null){
		return ET_ERR_MEM_NULL;
	}
	pList->iterator = pList->head;

	return 0;
}

bool et_lists_iterator_check(EtLists* pList){
	if(pList == null){
		return false;
	}
	if(pList->iterator != null){
		return true;
	}

	return false;
}

char* et_lists_iterator_value(EtLists* pList){
	if(pList == null){
		return null;
	}
	if(pList->iterator == null){
		return null;
	}

	return pList->iterator->value;
}


int et_lists_iterator_move(EtLists* pList){
	if(pList == null){
		return ET_ERR_MEM_NULL;
	}
	if(pList->iterator == null){
		return ET_ERR_MEM_NULL;
	}

	pList->iterator = pList->iterator->next;

	return 0;
}

int et_lists_add(EtLists* pList, const char* szData){
	int iRet = 0;
	EtListsNode* pNodeNew = null;

	if(pList == null){
		return -1;
	}


	iRet = et_lists_node_create(&pNodeNew,szData);
	if(iRet < 0){
		return iRet;
	}

	if(pList->size == 0){
		pList->head = pNodeNew;
	}else{
		pList->foot->next = pNodeNew;
		pNodeNew->prev = pList->foot;
	}

	pList->foot = pNodeNew;
	pList->size++;
	return 0;
}

int et_lists_remove(EtLists* pList){
	EtListsNode* pFootNode = null;

	if(pList == null){
		return -1;
	}

	if(pList->size == 0){
		return 0;
	}

	pFootNode = pList->foot;
	if(pList->size == 1){
		pList->head = null;
		pList->foot = null;
	}else{
		pList->foot = pList->foot->prev;
		pList->foot->next = null;
	}
	et_lists_node_destroy(&pFootNode);
	pList->size--;

	return 0;
}

int et_lists_copy(EtLists* pListDest, const EtLists* pListSrc){
	EtListsNode* pNode = null;

	if(pListDest == null || pListSrc == null){
		return -1;
	}

	et_lists_clear(pListDest);
	pNode = pListSrc->head;
	while(pNode != null){
		et_lists_add(pListDest,pNode->value);
		pNode = pNode->next;
	}

	return 0;
}

int et_lists_clear(EtLists* pList){
	if(pList == null){
		return -1;
	}


	while(pList->size > 0){
		et_lists_remove(pList);
	}

	return 0;
}

bool et_lists_contains(const EtLists* pList, const char* szData){
	EtListsNode* pNode = null;

	if(pList == null){
		return -1;
	}
	if(szData == null){
		return -1;
	}


	pNode = pList->head;
	while(pNode != null){
		if(et_strcmp(szData, pNode->value) == 0){
			return true;
		}
		pNode = pNode->next;
	}
	return false;
}

bool et_lists_count(const EtLists* pList, const char* szData){
	int iCount = 0;
	EtListsNode* pNode = null;

	if(pList == null){
		return -1;
	}

	if(szData == null){
		return -1;
	}

	pNode = pList->head;
	while(pNode != null){
		if(et_strcmp(szData, pNode->value) == 0){
			iCount++;
		}
		pNode = pNode->next;
	}

	return iCount;
}

int et_lists_get(const EtLists* pList, int iIndex, char* szData){
	int iCount = 0;
	EtListsNode* pNode = null;
	bool bFound = false;

	if(pList == null){
		return -1;
	}
	if(szData == null){
		return -1;
	}

	if(iIndex >= pList->size){
		return -1;
	}

	pNode = pList->head;
	while(iCount <= iIndex){
		if(iCount == iIndex){
			et_strcpy(szData,pNode->value);
			bFound = true;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	if(!bFound){
		return ET_ERR_LOST;
	}
	return 0;
}

int et_lists_get_len(const EtLists* pList, int iIndex){
	int iCount = 0;
	EtListsNode* pNode = null;

	if(pList == null){
		return -1;
	}

	if(iIndex >= pList->size){
		return -1;
	}

	pNode = pList->head;
	while(iCount <= iIndex){
		if(iCount == iIndex){
			return et_strlen(pNode->value);
		}
		iCount++;
		pNode = pNode->next;
	}

	return ET_ERR_LOST;
}

int et_lists_set(EtLists* pList, int iIndex, const char* szData){
	int iCount = 0;
	EtListsNode* pNode = null;

	if(pList == null){
		return -1;
	}
	if(szData == null){
		return -1;
	}

	if(iIndex >= pList->size){
		return -1;
	}
	if(szData == null){
		return -1;
	}

	pNode = pList->head;
	while(iCount <= iIndex){
		if(iCount == iIndex){
			et_free(pNode->value);
			pNode->value = (char*)et_alloc(et_strlen(szData) + 1);
			et_strcpy(pNode->value,szData);
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	return 0;
}

int et_lists_insert(EtLists* pList, int iIndex, const char* szData){
	EtListsNode* pNodePrev = null;
	int iCount = 0;
	EtListsNode* pNode = null;
	EtListsNode* pNodeNew = null;
	int iRet = 0;

	if(pList == null){
		return -1;
	}

	if(iIndex > pList->size){
		return -1;
	}
	if(szData == null){
		return -1;
	}

	iRet = et_lists_node_create(&pNodeNew,szData);
	if(iRet < 0){
		return iRet;
	}
	pNode = pList->head;
	while(iCount <= iIndex){
		if(iCount == iIndex){
			pNodeNew->prev = pNodePrev;
			pNodeNew->next = pNode;

			if(pNodePrev != null){
				pNodePrev->next = pNodeNew;
			}else{
				pList->head = pNodeNew;
			}

			if(pNode != null){
				pNode->prev = pNodeNew;
			}else{
				pList->foot = pNodeNew;
			}

			pList->size++;
			
			break;
		}
		iCount++;
		pNodePrev = pNode;
		pNode = pNode->next;
	}

	return 0;
}


int et_lists_remove_at(EtLists* pList, int iIndex){
	EtListsNode* pNodePrev = null;
	int iCount = 0;
	EtListsNode* pNode = null;

	if(pList == null){
		return -1;
	}

	if(iIndex >= pList->size){
		return -1;
	}

	pNode = pList->head;

	while(iCount <= iIndex){
		if(iCount == iIndex){
			if(pNodePrev != null){
				pNodePrev->next = pNode->next;
			}else{
				pList->head = pNode->next;
			}

			if(pNode->next != null){
				pNode->next->prev = pNode->prev;
			}else{
				pList->foot = pNode->prev;
			}		

			pList->size--;
			break;
		}
		iCount++;
		pNodePrev = pNode;
		pNode = pNode->next;
	}
	et_lists_node_destroy(&pNode);

	return 0;
}

int et_lists_walk(EtLists* pList, et_lists_walk_fn fnWalk){
	EtListsNode* pNode = null;
	int iTempResult = 0;

	if(pList == null){
		return -1;
	}


	pNode = pList->head;
	while(pNode != null){
		iTempResult = fnWalk(&(pNode->value));
		if(iTempResult < 0){
			break;
		}
		pNode = pNode->next;
	}

	return 0;
}

int et_lists_sort(EtLists* pList, et_fn_cmp_sz fnCmp, bool isDesc){
	char* szTemp = null;
	EtListsNode* pNodeHead1 = null;
	EtListsNode* pNodeHead2 = null;
	int iCmpResult = 0;

	if(pList == null){
		return -1;
	}

	if(pList->size <= 1){
		return 0;
	}
	if(fnCmp == null){
		fnCmp = et_fn_cmp_sz_default;
	}

	pNodeHead1 = pList->head;
	while(pNodeHead1 != null){
		if(pNodeHead1->next == null){
			break;
		}
		pNodeHead2 = pNodeHead1->next;
		while(pNodeHead2 != null){
			iCmpResult = fnCmp(pNodeHead1->value,pNodeHead2->value);
			if( (iCmpResult > 0 && !isDesc)
				|| (iCmpResult < 0 && isDesc)
			){
				szTemp = pNodeHead1->value;
				pNodeHead1->value = pNodeHead2->value;
				pNodeHead2->value = szTemp;
			}
			pNodeHead2 = pNodeHead2->next;
		}
		pNodeHead1 = pNodeHead1->next;
	}

	return 0;
}



////////////////////////////////////////////////////////////////////////////


int et_map_ii_node_create(EtMapIiNode** ppNode, INT64 iKey, INT64 iValue){
	*ppNode = (EtMapIiNode*)et_alloc(sizeof(EtMapIiNode));
	et_memset(*ppNode,0,sizeof(EtMapIiNode));
	(*ppNode)->cb = sizeof(EtMapIiNode);
	(*ppNode)->prev = null;
	(*ppNode)->next = null;
	(*ppNode)->key = iKey;
	(*ppNode)->value = iValue;

	return 0;
}

int et_map_ii_node_destroy(EtMapIiNode** ppNode){
	if(*ppNode == null){
		return 0;
	}

	et_memset(*ppNode,0,sizeof(EtMapIiNode));
	et_free(*ppNode);
	*ppNode = null;

	return 0;
}

int et_map_ii_create(EtMapIi** ppMap){

	*ppMap = (EtMapIi*)et_alloc(sizeof(EtMapIi));
	et_memset(*ppMap,0,sizeof(EtMapIi));
	(*ppMap)->cb = sizeof(EtMapIi);
	(*ppMap)->head = null;
	(*ppMap)->foot = null;
	(*ppMap)->size = 0;
	
	return 0;
}

int et_map_ii_destroy(EtMapIi** ppMap){
	if(*ppMap == null){
		return 0;
	}

	et_map_ii_clear(*ppMap);
	et_memset(*ppMap,0,sizeof(EtMapIi));
	et_free(*ppMap);
	*ppMap = null;

	return 0;
}

int et_map_ii_size(const EtMapIi* pMap){
	if(pMap == null){
		return -1;
	}

	return pMap->size;
}

int et_map_ii_iterator_start(EtMapIi* pMap){
	if(pMap == null){
		return ET_ERR_MEM_NULL;
	}
	pMap->iterator = pMap->head;

	return 0;
}

bool et_map_ii_iterator_check(EtMapIi* pMap){
	if(pMap == null){
		return false;
	}

	if(pMap->iterator != null){
		return true;
	}

	return false;
}

INT64 et_map_ii_iterator_key(EtMapIi* pMap){
	if(pMap == null){
		return null;
	}

	if(pMap->iterator == null){
		return null;
	}

	return pMap->iterator->key;
}

INT64 et_map_ii_iterator_value(EtMapIi* pMap){
	if(pMap == null){
		return null;
	}

	if(pMap->iterator == null){
		return null;
	}

	return pMap->iterator->value;
}


int et_map_ii_iterator_move(EtMapIi* pMap){
	if(pMap == null){
		return ET_ERR_MEM_NULL;
	}

	if(pMap->iterator == null){
		return ET_ERR_MEM_NULL;
	}

	pMap->iterator = pMap->iterator->next;

	return 0;

}

int et_map_ii_add(EtMapIi* pMap, INT64 iKey, INT64 iValue){
	int iRet = 0;
	EtMapIiNode* pNodeNew = null;

	if(pMap == null){
		return -1;
	}


	iRet = et_map_ii_node_create(&pNodeNew,iKey,iValue);
	if(iRet < 0){
		return iRet;
	}

	if(pMap->size == 0){
		pMap->head = pNodeNew;
	}else{
		pMap->foot->next = pNodeNew;
		pNodeNew->prev = pMap->foot;
	}

	pMap->foot = pNodeNew;
	pMap->size++;

	return 0;
}

int et_map_ii_remove(EtMapIi* pMap){
	EtMapIiNode* pFootNode = null;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return 0;
	}

	pFootNode = pMap->foot;
	if(pMap->size == 1){
		pMap->head = null;
		pMap->foot = null;
	}else{
		pMap->foot = pMap->foot->prev;
		pMap->foot->next = null;
	}
	et_map_ii_node_destroy(&pFootNode);
	pMap->size--;
	return 0;
}

int et_map_ii_copy(EtMapIi* pMapDest, const EtMapIi* pMapSrc){
	EtMapIiNode* pNode = null;

	if(pMapDest == null || pMapSrc == null){
		return -1;
	}


	et_map_ii_clear(pMapDest);
	pNode = pMapSrc->head;
	while(pNode != null){
		et_map_ii_add(pMapDest,pNode->key,pNode->value);
		pNode = pNode->next;
	}

	return 0;
}

int et_map_ii_clear(EtMapIi* pMap){
	if(pMap == null){
		return -1;
	}


	while(pMap->size > 0){
		et_map_ii_remove(pMap);
	}

	return 0;
}

bool et_map_ii_contains_key(const EtMapIi* pMap, INT64 iKey){
	EtMapIiNode* pNode = null;

	if(pMap == null){
		return -1;
	}


	pNode = pMap->head;
	while(pNode != null){
		if(iKey == pNode->key){
			return true;
		}
		pNode = pNode->next;
	}
	return false;
}

bool et_map_ii_contains_value(const EtMapIi* pMap, INT64 iValue){
	EtMapIiNode* pNode = null;

	if(pMap == null){
		return -1;
	}


	pNode = pMap->head;
	while(pNode != null){
		if(iValue == pNode->value){
			return true;
		}
		pNode = pNode->next;
	}

	return false;
}

bool et_map_ii_count_value(const EtMapIi* pMap, INT64 iValue){
	int iCount = 0;
	EtMapIiNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		if(iValue == pNode->value){
			iCount++;
		}
		pNode = pNode->next;
	}

	return iCount;
}

int et_map_ii_insert(EtMapIi* pMap, int iIndex, INT64 iKey, INT64 iValue){
	int iRet = 0;
	EtMapIiNode* pNodePrev = null;
	int iCount = 0;
	EtMapIiNode* pNode = null;
	EtMapIiNode* pNodeNew = null;

	if(pMap == null){
		return -1;
	}

	if(iIndex > pMap->size){
		return -1;
	}

	iRet = et_map_ii_node_create(&pNodeNew,iKey,iValue);
	if(iRet < 0){
		return iRet;
	}
	pNode = pMap->head;
	
	while(iCount <= iIndex){
		if(iCount == iIndex){
			pNodeNew->prev = pNodePrev;
			pNodeNew->next = pNode;

			if(pNodePrev != null){
				pNodePrev->next = pNodeNew;
			}else{
				pMap->head = pNodeNew;
			}

			if(pNode != null){
				pNode->prev = pNodeNew;
			}else{
				pMap->foot = pNodeNew;
			}

			pMap->size++;
			
			break;
		}
		iCount++;
		pNodePrev = pNode;
		pNode = pNode->next;
	}

	return 0;
}

int et_map_ii_find_insert_pos(const EtMapIi* pMap, INT64 iKey, et_fn_cmp_int fnCmp, bool isDesc){
	EtMapIiNode* pNode = null;
	int iCmpResult = 0;
	int iCount = 0;

	if(pMap == null){
		return -1;
	}

	if(fnCmp == null){
		fnCmp = et_fn_cmp_int_default;
	}

	pNode = pMap->head;
	while(pNode != null){
		iCmpResult = fnCmp(pNode->key,iKey);
		if( (iCmpResult > 0 && !isDesc)
			|| (iCmpResult < 0 && isDesc)
		){
			return iCount;
		}
		pNode = pNode->next;
		iCount++;
	}

	return iCount;
}

int et_map_ii_get(const EtMapIi* pMap, INT64 iKey, INT64* pValue){
	int iCount = 0;
	EtMapIiNode* pNode = null;
	bool bFound = false;

	if(pMap == null){
		return -1;
	}


	pNode = pMap->head;
	while(iCount < pMap->size){
		if(pNode->key == iKey){
			*pValue = pNode->value;
			bFound = true;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	if(!bFound){
		return ET_ERR_LOST;
	}

	return 0;
}

int et_map_ii_set(EtMapIi* pMap, INT64 iKey, INT64 iValue){
	int iCount = 0;
	bool bFound = false;
	EtMapIiNode* pNode = null;

	if(pMap == null){
		return -1;
	}


	pNode = pMap->head;
	while(iCount < pMap->size){
		if(pNode->key == iKey){
			pNode->value = iValue;
			bFound = true;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}
	if(!bFound){
		et_map_ii_add(pMap,iKey,iValue);
	}

	return 0;
}

int et_map_ii_drop(EtMapIi* pMap, INT64 iKey){
	EtMapIiNode* pNodePrev = null;
	EtMapIiNode* pNode = null;
	bool bFound = false;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return -1;
	}

	pNode = pMap->head;

	while(pNode != null){
		if(iKey == pNode->key){
			if(pNodePrev != null){
				pNodePrev->next = pNode->next;
			}else{
				pMap->head = pNode->next;
			}

			if(pNode->next != null){
				pNode->next->prev = pNode->prev;
			}else{
				pMap->foot = pNode->prev;
			}		

			pMap->size--;
			bFound = true;
			break;
		}
		pNodePrev = pNode;
		pNode = pNode->next;
	}
	if(bFound){
		et_map_ii_node_destroy(&pNode);
	}

	return 0;
}

int et_map_ii_insert_by_order(EtMapIi* pMap, INT64 iKey, INT64 iValue, et_fn_cmp_int fnCmp, bool isDesc){
	int iInsertIndex = 0;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return et_map_ii_add(pMap,iKey,iValue);
	}

	iInsertIndex = et_map_ii_find_insert_pos(pMap,iKey,fnCmp,isDesc);
	if(iInsertIndex < 0){
		return iInsertIndex;
	}

	return et_map_ii_insert(pMap,iInsertIndex,iKey,iValue);
}

int et_map_ii_insert_asc(EtMapIi* pMap, INT64 iKey, INT64 iValue){
	return et_map_ii_insert_by_order(pMap,iKey,iValue,null,false);
}

int et_map_ii_insert_desc(EtMapIi* pMap, INT64 iKey, INT64 iValue){
	return et_map_ii_insert_by_order(pMap,iKey,iValue,null,true);
}

int et_map_ii_walk(EtMapIi* pMap, et_map_ii_walk_fn fnWalk){
	EtMapIiNode* pNode = null;
	int iTempResult = 0;

	if(pMap == null){
		return -1;
	}


	pNode = pMap->head;
	while(pNode != null){
		iTempResult = fnWalk(&(pNode->key),&(pNode->value));
		if(iTempResult < 0){
			break;
		}
		pNode = pNode->next;
	}

	return 0;
}

int et_map_ii_sort(EtMapIi* pMap, et_fn_cmp_int fnCmp, bool isDesc){
	INT64 iTempKey = 0;
	INT64 iTempValue = 0;
	EtMapIiNode* pNodeHead1 = null;
	EtMapIiNode* pNodeHead2 = null;
	int iCmpResult = 0;

	if(pMap == null){
		return -1;
	}

	if(pMap->size <= 1){
		return 0;
	}

	pNodeHead1 = pMap->head;
	while(pNodeHead1 != null){
		if(pNodeHead1->next == null){
			break;
		}
		pNodeHead2 = pNodeHead1->next;
		while(pNodeHead2 != null){
			iCmpResult = fnCmp(pNodeHead1->key,pNodeHead2->key);
			if( (iCmpResult > 0 && !isDesc)
				|| (iCmpResult < 0 && isDesc)
			){
				iTempKey = pNodeHead1->key;
				pNodeHead1->key = pNodeHead2->key;
				pNodeHead2->key = iTempKey;

				iTempValue = pNodeHead1->value;
				pNodeHead1->value = pNodeHead2->value;
				pNodeHead2->value = iTempValue;
			}
			pNodeHead2 = pNodeHead2->next;
		}
		pNodeHead1 = pNodeHead1->next;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int et_map_ss_node_create(EtMapSsNode** ppNode, const char* szKey, const char* szValue){
	char* szKeyNew = null;
	char* szValueNew = null;

	if(szKey == null){
		return null;
	}
	if(szValue == null){
		return null;
	}
	if((*ppNode) != null){
		return -1;
	}

	szKeyNew = (char*)et_alloc(et_strlen(szKey) + 1);
	if(szKeyNew == null){
		return null;
	}
	et_strcpy(szKeyNew,szKey);

	szValueNew = (char*)et_alloc(et_strlen(szValue) + 1);
	if(szValueNew == null){
		et_free(szKeyNew);
		return null;
	}
	et_strcpy(szValueNew,szValue);

	*ppNode = (EtMapSsNode*)et_alloc(sizeof(EtMapSsNode));
	et_memset(*ppNode,0,sizeof(EtMapSsNode));
	(*ppNode)->cb = sizeof(EtMapSsNode);
	(*ppNode)->prev = null;
	(*ppNode)->next = null;
	(*ppNode)->key = szKeyNew;
	(*ppNode)->value = szValueNew;

	return 0;
}

int et_map_ss_node_destroy(EtMapSsNode** ppNode){
	if(*ppNode == null){
		return 0;
	}

	et_free((*ppNode)->key);
	et_free((*ppNode)->value);
	et_memset(*ppNode,0,sizeof(EtMapSsNode));
	et_free(*ppNode);
	*ppNode = null;

	return 0;
}

int et_map_ss_create(EtMapSs** ppMap){
	*ppMap = (EtMapSs*)et_alloc(sizeof(EtMapSs));
	et_memset(*ppMap,0,sizeof(EtMapSs));
	(*ppMap)->cb = sizeof(EtMapSs);
	(*ppMap)->head = null;
	(*ppMap)->foot = null;
	(*ppMap)->size = 0;
	
	return 0;
}

int et_map_ss_destroy(EtMapSs** ppMap){
	if(*ppMap == null){
		return 0;
	}

	et_map_ss_clear(*ppMap);
	et_memset(*ppMap,0,sizeof(EtMapSs));
	et_free(*ppMap);
	*ppMap = null;

	return 0;
}

int et_map_ss_size(const EtMapSs* pMap){
	if(pMap == null){
		return -1;
	}

	return pMap->size;
}

int et_map_ss_iterator_start(EtMapSs* pMap){
	if(pMap == null){
		return ET_ERR_MEM_NULL;
	}
	pMap->iterator = pMap->head;

	return 0;
}

bool et_map_ss_iterator_check(EtMapSs* pMap){
	if(pMap == null){
		return false;
	}

	if(pMap->iterator != null){
		return true;
	}

	return false;
}

const char* et_map_ss_iterator_key(EtMapSs* pMap){
	if(pMap == null){
		return null;
	}

	if(pMap->iterator == null){
		return null;
	}

	return pMap->iterator->key;
}

char* et_map_ss_iterator_value(EtMapSs* pMap){
	if(pMap == null){
		return null;
	}

	if(pMap->iterator == null){
		return null;
	}

	return pMap->iterator->value;
}

int et_map_ss_iterator_move(EtMapSs* pMap){
	if(pMap == null){
		return ET_ERR_MEM_NULL;
	}

	if(pMap->iterator == null){
		return ET_ERR_MEM_NULL;
	}

	pMap->iterator = pMap->iterator->next;

	return 0;

}

int et_map_ss_add(EtMapSs* pMap, const char* szKey, const char* szValue){
	int iRet = 0;
	EtMapSsNode* pNodeNew = null;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}
	if(szValue == null){
		return -1;
	}

	iRet = et_map_ss_node_create(&pNodeNew,szKey,szValue);
	if(iRet < 0){
		return iRet;
	}

	if(pMap->size == 0){
		pMap->head = pNodeNew;
	}else{
		pMap->foot->next = pNodeNew;
		pNodeNew->prev = pMap->foot;
	}

	pMap->foot = pNodeNew;
	pMap->size++;
	return 0;
}

int et_map_ss_remove(EtMapSs* pMap){
	EtMapSsNode* pFootNode = null;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return 0;
	}

	pFootNode = pMap->foot;
	if(pMap->size == 1){
		pMap->head = null;
		pMap->foot = null;
	}else{
		pMap->foot = pMap->foot->prev;
		pMap->foot->next = null;
	}
	et_map_ss_node_destroy(&pFootNode);
	pMap->size--;
	return 0;
}

int et_map_ss_copy(EtMapSs* pMapDest, const EtMapSs* pMapSrc){
	EtMapSsNode* pNode = null;

	if(pMapDest == null || pMapSrc == null){
		return -1;
	}

	et_map_ss_clear(pMapDest);
	pNode = pMapSrc->head;
	while(pNode != null){
		et_map_ss_add(pMapDest,pNode->key,pNode->value);
		pNode = pNode->next;
	}

	return 0;
}

int et_map_ss_clear(EtMapSs* pMap){
	if(pMap == null){
		return -1;
	}

	while(pMap->size > 0){
		et_map_ss_remove(pMap);
	}

	return 0;
}

bool et_map_ss_contains_key(const EtMapSs* pMap, const char* szKey){
	EtMapSsNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		if(et_strcmp(szKey,pNode->key) == 0){
			return true;
		}
		pNode = pNode->next;
	}
	return false;
}

bool et_map_ss_contains_value(const EtMapSs* pMap, const char* szValue){
	EtMapSsNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	if(szValue == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		if(et_strcmp(szValue,pNode->value) == 0){
			return true;
		}
		pNode = pNode->next;
	}
	return false;
}

bool et_map_ss_count_value(const EtMapSs* pMap, const char* szValue){
	int iCount = 0;
	EtMapSsNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	if(szValue == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		if(et_strcmp(szValue,pNode->value) == 0){
			iCount++;
		}
		pNode = pNode->next;
	}

	return iCount;
}

int et_map_ss_get(const EtMapSs* pMap, const char* szKey, char* szValue){
	int iCount = 0;
	EtMapSsNode* pNode = null;
	bool bFound = false;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}
	if(szValue == null){
		return -1;
	}

	pNode = pMap->head;
	while(iCount < pMap->size){
		if(et_strcmp(szKey,pNode->key) == 0){
			et_strcpy(szValue,pNode->value);
			bFound = true;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	if(!bFound){
		return ET_ERR_LOST;
	}

	return 0;
}

int et_map_ss_get_len(const EtMapSs* pMap, const char* szKey){
	int iCount = 0;
	EtMapSsNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	pNode = pMap->head;
	while(iCount < pMap->size){
		if(et_strcmp(szKey,pNode->key) == 0){
			return et_strlen(pNode->value);
		}
		iCount++;
		pNode = pNode->next;
	}

	return ET_ERR_LOST;
}

int et_map_ss_set(EtMapSs* pMap, const char* szKey, const char* szValue){
	int iCount = 0;
	bool bFound = false;
	EtMapSsNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}
	if(szValue == null){
		return -1;
	}

	pNode = pMap->head;

	while(iCount < pMap->size){
		if(et_strcmp(szKey,pNode->key) == 0){
			et_free(pNode->value);
			pNode->value = (char*)et_alloc(et_strlen(szValue) + 1);
			et_strcpy(pNode->value,szValue);
			bFound = true;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	if(!bFound){
		et_map_ss_add(pMap,szKey,szValue);
	}

	return 0;
}

int et_map_ss_insert(EtMapSs* pMap, int iIndex, const char* szKey, const char* szValue){
	int iRet = 0;
	EtMapSsNode* pNodePrev = null;
	int iCount = 0;
	EtMapSsNode* pNode = null;
	EtMapSsNode* pNodeNew = null;

	if(pMap == null){
		return -1;
	}

	if(iIndex > pMap->size){
		return -1;
	}
	if(szKey == null){
		return -1;
	}
	if(szValue == null){
		return -1;
	}

	iRet = et_map_ss_node_create(&pNodeNew,szKey,szValue);
	if(iRet < 0){
		return iRet;
	}

	pNode = pMap->head;

	while(iCount <= iIndex){
		if(iCount == iIndex){
			pNodeNew->prev = pNodePrev;
			pNodeNew->next = pNode;

			if(pNodePrev != null){
				pNodePrev->next = pNodeNew;
			}else{
				pMap->head = pNodeNew;
			}

			if(pNode != null){
				pNode->prev = pNodeNew;
			}else{
				pMap->foot = pNodeNew;
			}

			pMap->size++;
			
			break;
		}
		iCount++;
		pNodePrev = pNode;
		pNode = pNode->next;
	}

	return 0;
}

int et_map_ss_find_insert_pos(const EtMapSs* pMap, const char* szKey, et_fn_cmp_sz fnCmp, bool isDesc){
	EtMapSsNode* pNode = null;
	int iCmpResult = 0;
	int iCount = 0;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	if(fnCmp == null){
		fnCmp = et_fn_cmp_sz_default;
	}

	pNode = pMap->head;
	while(pNode != null){
		iCmpResult = fnCmp(pNode->key,szKey);
		if( (iCmpResult > 0 && !isDesc)
			|| (iCmpResult < 0 && isDesc)
		){
			return iCount;
		}
		pNode = pNode->next;
		iCount++;
	}

	return iCount;
}

int et_map_ss_drop(EtMapSs* pMap, const char* szKey){
	EtMapSsNode* pNodePrev = null;
	EtMapSsNode* pNode = null;
	bool bFound = false;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return -1;
	}

	pNode = pMap->head;

	while(pNode != null){
		if(et_strcmp(szKey,pNode->key) == 0){
			if(pNodePrev != null){
				pNodePrev->next = pNode->next;
			}else{
				pMap->head = pNode->next;
			}

			if(pNode->next != null){
				pNode->next->prev = pNode->prev;
			}else{
				pMap->foot = pNode->prev;
			}		

			pMap->size--;
			bFound = true;
			break;
		}
		pNodePrev = pNode;
		pNode = pNode->next;
	}
	if(bFound){
		et_map_ss_node_destroy(&pNode);
	}

	return 0;
}

int et_map_ss_insert_by_order(EtMapSs* pMap, const char* szKey, const char* szValue, et_fn_cmp_sz fnCmp, bool isDesc){
	int iInsertIndex = 0;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return et_map_ss_add(pMap,szKey,szValue);
	}

	iInsertIndex = et_map_ss_find_insert_pos(pMap,szKey,fnCmp,isDesc);
	if(iInsertIndex < 0){
		return iInsertIndex;
	}

	return et_map_ss_insert(pMap,iInsertIndex,szKey,szValue);
}

int et_map_ss_insert_asc(EtMapSs* pMap, const char* szKey, const char* szValue){
	return et_map_ss_insert_by_order(pMap,szKey,szValue,null,false);
}

int et_map_ss_insert_desc(EtMapSs* pMap, const char* szKey, const char* szValue){
	return et_map_ss_insert_by_order(pMap,szKey,szValue,null,true);
}

int et_map_ss_walk(EtMapSs* pMap, et_map_ss_walk_fn fnWalk){
	EtMapSsNode* pNode = null;
	int iTempResult = 0;

	if(pMap == null){
		return -1;
	}


	pNode = pMap->head;
	while(pNode != null){
		iTempResult = fnWalk(&(pNode->key),&(pNode->value));
		if(iTempResult < 0){
			break;
		}
		pNode = pNode->next;
	}

	return 0;
}

int et_map_ss_sort(EtMapSs* pMap, et_fn_cmp_sz fnCmp, bool isDesc){
	int iCmpResult = 0;
	char* szTempKey = 0;
	char* szTempValue = 0;
	EtMapSsNode* pNodeHead1 = null;
	EtMapSsNode* pNodeHead2 = null;

	if(pMap == null){
		return -1;
	}

	if(pMap->size <= 1){
		return 0;
	}

	pNodeHead1 = pMap->head;
	while(pNodeHead1 != null){
		if(pNodeHead1->next == null){
			break;
		}
		pNodeHead2 = pNodeHead1->next;
		while(pNodeHead2 != null){
			iCmpResult = fnCmp(pNodeHead1->key,pNodeHead2->key);
			if( (iCmpResult > 0 && !isDesc)
				|| (iCmpResult < 0 && isDesc)
			){
				szTempKey = pNodeHead1->key;
				pNodeHead1->key = pNodeHead2->key;
				pNodeHead2->key = szTempKey;

				szTempValue = pNodeHead1->value;
				pNodeHead1->value = pNodeHead2->value;
				pNodeHead2->value = szTempValue;
			}
			pNodeHead2 = pNodeHead2->next;
		}
		pNodeHead1 = pNodeHead1->next;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////

int et_map_si_node_create(EtMapSiNode** ppNode, const char* szKey, INT64 iValue){
	char* szKeyNew = null;

	if(szKey == null){
		return null;
	}
	if((*ppNode) != null){
		return -1;
	}

	szKeyNew = (char*)et_alloc(et_strlen(szKey) + 1);
	if(szKeyNew == null){
		return null;
	}
	et_strcpy(szKeyNew,szKey);

	*ppNode = (EtMapSiNode*)et_alloc(sizeof(EtMapSiNode));
	et_memset(*ppNode,0,sizeof(EtMapSiNode));
	(*ppNode)->cb = sizeof(EtMapSiNode);
	(*ppNode)->prev = null;
	(*ppNode)->next = null;
	(*ppNode)->key = szKeyNew;
	(*ppNode)->value = iValue;

	return 0;
}

int et_map_si_node_destroy(EtMapSiNode** ppNode){
	if(*ppNode == null){
		return 0;
	}

	et_free((*ppNode)->key);
	et_memset(*ppNode,0,sizeof(EtMapSiNode));
	et_free(*ppNode);
	*ppNode = null;

	return 0;
}

int et_map_si_create(EtMapSi** ppMap){
	*ppMap = (EtMapSi*)et_alloc(sizeof(EtMapSi));
	et_memset(*ppMap,0,sizeof(EtMapSi));
	(*ppMap)->cb = sizeof(EtMapSi);
	(*ppMap)->head = null;
	(*ppMap)->foot = null;
	(*ppMap)->iterator = null;
	(*ppMap)->size = 0;
	
	return 0;
}

int et_map_si_destroy(EtMapSi** ppMap){
	if(*ppMap == null){
		return 0;
	}

	et_map_si_clear(*ppMap);
	et_memset(*ppMap,0,sizeof(EtMapSi));
	et_free(*ppMap);
	*ppMap = null;

	return 0;
}

int et_map_si_size(const EtMapSi* pMap){
	if(pMap == null){
		return -1;
	}

	return pMap->size;
}

int et_map_si_iterator_start(EtMapSi* pMap){
	if(pMap == null){
		return ET_ERR_MEM_NULL;
	}
	pMap->iterator = pMap->head;

	return 0;
}

bool et_map_si_iterator_check(EtMapSi* pMap){
	if(pMap == null){
		return false;
	}

	if(pMap->iterator != null){
		return true;
	}

	return false;
}

const char* et_map_si_iterator_key(EtMapSi* pMap){
	if(pMap == null){
		return null;
	}

	if(pMap->iterator == null){
		return null;
	}

	return pMap->iterator->key;
}

INT64 et_map_si_iterator_value(EtMapSi* pMap){
	if(pMap == null){
		return null;
	}

	if(pMap->iterator == null){
		return null;
	}

	return pMap->iterator->value;
}


int et_map_si_iterator_move(EtMapSi* pMap){
	if(pMap == null){
		return ET_ERR_MEM_NULL;
	}

	if(pMap->iterator == null){
		return ET_ERR_MEM_NULL;
	}

	pMap->iterator = pMap->iterator->next;

	return 0;

}

int et_map_si_add(EtMapSi* pMap, const char* szKey, INT64 iValue){
	int iRet = 0;
	EtMapSiNode* pNodeNew = null;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	iRet = et_map_si_node_create(&pNodeNew,szKey,iValue);
	if(iRet < 0){
		return iRet;
	}

	if(pMap->size == 0){
		pMap->head = pNodeNew;
	}else{
		pMap->foot->next = pNodeNew;
		pNodeNew->prev = pMap->foot;
	}

	pMap->foot = pNodeNew;
	pMap->size++;
	return 0;
}

int et_map_si_remove(EtMapSi* pMap){
	EtMapSiNode* pFootNode = null;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return 0;
	}

	pFootNode = pMap->foot;
	if(pMap->size == 1){
		pMap->head = null;
		pMap->foot = null;
	}else{
		pMap->foot = pMap->foot->prev;
		pMap->foot->next = null;
	}
	et_map_si_node_destroy(&pFootNode);
	pMap->size--;
	return 0;
}

int et_map_si_copy(EtMapSi* pMapDest, const EtMapSi* pMapSrc){
	EtMapSiNode* pNode = null;

	if(pMapDest == null || pMapSrc == null){
		return -1;
	}

	et_map_si_clear(pMapDest);
	pNode = pMapSrc->head;
	while(pNode != null){
		et_map_si_add(pMapDest,pNode->key,pNode->value);
		pNode = pNode->next;
	}

	return 0;
}

int et_map_si_clear(EtMapSi* pMap){
	if(pMap == null){
		return -1;
	}

	while(pMap->size > 0){
		et_map_si_remove(pMap);
	}

	return 0;
}

bool et_map_si_contains_key(const EtMapSi* pMap, const char* szKey){
	EtMapSiNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		if(et_strcmp(szKey,pNode->key) == 0){
			return true;
		}
		pNode = pNode->next;
	}
	return false;
}

bool et_map_si_contains_value(const EtMapSi* pMap, INT64 iValue){
	EtMapSiNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		if(pNode->value == iValue){
			return true;
		}
		pNode = pNode->next;
	}
	return false;
}

bool et_map_si_count_value(const EtMapSi* pMap, INT64 iValue){
	int iCount = 0;
	EtMapSiNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		if(pNode->value == iValue){
			iCount++;
		}
		pNode = pNode->next;
	}

	return iCount;
}

int et_map_si_get(const EtMapSi* pMap, const char* szKey, INT64 *pValue){
	int iCount = 0;
	EtMapSiNode* pNode = null;
	bool bFound = false;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	pNode = pMap->head;
	while(iCount < pMap->size){
		if(et_strcmp(szKey,pNode->key) == 0){
			*pValue = pNode->value;
			bFound = true;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	if(!bFound){
		return ET_ERR_LOST;
	}

	return 0;
}

int et_map_si_set(EtMapSi* pMap, const char* szKey, INT64 iValue){
	int iCount = 0;
	bool bFound = false;
	EtMapSiNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	pNode = pMap->head;

	while(iCount < pMap->size){
		if(et_strcmp(szKey,pNode->key) == 0){
			pNode->value = iValue;
			bFound = true;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	if(!bFound){
		et_map_si_add(pMap,szKey,iValue);
	}

	return 0;
}

int et_map_si_insert(EtMapSi* pMap, int iIndex, const char* szKey, INT64 iValue){
	int iRet = 0;
	EtMapSiNode* pNodePrev = null;
	int iCount = 0;
	EtMapSiNode* pNode = null;
	EtMapSiNode* pNodeNew = null;

	if(pMap == null){
		return -1;
	}

	if(iIndex > pMap->size){
		return -1;
	}
	if(szKey == null){
		return -1;
	}

	iRet = et_map_si_node_create(&pNodeNew,szKey,iValue);
	if(iRet < 0){
		return iRet;
	}

	pNode = pMap->head;

	while(iCount <= iIndex){
		if(iCount == iIndex){
			pNodeNew->prev = pNodePrev;
			pNodeNew->next = pNode;

			if(pNodePrev != null){
				pNodePrev->next = pNodeNew;
			}else{
				pMap->head = pNodeNew;
			}

			if(pNode != null){
				pNode->prev = pNodeNew;
			}else{
				pMap->foot = pNodeNew;
			}

			pMap->size++;
			
			break;
		}
		iCount++;
		pNodePrev = pNode;
		pNode = pNode->next;
	}

	return 0;
}

int et_map_si_find_insert_pos(const EtMapSi* pMap, const char* szKey, et_fn_cmp_sz fnCmp, bool isDesc){
	EtMapSiNode* pNode = null;
	int iCmpResult = 0;
	int iCount = 0;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	if(fnCmp == null){
		fnCmp = et_fn_cmp_sz_default;
	}

	pNode = pMap->head;
	while(pNode != null){
		iCmpResult = fnCmp(pNode->key,szKey);
		if( (iCmpResult > 0 && !isDesc)
			|| (iCmpResult < 0 && isDesc)
		){
			return iCount;
		}
		pNode = pNode->next;
		iCount++;
	}

	return iCount;
}

int et_map_si_drop(EtMapSi* pMap, const char* szKey){
	EtMapSiNode* pNodePrev = null;
	EtMapSiNode* pNode = null;
	bool bFound = false;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return -1;
	}

	pNode = pMap->head;

	while(pNode != null){
		if(et_strcmp(szKey,pNode->key) == 0){
			if(pNodePrev != null){
				pNodePrev->next = pNode->next;
			}else{
				pMap->head = pNode->next;
			}

			if(pNode->next != null){
				pNode->next->prev = pNode->prev;
			}else{
				pMap->foot = pNode->prev;
			}		

			pMap->size--;
			bFound = true;
			break;
		}
		pNodePrev = pNode;
		pNode = pNode->next;
	}
	if(bFound){
		et_map_si_node_destroy(&pNode);
	}

	return 0;
}

int et_map_si_insert_by_order(EtMapSi* pMap, const char* szKey, INT64 iValue, et_fn_cmp_sz fnCmp, bool isDesc){
	int iInsertIndex = 0;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return et_map_si_add(pMap,szKey,iValue);
	}

	iInsertIndex = et_map_si_find_insert_pos(pMap,szKey,fnCmp,isDesc);
	if(iInsertIndex < 0){
		return iInsertIndex;
	}

	return et_map_si_insert(pMap,iInsertIndex,szKey,iValue);
}

int et_map_si_insert_asc(EtMapSi* pMap, const char* szKey, INT64 iValue){
	return et_map_si_insert_by_order(pMap,szKey,iValue,null,false);
}

int et_map_si_insert_desc(EtMapSi* pMap, const char* szKey, INT64 iValue){
	return et_map_si_insert_by_order(pMap,szKey,iValue,null,true);
}

int et_map_si_walk(EtMapSi* pMap, et_map_si_walk_fn fnWalk){
	EtMapSiNode* pNode = null;
	int iTempResult = 0;

	if(pMap == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		iTempResult = fnWalk(&(pNode->key),&(pNode->value));
		if(iTempResult < 0){
			break;
		}
		pNode = pNode->next;
	}

	return 0;
}

int et_map_si_sort(EtMapSi* pMap, et_fn_cmp_sz fnCmp, bool isDesc){
	int iCmpResult = 0;
	char* szTempKey = 0;
	INT64 iTempValue = 0;
	EtMapSiNode* pNodeHead1 = null;
	EtMapSiNode* pNodeHead2 = null;

	if(pMap == null){
		return -1;
	}

	if(pMap->size <= 1){
		return 0;
	}

	if(fnCmp == null){
		fnCmp = et_fn_cmp_sz_default;
	}

	pNodeHead1 = pMap->head;
	while(pNodeHead1 != null){
		if(pNodeHead1->next == null){
			break;
		}
		pNodeHead2 = pNodeHead1->next;
		while(pNodeHead2 != null){
			iCmpResult = fnCmp(pNodeHead1->key,pNodeHead2->key);
			if( (iCmpResult > 0 && !isDesc)
				|| (iCmpResult < 0 && isDesc)
			){
				szTempKey = pNodeHead1->key;
				pNodeHead1->key = pNodeHead2->key;
				pNodeHead2->key = szTempKey;

				iTempValue = pNodeHead1->value;
				pNodeHead1->value = pNodeHead2->value;
				pNodeHead2->value = iTempValue;
			}
			pNodeHead2 = pNodeHead2->next;
		}
		pNodeHead1 = pNodeHead1->next;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////


int et_map_sv_node_create(EtMapSvNode** ppNode, const char* szKey, void* pValue){
	char* szKeyNew = null;

	if(szKey == null){
		return null;
	}
	if((*ppNode) != null){
		return -1;
	}

	szKeyNew = (char*)et_alloc(et_strlen(szKey) + 1);
	if(szKeyNew == null){
		return null;
	}
	et_strcpy(szKeyNew,szKey);

	*ppNode = (EtMapSvNode*)et_alloc(sizeof(EtMapSvNode));
	et_memset(*ppNode,0,sizeof(EtMapSvNode));
	(*ppNode)->cb = sizeof(EtMapSvNode);
	(*ppNode)->prev = null;
	(*ppNode)->next = null;
	(*ppNode)->key = szKeyNew;
	(*ppNode)->value = pValue;

	return 0;
}

int et_map_sv_node_destroy(EtMapSvNode** ppNode){
	if(*ppNode == null){
		return 0;
	}

	et_free((*ppNode)->key);
	et_memset(*ppNode,0,sizeof(EtMapSvNode));
	et_free(*ppNode);
	*ppNode = null;

	return 0;
}

int et_map_sv_create(EtMapSv** ppMap){
	*ppMap = (EtMapSv*)et_alloc(sizeof(EtMapSv));
	et_memset(*ppMap,0,sizeof(EtMapSv));
	(*ppMap)->cb = sizeof(EtMapSv);
	(*ppMap)->head = null;
	(*ppMap)->foot = null;
	(*ppMap)->iterator = null;
	(*ppMap)->size = 0;
	
	return 0;
}

int et_map_sv_destroy(EtMapSv** ppMap){
	if(*ppMap == null){
		return 0;
	}

	et_map_sv_clear(*ppMap);
	et_memset(*ppMap,0,sizeof(EtMapSv));
	et_free(*ppMap);
	*ppMap = null;

	return 0;
}

int et_map_sv_size(const EtMapSv* pMap){
	if(pMap == null){
		return -1;
	}

	return pMap->size;
}

int et_map_sv_iterator_start(EtMapSv* pMap){
	if(pMap == null){
		return ET_ERR_MEM_NULL;
	}
	pMap->iterator = pMap->head;

	return 0;
}

bool et_map_sv_iterator_check(EtMapSv* pMap){
	if(pMap == null){
		return false;
	}

	if(pMap->iterator != null){
		return true;
	}

	return false;
}

const char* et_map_sv_iterator_key(EtMapSv* pMap){
	if(pMap == null){
		return null;
	}

	if(pMap->iterator == null){
		return null;
	}

	return pMap->iterator->key;
}

void* et_map_sv_iterator_value(EtMapSv* pMap){
	if(pMap == null){
		return null;
	}

	if(pMap->iterator == null){
		return null;
	}

	return pMap->iterator->value;
}

int et_map_sv_iterator_move(EtMapSv* pMap){
	if(pMap == null){
		return ET_ERR_MEM_NULL;
	}

	if(pMap->iterator == null){
		return ET_ERR_MEM_NULL;
	}

	pMap->iterator = pMap->iterator->next;

	return 0;

}

int et_map_sv_add(EtMapSv* pMap, const char* szKey, void* pValue){
	int iRet = 0;
	EtMapSvNode* pNodeNew = null;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	iRet = et_map_sv_node_create(&pNodeNew,szKey,pValue);
	if(iRet < 0){
		return iRet;
	}

	if(pMap->size == 0){
		pMap->head = pNodeNew;
	}else{
		pMap->foot->next = pNodeNew;
		pNodeNew->prev = pMap->foot;
	}

	pMap->foot = pNodeNew;
	pMap->size++;
	return 0;
}

int et_map_sv_remove(EtMapSv* pMap){
	EtMapSvNode* pFootNode = null;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return 0;
	}

	pFootNode = pMap->foot;
	if(pMap->size == 1){
		pMap->head = null;
		pMap->foot = null;
	}else{
		pMap->foot = pMap->foot->prev;
		pMap->foot->next = null;
	}
	et_map_sv_node_destroy(&pFootNode);
	pMap->size--;
	return 0;
}

int et_map_sv_copy(EtMapSv* pMapDest, const EtMapSv* pMapSrc){
	EtMapSvNode* pNode = null;

	if(pMapDest == null || pMapSrc == null){
		return -1;
	}

	et_map_sv_clear(pMapDest);
	pNode = pMapSrc->head;
	while(pNode != null){
		et_map_sv_add(pMapDest,pNode->key,pNode->value);
		pNode = pNode->next;
	}

	return 0;
}

int et_map_sv_clear(EtMapSv* pMap){
	if(pMap == null){
		return -1;
	}

	while(pMap->size > 0){
		et_map_sv_remove(pMap);
	}

	return 0;
}

bool et_map_sv_contains_key(const EtMapSv* pMap, const char* szKey){
	EtMapSvNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		if(et_strcmp(szKey,pNode->key) == 0){
			return true;
		}
		pNode = pNode->next;
	}
	return false;
}

bool et_map_sv_contains_value(const EtMapSv* pMap, void* pValue){
	EtMapSvNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		if(pNode->value == pValue){
			return true;
		}
		pNode = pNode->next;
	}
	return false;
}

bool et_map_sv_count_value(const EtMapSv* pMap, void* pValue){
	int iCount = 0;
	EtMapSvNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		if(pNode->value == pValue){
			iCount++;
		}
		pNode = pNode->next;
	}

	return iCount;
}

int et_map_sv_get(const EtMapSv* pMap, const char* szKey, void** pValue){
	int iCount = 0;
	EtMapSvNode* pNode = null;
	bool bFound = false;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	pNode = pMap->head;
	while(iCount < pMap->size){
		if(et_strcmp(szKey,pNode->key) == 0){
			*pValue = pNode->value;
			bFound = true;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	if(!bFound){
		return ET_ERR_LOST;
	}

	return 0;
}

int et_map_sv_set(EtMapSv* pMap, const char* szKey, void* pValue){
	int iCount = 0;
	bool bFound = false;
	EtMapSvNode* pNode = null;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	pNode = pMap->head;

	while(iCount < pMap->size){
		if(et_strcmp(szKey,pNode->key) == 0){
			pNode->value = pValue;
			bFound = true;
			break;
		}
		iCount++;
		pNode = pNode->next;
	}

	if(!bFound){
		et_map_sv_add(pMap,szKey,pValue);
	}

	return 0;
}

int et_map_sv_insert(EtMapSv* pMap, int iIndex, const char* szKey, void* pValue){
	int iRet = 0;
	EtMapSvNode* pNodePrev = null;
	int iCount = 0;
	EtMapSvNode* pNode = null;
	EtMapSvNode* pNodeNew = null;

	if(pMap == null){
		return -1;
	}

	if(iIndex > pMap->size){
		return -1;
	}
	if(szKey == null){
		return -1;
	}

	iRet = et_map_sv_node_create(&pNodeNew,szKey,pValue);
	if(iRet < 0){
		return iRet;
	}

	pNode = pMap->head;

	while(iCount <= iIndex){
		if(iCount == iIndex){
			pNodeNew->prev = pNodePrev;
			pNodeNew->next = pNode;

			if(pNodePrev != null){
				pNodePrev->next = pNodeNew;
			}else{
				pMap->head = pNodeNew;
			}

			if(pNode != null){
				pNode->prev = pNodeNew;
			}else{
				pMap->foot = pNodeNew;
			}

			pMap->size++;
			
			break;
		}
		iCount++;
		pNodePrev = pNode;
		pNode = pNode->next;
	}

	return 0;
}

int et_map_sv_find_insert_pos(const EtMapSv* pMap, const char* szKey, et_fn_cmp_sz fnCmp, bool isDesc){
	EtMapSvNode* pNode = null;
	int iCmpResult = 0;
	int iCount = 0;

	if(pMap == null){
		return -1;
	}

	if(szKey == null){
		return -1;
	}

	if(fnCmp == null){
		fnCmp = et_fn_cmp_sz_default;
	}

	pNode = pMap->head;
	while(pNode != null){
		iCmpResult = fnCmp(pNode->key,szKey);
		if( (iCmpResult > 0 && !isDesc)
			|| (iCmpResult < 0 && isDesc)
		){
			return iCount;
		}
		pNode = pNode->next;
		iCount++;
	}

	return iCount;
}

int et_map_sv_drop(EtMapSv* pMap, const char* szKey){
	EtMapSvNode* pNodePrev = null;
	EtMapSvNode* pNode = null;
	bool bFound = false;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return -1;
	}

	pNode = pMap->head;

	while(pNode != null){
		if(et_strcmp(szKey,pNode->key) == 0){
			if(pNodePrev != null){
				pNodePrev->next = pNode->next;
			}else{
				pMap->head = pNode->next;
			}

			if(pNode->next != null){
				pNode->next->prev = pNode->prev;
			}else{
				pMap->foot = pNode->prev;
			}		

			pMap->size--;
			bFound = true;
			break;
		}
		pNodePrev = pNode;
		pNode = pNode->next;
	}
	if(bFound){
		et_map_sv_node_destroy(&pNode);
	}

	return 0;
}

int et_map_sv_insert_by_order(EtMapSv* pMap, const char* szKey, void* pValue, et_fn_cmp_sz fnCmp, bool isDesc){
	int iInsertIndex = 0;

	if(pMap == null){
		return -1;
	}

	if(pMap->size == 0){
		return et_map_sv_add(pMap,szKey,pValue);
	}

	iInsertIndex = et_map_sv_find_insert_pos(pMap,szKey,fnCmp,isDesc);
	if(iInsertIndex < 0){
		return iInsertIndex;
	}

	return et_map_sv_insert(pMap,iInsertIndex,szKey,pValue);
}

int et_map_sv_insert_asc(EtMapSv* pMap, const char* szKey, void* pValue){
	return et_map_sv_insert_by_order(pMap,szKey,pValue,null,false);
}

int et_map_sv_insert_desc(EtMapSv* pMap, const char* szKey, void* pValue){
	return et_map_sv_insert_by_order(pMap,szKey,pValue,null,true);
}

int et_map_sv_walk(EtMapSv* pMap, et_map_sv_walk_fn fnWalk){
	EtMapSvNode* pNode = null;
	int iTempResult = 0;

	if(pMap == null){
		return -1;
	}

	pNode = pMap->head;
	while(pNode != null){
		iTempResult = fnWalk(&(pNode->key),&(pNode->value));
		if(iTempResult < 0){
			break;
		}
		pNode = pNode->next;
	}

	return 0;
}

int et_map_sv_sort(EtMapSv* pMap, et_fn_cmp_sz fnCmp, bool isDesc){
	int iCmpResult = 0;
	char* szTempKey = 0;
	void* pTempValue = 0;
	EtMapSvNode* pNodeHead1 = null;
	EtMapSvNode* pNodeHead2 = null;

	if(pMap == null){
		return -1;
	}

	if(pMap->size <= 1){
		return 0;
	}

	if(fnCmp == null){
		fnCmp = et_fn_cmp_sz_default;
	}

	pNodeHead1 = pMap->head;
	while(pNodeHead1 != null){
		if(pNodeHead1->next == null){
			break;
		}
		pNodeHead2 = pNodeHead1->next;
		while(pNodeHead2 != null){
			iCmpResult = fnCmp(pNodeHead1->key,pNodeHead2->key);
			if( (iCmpResult > 0 && !isDesc)
				|| (iCmpResult < 0 && isDesc)
			){
				szTempKey = pNodeHead1->key;
				pNodeHead1->key = pNodeHead2->key;
				pNodeHead2->key = szTempKey;

				pTempValue = pNodeHead1->value;
				pNodeHead1->value = pNodeHead2->value;
				pNodeHead2->value = pTempValue;
			}
			pNodeHead2 = pNodeHead2->next;
		}
		pNodeHead1 = pNodeHead1->next;
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////////

INT64 et_math_power(INT64 iBase, INT64 iPow)
{
	int iResult = 1;
	int i = 0;

	if(iBase == 1 || iPow == 0)
	{
		return 1;
	}
	
	for(i = 0; i < iPow; i++)
	{
		iResult *= iBase;
	}
	
	return iResult;
}


INT64 et_math_round(FLOAT64 fSrc)
{
	INT64 iResult = 0;
	INT64 iTimes10 = 0;
	INT64 iRemain = 0;

	iResult = (INT64)fSrc;
	if(iResult == 0)
	{
		return 0;
	}

	iTimes10 = (INT64)(fSrc * 10);
	iRemain = iTimes10 % 10;
	if(iRemain >= 5)
	{
		iResult += 1;
	}

	return iResult;
}



///////////////////////////////////////////////////////////////



/**
 *md5 context data structure
 */
typedef struct
{
	//bits to mod 2^64
	UINT i[2];
	//temp buffer
	UINT buf[4];
	//input buffer
	unsigned char in[64];
	//the final degist numbers
	unsigned char digest[16];
}
et_md5_ctx_t;

static void et_md5_init(et_md5_ctx_t *mdContext);
static void et_md5_update(et_md5_ctx_t *mdContext, unsigned char *inBuf, unsigned int inLen);
static void et_md5_final(et_md5_ctx_t *mdContext);
static void et_md5_buffer_transform(UINT *buf, UINT *in);

char * et_md5_encode32(char *dst,const char *src);

//static void et_md5_buffer_transform();

/* Round 1 */
#define ET_MD5_ROUND_S11 7
#define ET_MD5_ROUND_S12 12
#define ET_MD5_ROUND_S13 17
#define ET_MD5_ROUND_S14 22
	/* Round 2 */
#define ET_MD5_ROUND_S21 5
#define ET_MD5_ROUND_S22 9
#define ET_MD5_ROUND_S23 14
#define ET_MD5_ROUND_S24 20
	/* Round 3 */
#define ET_MD5_ROUND_S31 4
#define ET_MD5_ROUND_S32 11
#define ET_MD5_ROUND_S33 16
#define ET_MD5_ROUND_S34 23
	/* Round 4 */
#define ET_MD5_ROUND_S41 6
#define ET_MD5_ROUND_S42 10
#define ET_MD5_ROUND_S43 15
#define ET_MD5_ROUND_S44 21

static unsigned char et_s_md5_padding[64] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/**
 *selection,majority,parity
 */
#define et_md5_single_f(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define et_md5_single_g(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define et_md5_single_h(x, y, z) ((x) ^ (y) ^ (z))
#define et_md5_single_i(x, y, z) ((y) ^ ((x) | (~z)))

/**
 *round 1,2,3,4
 */
#define et_md5_two_ff(a, b, c, d, x, s, ac) \
{	(a) += et_md5_single_f((b), (c), (d)) + (x) + (UINT)(ac); \
	(a) = et_rotate_left32((a), (s)); \
	(a) += (b); \
}

#define et_md5_two_gg(a, b, c, d, x, s, ac) \
{	(a) += et_md5_single_g((b), (c), (d)) + (x) + (UINT)(ac); \
	(a) = et_rotate_left32((a), (s)); \
	(a) += (b); \
}

#define et_md5_two_hh(a, b, c, d, x, s, ac) \
{	(a) += et_md5_single_h((b), (c), (d)) + (x) + (UINT)(ac); \
	(a) = et_rotate_left32((a), (s)); \
	(a) += (b); \
}

#define et_md5_two_ii(a, b, c, d, x, s, ac) \
{	(a) += et_md5_single_i((b), (c), (d)) + (x) + (UINT)(ac); \
	(a) = et_rotate_left32((a), (s)); \
	(a) += (b); \
}

/**
 *init ,and load magic initial constants
 */
static void et_md5_init(et_md5_ctx_t *mdContext){
  mdContext->i[0] = mdContext->i[1] = (UINT)0;

  mdContext->buf[0] = (UINT)0x67452301;
  mdContext->buf[1] = (UINT)0xefcdab89;
  mdContext->buf[2] = (UINT)0x98badcfe;
  mdContext->buf[3] = (UINT)0x10325476;
}

static void et_md5_update(et_md5_ctx_t *mdContext, unsigned char *inBuf, unsigned int inLen){
	UINT in[16];
	int mdi;
	unsigned int i, ii;

	/* compute number of bytes mod 64 */
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	/* update number of bits */
	if ((mdContext->i[0] + ((UINT)inLen << 3)) < mdContext->i[0]){
		mdContext->i[1]++;
	}

	mdContext->i[0] += ((UINT)inLen << 3);
	mdContext->i[1] += ((UINT)inLen >> 29);

	while (inLen--){
		/* add new character to buffer, increment mdi */
		mdContext->in[mdi++] = *inBuf++;

		/* transform if necessary */
		if (mdi == 0x40){
			for (i = 0, ii = 0; i < 16; i++, ii += 4){
				in[i] = (((UINT)mdContext->in[ii+3]) << 24)
					| (((UINT)mdContext->in[ii+2]) << 16)
					| (((UINT)mdContext->in[ii+1]) << 8)
					| ((UINT)mdContext->in[ii]);
			}
			et_md5_buffer_transform(mdContext->buf, in);
			mdi = 0;
		}
	}
}

static void et_md5_final(et_md5_ctx_t *mdContext){
	UINT in[16];
	int mdi;
	unsigned int i, ii;
	unsigned int padLen;

	/* save number of bits */
	in[14] = mdContext->i[0];
	in[15] = mdContext->i[1];

	/* compute number of bytes mod 64 */
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	/* pad out to 56 mod 64 */
	padLen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
	et_md5_update(mdContext, et_s_md5_padding, padLen);

	/* append length in bits and transform */
	for (i = 0, ii = 0; i < 14; i++, ii += 4){
		in[i] = (((UINT)mdContext->in[ii+3]) << 24)
			| (((UINT)mdContext->in[ii+2]) << 16)
			| (((UINT)mdContext->in[ii+1]) << 8)
			| ((UINT)mdContext->in[ii]);
	}

	et_md5_buffer_transform(mdContext->buf,in);

	/* store buffer in digest */
	for (i = 0, ii = 0; i < 4; i++, ii += 4){
		mdContext->digest[ii] = (unsigned char)(mdContext->buf[i] & 0xFF);
		mdContext->digest[ii+1] = (unsigned char)((mdContext->buf[i] >> 8) & 0xFF);
		mdContext->digest[ii+2] = (unsigned char)((mdContext->buf[i] >> 16) & 0xFF);
		mdContext->digest[ii+3] = (unsigned char)((mdContext->buf[i] >> 24) & 0xFF);
	}
}

/**
 *Basic MD5 step. transform buf based on in.
 */
static void et_md5_buffer_transform(UINT *buf, UINT *in){
	UINT a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	et_md5_two_ff( a, b, c, d, in[ 0], ET_MD5_ROUND_S11, 3614090360UL); /* 1 */
	et_md5_two_ff( d, a, b, c, in[ 1], ET_MD5_ROUND_S12, 3905402710UL); /* 2 */
	et_md5_two_ff( c, d, a, b, in[ 2], ET_MD5_ROUND_S13,  606105819UL); /* 3 */
	et_md5_two_ff( b, c, d, a, in[ 3], ET_MD5_ROUND_S14, 3250441966UL); /* 4 */
	et_md5_two_ff( a, b, c, d, in[ 4], ET_MD5_ROUND_S11, 4118548399UL); /* 5 */
	et_md5_two_ff( d, a, b, c, in[ 5], ET_MD5_ROUND_S12, 1200080426UL); /* 6 */
	et_md5_two_ff( c, d, a, b, in[ 6], ET_MD5_ROUND_S13, 2821735955UL); /* 7 */
	et_md5_two_ff( b, c, d, a, in[ 7], ET_MD5_ROUND_S14, 4249261313UL); /* 8 */
	et_md5_two_ff( a, b, c, d, in[ 8], ET_MD5_ROUND_S11, 1770035416UL); /* 9 */
	et_md5_two_ff( d, a, b, c, in[ 9], ET_MD5_ROUND_S12, 2336552879UL); /* 10 */
	et_md5_two_ff( c, d, a, b, in[10], ET_MD5_ROUND_S13, 4294925233UL); /* 11 */
	et_md5_two_ff( b, c, d, a, in[11], ET_MD5_ROUND_S14, 2304563134UL); /* 12 */
	et_md5_two_ff( a, b, c, d, in[12], ET_MD5_ROUND_S11, 1804603682UL); /* 13 */
	et_md5_two_ff( d, a, b, c, in[13], ET_MD5_ROUND_S12, 4254626195UL); /* 14 */
	et_md5_two_ff( c, d, a, b, in[14], ET_MD5_ROUND_S13, 2792965006UL); /* 15 */
	et_md5_two_ff( b, c, d, a, in[15], ET_MD5_ROUND_S14, 1236535329UL); /* 16 */
	
	et_md5_two_gg( a, b, c, d, in[ 1], ET_MD5_ROUND_S21, 4129170786UL); /* 17 */
	et_md5_two_gg( d, a, b, c, in[ 6], ET_MD5_ROUND_S22, 3225465664UL); /* 18 */
	et_md5_two_gg( c, d, a, b, in[11], ET_MD5_ROUND_S23,  643717713UL); /* 19 */
	et_md5_two_gg( b, c, d, a, in[ 0], ET_MD5_ROUND_S24, 3921069994UL); /* 20 */
	et_md5_two_gg( a, b, c, d, in[ 5], ET_MD5_ROUND_S21, 3593408605UL); /* 21 */
	et_md5_two_gg( d, a, b, c, in[10], ET_MD5_ROUND_S22,   38016083UL); /* 22 */
	et_md5_two_gg( c, d, a, b, in[15], ET_MD5_ROUND_S23, 3634488961UL); /* 23 */
	et_md5_two_gg( b, c, d, a, in[ 4], ET_MD5_ROUND_S24, 3889429448UL); /* 24 */
	et_md5_two_gg( a, b, c, d, in[ 9], ET_MD5_ROUND_S21,  568446438UL); /* 25 */
	et_md5_two_gg( d, a, b, c, in[14], ET_MD5_ROUND_S22, 3275163606UL); /* 26 */
	et_md5_two_gg( c, d, a, b, in[ 3], ET_MD5_ROUND_S23, 4107603335UL); /* 27 */
	et_md5_two_gg( b, c, d, a, in[ 8], ET_MD5_ROUND_S24, 1163531501UL); /* 28 */
	et_md5_two_gg( a, b, c, d, in[13], ET_MD5_ROUND_S21, 2850285829UL); /* 29 */
	et_md5_two_gg( d, a, b, c, in[ 2], ET_MD5_ROUND_S22, 4243563512UL); /* 30 */
	et_md5_two_gg( c, d, a, b, in[ 7], ET_MD5_ROUND_S23, 1735328473UL); /* 31 */
	et_md5_two_gg( b, c, d, a, in[12], ET_MD5_ROUND_S24, 2368359562UL); /* 32 */

	et_md5_two_hh( a, b, c, d, in[ 5], ET_MD5_ROUND_S31, 4294588738UL); /* 33 */
	et_md5_two_hh( d, a, b, c, in[ 8], ET_MD5_ROUND_S32, 2272392833UL); /* 34 */
	et_md5_two_hh( c, d, a, b, in[11], ET_MD5_ROUND_S33, 1839030562UL); /* 35 */
	et_md5_two_hh( b, c, d, a, in[14], ET_MD5_ROUND_S34, 4259657740UL); /* 36 */
	et_md5_two_hh( a, b, c, d, in[ 1], ET_MD5_ROUND_S31, 2763975236UL); /* 37 */
	et_md5_two_hh( d, a, b, c, in[ 4], ET_MD5_ROUND_S32, 1272893353UL); /* 38 */
	et_md5_two_hh( c, d, a, b, in[ 7], ET_MD5_ROUND_S33, 4139469664UL); /* 39 */
	et_md5_two_hh( b, c, d, a, in[10], ET_MD5_ROUND_S34, 3200236656UL); /* 40 */
	et_md5_two_hh( a, b, c, d, in[13], ET_MD5_ROUND_S31,  681279174UL); /* 41 */
	et_md5_two_hh( d, a, b, c, in[ 0], ET_MD5_ROUND_S32, 3936430074UL); /* 42 */
	et_md5_two_hh( c, d, a, b, in[ 3], ET_MD5_ROUND_S33, 3572445317UL); /* 43 */
	et_md5_two_hh( b, c, d, a, in[ 6], ET_MD5_ROUND_S34,   76029189UL); /* 44 */
	et_md5_two_hh( a, b, c, d, in[ 9], ET_MD5_ROUND_S31, 3654602809UL); /* 45 */
	et_md5_two_hh( d, a, b, c, in[12], ET_MD5_ROUND_S32, 3873151461UL); /* 46 */
	et_md5_two_hh( c, d, a, b, in[15], ET_MD5_ROUND_S33,  530742520UL); /* 47 */
	et_md5_two_hh( b, c, d, a, in[ 2], ET_MD5_ROUND_S34, 3299628645UL); /* 48 */
	
	et_md5_two_ii( a, b, c, d, in[ 0], ET_MD5_ROUND_S41, 4096336452UL); /* 49 */
	et_md5_two_ii( d, a, b, c, in[ 7], ET_MD5_ROUND_S42, 1126891415UL); /* 50 */
	et_md5_two_ii( c, d, a, b, in[14], ET_MD5_ROUND_S43, 2878612391UL); /* 51 */
	et_md5_two_ii( b, c, d, a, in[ 5], ET_MD5_ROUND_S44, 4237533241UL); /* 52 */
	et_md5_two_ii( a, b, c, d, in[12], ET_MD5_ROUND_S41, 1700485571UL); /* 53 */
	et_md5_two_ii( d, a, b, c, in[ 3], ET_MD5_ROUND_S42, 2399980690UL); /* 54 */
	et_md5_two_ii( c, d, a, b, in[10], ET_MD5_ROUND_S43, 4293915773UL); /* 55 */
	et_md5_two_ii( b, c, d, a, in[ 1], ET_MD5_ROUND_S44, 2240044497UL); /* 56 */
	et_md5_two_ii( a, b, c, d, in[ 8], ET_MD5_ROUND_S41, 1873313359UL); /* 57 */
	et_md5_two_ii( d, a, b, c, in[15], ET_MD5_ROUND_S42, 4264355552UL); /* 58 */
	et_md5_two_ii( c, d, a, b, in[ 6], ET_MD5_ROUND_S43, 2734768916UL); /* 59 */
	et_md5_two_ii( b, c, d, a, in[13], ET_MD5_ROUND_S44, 1309151649UL); /* 60 */
	et_md5_two_ii( a, b, c, d, in[ 4], ET_MD5_ROUND_S41, 4149444226UL); /* 61 */
	et_md5_two_ii( d, a, b, c, in[11], ET_MD5_ROUND_S42, 3174756917UL); /* 62 */
	et_md5_two_ii( c, d, a, b, in[ 2], ET_MD5_ROUND_S43,  718787259UL); /* 63 */
	et_md5_two_ii( b, c, d, a, in[ 9], ET_MD5_ROUND_S44, 3951481745UL); /* 64 */

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

char * et_md5_encode32(char *dst,const char *src){
	int i = 0;
	char tmp_hex[10] = "";
	et_md5_ctx_t mdContext;
	unsigned int len = et_strlen(src);
	unsigned char *tmp_src = (unsigned char *)et_alloc((et_strlen(src) + 1) * sizeof(src));

	et_memcpy(tmp_src,src,et_strlen(src) + 1);
	et_md5_init(&mdContext);
	et_md5_update(&mdContext,tmp_src,len);
	et_md5_final(&mdContext);

	dst[0] = 0;
	for (i = 0; i < 16; i++){
		et_memset(tmp_hex,0,10);
		et_sz_by_int(tmp_hex,(&mdContext)->digest[i]);
		et_sz_dec2hex(tmp_hex);
		et_sz_pad_left(tmp_hex,'0',2);
		et_strcat(dst,tmp_hex);
	}
	dst[32] = '\0';
	et_sz_lower(dst);
	et_free(tmp_src);
	return dst;
}


///////////////////////////////////////////////////////////////////////////////////////////////

#include <time.h>

INT64 et_rand_between(INT64 iBegin, INT64 iEnd)
{
	static bool bSeedInited = false;

	INT64 iResult = 0;
	INT64 iDiff = 0;
	INT64 iTimeSeed = 0;
	INT64 iSec = 0;
	INT64 iSeed = 0;

	if(!bSeedInited)
	{
		iTimeSeed = time((long int*)null);
		iSec = iTimeSeed % 10;
		iSeed = (iTimeSeed / 10) * iSec;
		srand(iSeed);
		rand();
		bSeedInited = true;
	}

	iDiff = iEnd - iBegin;
 	iResult = rand() % iDiff + 1;
	return iResult;
}

///////////////////////////////////////////////////////////////////////

#include <time.h>
#include <sys/time.h>

int et_time_create(EtTime** ppTime){
	if(*ppTime != null){
		return -1;
	}

	*ppTime = (EtTime*)et_alloc(sizeof(EtTime));
	et_memset(*ppTime,0,sizeof(EtTime));
	(*ppTime)->cb = sizeof(EtTime);
	(*ppTime)->year = 1970;
	(*ppTime)->month = 1;
	(*ppTime)->date = 1;
	(*ppTime)->day = 4;

	return 0;
}

int et_time_destroy(EtTime** ppTime){
	if(*ppTime == null){
		return -1;
	}
	if((*ppTime)->cb != sizeof(EtTime)){
		return -1;
	}

	et_memset(*ppTime,0,sizeof(EtTime));
	et_free(*ppTime);
	*ppTime = null;
	return 0;
}

int et_time_refresh(EtTime* pTime){
	int iRet = 0;

	if(pTime == null){
		return -1;
	}

	if( et_time_get_date(pTime)
		> et_time_count_days_by_month(et_time_get_year(pTime),et_time_get_month(pTime))
	){
		return -1;
	}

	iRet = et_time_make_time(pTime);
	if(iRet < 0){
		return iRet;
	}

	iRet = et_time_make_day(pTime);
	if(iRet < 0){
		return iRet;
	}

	return 0;
}

int et_time_now(EtTime* pTime){
	return et_time_now_by_platform(pTime);
}

int et_time_get_year(const EtTime* pTime){
	if(pTime == null){
		return -1;
	}
	return pTime->year;
}

int et_time_get_month(const EtTime* pTime){
	if(pTime == null){
		return -1;
	}
	return pTime->month;
}

int et_time_get_date(const EtTime* pTime){
	if(pTime == null){
		return -1;
	}
	return pTime->date;
}

int et_time_get_hour(const EtTime* pTime){
	if(pTime == null){
		return -1;
	}
	return pTime->hour;
}

int et_time_get_minute(const EtTime* pTime){
	if(pTime == null){
		return -1;
	}
	return pTime->minute;
}

int et_time_get_second(const EtTime* pTime){
	if(pTime == null){
		return -1;
	}
	return pTime->second;
}

int et_time_get_day(const EtTime* pTime){
	if(pTime == null){
		return -1;
	}
	return pTime->day;
}

INT64 et_time_get_stamp10(const EtTime* pTime){
	if(pTime == null){
		return -1;
	}
	return pTime->stamp10;
}

INT64 et_time_get_stamp13(const EtTime* pTime){
	INT64 iStamp13 = 0;

	if(pTime == null){
		return -1;
	}

	if(pTime->stamp10 > 0){
		iStamp13 = pTime->stamp10;
		iStamp13 *= 1000;
		iStamp13 += pTime->millisec;
	}else{
		iStamp13 = pTime->stamp10;
		iStamp13 *= 1000;
		iStamp13 = iStamp13 - pTime->millisec;
	}

	return iStamp13;
}

int et_time_get_millisec(const EtTime* pTime){
	if(pTime == null){
		return -1;
	}
	return pTime->millisec;
}

int et_time_get_zone_offset(const EtTime* pTime){
	if(pTime == null){
		return -1;
	}
	return pTime->zone_offset;
}

int et_time_set_year(EtTime* pTime, int iYear){
	if(pTime == null){
		return -1;
	}
	pTime->year = iYear;
	return 0;
}

int et_time_set_month(EtTime* pTime, int iMonth){
	if(pTime == null){
		return -1;
	}
	if(iMonth > 12 || iMonth < 1){
		return -1;
	}
	pTime->month = iMonth;
	return 0;
}

int et_time_set_date(EtTime* pTime, int iDate){
	if(pTime == null){
		return -1;
	}
	if(iDate > 31 || iDate < 1){
		return -1;
	}
	pTime->date = iDate;
	return 0;
}

int et_time_set_hour(EtTime* pTime, int iHour){
	if(pTime == null){
		return -1;
	}
	if( iHour > 23 || iHour < 0){
		return -1;
	}
	pTime->hour = iHour;
	return 0;
}

int et_time_set_minute(EtTime* pTime, int iMinute){
	if(pTime == null){
		return -1;
	}
	if( iMinute > 60 || iMinute < 0){
		return -1;
	}
	pTime->minute = iMinute;
	return 0;
}

int et_time_set_second(EtTime* pTime, int iSecond){
	if(pTime == null){
		return -1;
	}
	if( iSecond > 60 || iSecond < 0){
		return -1;
	}
	pTime->second = iSecond;
	return 0;
}

int et_time_set_day(EtTime* pTime, int iDay){
	if(pTime == null){
		return -1;
	}
	if( iDay > 6 || iDay < 0){
		return -1;
	}
	pTime->day = iDay;
	return 0;
}

int et_time_set_stamp10(EtTime* pTime, INT64 iStamp){
	if(pTime == null){
		return -1;
	}
	pTime->stamp10 = iStamp;
	return 0;
}

int et_time_set_zone_offset(EtTime* pTime, int iZoneOffset){
	if(pTime == null){
		return -1;
	}
	pTime->zone_offset = iZoneOffset;
	return 0;
}

int et_time_set_stamp13(EtTime* pTime, INT64 iStamp13){
	if(pTime == null){
		return -1;
	}
	pTime->stamp10 = et_time_stamp13_to_10(iStamp13);
	pTime->millisec = iStamp13 % 1000;
	if(iStamp13 < 0){
		pTime->millisec = 1000 - pTime->millisec - 1;
	}
	return 0;
}

int et_time_set_millisec(EtTime* pTime, int iMillisec){
	if(pTime == null){
		return -1;
	}
	if( iMillisec > 999 || iMillisec < 0){
		return -1;
	}
	pTime->millisec = iMillisec;
	return 0;
}

int et_time_add_day(EtTime* pTime, int iDay){
	INT64 iStamp13 = 0;

	if(pTime == null){
		return -1;
	}

	iStamp13 = et_time_get_stamp13(pTime);
	iStamp13 += iDay * 24 * 60 * 60 * 1000;
	return et_time_stamp13_to_object(pTime,iStamp13);
}

int et_time_add_hour(EtTime* pTime, int iHour){
	INT64 iStamp13 = 0;

	if(pTime == null){
		return -1;
	}

	iStamp13 = et_time_get_stamp13(pTime);
	iStamp13 += iHour * 60 * 60 * 1000;

	return et_time_stamp13_to_object(pTime,iStamp13);
}

int et_time_add_minute(EtTime* pTime, int iMinute){
	INT64 iStamp13 = 0;

	if(pTime == null){
		return -1;
	}

	iStamp13 = et_time_get_stamp13(pTime);
	iStamp13 += iMinute * 60 * 1000;

	return et_time_stamp13_to_object(pTime,iStamp13);
}

int et_time_add_second(EtTime* pTime, int iSecond){
	INT64 iStamp13 = 0;

	if(pTime == null){
		return -1;
	}

	iStamp13 = et_time_get_stamp13(pTime);
	iStamp13 += iSecond * 1000;

	return et_time_stamp13_to_object(pTime,iStamp13);
}

int et_time_add_millisec(EtTime* pTime, INT64 iMillisec){
	INT64 iStamp13 = 0;

	if(pTime == null){
		return -1;
	}


	iStamp13 = et_time_get_stamp13(pTime);
	iStamp13 += iMillisec;

	return et_time_stamp13_to_object(pTime,iStamp13);
}


int et_time_to_sz(const EtTime* pTime, char* szDest){
	if(pTime == null){
		return -1;
	}
	if(szDest == null){
		return -1;
	}
	return et_time_format(pTime,szDest,"y-m-d h:i:s l w");
}

int et_time_make_time(EtTime* pTime){
	INT64 iStamp13 = 0;
	INT64 iStamp10 = 0;
	int iMillisec = 0;

	if(pTime == null){
		return -1;
	}

	iStamp13 = et_time_object_to_stamp13(pTime);
	iStamp10 = et_time_stamp13_to_10(iStamp13);
	iMillisec = (int)(iStamp13 % 1000);

	et_time_set_stamp10(pTime,iStamp10);
	et_time_set_millisec(pTime,iMillisec);

	return 0;
}

int et_time_make_day(EtTime* pTime){
	INT64 iStampRef = 0;
	INT64 iStampDiff = 0;
	int iDays = 0;
	int iDayBase = 0;
	int iRet = 0;
	int iDayResult = 0;

	if(pTime == null){
		return -1;
	}

	if(et_time_get_stamp10(pTime) == 0){
		iRet = et_time_make_time(pTime);
		if(iRet < 0){
			return iRet;
		}
	}

	//1970-1-1 0:0:0 thursday 4
	iStampRef = 0;
	iDayBase = 4;
	iStampDiff = et_time_get_stamp10(pTime) - iStampRef;
	if(iStampDiff >= 0){
		iDays =  (int) ( iStampDiff / (24 * 60 * 60) );
		iDays = (iDays) % 7;
		iDayResult = (iDays + iDayBase) % 7;
	}else{
		iDays =  (int) (iStampDiff / (24 * 60 * 60)) ;
		if(iStampDiff % (24 * 60 * 60) != 0){
			iDays -= 1;
		}
		iDays = et_abs(iDays);
		iDays = (iDays) % 7;
		iDayResult = (7 - iDays + iDayBase) % 7;
	}

	et_time_set_day(pTime,iDayResult);

	return 0;
}

bool et_time_is_unix_start(const EtTime* pTime){
	if(pTime == null){
		return false;
	}

	if(et_time_get_year(pTime) != 1970){
		return false;
	}
	if(et_time_get_month(pTime) != 1){
		return false;
	}
	if(et_time_get_date(pTime) != 1){
		return false;
	}
	if(et_time_get_hour(pTime) != 0){
		return false;
	}
	if(et_time_get_minute(pTime) != 0){
		return false;
	}
	if(et_time_get_second(pTime) != 0){
		return false;
	}
	return true;
}

int et_time_stamp10_to_object(EtTime* pTime, INT64 iStamp10){
	INT64 iStamp13 = 0;

	iStamp13 = iStamp10;
	iStamp13 *= 1000;

	return et_time_stamp13_to_object(pTime,iStamp13);
}

INT64 et_time_object_to_stamp10(const EtTime* pTime){
	INT64 iStamp13 = et_time_object_to_stamp13(pTime);
	INT64 iStamp10 = et_time_stamp13_to_10(iStamp13);
	return iStamp10;
}

int et_time_stamp13_to_object(EtTime* pTime, INT64 iStamp13){
	INT64 iStamp10 = 0;
	int iSecOfDay = 24 * 60 * 60;
	int iDayOfYear = 0;
	int iDayOfMonth = 0;
	int iYear = 1970;
	int iMonth = 1;
	int iDate = 1;
	int iHour = 0;
	int iMinute = 0;
	int iSecond = 0;
	INT64 iSecLeft = 0;
	int iMillisec = 0;

	if(pTime == null){
		return -1;
	}

	iMillisec = iStamp13 % 1000;
	if(iMillisec < 0){
		iMillisec = 1000 + iMillisec;
	}

	iStamp10 = et_time_stamp13_to_10(iStamp13);

	iSecLeft = iStamp10;

	if(iSecLeft > 0){
		iYear = 1970;
		iMonth = 1;
		iDate = 1;
		iHour = 0;
		iMinute = 0;
		iSecond = 0;
		while(true){
			if(iSecLeft <= 0){
				break;
			}
			iDayOfYear = et_time_count_days_by_year(iYear);
			if(iDayOfYear * iSecOfDay > iSecLeft){
				break;
			}else{
				iSecLeft -= iDayOfYear * iSecOfDay;
				iYear++;
			}
		}

		while(true){
			if(iSecLeft <= 0){
				break;
			}
			iDayOfMonth = et_time_count_days_by_month(iYear,iMonth);
			if(iDayOfMonth * iSecOfDay > iSecLeft){
				break;
			}else{
				iSecLeft -= iDayOfMonth * iSecOfDay;
				iMonth++;
			}
		}

		while(true){
			if(iSecLeft <= 0){
				break;
			}
			if(iSecOfDay > iSecLeft){
				break;
			}else{
				iSecLeft -= iSecOfDay;
				iDate++;
			}
		}

		while(true){
			if(iSecLeft <= 0){
				break;
			}
			if(3600 > iSecLeft){
				break;
			}else{
				iSecLeft -= 3600;
				iHour++;
			}
		}

		while(true){
			if(iSecLeft <= 0){
				break;
			}
			if(60 > iSecLeft){
				break;
			}else{
				iSecLeft -= 60;
				iMinute++;
			}
		}

		iSecond = iSecLeft;
	}else if(iSecLeft < 0){
		iYear = 1969;
		while(true){
			if(iSecLeft >= 0){
				break;
			}
			iDayOfYear = et_time_count_days_by_year(iYear);
			if((iDayOfYear * iSecOfDay) >= et_abs(iSecLeft)){
				break;
			}else{
				iSecLeft += iDayOfYear * iSecOfDay;
				iYear--;
			}
		}

		iMonth = 12;
		while(true){
			if(iSecLeft >= 0){
				break;
			}
			iDayOfMonth = et_time_count_days_by_month(iYear,iMonth);
			if(iDayOfMonth * iSecOfDay >= et_abs(iSecLeft)){
				break;
			}else{
				iSecLeft += iDayOfMonth * iSecOfDay;
				iMonth--;
			}
		}

		iDate = et_time_count_days_by_month(iYear,iMonth);
		while(true){
			if(iSecLeft >= 0){
				break;
			}
			if(iSecOfDay >= et_abs(iSecLeft)){
				break;
			}else{
				iSecLeft += iSecOfDay;
				iDate--;
			}
		}

		iHour = 23;
		while(true){
			if(iSecLeft >= 0){
				break;
			}
			if( 3600 >= et_abs(iSecLeft) ){
				break;
			}else{
				iSecLeft += 3600;
				iHour--;
			}
		}

		iMinute = 60 - 1;
		while(true){
			if(iSecLeft >= 0){
				break;
			}
			if( 60 >= et_abs(iSecLeft) ){
				break;
			}else{
				iSecLeft += 60;
				iMinute--;
			}
		}

		iSecond = 60 + iSecLeft;
	}

	et_time_set_year(pTime,iYear);
	et_time_set_month(pTime,iMonth);
	et_time_set_date(pTime,iDate);
	et_time_set_hour(pTime,iHour);
	et_time_set_minute(pTime,iMinute);
	et_time_set_second(pTime,iSecond);
	et_time_set_millisec(pTime,iMillisec);
	et_time_set_stamp10(pTime,iStamp10);
	et_time_make_day(pTime);

	return 0;
}

INT64 et_time_object_to_stamp13(const EtTime* pTime){
	INT64 iStamp13 = 0;
	int iYearLoop = 0;
	int iMonthLoop = 0;

	if(pTime == null){
		return -1;
	}

	if( et_time_get_year(pTime) >= 1970 ){
		iYearLoop = 1970;
		iMonthLoop = 1;
		while(true){
			if(iYearLoop >= et_time_get_year(pTime)){
				break;
			}
			iStamp13 += et_time_count_days_by_year(iYearLoop) * 24 * 60 * 60;
			iYearLoop++;
		}
		while(true){
			if(iMonthLoop >= et_time_get_month(pTime)){
				break;
			}
			iStamp13 += et_time_count_days_by_month(iYearLoop,iMonthLoop) * 24 * 60 * 60;
			iMonthLoop++;
		}

		iStamp13 += (et_time_get_date(pTime) - 1) * 24 * 60 * 60;
		iStamp13 += et_time_get_hour(pTime) * 60 * 60;
		iStamp13 += et_time_get_minute(pTime) * 60;
		iStamp13 += et_time_get_second(pTime);

		iStamp13 *= 1000;

		iStamp13 += et_time_get_millisec(pTime);
	}else{
		iYearLoop = 1970 - 1;
		while(true){
			if(iYearLoop <= et_time_get_year(pTime)){
				break;
			}
			iStamp13 -= et_time_count_days_by_year(iYearLoop) * 24 * 60 * 60;
			iYearLoop--;
		}

		iMonthLoop = 12;
		while(true){
			if(iMonthLoop <= et_time_get_month(pTime)){
				break;
			}
			iStamp13 -= et_time_count_days_by_month(iYearLoop,iMonthLoop) * 24 * 60 * 60;
			iMonthLoop--;
		}

		iStamp13 -= (et_time_count_days_by_month(iYearLoop,iMonthLoop) - et_time_get_date(pTime)) * 24 * 60 * 60;
		iStamp13 -= (24 - et_time_get_hour(pTime) - 1) * 60 * 60;
		iStamp13 -= (60 - et_time_get_minute(pTime) - 1) * 60;
		iStamp13 -= (60 - et_time_get_second(pTime) - 1);
		iStamp13 *= 1000;
		iStamp13 -= (1000 - et_time_get_millisec(pTime) - 1);
	}


	return iStamp13;
}

int et_time_count_days_by_month(int iYear, int iMonth){
	int iDays = 0;
	bool bIsLeap = et_time_is_leap_year(iYear);

	switch(iMonth)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		iDays = 31;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		iDays = 30;
		break;
	case 2:
		iDays = bIsLeap ? 29 : 28;
		break;
	default:
		break;
	}
	return iDays;
}

int et_time_count_days_by_year(int iYear){
	return et_time_is_leap_year(iYear) ? 366 : 365;
}

int et_time_format(const EtTime* pTime, char* szDest, const char* szFormat){
	char szYear[10] = {0};
	char szMonth[10] = {0};
	char szDate[10] = {0};
	char szHour[10] = {0};
	char szMinute[10] = {0};
	char szSecond[10] = {0};
	char szMilliSec[10] = {0};
	char szDayOfWeek[10] = {0};

	if(pTime == null){
		return -1;
	}

	if(szDest == null){
		return null;
	}

	if(szFormat == null){
		return null;
	}

	et_sz_by_int(szYear,et_time_get_year(pTime));
	et_sz_by_int(szMonth,et_time_get_month(pTime));
	et_sz_by_int(szDate,et_time_get_date(pTime));
	et_sz_by_int(szHour,et_time_get_hour(pTime));
	et_sz_by_int(szMinute,et_time_get_minute(pTime));
	et_sz_by_int(szSecond,et_time_get_second(pTime));
	et_sz_by_int(szMilliSec,et_time_get_millisec(pTime));
	et_sz_by_int(szDayOfWeek,et_time_get_day(pTime));

	et_sz_pad_left(szYear,'0',2);
	et_sz_pad_left(szMonth,'0',2);
	et_sz_pad_left(szDate,'0',2);
	et_sz_pad_left(szHour,'0',2);
	et_sz_pad_left(szMinute,'0',2);
	et_sz_pad_left(szSecond,'0',2);
	et_sz_pad_left(szMilliSec,'0',3);

	et_strcpy(szDest,szFormat);

	et_sz_lower(szDest);

	et_sz_replace(szDest,"y",szYear);
	et_sz_replace(szDest,"m",szMonth);
	et_sz_replace(szDest,"d",szDate);
	et_sz_replace(szDest,"h",szHour);
	et_sz_replace(szDest,"i",szMinute);
	et_sz_replace(szDest,"s",szSecond);
	et_sz_replace(szDest,"w",szDayOfWeek);
	et_sz_replace(szDest,"l",szMilliSec);

	return 0;
}

char* et_time_current_format(char* szDest, const char* szFormat){
	int iRet = 0;
	EtTime* pTime = null;

	if(szFormat == null || szDest == null){
		return null;
	}

	iRet = et_time_create(&pTime);
	et_time_now(pTime);
	et_time_format(pTime,szDest,szFormat);
	et_time_destroy(&pTime);

	return szDest;
}

int et_time_current_year(){
	int iYear = 0;
	EtTime* pTime = null;

	et_time_create(&pTime);
	et_time_now(pTime);
	iYear = et_time_get_year(pTime);
	et_time_destroy(&pTime);

	return iYear;
}

int et_time_current_month(){
	int iMonth = 0;
	EtTime* pTime = null;

	et_time_create(&pTime);
	et_time_now(pTime);
	iMonth = et_time_get_month(pTime);
	et_time_destroy(&pTime);

	return iMonth;
}

int et_time_current_date(){
	int iDate = 0;
	EtTime* pTime = null;

	et_time_create(&pTime);
	et_time_now(pTime);
	iDate = et_time_get_date(pTime);
	et_time_destroy(&pTime);

	return iDate;
}

int et_time_current_hour(){
	int iHour = 0;
	EtTime* pTime = null;

	et_time_create(&pTime);
	et_time_now(pTime);
	iHour = et_time_get_hour(pTime);
	et_time_destroy(&pTime);

	return iHour;
}

int et_time_current_minute(){
	int iMinute = 0;
	EtTime* pTime = null;

	et_time_create(&pTime);
	et_time_now(pTime);
	iMinute = et_time_get_minute(pTime);
	et_time_destroy(&pTime);

	return iMinute;
}

int et_time_current_second(){
	int iSecond = 0;
	EtTime* pTime = null;

	et_time_create(&pTime);
	et_time_now(pTime);
	iSecond = et_time_get_second(pTime);
	et_time_destroy(&pTime);

	return iSecond;
}

int et_time_current_day(){
	int iDay = 0;
	EtTime* pTime = null;

	et_time_create(&pTime);
	et_time_now(pTime);
	iDay = et_time_get_day(pTime);
	et_time_destroy(&pTime);

	return iDay;
}

int et_time_current_millisec(){
	int iMillisec = 0;
	EtTime* pTime = null;

	et_time_create(&pTime);
	et_time_now(pTime);
	iMillisec = et_time_get_millisec(pTime);
	et_time_destroy(&pTime);

	return iMillisec;
}

INT64 et_time_current_stamp10(){
	INT64 iStamp10 = 0;
	EtTime* pTime = null;

	et_time_create(&pTime);
	et_time_now(pTime);
	iStamp10 = et_time_get_stamp10(pTime);
	et_time_destroy(&pTime);

	return iStamp10;
}

INT64 et_time_current_stamp13(){
	INT64 iStamp13 = 0;
	EtTime* pTime = null;

	et_time_create(&pTime);
	et_time_now(pTime);

	iStamp13 = et_time_get_stamp13(pTime);
	et_time_destroy(&pTime);

	return iStamp13;
}

char* et_time_current_sz(char* szDest){
	return et_time_current_format(szDest,ET_TIME_FORMAT_DEFAULT);
}

int et_time_zone_offset(){
	int iOffset = 0;
	EtTime* pTime = null;

	et_time_create(&pTime);
	et_time_now(pTime);

	iOffset = pTime->zone_offset;
	et_time_destroy(&pTime);

	return iOffset;
}


bool et_time_is_leap_year(UINT iYear){
	if(iYear % 400 == 0){
		return true;
	}else if(iYear % 100 != 0 && iYear % 4 == 0){
		return true;
	}
	return false;
}

int et_time_sec_to_len(EtTimeLen* pLen, int iTotalSeconds){
	int iSecLeft = iTotalSeconds;

	if(pLen == null){
		return -1;
	}

	pLen->days = iSecLeft / (24 * 60 * 60);
	iSecLeft = iSecLeft % (24 * 60 * 60);

	pLen->hours = iSecLeft / (60 * 60);
	iSecLeft = iSecLeft % (60 * 60);

	pLen->minutes = iSecLeft / 60;
	iSecLeft = iSecLeft % 60;

	pLen->seconds = iSecLeft;

	return 0;
}

int et_time_len_to_sec(const EtTimeLen* pLen){
	int iTotalSeconds = 0;

	if(pLen == null){
		return -1;
	}

	iTotalSeconds += pLen->days * 24 * 60 * 60;
	iTotalSeconds += pLen->hours * 60 * 60;
	iTotalSeconds += pLen->minutes * 60;
	iTotalSeconds += pLen->seconds;

	return iTotalSeconds;
}

int et_time_sz_to_object(EtTime* pTime, const char* szSrc){
	int iSrcLen = 0;

	if(szSrc == null){
		return -1;
	}

	if(pTime == null){
		return -1;
	}

	iSrcLen = et_strlen(szSrc);
	if(iSrcLen != 19){
		return -1;
	}

	if(!et_sz_is_datetime(szSrc)){
		return -1;
	}

	char szYear[5] = "";
	char szMonth[3] = "";
	char szDate[3] = "";
	char szHour[3] = "";
	char szMinute[3] = "";
	char szSecond[3] = "";

	et_sz_substr(szYear,szSrc,0,4);
	et_sz_substr(szMonth,szSrc,5,2);
	et_sz_substr(szDate,szSrc,8,2);
	et_sz_substr(szHour,szSrc,11,2);
	et_sz_substr(szMinute,szSrc,14,2);
	et_sz_substr(szSecond,szSrc,17,2);

	et_time_set_year(pTime,et_sz_to_int( szYear ));
	et_time_set_month(pTime,et_sz_to_int( szMonth ));
	et_time_set_date(pTime,et_sz_to_int( szDate ));
	et_time_set_hour(pTime,et_sz_to_int( szHour ));
	et_time_set_minute(pTime,et_sz_to_int( szMinute ));
	et_time_set_second(pTime,et_sz_to_int( szSecond ));
	et_time_set_millisec(pTime,0);
	et_time_refresh(pTime);

	return 0;
}

INT64 et_time_sz_to_stamp13(const char* szSrc){
	INT64 iStamp13 = 0;
	int iRet = 0;
	EtTime* pTime = null;

	iRet = et_time_create(&pTime);
	if(iRet < 0){
		et_time_destroy(&pTime);
		return iRet;
	}

	iRet = et_time_sz_to_object(pTime,szSrc);
	if(iRet < 0){
		et_time_destroy(&pTime);
		return iRet;
	}

	iStamp13 = et_time_object_to_stamp13(pTime);
	if(iRet < 0){
		et_time_destroy(&pTime);
		return iRet;
	}

	iRet = et_time_destroy(&pTime);

	return iStamp13;
}

int et_time_stamp13_to_sz(char* szResult, INT64 iStamp13){
	int iRet = 0;
	EtTime* pTime = null;

	iRet = et_time_create(&pTime);
	if(iRet < 0){
		return iRet;
	}
	iRet = et_time_stamp13_to_object(pTime,iStamp13);
	if(iRet < 0){
		return iRet;
	}
	iRet = et_time_to_sz(pTime,szResult);
	if(iRet < 0){
		return iRet;
	}
	iRet = et_time_destroy(&pTime);
	if(iRet < 0){
		return iRet;
	}

	return 0;
}

INT64 et_time_sz_to_stamp10(const char* szSrc){
	INT64 iStamp13 = et_time_sz_to_stamp13(szSrc);
	INT64 iStamp10 = et_time_stamp13_to_10(iStamp13);
	return iStamp10;
}

INT64 et_time_stamp13_to_10(INT64 iStamp13){
	INT64 iStamp10 = (INT64)(iStamp13 / 1000);
	int iMod = iStamp13 % 1000;
	if(iMod < 0){
		iStamp10 -= 1;
	}
	return iStamp10;
}

INT64 et_time_stamp10_to_13(INT64 iStamp10){
	INT64 iStamp13 = 0;
	iStamp13 = iStamp10;
	iStamp13 *= 1000;
	if(iStamp10 < 0){
		iStamp13 += 1;
	}
	return iStamp13;
}

int et_time_stamp10_to_sz(char* szResult, INT64 iStamp10){
	INT64 iStamp13 = et_time_stamp10_to_13(iStamp10);
	return et_time_stamp13_to_sz(szResult,iStamp13);
}

int et_time_now_by_platform(EtTime* pTime){
	int iMillisec = 0;

	struct tm* pTm = null;
	struct timeval stTimeVal = {0};
	INT64 iStamp10Gmt = 0;
	INT64 iStamp10Offset = 0;

	if(pTime == null){
		return -1;
	}

	gettimeofday(&stTimeVal,null);
	iMillisec = stTimeVal.tv_usec / 1000;

	iStamp10Gmt = time(null);
	pTm = localtime((time_t*)&iStamp10Gmt);
	et_time_set_millisec(pTime,iMillisec);

	et_time_set_year(pTime,pTm->tm_year + 1900);
	et_time_set_month(pTime,pTm->tm_mon + 1);
	et_time_set_date(pTime,pTm->tm_mday);
	et_time_set_hour(pTime,pTm->tm_hour);
	et_time_set_minute(pTime,pTm->tm_min);
	et_time_set_second(pTime,pTm->tm_sec);
	et_time_set_day(pTime,pTm->tm_wday);

	et_time_make_time(pTime);
	iStamp10Offset = et_time_get_stamp10(pTime);

	et_time_set_zone_offset(pTime,iStamp10Offset - iStamp10Gmt);

	return 0;
}

////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int et_proc_exec_new(const char* szPath, char* argv[], char* envp[]){
	int iNewPid = fork();
	if(iNewPid < 0){
		return iNewPid;
	}else if(iNewPid > 0){
		return iNewPid;
	}else{
		execve(szPath,argv,envp);
		exit(0);
	}
}

int et_proc_exec_wait(const char* szPath, char* argv[], char* envp[]){
	return execve(szPath,argv,envp);
}

int et_proc_exec_read(const char* szCommand, char *bufResult, int iResultLimit){
	int iResult = -1;
	FILE* pPipeFile = null;
	char bufTemp[256] = "";
	int iReadTotal = 0;
	int iReadTemp = 0;

	if(bufResult == null){
		return -1;
	}

	if(iResultLimit <= 0){
		return -1;
	}

	pPipeFile = popen(szCommand,"r");
	if(pPipeFile < 0){
		return -1;
	}
	while(true){
		iReadTemp = fread(bufTemp,1,256,pPipeFile);
		if(iReadTemp <= 0){
			break;
		}
		if(iReadTemp + iReadTotal > iResultLimit){
			pclose(pPipeFile);
			return -1;
		}
		et_memcpy(bufResult + iReadTotal,bufTemp,iReadTemp);
		iReadTotal += iReadTemp;
	}

	bufResult[iReadTotal] = 0;
	iResult = pclose(pPipeFile);
	iResult = iReadTotal;

	return iResult;
}

int et_proc_pid(){
	return getpid();
}

void et_proc_exit(int iCode){
	exit(iCode);
}

int et_proc_kill(int iProcId){
	int iKillResult = kill(iProcId,SIGKILL);
	if(iKillResult < 0){
		return -1;
	}
	return 0;
}

int et_proc_wait_child(int iChildProcId){
	int iRet = 0;
	iRet = wait(iChildProcId,null,0);
	if(iRet < 0){
		return -1;
	}
	return 0;
}

int et_proc_exec_pipe(const char* szPath, et_proc_exec_pipe_fn pFn, void* pParam){
	int iResult = -1;
	char bufReadTemp[255];
	char bufWriteTemp[255];
	int pipe_in[2];
	int pipe_out[2];
	int iReadTotal = 0;
	int iReadTemp = 0;
	int iBufSize = 255;
	pid_t pid_new = -1;
	int iTempWriteSize = 0;

	if(szPath == null){
		return -1;
	}
	if(pFn == null){
		return -1;
	}
	if(iBufSize <= 0){
		iBufSize = 255;
	}
	
	pipe(pipe_in);
	pipe(pipe_out);
	pid_new = fork();
	if(pid_new < 0){
		return -1;
	}else if(pid_new > 0){
		close(pipe_in[1]);
		close(pipe_out[0]);
		while(true){
			iReadTemp = read(pipe_in[0],bufReadTemp,iBufSize);
			if(iReadTemp <= 0){
				break;
			}
			bufReadTemp[iReadTemp] = 0;
			iResult = pFn(bufReadTemp,iReadTemp,bufWriteTemp,pParam);
			if(iResult < 0){
				break;
			}else if(iResult > 0){
				iTempWriteSize = iResult;
				iResult = write(pipe_out[1],bufWriteTemp,iTempWriteSize);
				if(iResult < 0){
					return -1;
				}
			}
		}
		close(pipe_in[0]);
		close(pipe_out[1]);
	}else{
		close(pipe_in[0]);
		close(pipe_out[1]);
		dup2(pipe_in[1],STDOUT_FILENO);
		dup2(pipe_out[0],STDIN_FILENO);
		close(pipe_in[1]);
		close(pipe_out[0]);
		execve(szPath,null,null);
		exit(0);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ET_ENABLE_THREAD

#include <pthread.h>

int et_thread_start(et_thread_start_fn pFnStart, void* pParam){
	int iRet = -1;
	int iThreadId = -1;

	iRet = pthread_create((pthread_t*)&iThreadId,null,pFnStart,pParam);
	if(iRet < 0){
		return -1;
	}

	return iThreadId;
}

int et_thread_join(int iThreadId){
	return pthread_join(iThreadId,null);
}

#endif

//////////////////////////////////////////////////////////////////////////



#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>



int et_file_make_single_dir(const char* szPath){
	int iResult = -1;


	if(szPath == null){
		return ET_ERR_MEM_NULL;
	}

	if(et_file_exist(szPath)){
		if(et_file_is_dir(szPath)){
			return 0;
		}else{
			return ET_ERR_FILE_EXIST;
		}
	}

	iResult = mkdir(szPath,0755);
	if(iResult < 0){
		return ET_ERR_FILE_CREATE;
	}

	return 0;
}

bool et_file_exist(const char* szPath){
	int iResult = 0;

	if(szPath == null){
		return false;
	}

	iResult = access(szPath,F_OK);
	if(iResult == 0){
		return true;
	}

	return false;
}

bool et_file_is_file(const char* szPath){
	int iResult = 0;
	struct stat stStat;

	if(szPath == null){
		return false;
	}

	et_memset(&stStat,0,sizeof(stStat));
	iResult = lstat(szPath,&stStat);
	if(iResult < 0){
		return false;
	}

	if(!S_ISDIR(stStat.st_mode)){
		return true;
	}

	return false;
}

bool et_file_is_dir(const char* szPath){
	int iResult = 0;
	struct stat stStat;

	if(szPath == null){
		return false;
	}

	et_memset(&stStat,0,sizeof(stStat));
	iResult = lstat(szPath,&stStat);
	if(iResult < 0){
		return false;
	}

	if(S_ISDIR(stStat.st_mode)){
		return true;
	}

	return false;
}

int et_file_make_long_dir(const char* szPath){
	int iResult = -1;
	char szDirTemp[2048] = "";
	int iSlashPos = -1;

	if(szPath == null){
		return ET_ERR_MEM_NULL;
	}

	if(et_file_exist(szPath)){
		if(et_file_is_dir(szPath)){
			return 0;
		}else{
			return ET_ERR_FILE_EXIST;
		}
	}

	while(true){
		iSlashPos = et_sz_find_char(szPath,'/',iSlashPos + 1);
		if(iSlashPos == -1){
			break;
		}

		et_strncpy(szDirTemp,szPath,iSlashPos + 1);
		if(et_strlen(szDirTemp) == 0){
			break;
		}

		iResult = et_file_make_single_dir(szDirTemp);
		if(iResult < 0){
			return iResult;
		}
	}

	if(et_strlen(szDirTemp) == 0){
		return 0;
	}

	iResult = et_file_make_single_dir(szPath);
	if(iResult < 0){
		return iResult;
	}

	return 0;
}

int et_file_check_dir(const char* szPath){
	int iResult = 0;
	char szDir[2048] = "";

	if(szPath == null){
		return ET_ERR_MEM_NULL;
	}

	et_sz_file_dir(szDir,szPath);

	if(et_strlen(szDir) == 0){
		return 0;
	}

	iResult = et_file_make_long_dir(szDir);

	return iResult;
}

int et_file_write_buf(const char* szFileName, const char * bufWrite, int iBufSize){
	int iWritten = -1;
	int iFd = -1;
	int iResult =-1;

	if(et_file_check_dir(szFileName) < 0){
		return ET_ERR_FILE_CREATE;
	}

	if(szFileName == null
			|| et_strlen(szFileName) == 0){
		return ET_ERR_FILE_NAME;
	}

	if(bufWrite == null){
		return ET_ERR_MEM_NULL;
	}

	if(iBufSize == -1){
		iBufSize = et_strlen(bufWrite);
	}

	iFd = open(szFileName,O_WRONLY|O_CREAT|O_TRUNC,0777);
	if(iFd < 0){
		return ET_ERR_FILE_OPEN;
	}

	iResult = flock(iFd,LOCK_EX);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iWritten = write(iFd,bufWrite,iBufSize);
	if(iWritten < 0){
		return ET_ERR_FILE_WRITE;
	}

	iResult = flock(iFd,LOCK_UN);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iResult = close(iFd);
	if(iResult < 0){
		return ET_ERR_FILE_CLOSE;
	}

	return iWritten;
}

int et_file_read_buf(const char* szFileName, char * bufRead, int iReadLimit){
	INT64 iReadSize = 0;
	INT64 iResult = 0;
	INT64 iFd = -1;

	if(szFileName == null
			|| et_strlen(szFileName) == 0){
		return ET_ERR_FILE_NAME;
	}

	if(bufRead == null){
		return ET_ERR_MEM_NULL;
	}

	iFd = open(szFileName,O_RDONLY);
	if(iFd < 0){
		return ET_ERR_FILE_OPEN;
	}

	iResult = flock(iFd,LOCK_EX);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iReadSize = read(iFd,(void*)bufRead,iReadLimit);
	if(iReadSize < 0){
		return ET_ERR_FILE_READ;
	}

	iResult = flock(iFd,LOCK_UN);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iResult = close(iFd);
	if(iResult < 0){
		return ET_ERR_FILE_CLOSE;
	}

	return iReadSize;
}

INT64 et_file_size(const char* szPath){
	INT64 iSize = 0;
	int iResult = 0;
	struct stat stStat;

	if(szPath == null){
		return ET_ERR_MEM_NULL;
	}

	et_memset(&stStat,0,sizeof(stStat));
	iResult = stat(szPath,&stStat);
	if(iResult < 0){
		return ET_ERR_FILE_LOST;
	}
	if(!S_ISREG(stStat.st_mode)){
		return ET_ERR_FILE_READ;
	}
	iSize = stStat.st_size;

	return iSize;
}

int et_file_append_buf(const char* szFileName, const char * bufWrite, int iBufSize){
	int iWritten = 0;
	int iFd = -1;
	int iResult = 0;

	if(et_file_check_dir(szFileName) < 0){
		return ET_ERR_FILE_CREATE;
	}

	if(szFileName == null
			|| et_strlen(szFileName) == 0){
		return ET_ERR_FILE_NAME;
	}

	if(bufWrite == null){
		return ET_ERR_MEM_NULL;
	}

	if(iBufSize == -1){
		iBufSize = et_strlen(bufWrite);
	}

	iFd = open(szFileName,O_WRONLY|O_CREAT|O_APPEND,0777);
	if(iFd < 0){
		return ET_ERR_FILE_OPEN;
	}
	iResult = flock(iFd,LOCK_EX);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iWritten = write(iFd,bufWrite,iBufSize);
	if(iWritten < 0){
		return ET_ERR_FILE_WRITE;
	}

	iResult = flock(iFd,LOCK_UN);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iResult = close(iFd);
	if(iResult < 0){
		return ET_ERR_FILE_CLOSE;
	}

	return iWritten;
}

int et_file_set_size(const char* szFileName, INT64 iFileSize){
	int iFd = -1;
	int i = 0;
	int iResult = 0;
	INT64 iSeekRet = 0;

	if(szFileName == null){
		return ET_ERR_FILE_NAME;
	}

	if(iFileSize < 0){
		return ET_ERR_FILE_SIZE;
	}
	
	iFd = open(szFileName,O_WRONLY|O_CREAT|O_TRUNC|O_SYNC,0777);
	if(iFd < 0){
		return ET_ERR_FILE_OPEN;
	}

	iResult = flock(iFd,LOCK_EX);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iFileSize -= 1;

	if(iFileSize > 0){
		iSeekRet = lseek(iFd,iFileSize,SEEK_SET);
		if(iSeekRet < 0){
			close(iFd);
			return ET_ERR_FILE_SEEK;
		}
	}
	
	iResult = write(iFd,"1",1);
	if(iResult < 0){
		close(iFd);
		return ET_ERR_FILE_WRITE;
	}

	iResult = flock(iFd,LOCK_UN);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	close(iFd);

	return 0;
}


int et_file_get_bom_utf8(BYTE *bufBom){                              
	if(bufBom == NULL){
		return ET_ERR_MEM_NULL;
	}

	BYTE bufUtf8[3] = ET_FILE_BOM_UTF8;
	et_memcpy(bufBom,bufUtf8,3);

	return 0;
}

int et_file_get_bom_unicode(BYTE *bufBom){
	if(bufBom == NULL){
		return ET_ERR_MEM_NULL;
	}

	BYTE bufUnicode[2] = ET_FILE_BOM_UNICODE;
	et_memcpy(bufBom,bufUnicode,2);

	return 0;
}

int et_file_read_buf_by_pos(const char* szFileName, char * bufRead, UINT iPos, UINT iSize){
	INT64 iReadSize = 0;
	INT64 iResult = 0;
	int iFd = 0;

	if(szFileName == null
			|| et_strlen(szFileName) == 0){
		return ET_ERR_FILE_NAME;
	}

	if(bufRead == null){
		return ET_ERR_MEM_NULL;
	}

	iFd = open(szFileName,O_RDONLY);
	if(iFd < 0){
		return ET_ERR_FILE_OPEN;
	}
	
	iResult = flock(iFd,LOCK_EX);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iResult = lseek(iFd,iPos,SEEK_SET);
	if(iResult < 0){
		return ET_ERR_FILE_SEEK;
	}

	iReadSize = read(iFd,(void*)bufRead,iSize);
	if(iReadSize < 0){
		return ET_ERR_FILE_READ;
	}

	iResult = flock(iFd,LOCK_UN);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iResult = close(iFd);
	if(iResult < 0){
		return ET_ERR_FILE_CLOSE;
	}

	return iReadSize;
}

int et_file_write_buf_by_pos(const char* szFileName, const char * bufWrite, UINT iPos, UINT iSize){
	INT64 iReadSize = 0;
	INT64 iResult = 0;
	int iFd = 0;

	if(szFileName == null
			|| et_strlen(szFileName) == 0){
		return ET_ERR_FILE_NAME;
	}

	if(bufWrite == null){
		return ET_ERR_MEM_NULL;
	}

	iFd = open(szFileName,O_WRONLY);
	if(iFd < 0){
		return ET_ERR_FILE_OPEN;
	}
	
	iResult = flock(iFd,LOCK_EX);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iResult = lseek(iFd,iPos,SEEK_SET);
	if(iResult < 0){
		return ET_ERR_FILE_SEEK;
	}

	iReadSize = write(iFd,(void*)bufWrite,iSize);
	if(iReadSize < 0){
		return ET_ERR_FILE_READ;
	}

	iResult = flock(iFd,LOCK_UN);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iResult = close(iFd);
	if(iResult < 0){
		return ET_ERR_FILE_CLOSE;
	}

	return iReadSize;
}

int et_file_write_bom_utf8(const char* szFileName){
	BYTE bufBom[3] = ET_FILE_BOM_UTF8;
	return et_file_write_buf(szFileName,(const char*)bufBom,3);
}

int et_file_write_bom_unicode(const char* szFileName){
	BYTE bufBom[2] = ET_FILE_BOM_UNICODE;
	return et_file_write_buf(szFileName,(const char*)bufBom,2);
}

int et_file_delete_file(const char* szPath){
	int iFd = -1;
	int iResult = 0;

	if(szPath == null){
		return ET_ERR_MEM_NULL;
	}

	if(!et_file_is_file(szPath)){
		return 0;
	}

	iFd = open(szPath,O_RDWR,0777);
	if(iFd < 0){
		return ET_ERR_FILE_OPEN;
	}
	iResult = flock(iFd,LOCK_EX);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iResult = unlink(szPath);
	if(iResult < 0){
		return ET_ERR_FILE_DELETE;
	}

	iResult = flock(iFd,LOCK_UN);
	if(iResult < 0){
		return ET_ERR_FILE_LOCK;
	}

	iResult = close(iFd);
	if(iResult < 0){
		return ET_ERR_FILE_CLOSE;
	}

	return 0;
}

static void et_file_delete_dir_recur(const char* szPath, bool deleteMe, int* pError){
	int iResult = 0;
	DIR* pDir = null;
	struct dirent* pDirEntry = null;
	struct stat stStat;
	char szCurrent[2048] = "";
	char szNext[2048] = "";

	if(szPath == null){
		return;
	}

	if(!et_file_is_dir(szPath)){
		return;
	}

	et_sz_file_fix_dir(szCurrent,szPath);
	pDir = opendir(szCurrent);
	if(pDir == null){
		return;
	}

	while(true){
		pDirEntry = readdir(pDir);
		if(pDirEntry == null){
			break;
		}

		if(et_sz_equal(".",pDirEntry->d_name)){
			continue;
		}

		if(et_sz_equal("..",pDirEntry->d_name)){
			continue;
		}

		et_strcpy(szNext,szCurrent);
		et_strcat(szNext,"/");
		et_strcat(szNext,pDirEntry->d_name);
		iResult = lstat(szNext,&stStat);
		if(iResult < 0){
			break;
		}

		if(S_ISDIR(stStat.st_mode)){
			et_file_delete_dir_recur(szNext,true,pError);
		}else{
			iResult = et_file_delete_file(szNext);
			if(iResult < 0){
				*pError = ET_ERR_FILE_DELETE;
				break;
			}
		}
	}
	iResult = closedir(pDir);

	if(deleteMe){
		iResult = rmdir(szPath);
		if(iResult < 0){
			*pError = ET_ERR_FILE_DELETE;
			return;
		}
	}
}

int et_file_delete_dir(const char* szPath, bool deleteMe){
	int iError = 0;
	et_file_delete_dir_recur(szPath,deleteMe,&iError);
	if(iError < 0){
		return iError;
	}
	return 0;
}

int et_file_delete(const char* szPath){
	if(!et_file_exist(szPath)){
		return 0;
	}else if(et_file_is_dir(szPath)){
		return et_file_delete_dir(szPath,true);
	}else{
		return et_file_delete_file(szPath);
	}
}

int et_file_rename(const char* szPathOld, const char* szPathNew){
	int iResult = 0;

	if(szPathNew == null || szPathOld == null){
		return ET_ERR_FILE_NAME;
	}

	if(!et_file_exist(szPathOld)){
		return ET_ERR_FILE_LOST;
	}

	if(et_file_exist(szPathNew)){
		return ET_ERR_FILE_EXIST;
	}

	if(et_sz_equal(szPathOld,szPathNew)){
		return 0;
	}

	iResult = rename(szPathOld,szPathNew);
	if(iResult < 0){
		return ET_ERR_FILE_RENAME;
	}

	return 0;
}

int et_file_create_empty(const char* szPath){
	int iWritten = -1;
	int iFd = -1;
	int iResult =-1;

	if(et_file_check_dir(szPath) < 0){
		return ET_ERR_FILE_CREATE;
	}

	if(et_file_exist(szPath)){
		iResult = et_file_delete(szPath);
		if(iResult < 0){
			return iResult;
		}
	}

	if(szPath == null
			|| et_strlen(szPath) == 0){
		return ET_ERR_FILE_NAME;
	}

	iFd = open(szPath,O_WRONLY|O_CREAT|O_TRUNC,0777);
	if(iFd < 0){
		return ET_ERR_FILE_OPEN;
	}

	iResult = close(iFd);
	if(iResult < 0){
		return ET_ERR_FILE_CLOSE;
	}

	return 0;
}

static int et_file_move_file_with_delete(const char* szPathOld, const char* szPathNew, bool deleteOld){
	int iResult = 0;
	char* pBuf = null;
	int iOldSize = 0;
	int iCompleteSize = 0;
	int iTempSize = 0;

	if(et_sz_equal(szPathOld,szPathNew)){
		return 0;
	}

	if(szPathNew == null || szPathOld == null){
		return ET_ERR_FILE_NAME;
	}

	if(!et_file_exist(szPathOld)){
		return ET_ERR_FILE_LOST;
	}

	if(et_file_exist(szPathNew)){
		return ET_ERR_FILE_EXIST;
	}

	if(et_sz_equal(szPathOld,szPathNew)){
		return 0;
	}

	iResult = et_file_create_empty(szPathNew);
	if(iResult < 0){
		return iResult;
	}

	iOldSize = et_file_size(szPathOld);
	if(iOldSize < 0){
		return iOldSize;
	}

	if(iOldSize > 0){
		pBuf = (char*)et_alloc(ET_FILE_BUF_SIZE);
		
		while(true){
			if( iOldSize >= (iCompleteSize + ET_FILE_BUF_SIZE) ){
				iTempSize = ET_FILE_BUF_SIZE;
			}else{
				iTempSize = iOldSize - iCompleteSize;
			}

			iResult = et_file_read_buf_by_pos(szPathOld,pBuf,iCompleteSize,iTempSize);
			if(iResult < 0){
				et_free(pBuf);
				return iResult;
			}

			iResult = et_file_append_buf(szPathNew,pBuf,iTempSize);
			if(iResult < iTempSize){
				et_free(pBuf);
				return ET_ERR_FILE_WRITE;
			}

			iCompleteSize += iTempSize;

			if(iCompleteSize >= iOldSize){
				break;
			}
			
		}
		
		et_free(pBuf);
	}

	if(deleteOld){
		iResult = et_file_delete_file(szPathOld);
		if(iResult < 0){
			return iResult;
		}
	}

	return 0;
}

int et_file_move_file(const char* szPathOld, const char* szPathNew){
	return et_file_move_file_with_delete(szPathOld,szPathNew,true);
}

int et_file_copy_file(const char* szPathOld, const char* szPathNew){
	return et_file_move_file_with_delete(szPathOld,szPathNew,false);
}

static void et_file_move_dir_recur(const char* szPathOld, const char* szPathNew, const char* szSubPath, bool deleteMe, int* pError){
	int iResult = 0;
	DIR* pDir = null;
	struct dirent* pDirEntry = null;
	struct stat stStat;
	char szCurrent[2048] = "";
	char szNext[2048] = "";
	char szFullOld[2048] = "";
	char szFullNew[2048] = "";
	

	et_strcpy(szCurrent,szPathOld);
	et_strcat(szCurrent,szSubPath);

	pDir = opendir(szCurrent);
	if(pDir == null){
		return;
	}

	while(true){
		pDirEntry = readdir(pDir);
		if(pDirEntry == null){
			break;
		}

		if(et_sz_equal(".",pDirEntry->d_name)){
			continue;
		}

		if(et_sz_equal("..",pDirEntry->d_name)){
			continue;
		}

		et_strcpy(szNext,szCurrent);
		et_strcat(szNext,"/");
		et_strcat(szNext,pDirEntry->d_name);

		iResult = lstat(szNext,&stStat);
		if(iResult < 0){
			break;
		}

		et_strcpy(szNext,szSubPath);
		et_strcat(szNext,"/");
		et_strcat(szNext,pDirEntry->d_name);

		if(S_ISDIR(stStat.st_mode)){
			et_strcpy(szFullNew,szPathNew);
			et_strcat(szFullNew,szNext);
			et_file_make_long_dir(szFullNew);
			et_file_move_dir_recur(szPathOld,szPathNew,szNext,deleteMe,pError);
		}else{
			et_strcpy(szFullOld,szPathOld);
			et_strcat(szFullOld,szNext);

			et_strcpy(szFullNew,szPathNew);
			et_strcat(szFullNew,szNext);

			iResult = et_file_move_file_with_delete(szFullOld,szFullNew,deleteMe);
			if(iResult < 0){
				*pError = iResult;
				break;
			}

			//et_file_delete(szFullOld);
		}
	}
	iResult = closedir(pDir);

	if(deleteMe){
		iResult = rmdir(szCurrent);
		if(iResult < 0){
			*pError = ET_ERR_FILE_DELETE;
			return;
		}
	}
}

static int et_file_move_dir_with_delete(const char* szPathOld, const char* szPathNew, bool deleteMe){
	int iResult = 0;
	char szPathOldFix[2048] = "";
	char szPathNewFix[2048] = "";

	if(!et_file_is_dir(szPathOld)){
		return ET_ERR_FILE_LOST;
	}

	et_sz_file_fix_dir(szPathOldFix,szPathOld);
	et_sz_file_fix_dir(szPathNewFix,szPathNew);

	if(et_sz_equal(szPathOldFix,szPathNewFix)){
		return 0;
	}


	
	iResult = et_file_delete(szPathNewFix);
	if(iResult < 0){
		return iResult;
	}

	iResult = et_file_make_long_dir(szPathNewFix);
	if(iResult < 0){
		return iResult;
	}

	et_file_move_dir_recur(szPathOldFix,szPathNewFix,"",deleteMe,&iResult);
	return iResult;
}

int et_file_move_dir(const char* szPathOld, const char* szPathNew){
	return et_file_move_dir_with_delete(szPathOld,szPathNew,true);
}

int et_file_copy_dir(const char* szPathOld, const char* szPathNew){
	return et_file_move_dir_with_delete(szPathOld,szPathNew,false);
}

int et_file_read_utf8(const char* szPath, char* bufUtf8, int iReadLimit){
	int iResult = 0;
	int iReadCount = 0;
	int iFileSize = 0;
	BYTE bufHead[3] = {0};
	BYTE bufBomUtf8[3] = ET_FILE_BOM_UTF8;

	if(szPath == null || bufUtf8 == null){
		return ET_ERR_MEM_NULL;
	}

	if(!et_file_is_file(szPath)){
		return ET_ERR_FILE_LOST;
	}

	iFileSize = et_file_size(szPath);
	if(iFileSize < 0){
		return iFileSize;
	}

	if(iReadLimit < 0){
		iReadLimit = iFileSize;
	}

	if(iReadLimit > iFileSize){
		iReadCount = iFileSize;
	}else{
		iReadCount = iReadLimit;
	}

	if(iReadCount == 0){
		bufUtf8[0] = 0;
		return 0;
	}

	if(iFileSize < 3){
		iResult = et_file_read_buf_by_pos(szPath,bufUtf8,0,iReadCount);
		if(iResult < 0){
			return iResult;
		}
	}else{
		iResult = et_file_read_buf_by_pos(szPath,bufHead,0,3);
		if(iResult < 0){
			return iResult;
		}

		if(et_strncmp(bufHead,bufBomUtf8,3) == 0){
			if(iFileSize == 3){
				return 0;
			}
			iReadCount -= 3;
			iResult = et_file_read_buf_by_pos(szPath,bufUtf8,3,iReadCount);
		}else{
			iResult = et_file_read_buf_by_pos(szPath,bufUtf8,0,iReadCount);
		}
	}

	iReadCount = iResult;
	bufUtf8[iReadCount] = 0;
	return iReadCount;
}

int et_file_write_utf8(const char* szPath, const char* bufUtf8){
	int iResult = 0;
	int iWriteCount = 0;
	int iBufSize = 0;
	
	iResult = et_file_write_bom_utf8(szPath);
	if(iResult < 0){
		return iResult;
	}

	iBufSize = et_strlen(bufUtf8);
	if(iBufSize <= 0){
		return 0;
	}

	iResult = et_file_append_buf(szPath,bufUtf8,iBufSize);
	if(iResult < 0){
		return iResult;
	}

	iWriteCount = iResult;

	return iWriteCount;
}

int et_file_read_unicode(const char* szPath, UINT16* bufUnicode, int iReadLimit){
	int iResult = 0;
	int iReadCount = 0;
	int iFileSize = 0;
	BYTE bufHead[2] = {0};
	BYTE bufBomUnicode[3] = ET_FILE_BOM_UNICODE;

	if(szPath == null || bufUnicode == null){
		return ET_ERR_MEM_NULL;
	}

	if(!et_file_is_file(szPath)){
		return ET_ERR_FILE_LOST;
	}

	iFileSize = et_file_size(szPath);
	if(iFileSize < 0){
		return iFileSize;
	}

	if(iReadLimit < 0){
		iReadLimit = iFileSize;
	}

	iReadLimit *= 2;

	if(iReadLimit > iFileSize){
		iReadCount = iFileSize;
	}else{
		iReadCount = iReadLimit;
	}

	if(iReadCount == 0){
		bufUnicode[0] = 0;
		return 0;
	}

	if(iFileSize < 2){
		iResult = et_file_read_buf_by_pos(szPath,(char*)bufUnicode,0,2);
		if(iResult < 0){
			return iResult;
		}
	}else{
		iResult = et_file_read_buf_by_pos(szPath,(char*)bufHead,0,2);
		if(iResult < 0){
			return iResult;
		}

		if(et_strncmp((const char*)bufHead,(const char*)bufBomUnicode,2) == 0){
			if(iFileSize == 2){
				return 0;
			}
			iReadCount -= 2;
			iResult = et_file_read_buf_by_pos(szPath,(char*)bufUnicode,2,iReadCount);
		}else{
			iResult = et_file_read_buf_by_pos(szPath,(char*)bufUnicode,0,iReadCount);
		}
		iReadCount = iResult / 2;
		bufUnicode[iReadCount] = 0;
	}

	return iReadCount;
}

int et_file_write_unicode(const char* szPath, const UINT16* bufUnicode){
	int iResult = 0;
	int iWriteCount = 0;
	int iBufSize = 0;

	iResult = et_file_write_bom_unicode(szPath);
	if(iResult < 0){
		return iResult;
	}

	iBufSize = et_wcslen(bufUnicode) * 2;
	if(iBufSize <= 0){
		return 0;
	}

	iResult = et_file_append_buf(szPath,(const char*)bufUnicode,iBufSize);
	if(iResult < 0){
		return iResult;
	}

	iWriteCount = iResult;

	return iWriteCount;
}

int et_file_list_child(const char* szDir, EtLists* pList){
	int iResult = 0;
	DIR* pDir = null;
	struct dirent* pDirEntry = null;
	struct stat stStat;
	char szCurrent[2048] = "";
	char szNext[2048] = "";
	int iCount = 0;

	if(pList == null){
		return ET_ERR_MEM_NULL;
	}

	if(!et_file_is_dir(szDir)){
		return ET_ERR_FILE_LOST;
	}
	
	et_sz_file_fix_dir(szCurrent,szDir);
	et_lists_clear(pList);

	pDir = opendir(szCurrent);
	if(pDir == null){
		return ET_ERR_FILE_OPEN;
	}

	while(true){
		pDirEntry = readdir(pDir);
		if(pDirEntry == null){
			break;
		}

		
		if(et_sz_equal(".",pDirEntry->d_name)){
			continue;
		}

		if(et_sz_equal("..",pDirEntry->d_name)){
			continue;
		}

		
		
		et_strcpy(szNext,szCurrent);
		et_strcat(szNext,"/");
		et_strcat(szNext,pDirEntry->d_name);

		iResult = lstat(szNext,&stStat);
		if(iResult < 0){
			break;
		}
		
		if(S_ISDIR(stStat.st_mode)){
			//et_strcat(szNext,"/");
			et_lists_add(pList,szNext);
		}else{
			et_lists_add(pList,szNext);
		}
		iCount++;
	}
	iResult = closedir(pDir);
	return iCount;
}

void et_file_list_all_recur(const char* szDir, EtLists* pList, int* pError){
	int iResult = 0;
	DIR* pDir = null;
	struct dirent* pDirEntry = null;
	struct stat stStat;
	char szCurrent[2048] = "";
	char szNext[2048] = "";
	int iCount = 0;

	et_strcpy(szCurrent,szDir);
	
	pDir = opendir(szCurrent);
	if(pDir == null){
		*pError = ET_ERR_FILE_OPEN;
	}

	while(true){
		pDirEntry = readdir(pDir);
		if(pDirEntry == null){
			break;
		}

		if(et_sz_equal(".",pDirEntry->d_name)){
			continue;
		}

		if(et_sz_equal("..",pDirEntry->d_name)){
			continue;
		}

		et_strcpy(szNext,szCurrent);
		et_strcat(szNext,"/");
		et_strcat(szNext,pDirEntry->d_name);

		iResult = lstat(szNext,&stStat);
		if(iResult < 0){
			break;
		}
		
		if(S_ISDIR(stStat.st_mode)){
			et_lists_add(pList,szNext);
			et_file_list_all_recur(szNext,pList,pError);
		}else{
			et_lists_add(pList,szNext);
		}
		iCount++;
	}
	iResult = closedir(pDir);
	if(iResult < 0){
		*pError = iResult;
	}
}

int et_file_list_all(const char* szDir, EtLists* pList){
	int iResult = 0;
	char szDirFix[2048] = "";
	int iError = 0;

	if(szDir == null){
		return ET_ERR_FILE_NAME;
	}

	if(pList == null){
		return ET_ERR_MEM_NULL;
	}
	
	et_sz_file_fix_dir(szDirFix,szDir);

	if(!et_file_is_dir(szDirFix)){
		return ET_ERR_FILE_LOST;
	}
	
	et_lists_clear(pList);

	et_file_list_all_recur(szDirFix,pList,&iError);

	if(iError < 0){
		return iError;
	}else{
		return et_lists_size(pList);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

int et_ini_create(EtIni **ppIni){
	int iResult = 0;

	*ppIni = (EtIni*)et_alloc(sizeof(EtIni));
	if(*ppIni == null){
		return ET_ERR_MEM_ALLOC;
	}
	(*ppIni)->cb = sizeof(EtIni);
	(*ppIni)->valueTable = null;
	iResult = et_map_sv_create(&((*ppIni)->valueTable));
	if(iResult < 0){
		return iResult;
	}
	(*ppIni)->fileName[0] = 0;

	return 0;
}

int et_ini_clear(EtIni *pIni){
	int iResult = 0;
	const char* szKey = null;
	EtMapSs* pMapSs = null;
	
	if(pIni == null){
		return ET_ERR_MEM_NULL;
	}

	if( et_map_sv_size( pIni->valueTable ) == 0){
		return 0;
	}

	et_map_sv_iterator_start(pIni->valueTable);

	while(true){
		if( !et_map_sv_iterator_check(pIni->valueTable)){
			break;
		}

		pMapSs = (EtMapSs*)et_map_sv_iterator_value(pIni->valueTable);
		et_map_ss_destroy(&pMapSs);

		et_map_sv_iterator_move(pIni->valueTable);
	}
	et_map_sv_clear(pIni->valueTable);

	return 0;
}

int et_ini_destroy(EtIni **ppIni){
	int iResult = 0;

	if(*ppIni == null){
		return ET_ERR_MEM_NULL;
	}
	et_ini_clear(*ppIni);
	iResult = et_map_sv_destroy(&((*ppIni)->valueTable));
	if(iResult < 0){
		return iResult;
	}
	et_free(*ppIni);
	*ppIni = null;

	return 0;
}

int et_ini_load_text(EtIni *pIni, const char* szText){
	char *szSrc = null; 
	int iSrcLen = 0;
	EtLists* pListLines = null;
	char szLine[512] = {0};
	char szKey[512] = {0};
	char szValue[512] = {0};
	char szSection[512] = {0};
	int i = 0;
	int iResult = 0;
	int iLineLen = 0;
	int iPosEqual = -1;
	int iCountEqual = 0;
	int iLines = 0;
	EtMapSs* pMapDefault = null;
	EtMapSs* pMapTemp = null;

	if(szText == null){
		return ET_ERR_MEM_NULL;
	}
	et_ini_clear(pIni);

	iSrcLen = et_strlen(szText);
	if(iSrcLen < 2){
		return 0;
	}

	szSrc = (char*)et_alloc(iSrcLen + 1);
	et_memset(szSrc,0,iSrcLen + 1);
	et_strcpy(szSrc,szText);

	et_sz_replace(szSrc,"\r\n","\n");

	et_lists_create(&pListLines);
	et_map_ss_create(&pMapDefault);

	et_map_sv_set(pIni->valueTable,"",pMapDefault);
	et_map_sv_get(pIni->valueTable,"",(void**)&pMapTemp);

	iLines = et_sz_split(pListLines,szSrc,"\n");
	if(iLines <= 0){
		et_lists_destroy(&pListLines);
		et_free(szSrc);
		return 0;
	}

	for(i = 0; i < iLines; i++){
		et_lists_get(pListLines,i,szLine);
		et_sz_trim(szLine);

		iLineLen = et_strlen(szLine);
		if(iLineLen <= 0){
			continue;
		}
	
		iPosEqual = et_sz_find(szLine,"=");
		iCountEqual = et_sz_count_sz(szLine,"=");

		if( szLine[0] == ';'
			|| szLine[0] == '#')
		{
			continue;
		}

		if(szLine[0] == '[' && et_sz_is_end_with(szLine,"]")){
			et_sz_substr(szSection,szLine,1,iLineLen - 2);
			et_sz_trim(szSection);
			if(!et_map_sv_contains_key(pIni->valueTable,szSection)){
				et_map_ss_create(&pMapTemp);
				et_map_sv_set(pIni->valueTable,szSection,pMapTemp);
			}
			(EtMapSs*)et_map_sv_get(pIni->valueTable,szSection,(void**)&pMapTemp);
			continue;
		}

		if(iCountEqual >= 1){
			et_sz_substr(szKey,szLine,0,iPosEqual);
			et_sz_trim(szKey);

			et_sz_substr(szValue,szLine,iPosEqual + 1,-1);
			et_sz_trim(szValue);

			et_map_ss_set(pMapTemp,szKey,szValue);
			continue;
		}
	}

	et_lists_destroy(&pListLines);
	et_free(szSrc);

	return 0;
}

int et_ini_load_file(EtIni *pIni, const char* szFileName){
	char* szContent = null;
	int iFileSize = 0;
	int iResult = 0;

	if(pIni == null || szFileName == null){
		return ET_ERR_MEM_NULL;
	}

	iFileSize = et_file_size(szFileName);
	if(iFileSize < 2){
		return 0;
	}

	et_strcpy(pIni->fileName,szFileName);

	szContent = (char*)et_alloc(iFileSize + 1);
	et_file_read_utf8(szFileName,szContent,iFileSize);

	iResult = et_ini_load_text(pIni,szContent);

	et_free(szContent);

	return iResult;

}
int et_ini_to_text(EtIni *pIni, char* szText){
	int iResult = 0;
	EtMapSs* pMapSs = null;
	const char* szSection = null;
	const char* szKey = null;
	const char* szValue = null;

	if(pIni == null || szText == null){
		return ET_ERR_MEM_NULL;
	}

	szText[0] = 0;

	if(et_map_sv_size(pIni->valueTable) == 0){
		return 0;
	}

	et_map_sv_iterator_start(pIni->valueTable);
	while(true){
		if(!et_map_sv_iterator_check(pIni->valueTable)){
			break;
		}

		szSection = et_map_sv_iterator_key(pIni->valueTable);
		pMapSs = (EtMapSs*)et_map_sv_iterator_value(pIni->valueTable);
		if(et_strlen(szSection) > 0){
			et_strcat(szText,"[");
			et_strcat(szText,szSection);
			et_strcat(szText,"]");
			et_strcat(szText,"\r\n");
		}
		

		et_map_ss_iterator_start(pMapSs);
		while(true){
			if(!et_map_ss_iterator_check(pMapSs)){
				break;
			}

			szKey = et_map_ss_iterator_key(pMapSs);
			szValue = et_map_ss_iterator_value(pMapSs);
			et_strcat(szText,szKey);
			et_strcat(szText,"=");
			et_strcat(szText,szValue);
			et_strcat(szText,"\r\n");
			et_map_ss_iterator_move(pMapSs);
		}
		et_map_sv_iterator_move(pIni->valueTable);
	}

	return 0;
}

int et_ini_save_as(EtIni *pIni, const char* szFileName){
	int iResult = 0;
	char* szText = null;

	if(pIni == null || szFileName == null){
		return ET_ERR_MEM_NULL;
	}

	if(et_strlen(szFileName) == 0){
		return 0;
	}

	szText = (char*)et_alloc(3*10240);

	iResult = et_ini_to_text(pIni,szText);
	if(iResult < 0){
		et_free(szText);
		return iResult;
	}

	iResult = et_file_write_buf(szFileName,szText,et_strlen(szText));
	if(iResult < 0){
		et_free(szText);
		return iResult;
	}

	et_free(szText);
	return 0;
}

int et_ini_save(EtIni *pIni){
	int iResult = 0;
	char* szText = null;

	if(pIni == null){
		return ET_ERR_MEM_NULL;
	}

	if(et_strlen(pIni->fileName) == 0){
		return 0;
	}

	szText = (char*)et_alloc(3*10240);

	iResult = et_ini_to_text(pIni,szText);
	if(iResult < 0){
		et_free(szText);
		return iResult;
	}

	iResult = et_file_write_buf(pIni->fileName,szText,et_strlen(szText));
	if(iResult < 0){
		et_free(szText);
		return iResult;
	}

	et_free(szText);
	return 0;
}

int et_ini_set(EtIni *pIni, const char* szSection, const char* szKey, const char* szValue){
	int iResult = 0;
	EtMapSs* pMapSs = null;

	if(pIni == null
		|| szSection == null
		|| szKey == null
		|| szValue == null
	){
		return ET_ERR_MEM_NULL;
	}

	if(!et_map_sv_contains_key(pIni->valueTable,szSection)){
		et_map_ss_create(&pMapSs);
		et_map_sv_set(pIni->valueTable,szSection,pMapSs);
	}else{
		et_map_sv_get(pIni->valueTable,szSection,(void**)&pMapSs);
	}

	et_map_ss_set(pMapSs,szKey,szValue);

	return 0;
}

int et_ini_get(EtIni *pIni, const char* szSection, const char* szKey, char* szValue){
	int iResult = 0;
	EtMapSs* pMapSs = null;

	if(pIni == null
		|| szSection == null
		|| szKey == null
		|| szValue == null
	){
		return ET_ERR_MEM_NULL;
	}

	if(!et_map_sv_contains_key(pIni->valueTable,szSection)){
		szValue[0] = 0;
		return ET_ERR_LOST;
	}else{
		et_map_sv_get(pIni->valueTable,szSection,(void**)&pMapSs);
		if(!et_map_ss_contains_key(pMapSs,szKey)){
			szValue[0] = 0;
			return ET_ERR_LOST;
		}
		return et_map_ss_get(pMapSs,szKey,szValue);
	}

	return 0;
}

int et_ini_fast_set(const char* szFileName, const char* szSection, const char* szKey, const char* szValue){
	int iResult = 0;
	EtIni* pIni = null;
	
	et_ret_if_neg(iResult,et_ini_create(&pIni))
	et_ret_if_neg(iResult,et_ini_load_file(pIni,szFileName))
	et_ret_if_neg(iResult,et_ini_set(pIni,szSection,szKey,szValue))
	et_ret_if_neg(iResult,et_ini_save(pIni))
	et_ret_if_neg(iResult,et_ini_destroy(&pIni))
	return iResult;
}

int et_ini_fast_get(const char* szFileName, const char* szSection, const char* szKey, char* szValue){
	int iResult = 0;
	EtIni* pIni = null;
	
	et_ret_if_neg(iResult,et_ini_create(&pIni))
	et_ret_if_neg(iResult,et_ini_load_file(pIni,szFileName))
	et_ret_if_neg(iResult,et_ini_get(pIni,szSection,szKey,szValue))
	et_ret_if_neg(iResult,et_ini_destroy(&pIni))
	return iResult;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

int et_wcslen(const UINT16* wszSrc){
	int i = 0;
	int iResult = 0;
	const char* mbsSrc = null;
	int iMax = 32767;

	if(wszSrc == null){
		return -1;
	}
	mbsSrc = (const char*)wszSrc;

	while(1){
		if(mbsSrc[i + 0] == 0
			&& mbsSrc[i + 1] == 0
		){
			break;
		}

		if(i >= iMax){
			break;
		}

		i += 2;
	}

	iResult = i / 2;

	return iResult;
}

////////////////////////////////////////////////////////////////////////////////////////////

int et_charset_unicode_to_utf8(char* szUtf8, const UINT16* wszUnicode){
	int iLen = 0;
	UINT16 chWcsTemp = 0;
	int i = 0;
	int iUnicodeSize = 0;

	if(szUtf8 == NULL || wszUnicode == NULL){
		return 0;
	}

	szUtf8[0] = 0;

	iUnicodeSize = et_wcslen(wszUnicode);
	if(iUnicodeSize == 0){
		return 0;
	}

	for(i = 0; i < iUnicodeSize; i++){
		chWcsTemp = wszUnicode[i];
		if(chWcsTemp == 0){
			break;
		}else if(chWcsTemp < 0x0000007F){
			szUtf8[iLen++] = (char)chWcsTemp;
		}else if(chWcsTemp < 0x0000007FF){
			szUtf8[iLen++] = 0xC0 | (chWcsTemp >> 6);
			szUtf8[iLen++] = 0x80 | (chWcsTemp & 0x3F);
		}else if(chWcsTemp < 0x00000FFFF){
			szUtf8[iLen++] = 0xE0 | (chWcsTemp >> 12);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 6) & 0x3F);
			szUtf8[iLen++] = 0x80 | (chWcsTemp & 0x3F);
		}else if(chWcsTemp < 0x0001FFFFF){
			szUtf8[iLen++] = 0xF0 | (((int)chWcsTemp) >> 18);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 12) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 6) & 0x3F);
			szUtf8[iLen++] = 0x80 | (chWcsTemp & 0x3F);
		}else if(chWcsTemp < 0x03FFFFFFF){
			szUtf8[iLen++] = 0xF8 | (((int)chWcsTemp) >> 24);
			szUtf8[iLen++] = 0x80 | ((((int)chWcsTemp) >> 18) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 12) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 6) & 0x3F);
			szUtf8[iLen++] = 0x80 | (chWcsTemp & 0x3F);
		}else if(chWcsTemp < 0x7FFFFFFFF){
			szUtf8[iLen++] = 0xFC | (((int)chWcsTemp) >> 30);
			szUtf8[iLen++] = 0x80 | ((((int)chWcsTemp) >> 24) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((((int)chWcsTemp) >> 18) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 12) & 0x3F);
			szUtf8[iLen++] = 0x80 | ((chWcsTemp >> 6) & 0x3F);
			szUtf8[iLen++] = 0x80 | (chWcsTemp & 0x3F);
		}
	}

	((unsigned char*)szUtf8)[iLen] = 0;

	return iLen;
}

int et_charset_utf8_to_unicode(UINT16* wszUnicode, const char* szUtf8){
	int iLen = 0;
	int iLoop = 0;
	unsigned char chTemp = 0;
	UINT16 chWcsTemp = 0;
	int iSizeTemp = 0;
	unsigned int iMax = 1024*1024*1024;
	unsigned int iCount = 0;

	if(wszUnicode == NULL || wszUnicode == NULL){
		return 0;
	}

	wszUnicode[0] = 0;

	int iUtf8Size = et_strlen(szUtf8);
	if(iUtf8Size == 0){
		return 0;
	}

	while(1){
		if(iLoop > iUtf8Size){
			break;
		}
		if(iCount++ > iMax){
			break;
		}
		chWcsTemp = 0;
		chTemp = szUtf8[iLoop];

		if(chTemp == 0){
			break;
		}

		if((chTemp & 0x80) == 0){
			iSizeTemp = 1;
			chWcsTemp = chTemp;
		}else if((chTemp & 0xE0) == 0xC0){
			iSizeTemp = 2;

			chWcsTemp = (szUtf8[iLoop] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+1] & 0x3F);

		}else if((chTemp & 0xF0) == 0xE0){
			iSizeTemp = 3;

			chWcsTemp = (szUtf8[iLoop] & 0x0F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+1] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+2] & 0x3F);

		}else if((chTemp & 0xF8) == 0xF0){
			iSizeTemp = 4;

			chWcsTemp = (szUtf8[iLoop] & 0x0F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+1] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+2] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+3] & 0x3F);
		}else if((chTemp & 0xFC) == 0xF8){
			iSizeTemp = 5;

			chWcsTemp = (szUtf8[iLoop] & 0x07);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+1] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+2] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+3] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+4] & 0x3F);
		}else if((chTemp & 0xFE) == 0xFC){
			iSizeTemp = 6;

			chWcsTemp = (szUtf8[iLoop] & 0x03);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+1] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+2] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+3] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+4] & 0x3F);
			chWcsTemp = chWcsTemp << 6;

			chWcsTemp = chWcsTemp | (szUtf8[iLoop+5] & 0x3F);
		}
		iLoop += iSizeTemp;
		wszUnicode[iLen++] = chWcsTemp;
	}
	wszUnicode[iLen] = 0;
	return iLen;
}

int et_charset_unicode_to_utf8_size( const UINT16* wszUnicode ){
	int iLen = 0;
	UINT16 chWcsTemp = 0;
	int iUnicodeSize = 0;
	int i = 0;

	if(wszUnicode == NULL){
		return 0;
	}

	iUnicodeSize = et_wcslen(wszUnicode);
	if(iUnicodeSize == 0){
		return 0;
	}

	for(i = 0; i < iUnicodeSize; i++){
		chWcsTemp = wszUnicode[i];
		if(chWcsTemp == 0){
			break;
		}else if(chWcsTemp < 0x0000007F){
			iLen += 1;
		}else if(chWcsTemp < 0x0000007FF){
			iLen += 2;
		}else if(chWcsTemp < 0x00000FFFF){
			iLen += 3;
		}else if(chWcsTemp < 0x0001FFFFF){
			iLen += 4;
		}else if(chWcsTemp < 0x03FFFFFFF){
			iLen += 5;
		}else if(chWcsTemp < 0x7FFFFFFFF){
			iLen += 6;
		}
	}

	return iLen;
}

int et_charset_utf8_to_unicode_size( const char* szUtf8 ){
	int iLen = 0;
	int iLoop = 0;
	int iUtf8Size = 0;
	unsigned char chTemp = 0;
	UINT16 chWcsTemp = 0;
	int iSizeTemp = 0;

	if(szUtf8 == NULL){
		return 0;
	}

	iUtf8Size = et_strlen(szUtf8);
	if(iUtf8Size == 0){
		return 0;
	}

	while(1){
		if(iLoop >= iUtf8Size){
			break;
		}
		chWcsTemp = 0;
		chTemp = szUtf8[iLoop];

		if(chTemp == 0){
			break;
		}

		if((chTemp & 0x80) == 0){
			iSizeTemp = 1;
		}else if((chTemp & 0xE0) == 0xC0){
			iSizeTemp = 2;
		}else if((chTemp & 0xF0) == 0xE0){
			iSizeTemp = 3;
		}else if((chTemp & 0xF8) == 0xF0){
			iSizeTemp = 4;
		}else if((chTemp & 0xFC) == 0xF8){
			iSizeTemp = 5;
		}else if((chTemp & 0xFE) == 0xFC){
			iSizeTemp = 6;
		}
		iLoop += iSizeTemp;
		iLen++;
	}
	return iLen;
}

int et_xml_node_create(EtXmlNode** ppNode){
	int iResult = 0;
	*ppNode = (EtXmlNode*)et_alloc(sizeof(EtXmlNode));
	if(*ppNode == null){
		return ET_ERR_MEM_ALLOC;
	}
	et_memset(*ppNode,0,sizeof(EtXmlNode));
	
	(*ppNode)->cb = sizeof(EtXmlNode);
	et_ret_if_neg(iResult, et_map_ss_create(&(*ppNode)->attrList))

	return 0;
}

int et_xml_node_destroy(EtXmlNode** ppNode){
	int iResult = 0;

	if(*ppNode == null){
		return 0;
	}

	et_xml_node_clear(*ppNode);

	et_ret_if_neg(iResult, et_xml_node_clear(*ppNode))
	et_ret_if_neg(iResult, et_map_ss_destroy(&(*ppNode)->attrList))
	if((*ppNode)->text != null){
		et_free((*ppNode)->text);
	}
	et_memset(*ppNode,0,sizeof(EtXmlNode));
	et_free(*ppNode);

	return 0;
}

void et_xml_node_clear_recur(EtXmlNode* pNode,bool deleteMe, int* pError){
	EtXmlNode* pTempNode = pNode;

	if(pNode->firstChild != null){
		pTempNode = pNode->firstChild;
		while(1){
			if(pTempNode == null){
				break;
			}
			et_xml_node_clear_recur(pTempNode,true,pError);
			if(*pError < 0){
				return;
			}
			pTempNode = pTempNode->nextSibling;
		}
	}

	if(deleteMe){
		et_xml_node_destroy(&pNode);
	}
}

int et_xml_node_clear(EtXmlNode* pNode){
	int iResult = 0;

	if(pNode == null){
		return ET_ERR_MEM_NULL;
	}

	et_xml_node_clear_recur(pNode,false,&iResult);

	return iResult;
}

int et_xml_doc_create(EtXmlDoc** ppDoc){
	int iResult = 0;
	*ppDoc = (EtXmlDoc*)et_alloc(sizeof(EtXmlDoc));
	if(*ppDoc == null){
		return ET_ERR_MEM_ALLOC;
	}
	et_memset(*ppDoc,0,sizeof(EtXmlDoc));
	
	(*ppDoc)->cb = sizeof(EtXmlDoc);
	(*ppDoc)->standalone = true;

	return 0;
}

int et_xml_doc_destroy(EtXmlDoc** ppDoc){
	int iResult = 0;

	if(*ppDoc == null){
		return 0;
	}

	et_xml_doc_clear(*ppDoc);

	et_memset(*ppDoc,0,sizeof(EtXmlDoc));
	et_free(*ppDoc);

	return 0;
}

int et_xml_doc_clear(EtXmlDoc* pDoc){
	et_xml_node_destroy(&pDoc->rootNode);

	pDoc->encoding[0] = 0;
	pDoc->fileName[0] = 0;
	pDoc->rootNode = null;
	pDoc->standalone = true;
	pDoc->version[0] = 0;
	return 0;
}

int et_xml_doc_load_text(EtXmlDoc* pDoc, const char* szContent){
	int iResult = 0;
	EtXmlStatus xStatus;
	char* szSrc = null;
	int iSrcLen = 0;
	int iPos = 0;
	char chTemp = 0;

	char* szTemp = null;

	if(szContent == null || pDoc == null){
		iResult = ET_ERR_MEM_NULL;
		goto go_ret;
	}

	iSrcLen = et_strlen(szContent);
	if(iSrcLen == 0){
		iResult = 0;
		goto go_ret;
	}

	szTemp = (char*)et_alloc(1024*10 + 1);
	szTemp[0] = 0;

	et_memset(&xStatus,0,sizeof(EtXmlStatus));
	szSrc = (char*)et_alloc(iSrcLen + 1);
	et_memset(szSrc,0,iSrcLen + 1);
	et_strcpy(szSrc,szContent);

	et_sz_replace(szSrc,"\r\n","\n");

	printf("\n%s\n",szSrc);

	iSrcLen = et_strlen(szSrc);

	while(true){
		if(iPos >= iSrcLen){
			break;
		}

		chTemp = szSrc[iPos];

		if(!xStatus.hasStart){
			if(chTemp == '<'){
				xStatus.hasStart = true;
				xStatus.posType = ET_XML_POS_LT;
			}else if(et_char_is_space(chTemp)){
			}else{
				iResult = ET_ERR_XML_PARSE;
				goto go_ret;
			}
			iPos++;
			continue;
		}

		if(xStatus.posType == ET_XML_POS_LT){
			if(chTemp == '?'){
				if(xStatus.hasHead){
					iResult = ET_ERR_XML_PARSE;
					goto go_ret;
				}
				xStatus.posType = ET_XML_POS_PUNC;
			}else if(chTemp == '!'){
				xStatus.posType = ET_XML_POS_PUNC;
			}else if(chTemp == '/'){
				xStatus.posType = ET_XML_POS_END;
			}else if(et_char_is_ascii(chTemp)){
				xStatus.posType = ET_XML_POS_BEGIN_NAME;
				xStatus.isHead = false;
				xStatus.isBegin = true;
			}else{
				iResult = ET_ERR_XML_PARSE;
				goto go_ret;
			}
			iPos++;
			continue;
		}else if(xStatus.posType == ET_XML_POS_PUNC){
			if(szSrc[iPos-1] == '!' && chTemp == '-'){
				xStatus.posType = ET_XML_POS_COMMENT;
			}else if(szSrc[iPos-1] == '!' && chTemp == '['){
				xStatus.posType = ET_XML_POS_CDATA;
			}else if(szSrc[iPos-1] == '?' && chTemp == 'x'){
				xStatus.posType = ET_XML_POS_HEAD_BEGIN;
			}
		}else if(xStatus.posType == ET_XML_POS_CDATA){
		}else if(xStatus.posType == ET_XML_POS_COMMENT){
		}else if(xStatus.posType == ET_XML_POS_END){
		}

		if(xStatus.isHead){
		}else if(xStatus.isBegin){
		}
		
	}

go_ret:
	et_free(szTemp);
	et_free(szSrc);
	return iResult;
}

int et_xml_doc_load_file(EtXmlDoc* pDoc, const char* szFileName){
	char* szContent = null;
	int iFileSize = 0;
	int iResult = 0;
	if(pDoc == null || szFileName == null){
		return ET_ERR_MEM_NULL;
	}

	iFileSize = et_file_size(szFileName);

	if(iFileSize < 2){
		return 0;
	}

	et_strcpy(pDoc->fileName,szFileName);

	szContent = (char*)et_alloc(iFileSize + 1);
	et_file_read_utf8(szFileName,szContent,iFileSize);

	iResult = et_xml_doc_load_text(pDoc,szContent);

	et_free(szContent);

	return iResult;
}

int et_xml_node_to_text(EtXmlNode* pNode, char* szContent){
	return 0;
}

int et_xml_text_to_node(EtXmlNode* pNode, const char* szContent){
	return 0;
}

int et_xml_doc_to_text(EtXmlDoc* pDoc, const char* szContent){
	return 0;
}

int et_xml_doc_save_as(EtXmlDoc* pDoc, const char* szFileName){
	int iResult = 0;
	char* szText = null;

	if(pDoc == null || szFileName == null){
		return ET_ERR_MEM_NULL;
	}

	if(et_strlen(szFileName) == 0){
		return 0;
	}

	szText = (char*)et_alloc(3*10240);

	iResult = et_xml_doc_to_text(pDoc,szText);
	if(iResult < 0){
		et_free(szText);
		return iResult;
	}

	iResult = et_file_write_buf(szFileName,szText,et_strlen(szText));
	if(iResult < 0){
		et_free(szText);
		return iResult;
	}

	et_free(szText);
	return 0;
}

int et_xml_doc_save(EtXmlDoc* pDoc){
	int iResult = 0;
	char* szText = null;

	if(pDoc == null){
		return ET_ERR_MEM_NULL;
	}

	if(et_strlen(pDoc->fileName) == 0){
		return 0;
	}

	szText = (char*)et_alloc(3*10240);

	iResult = et_xml_doc_to_text(pDoc,szText);
	if(iResult < 0){
		et_free(szText);
		return iResult;
	}

	iResult = et_file_write_buf(pDoc->fileName,szText,et_strlen(szText));
	if(iResult < 0){
		et_free(szText);
		return iResult;
	}

	et_free(szText);
	return 0;
}

int et_xml_node_insert_before(EtXmlNode* pRefNode, EtXmlNode* pNewNode){
	int iResult = 0;
	EtXmlNode* pPrevNode = null;
	EtXmlNode* pParentNode = null;

	if(pRefNode == null || pNewNode == null){
		return ET_ERR_MEM_NULL;
	}

	if(pRefNode->nodeType == ET_XML_NODE_BLANK
		|| pRefNode->nodeType == ET_XML_NODE_HEAD
	){
		return ET_ERR_XML_PARSE;
	}

	if(pNewNode->nodeType == ET_XML_NODE_BLANK
		|| pNewNode->nodeType == ET_XML_NODE_HEAD
	){
		return ET_ERR_XML_PARSE;
	}

	pPrevNode = pRefNode->prevSibling;
	pParentNode = pRefNode->parentNode;

	if(pPrevNode != null){
		pPrevNode->nextSibling = pNewNode;
	}
	pNewNode->prevSibling = pPrevNode;

	pRefNode->prevSibling = pNewNode;
	pNewNode->nextSibling = pRefNode;

	pNewNode->parentNode = pParentNode;

	if(pNewNode->prevSibling == null){
		pParentNode->firstChild = pNewNode;
	}

	return iResult;
}

int et_xml_node_insert_after(EtXmlNode* pRefNode, EtXmlNode* pNewNode){
	int iResult = 0;
	EtXmlNode* pNextNode = null;
	EtXmlNode* pParentNode = null;

	if(pRefNode == null || pNewNode == null){
		return ET_ERR_MEM_NULL;
	}

	if(pRefNode->nodeType == ET_XML_NODE_BLANK
		|| pRefNode->nodeType == ET_XML_NODE_HEAD
	){
		return ET_ERR_XML_PARSE;
	}

	if(pNewNode->nodeType == ET_XML_NODE_BLANK
		|| pNewNode->nodeType == ET_XML_NODE_HEAD
	){
		return ET_ERR_XML_PARSE;
	}

	pNextNode = pRefNode->nextSibling;
	pParentNode = pRefNode->parentNode;

	if(pNextNode != null){
		pNextNode->prevSibling = pNewNode;
	}
	pNewNode->nextSibling = pNextNode;

	pRefNode->nextSibling = pNewNode;
	pNewNode->prevSibling = pRefNode;

	pNewNode->parentNode = pParentNode;

	return iResult;
}

int et_xml_node_append_head(EtXmlNode* pParentNode, EtXmlNode* pChildNode){
	int iResult = 0;
	EtXmlNode* pHeadNode = null;

	if(pParentNode == null || pChildNode == null){
		return ET_ERR_MEM_NULL;
	}

	if( pParentNode->nodeType != ET_XML_NODE_ELEMENT
		&& pParentNode->nodeType != ET_XML_NODE_BLANK
	){
		return ET_ERR_XML_PARSE;
	}

	if(pChildNode->nodeType == ET_XML_NODE_HEAD
		|| pChildNode->nodeType == ET_XML_NODE_BLANK
	){
		return ET_ERR_XML_PARSE;
	}

	pHeadNode = pParentNode->firstChild;

	if(pHeadNode != null){
		pHeadNode->prevSibling = pChildNode;
	}
	pChildNode->nextSibling = pHeadNode;

	pChildNode->parentNode = pParentNode;
	pParentNode->firstChild = pChildNode;

	return iResult;
}

EtXmlNode* et_xml_node_last_child(EtXmlNode* pParentNode){
	EtXmlNode* pFootNode = null;
	while(1){
		pFootNode = pParentNode->firstChild;
		if(pFootNode == null){
			break;
		}

		if(pFootNode->nextSibling == null){
			break;
		}

		pFootNode = pFootNode->nextSibling;
	}

	return pFootNode;
}

int et_xml_node_append_foot(EtXmlNode* pParentNode, EtXmlNode* pChildNode){
	int iResult = 0;
	EtXmlNode* pFootNode = null;

	if(pParentNode == null || pChildNode == null){
		return ET_ERR_MEM_NULL;
	}

	if( pParentNode->nodeType != ET_XML_NODE_ELEMENT
		&& pParentNode->nodeType != ET_XML_NODE_BLANK
	){
		return ET_ERR_XML_PARSE;
	}

	if(pChildNode->nodeType == ET_XML_NODE_HEAD
		|| pChildNode->nodeType == ET_XML_NODE_BLANK
	){
		return ET_ERR_XML_PARSE;
	}

	pFootNode = et_xml_node_last_child(pParentNode);

	if(pFootNode != null){
		pFootNode->nextSibling = pChildNode;
	}
	pChildNode->prevSibling = pFootNode;

	pChildNode->parentNode = pParentNode;

	return iResult;
}

int et_xml_node_remove(EtXmlNode* pNode){
	int iResult = 0;
	EtXmlNode* pParentNode = null;
	EtXmlNode* pPrevNode = null;
	EtXmlNode* pNextNode = null;

	if(pNode == null){
		return ET_ERR_MEM_NULL;
	}

	if(pNode->nodeType == ET_XML_NODE_BLANK){
		return ET_ERR_XML_PARSE;
	}

	pParentNode = pNode->parentNode;
	pPrevNode = pNode->prevSibling;
	pNextNode = pNode->nextSibling;

	pParentNode->firstChild = pNextNode;

	if(pPrevNode != null){
		pPrevNode->nextSibling = pNextNode;
	}

	if(pNextNode != null){
		pNextNode->prevSibling = pPrevNode;
	}

	return iResult;
}

int et_xml_node_set_attr(EtXmlNode* pNode, const char* szKey, const char* szValue){
	if(pNode == null || szKey == null || szValue == null){
		return ET_ERR_MEM_NULL;
	}

	return et_map_ss_set(pNode->attrList,szKey,szValue);
}

int et_xml_node_get_attr(EtXmlNode* pNode, const char* szKey, char* szValue){
	if(pNode == null || szKey == null || szValue == null){
		return ET_ERR_MEM_NULL;
	}

	return et_map_ss_get(pNode->attrList,szKey,szValue);
}

int et_xml_node_set_text(EtXmlNode* pNode, const char* szText){
	int iSrcLen = 0;
	char* szNew = null;

	if(pNode == null || szText == null){
		return ET_ERR_MEM_NULL;
	}

	et_free(pNode->text);
	iSrcLen = et_strlen(szText);
	szNew = (char*)et_alloc(iSrcLen + 1);
	et_strcpy(szNew,szText);
	pNode->text = szNew;

	return 0;
}

int et_xml_node_get_text(EtXmlNode* pNode, char* szText){
	if(pNode == null || szText == null){
		return ET_ERR_MEM_NULL;
	}

	szText[0] = 0;
	if(pNode->text != null){
		et_strcpy(szText,pNode->text);
	}

	return 0;
}