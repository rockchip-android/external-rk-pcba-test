#!/system/bin/busybox sh

result_file=/data/scan_result.txt
result_file2=/data/scan_result2.txt
result_file3=/data/scan_result3.txt
result_file4=/data/scan_result4.txt
result_file5=/data/scan_result5.txt
result_file6=/data/scan_result6.txt
result_file7=/data/scan_result7.txt
wpa_supplicant_file=/data/misc/wifi/wpa_supplicant.conf
version_path=/proc/version
chip_broadcom=false
interface_up=true
version=.3.0.36+

jmax=3

j=0

bb=1

if [ -e $result_file ] ; then
busybox rm -f $result_file
fi

if [ -e $result_file2 ] ; then
busybox rm -f $result_file2
fi

echo "get scan results"
while [ $j -lt $jmax ]; 
do
    if busybox ifconfig wlan0; then
        if [ $interface_up = "true" ]; then
            busybox ifconfig wlan0 up
        fi

		busybox cp /etc/wifi/wpa_supplicant.conf /data/misc/wifi/wpa_supplicant.conf
		busybox chmod 0777 /data/misc/wifi/wpa_supplicant.conf
        
        start wpa_supplicant

        wpa_cli -iwlan0 scan

        echo "sleep 2s"
        busybox sleep 2
        
        wpa_cli -iwlan0 scan_results > $result_file4
        if [ $(busybox sed -n '$=' $result_file4) -gt $bb ] ; then
        	busybox tail -1 $result_file4 > $result_file3
        	busybox tail -1 $result_file4 > $result_file7
        	cat $result_file3 | busybox awk '{print $5}' > $result_file5
        	busybox awk '{printf $0; if (getline) print}' $result_file5 > $result_file
        	
        	cat $result_file7 | busybox awk '{print $3}' > $result_file6
        	busybox awk '{printf $0; if (getline) print}' $result_file6 > $result_file2
        	busybox rm -f $result_file3
        	busybox rm -f $result_file5
        	busybox rm -f $result_file6
        	busybox rm -f $result_file7
        fi
		busybox rm -f $result_file4
        echo "success"
	if [ -e $wpa_supplicant_file ] ; then
		busybox rm -f $wpa_supplicant_file
	fi
        exit 1
    fi
done

echo "wlan test failed"
if [ -e $wpa_supplicant_file ] ; then
	busybox rm -f $wpa_supplicant_file
fi
exit 0

