#!/bin/bash
echo $1 $2
TARGET_PRODUCT=$1
PRODUCT_OUT=$2
cp device/rockchip/$TARGET_PRODUCT/8188eu.ko $PRODUCT_OUT/recovery/root/res/
cp device/rockchip/$TARGET_PRODUCT/8192cu.ko $PRODUCT_OUT/recovery/root/res/
cp device/rockchip/$TARGET_PRODUCT/rda5890.ko $PRODUCT_OUT/recovery/root/res/
cp device/rockchip/$TARGET_PRODUCT/rt5370sta.ko $PRODUCT_OUT/recovery/root/res/
cp device/rockchip/$TARGET_PRODUCT/rkwifi.ko $PRODUCT_OUT/recovery/root/res/
cp device/rockchip/$TARGET_PRODUCT/proprietary/libipp/rk29-ipp.ko $PRODUCT_OUT/recovery/root/
