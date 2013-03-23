#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#define i64s long long int
#define i64u unsigned long long int
#define i32s int
#define i32u unsigned int
#define i16s short
#define i16u unsigned short
#define i8s char
#define i8u unsigned char

#define f64s double
#define f64u unsigned double
#define f32s float
#define f32u unsigned float

#define INT64 long long int
#define UINT64 unsigned long long int
#define INT32 int
#define UINT32 unsigned int
#define INT16 short
#define UINT16 unsigned short
#define INT8 char
#define UINT8 unsigned char
#define UINT unsigned int
#define UCHAR unsigned char
#define BYTE unsigned char
#define FLOAT64 double
#define UFLOAT64 unsigned double
#define FLOAT32 float
#define UFLOAT32 unsigned float
#define USHORT unsigned short
#define ULONG unsigned long
#define WCHAR unsigned short

#define bool char

#define null 0
#define true 1
#define false 0

//#define NULL 0
#define TRUE 1
#define FALSE 0

typedef int (*et_fn_int_ptr)(INT64* piValue);
typedef int (*et_fn_2int_ptr)(INT64* piKey, INT64* piValue);
typedef int (*et_fn_sz_ptr)(char** pszData);
typedef int (*et_fn_2sz_ptr)(char** pszData1, char** pszData2);
typedef int (*et_fn_cmp_int)(INT64 iLeft, INT64 iRight);
typedef int (*et_fn_cmp_sz)(const char* szLeft, const char* szRight);

int et_fn_cmp_sz_default(const char* szLeft, const char* szRight);
int et_fn_cmp_int_default(INT64 iLeft, INT64 iRight);

/////////////////////////////////////////////////////////////////////////////

#define ET_SZ_EMPTY ""
#define ET_SZ_R "\r"
#define ET_SZ_N "\n"
#define ET_SZ_RN "\r\n"
#define ET_SZ_SLASH "/"
#define ET_SZ_BSLASH "\\"
#define ET_SZ_HEX_UP "0123456789ABCDEF"
#define ET_SZ_HEX_LOW "0123456789abcdef"
#define ET_SZ_MAX_LEN 0x00FFFFFF

#define ET_CH_EMPTY ''
#define ET_CH_SPACE 32
#define ET_CH_TAB 9
#define ET_CH_R 13
#define ET_CH_N 10
#define ET_CH_SLASH 47
#define ET_CH_BSLASH 92

#define ET_NOT_FOUND -1



///////////////////////////////////////////////////////////////////////////////////////

#define ET_ERR_OK 0//no error
#define ET_ERR_UNKNOWN -1//unknown error
#define ET_ERR_MISC -100//
#define ET_ERR_MEM -200//for memory
#define ET_ERR_OBJ -300//for object
#define ET_ERR_STR -400//for string
#define ET_ERR_FILE -500 //for file
#define ET_ERR_PROC -600//for process
#define ET_ERR_THREAD -700//for thread
#define ET_ERR_NET -800//for network
#define ET_ERR_PIPE -900//for pipe
#define ET_ERR_XML -1000
#define ET_ERR_HTTP -1100

#define ET_ERR_LOST -2

#define ET_ERR_MISC_TIME (ET_ERR_MISC - 1)

#define ET_ERR_MEM_ALLOC          (ET_ERR_MEM - 1)//alloc memory
#define ET_ERR_MEM_NULL          (ET_ERR_MEM - 2)//null pointer
#define ET_ERR_MEM_OVERFLOW          (ET_ERR_MEM - 3)

#define ET_ERR_OBJ_INIT          (ET_ERR_OBJ - 1)//not initialized

#define ET_ERR_STR_EMPTY			(ET_ERR_STR - 1)//empty str
#define ET_ERR_STR_FORMAT          (ET_ERR_STR - 2)//invalid format

#define ET_ERR_PROC_CREATE (ET_ERR_PROC - 1)//process create fail
#define ET_ERR_PROC_TERMINATE (ET_ERR_PROC - 2)//process destroy fail
#define ET_ERR_PROC_EDIT (ET_ERR_PROC - 3)//process edit fail
#define ET_ERR_PROC_WAIT (ET_ERR_PROC - 4)
#define ET_ERR_PROC_LIST (ET_ERR_PROC - 5)

#define ET_ERR_THREAD_CREATE (ET_ERR_THREAD - 1)//thread create fail
#define ET_ERR_THREAD_DESTROY (ET_ERR_THREAD - 2)//thread destroy fail
#define ET_ERR_THREAD_EDIT (ET_ERR_THREAD - 3)//thread edit fail

#define ET_ERR_FILE_LOST          (ET_ERR_FILE - 1)//not found
#define ET_ERR_FILE_EXIST       (ET_ERR_FILE - 2)//existed
#define ET_ERR_FILE_READ       (ET_ERR_FILE - 3)//existed
#define ET_ERR_FILE_WRITE       (ET_ERR_FILE - 4)//existed
#define ET_ERR_FILE_OPEN     (ET_ERR_FILE - 5)//open fail,can not create handle
#define ET_ERR_FILE_CLOSE     (ET_ERR_FILE - 6)//file empty or lost
#define ET_ERR_FILE_CREATE     (ET_ERR_FILE - 7)//create fail,
#define ET_ERR_FILE_DELETE     (ET_ERR_FILE - 8)//delete fail,
#define ET_ERR_FILE_NAME     (ET_ERR_FILE - 9)//invalid name,
#define ET_ERR_FILE_MOVE     (ET_ERR_FILE - 10)//move fail,
#define ET_ERR_FILE_COPY     (ET_ERR_FILE - 11)//copy fail,
#define ET_ERR_FILE_LOCK     (ET_ERR_FILE - 12)//lock fail,
#define ET_ERR_FILE_RENAME     (ET_ERR_FILE - 13)//rename fail,
#define ET_ERR_FILE_FIND     (ET_ERR_FILE - 14)//find fail, for FindFirstFile() or FindNextFile()
#define ET_ERR_FILE_POS     (ET_ERR_FILE - 15)//invalid position in file
#define ET_ERR_FILE_EMPTY     (ET_ERR_FILE - 16)//file empty or lost
#define ET_ERR_FILE_SEEK     (ET_ERR_FILE - 17)//file seek error
#define ET_ERR_FILE_SIZE     (ET_ERR_FILE - 18)//file seek error

#define ET_ERR_NET_STARTUP          (ET_ERR_NET - 1)//startup system sock fail
#define ET_ERR_NET_CLEANUP          (ET_ERR_NET - 2)//cleanup system sock fail
#define ET_ERR_NET_CREATE          (ET_ERR_NET - 3)//create socket fail
#define ET_ERR_NET_CLOSE          (ET_ERR_NET - 4)//close socket fail
#define ET_ERR_NET_SHUTDOWN          (ET_ERR_NET - 5)//shutdown socket fail
#define ET_ERR_NET_HOSTNAME          (ET_ERR_NET - 6)//invalid domain
#define ET_ERR_NET_BIND          (ET_ERR_NET - 7)//bind fail
#define ET_ERR_NET_LISTEN          (ET_ERR_NET - 8)//listen fail
#define ET_ERR_NET_ACCEPT          (ET_ERR_NET - 9)//accept fail
#define ET_ERR_NET_SEND          (ET_ERR_NET - 10)//send fail
#define ET_ERR_NET_RECV          (ET_ERR_NET - 11)//recv fail
#define ET_ERR_NET_SETOPT          (ET_ERR_NET - 12)//setopt fail
#define ET_ERR_NET_IOCTL          (ET_ERR_NET - 13)//ioctlsocket fail
#define ET_ERR_NET_CONNECT          (ET_ERR_NET - 14)//CONNECT
#define ET_ERR_NET_TIMEOUT         (ET_ERR_NET - 15)//timeout
#define ET_ERR_NET_OVERFLOW          (ET_ERR_NET - 16)//recv overflow
#define ET_ERR_NET_SELECT          (ET_ERR_NET - 17)

#define ET_ERR_PIPE_OPEN          (ET_ERR_PIPE - 1)//
#define ET_ERR_PIPE_CLOSE          (ET_ERR_PIPE - 2)//
#define ET_ERR_PIPE_READ          (ET_ERR_PIPE - 3)//
#define ET_ERR_PIPE_WRITE          (ET_ERR_PIPE - 4)//

#define ET_ERR_XML_PARSE          (ET_ERR_XML - 1)//
#define ET_ERR_XML_ROOT 			(ET_ERR_XML - 2)

#define ET_ERR_HTTP_METHOD 			(ET_ERR_HTTP - 1)

/////////////////////////////////////////////////////////////////////////////////////////////////////


//#define ET_TIME_ZONE_OK 1
//#define ET_IN_KERNEL 1
#define ET_IS_BIGENDIAN 0
#define ET_BITS 32
#define ET_ENABLE_THREAD 1

#define et_min(x,y) ( (x) < (y) ? (x) : (y) )
#define et_max(x,y) ( (x) > (y) ? (x) : (y) )
#define et_abs(x) ( (x) > 0 ? (x) : -(x) )
#define et_floor(x) (  (INT64)(x)  )
#define et_ceil(x) (  (INT64) ((x) + 1)  )
#define et_round8(x) ( ( ((x) + 7) / 8 ) * 8 )
#define et_round10(x) ( ( ((x) + 9) / 10 ) * 10 )
#define et_round16(x) ( ( ((x) + 15) / 16 ) * 16 )

#define et_word_lo(x) (  (UINT8)  ( (UINT16)(x) & 0xFF )  )
#define et_word_hi(x) (  (UINT8)  ( (UINT16)(x) >> 8 )  )

#define et_arr_size(a) (sizeof(a) / sizeof(a[0]))

#define et_char_is_ascii(x)    ( (UINT8)(x) < 128 ? 1 : 0 )
#define et_char_is_num(x)    (  ((INT8)(x) >= 0x30 ) && ((INT8)(x) <= 0x39) ? 1 : 0  )
#define et_char_is_letter(x)    (  (  (  (INT8)(x) >= 65 && (INT8)(x) >= 90  )  ||  (  (INT8)(x) >= 97 && (INT8)(x) <= 122  )  ) ? 1 : 0  )
#define et_char_is_upper(x)    (  (  (INT8)(x) >= 65 && (INT8)(x) <= 90  )  ? 1  :  0  )
#define et_char_is_lower(x)    (  (  (INT8)(x) >= 97 && (INT8)(x) <= 122  )  ? 1  :  0  )
#define et_char_upper(x)    (  (  (INT8)(x) >= 97 && (INT8)(x) <= 122  )  ?  ((x) - 32)  :  (x)  )
#define et_char_lower(x)    (  (  (INT8)(x) >= 65 && (INT8)(x) <= 90  )  ?  ((x) + 32)  :  (x)  )


#define et_ret_if_neg(iVar,xExpression) iVar = xExpression; if(iVar < 0) { return iVar; }

#define et_free_null(pBuf) if(pBuf != NULL){free(pBuf); pBuf = NULL;}

#define et_rotate_left32(x, n) (((x) << (n)) | ((x) >> (32-(n))))


///////////////////////////////////////////////////////

int et_char_to_int(char chSrc);
char et_char_by_int(int iSrc);
int et_char_hex_to_int(char chHex);
char et_char_int_to_hex(int iNum);
bool et_char_equal_no_case(char chLeft, char chRight);
bool et_char_is_space(unsigned char chSrc);

///////////////////////////////////////////////////////////////////////////

void* et_memmove(void* pDest, void* pSrc, UINT iCount);
void* et_memcpy(void* pDest, const void* pSrc, UINT iCount);
void* et_memset(void* pDest, char chValue, UINT iCount);

UINT et_strlen(const char* szSrc);
char* et_strcpy(char* szDest, const char* szSrc);
char* et_strncpy(char* szDest, const char* szSrc, UINT iNum);
char* et_strcat(char* szDest, const char* szRight);
char* et_strncat(char* szDest, const char* szRight, UINT iNum);
int et_strcmp(const char* szLeft, const char* szRight);
int et_strncmp(const char* szLeft, const char* szRight, UINT iNum);
const char* et_strchr(const char* szSrc, char chFind);

////////////////////////////////////////////////////

INT64 et_limit_between(INT64 iNum, INT64 iMin, INT64 iMax);
void* et_alloc(unsigned int iSize);
void et_free(void* pPtr);

/////////////////////////////////////////////////

char* et_url_encode(char *szDest, const char *szSrc, bool bEncodeAll);
char* et_url_decode(char *szDest, const char *szSrc);

/////////////////////////////////////////////////////////////

char* et_base64_encode(char *szDest, const char *szSrc);
char* et_base64_decode(char *szDest, const char *szSrc);

///////////////////////////////////////////////////////////////

char* et_sz_by_char(char *szDest, char chSrc);
char et_sz_to_char(const char* szSrc);
char* et_sz_by_int(char* szDest, int iNum);
int et_sz_to_int(const char* szSrc);
char* et_sz_by_long(char* szDest, INT64 iNum);
INT64 et_sz_to_long(const char* szSrc);
char* et_sz_by_float(char *szDest, double fNum, int iAfterPoint);

int et_sz_find_char(const char *szSrc, char chFind, UINT iFromPos);
int et_sz_find_not_char(const char* szSrc, char chFind, UINT iFromPos);
int et_sz_find_from(const char *szSrc, const char *szFind, UINT iFromPos);
int et_sz_find(const char *szSrc, const char *szFind);
int et_sz_find_reverse(const char* szSrc, const char* szFind);
int et_sz_find_safe(const char* szSrc, const char* szFind, UINT iLimit);
int et_sz_is_end_with(const char* szSrc, const char* szFind);

char* et_sz_repeat(char* szDest, const char* szSrc, int iCount);
char* et_sz_add_chars(char* szDest, char chAdd, UINT iAddCount);
char* et_sz_reverse(char* szSrc);
char* et_sz_lower(char* szSrc);
char* et_sz_upper(char* szSrc);
bool et_sz_equal(const char* szLeft, const char* szRight);
bool et_sz_equal_no_case(const char* szLeft, const char* szRight);

int et_sz_count_char(const char* szSrc, char chFind);
int et_sz_count_sz(const char* szSrc, const char* szFind);

char* et_sz_prefix(char* szSrc,const char* szPrefix);
char* et_sz_pad_left(char* szSrc, char chPad, int iTotalLen);
char* et_sz_pad_right(char* szSrc, char chPad, int iTotalLen);

char* et_sz_html_enc(char* szDest, const char* szSrc);
char* et_sz_html_dec(char* szDest, const char* szSrc);
/*
 * if @iSubLen==-1,then cut the string until end
 */
char* et_sz_substr(char *szDest, const char *szSrc, UINT iStart, int iSubLen);

char* et_sz_add_slashes(char* szSrc, char chFind);
char* et_sz_strip_slashes(char* szSrc);

char* et_sz_trim_left(char* szSrc);
char* et_sz_trim_right(char* szSrc);
char* et_sz_trim(char* szSrc);
char* et_sz_trim_chars_left(char* szSrc, char chFind);
char* et_sz_trim_chars_right(char* szSrc, char chFind);

char* et_sz_replace( char* szSrc, const char* szFind, const char* szReplace);
char* et_sz_cut(char* szSrc, UINT nStart, UINT nLen);
char* et_sz_insert(char* szSrc, const char* szInsert, UINT nStart);
char* et_sz_splice(char* szSrc, const char* szInsert, UINT nStart, UINT nLen);

bool et_sz_is_int(const char* szSrc);
bool et_sz_is_hex(const char* szSrc);
bool et_sz_is_num(const char* szSrc);
bool et_sz_is_domain(const char* szSrc);
bool et_sz_is_email(const char* szSrc);
bool et_sz_is_ip(const char* szSrc);
bool et_sz_is_url(const char* szSrc);
bool et_sz_is_ascii(const char* szSrc);
bool et_sz_is_limited_in( const char* szSrc, const char* szLimited );
/*yyyy-mm-dd hh:ii:ss*/
bool et_sz_is_datetime(const char* szSrc);

char* et_sz_dec2bin(char* szSrc);
char* et_sz_bin2dec(char* szSrc);
char* et_sz_hex2dec(char* szSrc);
char* et_sz_dec2hex(char* szSrc);
char* et_sz_bin2hex(char* szSrc);
char* et_sz_hex2bin(char* szSrc);
char* et_sz_format_num( char* szSrc, UINT nLenAfterPoint);

void et_sz_alloc_array(char ***szArr, int iRow, int iCol);
void et_sz_free_array(char ***szArr, int iRow);

INT64 et_sz_ip_text2num(const char* szIpText);
char* et_sz_ip_num2text(char *szDest, INT64 iIpNum);

char* et_sz_file_dir(char* szDest, const char* szSrc);
char* et_sz_file_name(char* szDest, const char* szSrc);
char* et_sz_file_end(char* szDest, const char* szSrc);
char* et_sz_file_title(char* szDest, const char* szSrc);
char* et_sz_file_ext(char* szDest, const char* szSrc);
char* et_sz_file_fix_dir(char* szDest, const char* szSrc);

char* et_sz_url_protocol(char *szDest, const char* szSrc);
char* et_sz_url_host(char *szDest, const char* szSrc);
USHORT et_sz_url_port(const char* szSrc);
char* et_sz_url_path(char *szDest, const char* szSrc);
char* et_sz_url_param(char *szDest, const char* szSrc);
char* et_sz_url_path_with_param(char *szDest, const char* szSrc);

char* et_sz_hex_increase(char *szSrc);
int et_sz_hex_to_bytes(BYTE* pByte, const char* szStr);
char* et_sz_hex_from_bytes(char* szHex, const BYTE* pBytes, int iByteCount);
char* et_sz_delimit(char* szSrc, int iUnitLen, char chDeimiter);

char* et_sz_fix_time(char* szDest, const char* szSrc);


///////////////////////////////////////////////

char* et_md5_encode32(char *szDest, const char *szSrc);

////////////////////////////////////


INT64 et_rand_between(INT64 iBegin, INT64 iEnd);

/////////////////////////////////

INT64 et_math_power(INT64 iBase, INT64 iPow);
INT64 et_math_round(FLOAT64 fSrc);

/////////////////////////////////////

typedef struct EtListsNode{
	int cb;
	struct EtListsNode* prev;
	struct EtListsNode* next;
	char* value;
}EtListsNode;

typedef struct EtLists{
	int cb;
	struct EtListsNode* head;
	struct EtListsNode* foot;
	struct EtListsNode* iterator;
	int size;
	
}EtLists;

typedef int (*et_lists_walk_fn)(char** pValue);

int et_lists_node_create(EtListsNode** ppNode, const char* szData);
int et_lists_node_destroy(EtListsNode** ppNode);

int et_lists_create(EtLists** ppList);
int et_lists_destroy(EtLists** ppList);

int et_lists_head(const EtLists* pList, char* pszData);
int et_lists_head_len(const EtLists* pList);
int et_lists_foot(const EtLists* pList, char* pszData);
int et_lists_foot_len(const EtLists* pList);
int et_lists_size(const EtLists* pList);

int et_lists_iterator_start(EtLists* pList);
bool et_lists_iterator_check(EtLists* pList);
char* et_lists_iterator_value(EtLists* pList);
int et_lists_iterator_move(EtLists* pList);

int et_lists_copy(EtLists* pListDest, const EtLists* pListSrc);
int et_lists_clear(EtLists* pList);

int et_lists_add(EtLists* pList, const char* szData);
int et_lists_remove(EtLists* pList);

bool et_lists_contains(const EtLists* pList, const char* szData);
bool et_lists_count(const EtLists* pList, const char* szData);

int et_lists_get(const EtLists* pList, int iIndex, char* szData);
int et_lists_get_len(const EtLists* pList, int iIndex);
int et_lists_set(EtLists* pList, int iIndex, const char* szData);
int et_lists_insert(EtLists* pList, int iIndex, const char* szData);
int et_lists_remove_at(EtLists* pList, int iIndex);
int et_lists_walk(EtLists* pList, et_lists_walk_fn fnWalk);
int et_lists_sort(EtLists* pList, et_fn_cmp_sz fnCmp, bool isDesc);

////////////////////////////////////////////

typedef struct EtListiNode{
	int cb;
	struct EtListiNode* prev;
	struct EtListiNode* next;
	INT64 value;
}EtListiNode;

typedef struct EtListi{
	int cb;
	struct EtListiNode* head;
	struct EtListiNode* foot;
	struct EtListiNode* iterator;
	int size;
}EtListi;

typedef int (*et_listi_walk_fn)(INT64* pValue);

int et_listi_node_create(EtListiNode** ppNode, INT64 iValue);
int et_listi_node_destroy(EtListiNode** ppNode);

int et_listi_create(EtListi** ppList);
int et_listi_destroy(EtListi** ppList);

int et_listi_head(const EtListi* pList, INT64* pData);
int et_listi_foot(const EtListi* pList, INT64* pData);
int et_listi_size(const EtListi* pList);

int et_listi_iterator_start(EtListi* pList);
bool et_listi_iterator_check(EtListi* pList);
INT64 et_listi_iterator_value(EtListi* pList);
int et_listi_iterator_move(EtListi* pList);

int et_listi_copy(EtListi* pListDest, const EtListi* pListSrc);
int et_listi_clear(EtListi* pList);

int et_listi_add(EtListi* pList, INT64 iValue);
int et_listi_remove(EtListi* pList);

bool et_listi_contains(const EtListi* pList, INT64 iValue);
bool et_listi_count(const EtListi* pList, INT64 iValue);

int et_listi_get(const EtListi* pList, int iIndex, INT64* pData);
int et_listi_set(EtListi* pList, int iIndex, INT64 iValue);
int et_listi_insert(EtListi* pList, int iIndex, INT64 iValue);
int et_listi_remove_at(EtListi* pList, int iIndex);
int et_listi_walk(EtListi* pList, et_listi_walk_fn fnWalk);
int et_listi_sort(EtListi* pList, et_fn_cmp_int fnCmp, bool isDesc);

////////////////////////////////////////////

typedef struct EtListvNode{
	int cb;
	struct EtListvNode* prev;
	struct EtListvNode* next;
	void* value;
}EtListvNode;

typedef struct EtListv{
	int cb;
	struct EtListvNode* head;
	struct EtListvNode* foot;
	struct EtListvNode* iterator;
	int size;
}EtListv;

typedef int (*et_listv_walk_fn)(void** pValue);

int et_listv_node_create(EtListvNode** ppNode, void* pData);
int et_listv_node_destroy(EtListvNode** ppNode);

int et_listv_create(EtListv** ppList);
int et_listv_destroy(EtListv** ppList);

int et_listv_head(const EtListv* pList, void** pData);
int et_listv_foot(const EtListv* pList, void** pData);
int et_listv_size(const EtListv* pList);

int et_listv_iterator_start(EtListv* pList);
bool et_listv_iterator_check(EtListv* pList);
void* et_listv_iterator_value(EtListv* pList);
int et_listv_iterator_move(EtListv* pList);

int et_listv_copy(EtListv* pListDest, const EtListv* pListSrc);
int et_listv_clear(EtListv* pList);

int et_listv_add(EtListv* pList, void* pData);
int et_listv_remove(EtListv* pList);

bool et_listv_contains(const EtListv* pList, void* pData);
bool et_listv_count(const EtListv* pList, void* pData);

int et_listv_get(const EtListv* pList, int iIndex, void** pData);
int et_listv_set(EtListv* pList, int iIndex, void* pData);
int et_listv_insert(EtListv* pList, int iIndex, void* pData);
int et_listv_remove_at(EtListv* pList, int iIndex);
int et_listv_walk(EtListv* pList, et_listv_walk_fn fnWalk);
int et_listv_sort(EtListv* pList, et_fn_cmp_int fnCmp, bool isDesc);

//////////////////////////////////////

typedef struct EtMapIiNode{
	int cb;
	struct EtMapIiNode* prev;
	struct EtMapIiNode* next;
	INT64 key;
	INT64 value;
}EtMapIiNode;

typedef struct EtMapIi{
	int cb;
	struct EtMapIiNode* head;
	struct EtMapIiNode* foot;
	struct EtMapIiNode* iterator;
	int size;
}EtMapIi;

typedef int (*et_map_ii_walk_fn)(INT64* pKey, INT64* pValue);

int et_map_ii_node_create(EtMapIiNode** ppNode, INT64 iKey, INT64 iValue);
int et_map_ii_node_destroy(EtMapIiNode** ppNode);

int et_map_ii_create(EtMapIi** ppMap);
int et_map_ii_destroy(EtMapIi** ppMap);

int et_map_ii_size(const EtMapIi* pMap);
int et_map_ii_copy(EtMapIi* pMapDest, const EtMapIi* pMapSrc);
int et_map_ii_clear(EtMapIi* pMap);

int et_map_ii_iterator_start(EtMapIi* pMap);
bool et_map_ii_iterator_check(EtMapIi* pMap);
INT64 et_map_ii_iterator_key(EtMapIi* pMap);
INT64 et_map_ii_iterator_value(EtMapIi* pMap);
int et_map_ii_iterator_move(EtMapIi* pMap);

int et_map_ii_add(EtMapIi* pMap, INT64 iKey, INT64 iValue);
int et_map_ii_remove(EtMapIi* pMap);

int et_map_ii_drop(EtMapIi* pMap, INT64 iKey);

int et_map_ii_insert(EtMapIi* pMap, int iIndex, INT64 iKey, INT64 iValue);
int et_map_ii_find_insert_pos(const EtMapIi* pMap, INT64 iKey, et_fn_cmp_int fnCmp, bool isDesc);
int et_map_ii_insert_by_order(EtMapIi* pMap, INT64 iKey, INT64 iValue, et_fn_cmp_int fnCmp, bool isDesc);
int et_map_ii_insert_asc(EtMapIi* pMap, INT64 iKey, INT64 iValue);

bool et_map_ii_contains_key(const EtMapIi* pMap, INT64 iKey);
bool et_map_ii_contains_value(const EtMapIi* pMap, INT64 iValue);
bool et_map_ii_count_value(const EtMapIi* pMap, INT64 iValue);

int et_map_ii_get(const EtMapIi* pMap, INT64 iKey, INT64* pValue);
int et_map_ii_set(EtMapIi* pMap, INT64 iKey, INT64 iValue);
int et_map_ii_walk(EtMapIi* pMap, et_map_ii_walk_fn fnWalk);
int et_map_ii_sort(EtMapIi* pMap, et_fn_cmp_int fnCmp, bool isDesc);

///////////////////////////////////////////////////////////

typedef struct EtMapSsNode{
	int cb;
	struct EtMapSsNode* prev;
	struct EtMapSsNode* next;
	char* key;
	char* value;
}EtMapSsNode;

typedef struct EtMapSs{
	int cb;
	struct EtMapSsNode* head;
	struct EtMapSsNode* foot;
	struct EtMapSsNode* iterator;
	int size;
}EtMapSs;

typedef int (*et_map_ss_walk_fn)(char** pKey, char** pValue);

int et_map_ss_node_create(EtMapSsNode** ppNode, const char* szKey, const char* szValue);
int et_map_ss_node_destroy(EtMapSsNode** ppNode);

int et_map_ss_create(EtMapSs** ppMap);
int et_map_ss_destroy(EtMapSs** ppMap);

int et_map_ss_size(const EtMapSs* pMap);
int et_map_ss_copy(EtMapSs* pMapDest, const EtMapSs* pMapSrc);
int et_map_ss_clear(EtMapSs* pMap);

int et_map_ss_iterator_start(EtMapSs* pMap);
bool et_map_ss_iterator_check(EtMapSs* pMap);
const char* et_map_ss_iterator_key(EtMapSs* pMap);
char* et_map_ss_iterator_value(EtMapSs* pMap);
int et_map_ss_iterator_move(EtMapSs* pMap);

int et_map_ss_add(EtMapSs* pMap, const char* szKey, const char* szValue);
int et_map_ss_remove(EtMapSs* pMap);

int et_map_ss_drop(EtMapSs* pMap, const char* szKey);

int et_map_ss_insert(EtMapSs* pMap, int iIndex, const char* szKey, const char* szValue);
int et_map_ss_find_insert_pos(const EtMapSs* pMap, const char* szKey, et_fn_cmp_sz fnCmp, bool isDesc);
int et_map_ss_insert_by_order(EtMapSs* pMap, const char* szKey, const char* szValue, et_fn_cmp_sz fnCmp, bool isDesc);
int et_map_ss_insert_asc(EtMapSs* pMap, const char* szKey, const char* szValue);
int et_map_ss_insert_desc(EtMapSs* pMap, const char* szKey, const char* szValue);

bool et_map_ss_contains_key(const EtMapSs* pMap, const char* szKey);
bool et_map_ss_contains_value(const EtMapSs* pMap, const char* szValue);
bool et_map_ss_count_value(const EtMapSs* pMap, const char* szValue);

int et_map_ss_get(const EtMapSs* pMap, const char* szKey, char* szValue);
int et_map_ss_get_len(const EtMapSs* pMap, const char* szKey);
int et_map_ss_set(EtMapSs* pMap, const char* szKey, const char* szValue);
int et_map_ss_walk(EtMapSs* pMap, et_map_ss_walk_fn fnWalk);
int et_map_ss_sort(EtMapSs* pMap, et_fn_cmp_sz fnCmp, bool isDesc);

//////////////////////////////////////////////////////////////////

typedef struct EtMapSiNode{
	int cb;
	struct EtMapSiNode* prev;
	struct EtMapSiNode* next;
	char* key;
	INT64 value;
}EtMapSiNode;

typedef struct EtMapSi{
	int cb;
	struct EtMapSiNode* head;
	struct EtMapSiNode* foot;
	struct EtMapSiNode* iterator;
	int size;
}EtMapSi;

typedef int (*et_map_si_walk_fn)(char** pKey, INT64* pValue);

int et_map_si_node_create(EtMapSiNode** ppNode, const char* szKey, INT64 iValue);
int et_map_si_node_destroy(EtMapSiNode** ppNode);

int et_map_si_create(EtMapSi** ppMap);
int et_map_si_destroy(EtMapSi** ppMap);

int et_map_si_size(const EtMapSi* pMap);
int et_map_si_copy(EtMapSi* pMapDest, const EtMapSi* pMapSrc);
int et_map_si_clear(EtMapSi* pMap);

int et_map_si_iterator_start(EtMapSi* pMap);
bool et_map_si_iterator_check(EtMapSi* pMap);
const char* et_map_si_iterator_key(EtMapSi* pMap);
INT64 et_map_si_iterator_value(EtMapSi* pMap);
int et_map_si_iterator_move(EtMapSi* pMap);

int et_map_si_add(EtMapSi* pMap, const char* szKey, INT64 iValue);
int et_map_si_remove(EtMapSi* pMap);

int et_map_si_drop(EtMapSi* pMap, const char* szKey);

int et_map_si_insert(EtMapSi* pMap, int iIndex, const char* szKey, INT64 iValue);
int et_map_si_find_insert_pos(const EtMapSi* pMap, const char* szKey, et_fn_cmp_sz fnCmp, bool isDesc);
int et_map_si_insert_by_order(EtMapSi* pMap, const char* szKey, INT64 iValue, et_fn_cmp_sz fnCmp, bool isDesc);
int et_map_si_insert_asc(EtMapSi* pMap, const char* szKey, INT64 iValue);
int et_map_si_insert_desc(EtMapSi* pMap, const char* szKey, INT64 iValue);

bool et_map_si_contains_key(const EtMapSi* pMap, const char* szKey);
bool et_map_si_contains_value(const EtMapSi* pMap, INT64 iValue);
bool et_map_si_count_value(const EtMapSi* pMap, INT64 iValue);

int et_map_si_get(const EtMapSi* pMap, const char* szKey, INT64 *pValue);
int et_map_si_set(EtMapSi* pMap, const char* szKey, INT64 iValue);
int et_map_si_walk(EtMapSi* pMap, et_map_si_walk_fn fnWalk);
int et_map_si_sort(EtMapSi* pMap, et_fn_cmp_sz fnCmp, bool isDesc);

/////////////////////////////////////////////////////////////////////////

typedef struct EtMapSvNode{
	int cb;
	struct EtMapSvNode* prev;
	struct EtMapSvNode* next;
	char* key;
	void* value;
}EtMapSvNode;

typedef struct EtMapSv{
	int cb;
	struct EtMapSvNode* head;
	struct EtMapSvNode* foot;
	struct EtMapSvNode* iterator;
	int size;
}EtMapSv;

typedef int (*et_map_sv_walk_fn)(char** pKey, void** pValue);

int et_map_sv_node_create(EtMapSvNode** ppNode, const char* szKey, void* pValue);
int et_map_sv_node_destroy(EtMapSvNode** ppNode);

int et_map_sv_create(EtMapSv** ppMap);
int et_map_sv_destroy(EtMapSv** ppMap);

int et_map_sv_size(const EtMapSv* pMap);
int et_map_sv_copy(EtMapSv* pMapDest, const EtMapSv* pMapSrc);
int et_map_sv_clear(EtMapSv* pMap);

int et_map_sv_iterator_start(EtMapSv* pMap);
bool et_map_sv_iterator_check(EtMapSv* pMap);
const char* et_map_sv_iterator_key(EtMapSv* pMap);
void* et_map_sv_iterator_value(EtMapSv* pMap);
int et_map_sv_iterator_move(EtMapSv* pMap);

int et_map_sv_add(EtMapSv* pMap, const char* szKey, void* pValue);
int et_map_sv_remove(EtMapSv* pMap);

int et_map_sv_drop(EtMapSv* pMap, const char* szKey);

int et_map_sv_insert(EtMapSv* pMap, int iIndex, const char* szKey, void* pValue);
int et_map_sv_find_insert_pos(const EtMapSv* pMap, const char* szKey, et_fn_cmp_sz fnCmp, bool isDesc);
int et_map_sv_insert_by_order(EtMapSv* pMap, const char* szKey, void* pValue, et_fn_cmp_sz fnCmp, bool isDesc);
int et_map_sv_insert_asc(EtMapSv* pMap, const char* szKey, void* pValue);
int et_map_sv_insert_desc(EtMapSv* pMap, const char* szKey, void* pValue);

bool et_map_sv_contains_key(const EtMapSv* pMap, const char* szKey);
bool et_map_sv_contains_value(const EtMapSv* pMap, void* pValue);
bool et_map_sv_count_value(const EtMapSv* pMap, void* pValue);

int et_map_sv_get(const EtMapSv* pMap, const char* szKey, void** pValue);
int et_map_sv_set(EtMapSv* pMap, const char* szKey, void* pValue);
int et_map_sv_walk(EtMapSv* pMap, et_map_sv_walk_fn fnWalk);
int et_map_sv_sort(EtMapSv* pMap, et_fn_cmp_sz fnCmp, bool isDesc);

/////////////////////////////////////////////////////////////////////////

int et_sz_split(EtLists* pList, const char* szSrc, const char* szDelimiter);
char* et_sz_join(EtLists* pList, char* szDest, const char* szDelimiter);
/////////////////////////////////////////////////////////////////////
#define ET_TIME_FORMAT_DEFAULT "y-m-d h:i:s l w"

typedef struct EtTime{
	int cb;
	int year;
	int month;
	int date;
	int hour;
	int minute;
	int second;
	int day;
	int millisec;
	int zone_offset;
	INT64 stamp10;
}EtTime;

typedef struct EtTimeLen{
	int days;
	int hours;
	int minutes;
	int seconds;
}EtTimeLen;

int et_time_create(EtTime** ppTime);
int et_time_destroy(EtTime** ppTime);

int et_time_refresh(EtTime* pTime);
int et_time_now(EtTime* pTime);

int et_time_get_year(const EtTime* pTime);
int et_time_get_month(const EtTime* pTime);
int et_time_get_date(const EtTime* pTime);
int et_time_get_hour(const EtTime* pTime);
int et_time_get_minute(const EtTime* pTime);
int et_time_get_second(const EtTime* pTime);
int et_time_get_day(const EtTime* pTime);
INT64 et_time_get_stamp10(const EtTime* pTime);
INT64 et_time_get_stamp13(const EtTime* pTime);
int et_time_get_millisec(const EtTime* pTime);
int et_time_get_zone_offset(const EtTime* pTime);

int et_time_set_year(EtTime* pTime, int iYear);
int et_time_set_month(EtTime* pTime, int iMonth);
int et_time_set_date(EtTime* pTime, int iDate);
int et_time_set_hour(EtTime* pTime, int iHour);
int et_time_set_minute(EtTime* pTime, int iMinute);
int et_time_set_second(EtTime* pTime, int iSecond);
int et_time_set_day(EtTime* pTime, int iDay);
int et_time_set_stamp10(EtTime* pTime, INT64 iStamp10);
int et_time_set_stamp13(EtTime* pTime, INT64 iStamp13);
int et_time_set_millisec(EtTime* pTime, int iMillisec);
int et_time_set_zone_offset(EtTime* pTime, int iZoneOffset);

int et_time_add_day(EtTime* pTime, int iDay);
int et_time_add_hour(EtTime* pTime, int iHour);
int et_time_add_minute(EtTime* pTime, int iMinute);
int et_time_add_second(EtTime* pTime, int iSecond);
int et_time_add_millisec(EtTime* pTime, INT64 iMillisec);

int et_time_to_sz(const EtTime* pTime, char* szResult);
int et_time_make_time(EtTime* pTime);
int et_time_make_day(EtTime* pTime);

bool et_time_is_unix_start(const EtTime* pTime);
int et_time_stamp10_to_object(EtTime* pTime, INT64 iStamp10);
INT64 et_time_object_to_stamp10(const EtTime* pTime);
int et_time_stamp13_to_object(EtTime* pTime, INT64 iStamp13);
INT64 et_time_object_to_stamp13(const EtTime* pTime);
int et_time_count_days_by_month(int iYear, int iMonth);
int et_time_count_days_by_year(int iYear);
int et_time_format(const EtTime* pTime, char* szDest, const char* szFormat);

char* et_time_current_format(char* szDest, const char* szFormat);
int et_time_current_year();
int et_time_current_month();
int et_time_current_date();
int et_time_current_hour();
int et_time_current_minute();
int et_time_current_second();
int et_time_current_day();
int et_time_current_millisec();
INT64 et_time_current_stamp10();
INT64 et_time_current_stamp13();
char* et_time_current_sz(char* szDest);

INT64 et_time_stamp13_to_10(INT64 iStamp13);
INT64 et_time_stamp10_to_13(INT64 iStamp10);

int et_time_zone_offset();

bool et_time_is_leap_year(UINT iYear);

int et_time_sec_to_len(EtTimeLen* pLen, int iTotalSeconds);
int et_time_len_to_sec(const EtTimeLen* pLen);
int et_time_sz_to_object(EtTime* pTime, const char* szSrc);
INT64 et_time_sz_to_stamp13(const char* szSrc);
int et_time_stamp13_to_sz(char* szResult, INT64 iStamp13);
INT64 et_time_sz_to_stamp10(const char* szSrc);
int et_time_stamp10_to_sz(char* szResult, INT64 iStamp10);

int et_time_now_by_platform(EtTime* pTime);

///////////////////////////////////////////////////////////////////////////////

typedef int (*et_proc_exec_pipe_fn)(const char* bufRead, int iReadSize, char* bufWrite, void* pParam);

int et_proc_exec_new(const char* szPath, char* argv[], char* envp[]);
int et_proc_exec_wait(const char* szPath, char* argv[], char* envp[]);
int et_proc_exec_read(const char* szCommand, char *bufResult, int iResultLimit);
int et_proc_exec_pipe(const char* szPath, et_proc_exec_pipe_fn pFn, void* pParam);
int et_proc_pid();
void et_proc_exit(int iCode);
int et_proc_kill(int iProcId);
int et_proc_wait_child(int iChildProcId);

//////////////////////////////////////////////////////////////

#ifdef ET_ENABLE_THREAD

typedef void* (*et_thread_start_fn)(void* pParam);

int et_thread_start(et_thread_start_fn pFnStart, void* pParam);
int et_thread_join(int iThreadId);

#endif

///////////////////////////////////////////////////////////////////

#define ET_FILE_BOM_UTF8 {0xEF,0xBB,0xBF}
#define ET_FILE_BOM_UNICODE {0xFF,0xFE}
#define ET_FILE_BUF_SIZE 4096

bool et_file_exist(const char* szPath);
bool et_file_is_file(const char* szPath);
bool et_file_is_dir(const char* szPath);
INT64 et_file_size(const char* szPath);
int et_file_set_size(const char* szFileName, INT64 iFileSize);

int et_file_make_single_dir(const char* szPath);
int et_file_make_long_dir(const char* szPath);
int et_file_check_dir(const char* szPath);

int et_file_write_buf(const char* szFileName, const char* bufWrite, int iBufSize);
int et_file_read_buf(const char* szFileName, char * bufRead, int iReadLimit);
int et_file_append_buf(const char* szFileName, const char * bufWrite, int iBufSize);
int et_file_read_buf_by_pos(const char* szFileName, char * bufRead, UINT iPos, UINT iSize);
int et_file_write_buf_by_pos(const char* szFileName, const char * bufWrite, UINT iPos, UINT iSize);

int et_file_get_bom_utf8(BYTE *bufBom);
int et_file_get_bom_unicode(BYTE *bufBom);
int et_file_write_bom_utf8(const char* szFileName);
int et_file_write_bom_unicode(const char* szFileName);

int et_file_read_utf8(const char* szPath, char* bufUtf8, int iReadLimit);
int et_file_write_utf8(const char* szPath, const char* bufUtf8);

int et_file_read_unicode(const char* szPath, UINT16* bufUnicode, int iReadLimit);
int et_file_write_unicode(const char* szPath, const UINT16* bufUnicode);

int et_file_delete_file(const char* szPath);
int et_file_delete_dir(const char* szPath, bool deleteMe);
int et_file_delete(const char* szPath);

int et_file_rename(const char* szPathOld, const char* szPathNew);
int et_file_create_empty(const char* szPath);

int et_file_move_file(const char* szPathOld, const char* szPathNew);
int et_file_copy_file(const char* szPathOld, const char* szPathNew);

int et_file_move_dir(const char* szPathOld, const char* szPathNew);
int et_file_copy_dir(const char* szPathOld, const char* szPathNew);

int et_file_list_child(const char* szDir, EtLists* pList);
int et_file_list_all(const char* szDir, EtLists* pList);

///////////////////////////////////////////////////////

typedef struct EtIni{
	int cb;
	EtMapSv* valueTable;
	char fileName[1024];
}EtIni;

int et_ini_create(EtIni **ppIni);
int et_ini_destroy(EtIni **ppIni);
int et_ini_load_file(EtIni *pIni, const char* szFileName);
int et_ini_load_text(EtIni *pIni, const char* szText);
int et_ini_to_text(EtIni *pIni, char* szText);
int et_ini_save_as(EtIni *pIni, const char* szFileName);
int et_ini_save(EtIni *pIni);
int et_ini_clear(EtIni *pIni);
int et_ini_set(EtIni *pIni, const char* szSection, const char* szKey, const char* szValue);
int et_ini_get(EtIni *pIni, const char* szSection, const char* szKey, char* szValue);
int et_ini_fast_set(const char* szFileName, const char* szSection, const char* szKey, const char* szValue);
int et_ini_fast_get(const char* szFileName, const char* szSection, const char* szKey, char* szValue);

///////////////////////////////////////////////////////////////////////////////////////////////////////////

int et_wcslen(const UINT16* wszSrc);

///////////////////////////////////////////////////////////////////////////////////////////////////////////

int et_charset_unicode_to_utf8(char* szUtf8, const UINT16* wszUnicode);
int et_charset_utf8_to_unicode(UINT16* wszUnicode, const char* szUtf8);
int et_charset_unicode_to_utf8_size( const UINT16* wszUnicode );
int et_charset_utf8_to_unicode_size( const char* szUtf8 );

///////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum EtXmlNodeType{
	ET_XML_NODE_UNKNOWN = 0
	,ET_XML_NODE_ELEMENT
	,ET_XML_NODE_TEXT
	,ET_XML_NODE_COMMENT
	,ET_XML_NODE_CDATA
	,ET_XML_NODE_HEAD
	,ET_XML_NODE_BLANK
}EtXmlNodeType;

typedef enum EtXmlPosType{
	ET_XML_POS_UNKNOWN = 0
	,ET_XML_POS_LT
	,ET_XML_POS_PUNC
	
	,ET_XML_POS_CDATA // <![CDATA[ xxx ]]>
	,ET_XML_POS_COMMENT // <!---->
	,ET_XML_POS_END

	,ET_XML_POS_HEAD_BEGIN
	,ET_XML_POS_HEAD_SPACE
	,ET_XML_POS_HEAD_KEY
	,ET_XML_POS_HEAD_EQUAL
	,ET_XML_POS_HEAD_QUOTE
	,ET_XML_POS_HEAD_VALUE
	,ET_XML_POS_HEAD_CLOSE

	,ET_XML_POS_BEGIN_NAME
	,ET_XML_POS_BEGIN_SPACE
	,ET_XML_POS_BEGIN_KEY
	,ET_XML_POS_BEGIN_EQUAL
	,ET_XML_POS_BEGIN_QUOTE
	,ET_XML_POS_BEGIN_VALUE
	,ET_XML_POS_BEGIN_CLOSE

}EtXmlPosType;

typedef struct EtXmlNode{
	int cb;
	bool hasEnd;
	bool hasTag;
	EtXmlNodeType nodeType;
	char name[64];
	char* text;
	EtMapSs* attrList;
	struct EtXmlNode* parentNode;
	struct EtXmlNode* firstChild;
	struct EtXmlNode* prevSibling;
	struct EtXmlNode* nextSibling;
}EtXmlNode;

typedef struct EtXmlDoc{
	int cb;
	bool standalone;
	char fileName[1024];
	char encoding[16];
	char version[8];
	EtXmlNode* rootNode;
}EtXmlDoc;

typedef struct EtXmlStatus{
	int cb;
	EtXmlNodeType nodeType;
	EtXmlPosType posType;
	bool isHead;
	bool isBegin;
	bool hasStart;
	bool hasHead;
	bool useSingleQuote;
	char tempText[64];
	EtXmlNode* parentNode;

}EtXmlStatus;

int et_xml_node_create(EtXmlNode** ppNode);
int et_xml_node_destroy(EtXmlNode** ppNode);
int et_xml_node_clear(EtXmlNode* pNode);

int et_xml_doc_create(EtXmlDoc** ppDoc);
int et_xml_doc_destroy(EtXmlDoc** ppDoc);

int et_xml_doc_clear(EtXmlDoc* pDoc);
int et_xml_doc_load_text(EtXmlDoc* pDoc, const char* szContent);
int et_xml_node_to_text(EtXmlNode* pNode, char* szContent);
int et_xml_text_to_node(EtXmlNode* pNode, const char* szContent);
int et_xml_doc_to_text(EtXmlDoc* pDoc, const char* szContent);
int et_xml_doc_load_file(EtXmlDoc* pDoc, const char* szFileName);
int et_xml_doc_save_as(EtXmlDoc* pDoc, const char* szFileName);
int et_xml_doc_save(EtXmlDoc* pDoc);

int et_xml_node_insert_before(EtXmlNode* pRefNode, EtXmlNode* pNewNode);
int et_xml_node_insert_after(EtXmlNode* pRefNode, EtXmlNode* pNewNode);
int et_xml_node_append_head(EtXmlNode* pParentNode, EtXmlNode* pChildNode);
int et_xml_node_append_foot(EtXmlNode* pParentNode, EtXmlNode* pChildNode);
EtXmlNode* et_xml_node_last_child(EtXmlNode* pParentNode);
int et_xml_node_remove(EtXmlNode* pNode);

int et_xml_node_set_attr(EtXmlNode* pNode, const char* szKey, const char* szValue);
int et_xml_node_get_attr(EtXmlNode* pNode, const char* szKey, char* szValue);

int et_xml_node_set_text(EtXmlNode* pNode, const char* szText);
int et_xml_node_get_text(EtXmlNode* pNode, char* szText);

#ifdef __cplusplus
}
#endif