#!/sbin/busybox sh

module_path_8188eu=/sbin/8188eu.ko
module_path_8192cu=/sbin/8192cu.ko
module_path_rk903=/sbin/rkwifi.ko
module_path_rt5370=/sbin/rt5370sta.ko
module_path_wlan=/sbin/wlan.ko
result_file=/data/scan_result.txt
chip_type_path=/sys/class/rkwifi/chip
module_path=$module_path_wlan

if busybox cat $chip_type_path | busybox grep RK903; then
  module_path=$module_path_rk903
fi

if busybox cat $chip_type_path | busybox grep RK901; then
  module_path=$module_path_rk903
fi

if busybox cat $chip_type_path | busybox grep BCM4330; then
  module_path=$module_path_rk903
fi

if busybox cat $chip_type_path | busybox grep RTL8188CU; then
  module_path=$module_path_8192cu
fi

if busybox cat $chip_type_path | busybox grep RTL8188EU; then
  module_path=$module_path_8188eu
fi

if busybox cat $chip_type_path | busybox grep RT5370; then
  module_path=$module_path_rt5370
fi

echo "insmod $module_path"
insmod "$module_path"
if [ $? -ne 0 ]; then
    echo "insmod failed"
    exit 0
fi

echo "touch $result_file"
busybox touch $result_file

echo "get scan results"
for i in 1 2 3 4 5 6; do

    echo "sleep 3s"
    busybox sleep 3

    if busybox ifconfig wlan0; then
        busybox ifconfig wlan0 up
        #if [ $? -ne 0 ]; then
        #    echo "ifconfig wlan0 up failed"
        #    exit 0
        #fi
        
        iwlist wlan0 scan | busybox grep SSID > $result_file
        busybox cat $result_file
        echo "success"
        exit 1
    fi

    #if [ $i -eq 2 -o $i -eq 4 ]; then
        echo "rmmod wlan"
        rmmod wlan
        busybox sleep 1
        echo "insmod $module_path"
        insmod "$module_path"
    #fi
done

echo "wlan test failed"
exit 0

