#ifndef __ANY_TEST_H_
#define __ANY_TEST_H_
int any_test(void* argv);
struct any_msg {
	int result;
	int x;
	int y;
	int w;
	int h;
	char *ssid;
};

#endif //__ANY_TEST_H_