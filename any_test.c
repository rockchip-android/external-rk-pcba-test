#include "any_test.h"
#include "et_cc_linux_arm.h"
#include "common.h"


int any_test(void* argv){
	char szTemp1[255] = "";

	et_sz_repeat(szTemp1,"a",3);

	ui_print_xy_rgba(0,get_cur_print_y(),0,255,0,255,"ANY %s : [OK]\n",szTemp1);
	return 0;
}