#!/bin/bash
echo $1 $2
TARGET_PRODUCT=$1
PRODUCT_OUT=$2

if [ -e "device/rockchip/$TARGET_PRODUCT/8188eu.ko" ] ; then
cp device/rockchip/$TARGET_PRODUCT/8188eu.ko $PRODUCT_OUT/recovery/root/res/
fi

if [ -e "device/rockchip/$TARGET_PRODUCT/8192cu.ko" ] ; then
cp device/rockchip/$TARGET_PRODUCT/8192cu.ko $PRODUCT_OUT/recovery/root/res/
fi

if [ -e "device/rockchip/$TARGET_PRODUCT/rda5890.ko" ] ; then
cp device/rockchip/$TARGET_PRODUCT/rda5890.ko $PRODUCT_OUT/recovery/root/res/
fi

if [ -e "device/rockchip/$TARGET_PRODUCT/rt5370sta.ko" ] ; then
cp device/rockchip/$TARGET_PRODUCT/rt5370sta.ko $PRODUCT_OUT/recovery/root/res/
fi

if [ -e "device/rockchip/$TARGET_PRODUCT/rkwifi.ko" ] ; then
cp device/rockchip/$TARGET_PRODUCT/rkwifi.ko $PRODUCT_OUT/recovery/root/res/
fi

if [ -e "device/rockchip/$TARGET_PRODUCT/proprietary/libipp/rk29-ipp.ko" ] ; then
cp device/rockchip/$TARGET_PRODUCT/proprietary/libipp/rk29-ipp.ko $PRODUCT_OUT/recovery/root/
fi

if [ -e "device/rockchip/$TARGET_PRODUCT/bluetooth/pcba/rk903/system/bin" ] ; then
cp device/rockchip/$TARGET_PRODUCT/bluetooth/pcba/rk903/system/bin/ $PRODUCT_OUT/recovery/root/system/ -a
fi

if [ -e "device/rockchip/$TARGET_PRODUCT/bluetooth/pcba/rk903/system/etc" ] ; then
cp device/rockchip/$TARGET_PRODUCT/bluetooth/pcba/rk903/system/etc/ $PRODUCT_OUT/recovery/root/system/ -a 
fi