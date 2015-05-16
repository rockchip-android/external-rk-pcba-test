#!/system/bin/sh

result_file=/data/sd_capacity
insert_sd_info=/data/sd_insert_info

if [ -e $result_file ] ; then
busybox rm -f $result_file
fi

if [ -e insert_sd_info ] ; then
busybox rm -f insert_sd_info
fi

if [ ! -b "/dev/block/mmcblk1p1" ]; then
	busybox echo "not card insert" > /data/sd_insert_info
	exit 0
fi        

#if [ ! -d "/tmp/extsd" ]; then
#    busybox mkdir -p /tmp/extsd
#fi

umount /mnt/external_sd

mmcp=$mmcblk
su root

mount -t vfat /dev/block/mmcblk1p1 /mnt/external_sd

capacity=`df | grep "/mnt/external_sd" | busybox awk '{printf $2}'`
busybox echo "$mmcp: $capacity"

busybox sleep 1

umount /mnt/external_sd
busybox echo $capacity > /data/sd_capacity
exit 1

