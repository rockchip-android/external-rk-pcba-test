mmcblk="/dev/block/mmcblk1p1"
mmcp=$mmcblk

if [ ! -d "/data/sd_insert_info" ]; then
    busybox mkdir -p /data/sd_insert_info
fi

if [ ! -b "$mmcblk" ]; then
	busybox echo "not card insert" > /data/sd_insert_info
	exit 0
fi        

if [ ! -d "/tmp/extsd" ]; then
    busybox mkdir -p /tmp/extsd
fi

umount /tmp/extsd

mmcp=$mmcblk
su root

busybox mount -t vfat $mmcp /tmp/extsd

if [ $? -ne 0 ]; then
    mmcp=$mmcblk"p1"
    su root
    busybox mount -t vfat $mmcp /tmp/extsd
    if [ $? -ne 0 ]; then
        exit 0
        busybox sleep 3
        continue 2
    fi
fi

capacity=`df | grep "/tmp/extsd" | busybox awk '{printf $2}'`
busybox echo "$mmcp: $capacity"

umount /tmp/extsd
busybox echo $capacity > /data/sd_capacity
exit 1