#!/sbin/busybox sh

module_path1=/sbin/8188eu.ko
module_path2=/sbin/8192cu.ko
module_path3=/sbin/wlan.ko
result_file=/data/scan_result.txt

echo "insmod $module_path1"
insmod "$module_path1"
if [ $? -ne 0 ]; then
    echo "insmod failed"
    exit 0
fi

echo "insmod $module_path2"
insmod "$module_path2"
if [ $? -ne 0 ]; then
    echo "insmod failed"
    exit 0
fi

echo "insmod $module_path3"
insmod "$module_path3"
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
        if [ $? -ne 0 ]; then
            echo "ifconfig wlan0 up failed"
            exit 0
        fi
        
        iwlist wlan0 scan | busybox grep SSID > $result_file
        cat $result_file
        echo "success"
        exit 1
    fi

    #if [ $i -eq 2 -o $i -eq 4 ]; then
        echo "rmmod wlan"
        rmmod wlan
        busybox sleep 1
        echo "insmod $module_path3"
        insmod "$module_path3"
    #fi
done

echo "wlan test failed"
exit 0
