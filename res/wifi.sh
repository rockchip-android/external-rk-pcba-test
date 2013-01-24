#!/sbin/busybox sh

module_path_8188eu=/res/8188eu.ko
module_path_8192cu=/res/8192cu.ko
module_path_rk903=/res/rkwifi.ko
module_path_rt5370=/res/rt5370sta.ko
module_path_wlan=/res/wlan.ko
result_file=/data/scan_result.txt
result_file2=/data/scan_result2.txt
chip_type_path=/sys/class/rkwifi/chip
pcba_node=/sys/class/rkwifi/pcba
module_path=$module_path_wlan
chip_broadcom=false

jmax=1

if busybox cat $chip_type_path | busybox grep RK903; then
  module_path=$module_path_rk903
  chip_broadcom=true
  echo 1 > $pcba_node
fi

if busybox cat $chip_type_path | busybox grep RK901; then
  module_path=$module_path_rk903
  chip_broadcom=true
  echo 1 > $pcba_node
fi

if busybox cat $chip_type_path | busybox grep BCM4330; then
  module_path=$module_path_rk903
  chip_broadcom=true
  echo 1 > $pcba_node
fi

if busybox cat $chip_type_path | busybox grep RTL8188CU; then
  jmax=6
  module_path=$module_path_8192cu
fi

if busybox cat $chip_type_path | busybox grep RTL8188EU; then
  jmax=6
  module_path=$module_path_8188eu
fi

if busybox cat $chip_type_path | busybox grep RT5370; then
  jmax=6
  module_path=$module_path_rt5370
fi

echo "touch $result_file"
busybox touch $result_file

j=0

echo "get scan results"
while [ $j -lt $jmax ]; 
do
    echo "insmod $module_path"
    insmod "$module_path"
    if [ $? -ne 0 ]; then
        echo "insmod failed"
        exit 0
    fi

    echo "sleep 3s"
    busybox sleep 3

    if busybox ifconfig wlan0; then
        busybox ifconfig wlan0 up
        #if [ $? -ne 0 ]; then
        #    echo "ifconfig wlan0 up failed"
        #    exit 0
        #fi
    
        iwlist wlan0 scanning
        if [ $chip_broadcom = "true" ]; then
            echo "sleep 3s"
            busybox sleep 3    
        fi
        iwlist wlan0 scanning last | busybox grep SSID > $result_file
        busybox cat $result_file
        iwlist wlan0 scanning last | busybox grep "Signal level" > $result_file2
        busybox cat $result_file2
        echo "success"
        exit 1
    fi

    echo "rmmod wlan"
    rmmod wlan
    busybox sleep 1
    
    j=$((j+1))
done

echo "wlan test failed"
exit 0

