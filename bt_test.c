#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>

#include <cutils/log.h>
#include <cutils/properties.h>

#include "libbluetooth/bluetooth/bluetooth.h"
#include "libbluetooth/bluetooth/hci.h"
#include "libbluetooth/bluetooth/hci_lib.h"

#include "common.h"

#ifndef HCI_DEV_ID
#define HCI_DEV_ID 0
#endif

#define HCID_START_DELAY_SEC   3
#define HCID_STOP_DELAY_USEC 500000

#define LOG(x...) printf(x)

#define MIN(x,y) (((x)<(y))?(x):(y))


static int rfkill_id = -1;
static char *rfkill_state_path = NULL;

static int init_rfkill() {
    char path[64];
    char buf[16];
    int fd;
    int sz;
    int id;
    for (id = 0; ; id++) {
        snprintf(path, sizeof(path), "/sys/class/rfkill/rfkill%d/type", id);
        fd = open(path, O_RDONLY);
        if (fd < 0) {
            LOGW("open(%s) failed: %s (%d)\n", path, strerror(errno), errno);
            return -1;
        }
        sz = read(fd, &buf, sizeof(buf));
        close(fd);
        if (sz >= 9 && memcmp(buf, "bluetooth", 9) == 0) {
            rfkill_id = id;
            break;
        }
    }

    asprintf(&rfkill_state_path, "/sys/class/rfkill/rfkill%d/state", rfkill_id);
    return 0;
}

static int check_bluetooth_power() {
    int sz;
    int fd = -1;
    int ret = -1;
    char buffer;

    if (rfkill_id == -1) {
        if (init_rfkill()) goto out;
    }

    fd = open(rfkill_state_path, O_RDONLY);
    if (fd < 0) {
        printf("open(%s) failed: %s (%d)", rfkill_state_path, strerror(errno),
             errno);
        goto out;
    }
    sz = read(fd, &buffer, 1);
    if (sz != 1) {
        printf("read(%s) failed: %s (%d)", rfkill_state_path, strerror(errno),
             errno);
        goto out;
    }

    switch (buffer) {
    case '1':
        ret = 1;
        break;
    case '0':
        ret = 0;
        break;
    }

out:
    if (fd >= 0) close(fd);
    return ret;
}

static int set_bluetooth_power(int on) {
    int sz;
    int fd = -1;
    int ret = -1;
    const char buffer = (on ? '1' : '0');

    if (rfkill_id == -1) {
        if (init_rfkill()) goto out;
    }

    fd = open(rfkill_state_path, O_WRONLY);
    if (fd < 0) {
        printf("open(%s) for write failed: %s (%d)", rfkill_state_path,
             strerror(errno), errno);
        goto out;
    }
    sz = write(fd, &buffer, 1);
    if (sz < 0) {
        printf("write(%s) failed: %s (%d)", rfkill_state_path, strerror(errno),
             errno);
        goto out;
    }
    ret = 0;

out:
    if (fd >= 0) close(fd);
    return ret;
}

static inline int create_hci_sock() {
    int sk = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
    if (sk < 0) {
        printf("bluetooth_test Failed to create bluetooth hci socket: %s (%d)",
             strerror(errno), errno);
    }
    return sk;
}

static int bt_enable() {
    printf(__FUNCTION__);

    int ret = -1;
    int hci_sock = -1;
    int attempt = 0;

    if (set_bluetooth_power(1) < 0)
	{
		goto out;
	}
    sleep(2);
#if 0
	ui_print("register hciattach servicer\n");
	ret = system("/system/bin/hciattach -n -s 1500000 /dev/ttyS2 bcm4329_usi 1500000 flow");

	ui_print("register brcm_patchram_plus servicer\n");
	ret = system("/system/bin/brcm_patchram_plus --patchram /etc/bluez/bcm432x/BCM4329B1_TestOnly_0237_26MHz_SEMCO_B23.hcd --baudrate 1500000 --enable_hci /dev/ttyS0");
#endif
#if 0
    LOGI("Starting patch_plus");
    if (property_set("ctl.start", "patch_plus") < 0) {
        printf("bluetooth_test Failed to run brcm_patchram_plus");
        goto out;
    }
#else

    LOGI("Starting hciattach daemon");
    if (property_set("ctl.start", "hciattach") < 0) {
        printf("bluetooth_test Failed to start hciattach");
        goto out;
    }
#endif


    // Try for 10 seconds, this can only succeed once hciattach has sent the
    // firmware and then turned on hci device via HCIUARTSETPROTO ioctl
    for (attempt = 1000; attempt > 0;  attempt--) {
        hci_sock = create_hci_sock();
        if (hci_sock < 0) 
		{
			goto out;
		}

        if (!ioctl(hci_sock, HCIDEVUP, HCI_DEV_ID)) {
            break;
        }
        close(hci_sock);
        usleep(10000);  // 10 ms retry delay
    }
    if (attempt == 0) {
        printf("%s: Timeout waiting for HCI device to come up\n", __FUNCTION__);
        goto out;
    }

    LOGI("Starting bluetoothd deamon");
    if (property_set("ctl.start", "bluetoothd") < 0) {
        printf("bluetooth_test Failed to start bluetoothd");
        goto out;
    }
    sleep(HCID_START_DELAY_SEC);

    ret = 0;

out:
    if (hci_sock >= 0) close(hci_sock);
    return ret;
}

static int bt_disable() {
    printf(__FUNCTION__);

    int ret = -1;
    int hci_sock = -1;

    LOGI("Stopping bluetoothd deamon");
    if (property_set("ctl.stop", "bluetoothd") < 0) {
        printf("bluetooth_test Error stopping bluetoothd");
        goto out;
    }
    usleep(HCID_STOP_DELAY_USEC);

    hci_sock = create_hci_sock();
    if (hci_sock < 0) goto out;
    ioctl(hci_sock, HCIDEVDOWN, HCI_DEV_ID);

#if 1
    LOGI("Stopping hciattach deamon");
    if (property_set("ctl.stop", "hciattach") < 0) {
        printf("bluetooth_test Error stopping hciattach");
        goto out;
    }
#else
    LOGI("Stopping patch_plus");
    if (property_set("ctl.stop", "patch_plus") < 0) {
        printf("bluetooth_test Error stopping patch_plus");
        goto out;
    }
#endif
    if (set_bluetooth_power(0) < 0) {
        goto out;
    }
    ret = 0;

out:
    if (hci_sock >= 0) close(hci_sock);
    return ret;
}

static int bt_is_enabled() {
    printf(__FUNCTION__);

    int hci_sock = -1;
    int ret = -1;
    struct hci_dev_info dev_info;


    // Check power first
    ret = check_bluetooth_power();
    if (ret == -1 || ret == 0) goto out;

    ret = -1;

    // Power is on, now check if the HCI interface is up
    hci_sock = create_hci_sock();
    if (hci_sock < 0) goto out;

    dev_info.dev_id = HCI_DEV_ID;
    if (ioctl(hci_sock, HCIGETDEVINFO, (void *)&dev_info) < 0) {
        ret = 0;
        goto out;
    }

    ret = hci_test_bit(HCI_UP, &dev_info.flags);

out:
    if (hci_sock >= 0) close(hci_sock);
    return ret;
}
static int my_ba2str(const bdaddr_t *ba, char *str) {
    return sprintf(str, "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
                ba->b[5], ba->b[4], ba->b[3], ba->b[2], ba->b[1], ba->b[0]);
}

#if 0
static int my_str2ba(const char *str, bdaddr_t *ba) {
    int i;
    for (i = 5; i >= 0; i--) {
        ba->b[i] = (uint8_t) strtoul(str, &str, 16);
        str++;
    }
    return 0;
}
#endif

void test_bt(void)
{
	struct hci_dev_list_req *dl;
	struct hci_dev_req *dr;
	struct hci_dev_info di;
	int ctl;
	int i;

	if (!(dl = malloc(HCI_MAX_DEV * sizeof(struct hci_dev_req) + sizeof(uint16_t)))) {
		perror("can't allocate memory");
		exit(1);
	}
	dl->dev_num = HCI_MAX_DEV;
	dr = dl->dev_req;

	//
	if ((ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI)) < 0) {
		perror("can't open hci socket.");
		exit(1);
	}

	//
	if (ioctl(ctl, HCIGETDEVLIST, (void *)dl) < 0) {
		perror("can't get device list.");
		exit(1);
	}
	printf("dev num :%d\n", dl->dev_num);
	for (i = 0; i < dl->dev_num; i++) {
		di.dev_id = (dr + i)->dev_id;
		ioctl(ctl, HCIGETDEVINFO, (void *)&di);
		printf("dev_id =%d name = %s\n", di.dev_id, di.name);
	}

	close(ctl);
	free(dl);
}

int bt_test(void)
{
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i, ret = 0;
    char addr[19] = { 0 };
    char name[248] = { 0 };
	//dev_id = HCI_DEV_ID;
	
	printf("bluetooth_test main function started\n");


	ui_print("\nbluetooth test...\nplease wait...\n ");

	ret = bt_enable();
	/*
	if(ret < 0)
	{
		goto to_enable_error;
	}
	*/

	printf("bluetooth_test main function enable ok \n");

	//ui_print("register hciattach servicer\n");
	//ret = system("/system/bin/hciattach -n -s 1500000 /dev/ttyS2 bcm4329_usi 1500000 flow");

	//ret = __system("/system/bin/hciattach -n -s 1500000 /dev/ttyS2 bcm4329_usi 1500000 flow");
	//printf("register hciattach servicer =%d\n",ret);

	//ui_print("register brcm_patchram_plus servicer\n");
	//ret = system("/system/bin/brcm_patchram_plus --patchram /etc/bluez/bcm432x/BCM4329B1_TestOnly_0237_26MHz_SEMCO_B23.hcd --baudrate 1500000 --enable_hci /dev/ttyS0");
	ret = __system("/system/bin/brcm_patchram_plus --patchram bychip --baudrate 1500000 --enable_lpm --enable_hci /dev/ttyS0 &");
	printf("register brcm_patchram_plus servicer =%d\n",ret);
	sleep(1);

	for(i = 0; i < 7; i++)
	{
		ret = __system("/system/bin/hciconfig hci0 up");
		sleep(1);
		dev_id = hci_get_route(NULL);
		if(dev_id >= 0)
		{
			break;
		}
		sleep(1);
	}
	printf("hciconfig hci0 up  =%d\n",ret);

	printf("bluetooth_test main function hci_get_route dev_id=%d\n",dev_id);
	//sleep(1);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
		printf("bluetooth_test main function hci_open_dev fail\n");
		if(dev_id < 0)
		{
			printf("bluetooth_test main function hci_open_dev dev id error\n");
		}

		if(sock < 0)
		{
			printf("bluetooth_test main function hci_open_dev sock error\n");
		}
        //perror("opening socket");
		//ui_print("\n\n test faild\n");
		
        goto to_open_error;
    }

	printf("bluetooth_test main function hci_open_dev ok\n");
/*
    len = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
   
    printf("bluetooth_test main function scanning nearby device ...\n");
	ui_print("bluetooth_test scanning device...\n");


    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    for (i = 0; i < num_rsp; i++) {
        my_ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
            name, 0) < 0)
        strcpy(name, "[unknown]");
		printf("bluetooth_test main function nearby devices list addr=%s name=%s\n",addr,name);
        //ui_print("bluetooth device found : addr=%s name=%s\n", addr, name);
    }
	if (num_rsp <= 0)
	{
		printf("bluetooth_test main function no nearby device\n");
		ui_print("No Nearby Equipment.\n");
	}
	printf("bluetooth_test main function scanning nearby devices over\n");

	
    free( ii );
*/
	close( sock );
	bt_disable();

	ui_print_xy_rgba(0,get_cur_print_y(),0,0,255,255,"bluetooth: [ok]\n");

	printf("bluetooth_test main function over\n");
	return 0;

	to_enable_error:
	to_open_error:
	ui_print_xy_rgba(0,get_cur_print_y(),255,0,0,255,"bluetooth: [errror]\n");
	return 0;
}
