#!/sbin/busybox sh

module_path=/sbin/8188eu.ko
result_file=/data/scan_result.txt

echo "insmod $module_path"
insmod "$module_path"
if [ $? -ne 0 ]; then
    echo "insmod failed"
    exit 0
fi

echo "sleep 3s"
busybox sleep 3

echo "touch $result_file"
busybox touch $result_file

echo "get scan results"
while true; do
    if busybox ifconfig -a | busybox grep wlan0; then
        busybox ifconfig wlan0 up
        if [ $? -ne 0 ]; then
            echo "ifconfig wlan0 up failed"
            exit 0
        fi
        
        iwlist wlan0 scan | busybox grep SSID > $result_file
        echo "success"
        exit 1
    fi

    echo "sleep 5s"
    busybox sleep 5

    busybox ifconfig wlan0 down
    if [ $? -ne 0 ]; then
        echo "ifconfig wlan0 down failed"
        exit 0
    fi
done

