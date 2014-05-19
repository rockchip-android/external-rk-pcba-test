#!/bin/bash
echo res.sh $1 $2 $3
TARGET_PRODUCT=$1
PRODUCT_OUT=$2
TARGET_BOARD_PLATFORM=$3
TARGET_COMMON=common
PCBA_PATH=external/rk-pcba-test
BT_BLUEDROID=true
if [ $TARGET_BOARD_PLATFORM = "rk2928" ]; then
    MODULE="modules"
else
    MODULE="modules_smp"
fi
echo MODULE $MODULE
if [ ! -e "device/rockchip/$TARGET_COMMON/app" ] ; then
    if [ -e "device/rockchip/$TARGET_PRODUCT/8188eu.ko" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/8188eu.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/8188eu.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/8188eu.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/8192cu.ko" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/8192cu.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/8192cu.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/8192cu.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/rda5890.ko" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/rda5890.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/rda5890.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/rda5890.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/rt5370sta.ko" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/rt5370sta.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/rt5370sta.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/rt5370sta.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/mt7601sta.ko" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/mt7601sta.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/mtprealloc7601Usta.ko" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/mtprealloc7601Usta.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/8723au.ko" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/8723au.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/8723as.ko" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/8723as.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/rkwifi.ko" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/rkwifi.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/rkwifi.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/rkwifi.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/wlan.ko" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/wlan.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/wlan.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/wlan.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

	if [ ! -e "$PRODUCT_OUT/recovery/root/system/lib/" ] ; then
	mkdir $PRODUCT_OUT/recovery/root/system/lib/
	mkdir $PRODUCT_OUT/recovery/root/system/lib/modules/
	fi
	
	if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/esp8089.ko" ] ; then
	cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/esp8089.ko $PRODUCT_OUT/recovery/root/system/lib/modules/
	fi


	if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/init_data.conf" ] ; then
	cp device/rockchip/$TARGET_COMMON/wifi/lib/init_data.conf  $PRODUCT_OUT/recovery/root/system/lib/modules/
	fi
	
	if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/esp_init_data.bin" ] ; then
	cp device/rockchip/$TARGET_COMMON/wifi/lib/esp_init_data.bin  $PRODUCT_OUT/recovery/root/system/lib/modules/
	fi

    if [ ! -e "$PRODUCT_OUT/recovery/root/system/etc/" ] ; then
    mkdir $PRODUCT_OUT/recovery/root/system/etc/
    fi

    if [ ! -e "$PRODUCT_OUT/recovery/root/system/etc/firmware" ] ; then
    mkdir $PRODUCT_OUT/recovery/root/system/etc/firmware/
    fi

    if [ ! -e "$PRODUCT_OUT/recovery/root/etc/firmware" ] ; then
    mkdir $PRODUCT_OUT/recovery/root/etc/firmware/
    fi

    if [ -e "external/wlan_loader/firmware/" ] ; then
    cp external/wlan_loader/firmware/ $PRODUCT_OUT/recovery/root/system/etc/ -a
    fi
	
    if [ -e "external/wlan_loader/firmware/WIFI_RAM_CODE" ] ; then
    cp external/wlan_loader/firmware/WIFI_RAM_CODE $PRODUCT_OUT/recovery/root/etc/firmware/WIFI_RAM_CODE
    fi 

    if [ -e "device/rockchip/$TARGET_PRODUCT/proprietary/libipp/rk29-ipp.ko" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/proprietary/libipp/rk29-ipp.ko $PRODUCT_OUT/recovery/root/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/proprietary/libipp/rk29-ipp.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/proprietary/libipp/rk29-ipp.ko.3.0.36+ $PRODUCT_OUT/recovery/root/
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/bluetooth/pcba/rk903/system/bin" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/bluetooth/pcba/rk903/system/bin/ $PRODUCT_OUT/recovery/root/system/ -a
    fi

    if [ -e "device/rockchip/$TARGET_PRODUCT/bluetooth/pcba/rk903/system/etc" ] ; then
    cp device/rockchip/$TARGET_PRODUCT/bluetooth/pcba/rk903/system/etc/ $PRODUCT_OUT/recovery/root/system/ -a
    fi

else
    ##################################### WIFI MODULES #####################################################
    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8188eu.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8188eu.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8188eu.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8188eu.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8192cu.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8192cu.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8192cu.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8192cu.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rda5890.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rda5890.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rda5890.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rda5890.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rt5370sta.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rt5370sta.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rt5370sta.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rt5370sta.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/mt7601sta.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/mt7601sta.ko $PRODUCT_OUT/recovery/root/res/
    fi

		if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/mtprealloc7601Usta.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/mtprealloc7601Usta.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8723au.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8723au.ko $PRODUCT_OUT/recovery/root/res/
	  fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8723as.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/8723as.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/mt5931.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/mt5931.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rkwifi.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rkwifi.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rkwifi.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/rkwifi.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/bluetooth/lib/modules_smp/rtk_btusb.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/bluetooth/lib/modules_smp/rtk_btusb.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/wlan.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/wlan.ko $PRODUCT_OUT/recovery/root/res/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/wlan.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/wlan.ko.3.0.36+ $PRODUCT_OUT/recovery/root/res/
    fi

	if [ ! -e "$PRODUCT_OUT/recovery/root/system/" ] ; then
	mkdir $PRODUCT_OUT/recovery/root/system/
	fi

	if [ ! -e "$PRODUCT_OUT/recovery/root/system/lib/" ] ; then
	mkdir $PRODUCT_OUT/recovery/root/system/lib/
	mkdir $PRODUCT_OUT/recovery/root/system/lib/modules/
	fi

	if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/esp8089.ko" ] ; then
	cp device/rockchip/$TARGET_COMMON/wifi/lib/$MODULE/esp8089.ko $PRODUCT_OUT/recovery/root/system/lib/modules/
	fi


	if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/init_data.conf" ] ; then
	cp device/rockchip/$TARGET_COMMON/wifi/lib/init_data.conf  $PRODUCT_OUT/recovery/root/system/lib/modules/
	fi

	if [ -e "device/rockchip/$TARGET_COMMON/wifi/lib/esp_init_data.bin" ] ; then
	cp device/rockchip/$TARGET_COMMON/wifi/lib/esp_init_data.bin  $PRODUCT_OUT/recovery/root/system/lib/modules/
	fi

    #########################################################################################################
    if [ ! -e "$PRODUCT_OUT/recovery/root/system/etc/" ] ; then
    mkdir $PRODUCT_OUT/recovery/root/system/etc/
    fi

    if [ ! -e "$PRODUCT_OUT/recovery/root/system/etc/firmware" ] ; then
    mkdir $PRODUCT_OUT/recovery/root/system/etc/firmware/
    fi

    if [ ! -e "$PRODUCT_OUT/recovery/root/etc/firmware" ] ; then
    mkdir $PRODUCT_OUT/recovery/root/etc/firmware/
    fi
    
    if [ ! -e "$PRODUCT_OUT/recovery/root/system/etc/firmware/rtlbt" ] ; then
    mkdir $PRODUCT_OUT/recovery/root/system/etc/firmware/rtlbt/
    fi

    if [ -e "external/wlan_loader/firmware/" ] ; then
    cp external/wlan_loader/firmware/ $PRODUCT_OUT/recovery/root/system/etc/ -a
    fi 

    if [ -e "external/wlan_loader/firmware/WIFI_RAM_CODE" ] ; then
    cp external/wlan_loader/firmware/WIFI_RAM_CODE $PRODUCT_OUT/recovery/root/etc/firmware/WIFI_RAM_CODE
    fi

    if [ -e "hardware/mediatek/wlan/mt5931_6622/firmware/WIFI_RAM_CODE_MT5931" ] ; then
    cp hardware/mediatek/wlan/mt5931_6622/firmware/WIFI_RAM_CODE_MT5931 $PRODUCT_OUT/recovery/root/etc/firmware/WIFI_RAM_CODE_MT5931
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/ipp/lib/rk29-ipp.ko" ] ; then
    cp device/rockchip/$TARGET_COMMON/ipp/lib/rk29-ipp.ko $PRODUCT_OUT/recovery/root/
    fi

    if [ -e "device/rockchip/$TARGET_COMMON/ipp/lib/rk29-ipp.ko.3.0.36+" ] ; then
    cp device/rockchip/$TARGET_COMMON/ipp/lib/rk29-ipp.ko.3.0.36+ $PRODUCT_OUT/recovery/root/
    fi

    if [ -e "device/rockchip/common/bluetooth/pcba/system/bin" ] ; then
    cp device/rockchip/common/bluetooth/pcba/system/bin/ $PRODUCT_OUT/recovery/root/system/ -a
    fi

    if [ -e "device/rockchip/common/bluetooth/pcba/system/etc" ] ; then
    cp device/rockchip/common/bluetooth/pcba/system/etc/ $PRODUCT_OUT/recovery/root/system/ -a
    fi

    if [ -e "device/rockchip/common/bluetooth/realtek/bt/firmware/rtl8723au" ] ; then
    cp device/rockchip/common/bluetooth/realtek/bt/firmware/rtl8723au/* $PRODUCT_OUT/recovery/root/system/etc/firmware
    fi

    if [ -e "device/rockchip/common/bluetooth/realtek/bt/firmware/rtl8723bs" ] ; then
    cp device/rockchip/common/bluetooth/realtek/bt/firmware/rtl8723bs/rtl8723b_fw $PRODUCT_OUT/recovery/root/system/etc/firmware/rtlbt/rtlbt_fw
    cp $PRODUCT_OUT/system/etc/firmware/rtlbt/rtlbt_config $PRODUCT_OUT/recovery/root/system/etc/firmware/rtlbt/rtlbt_config
    #cp device/rockchip/common/bluetooth/realtek/bt/firmware/rtl8723bs/rtl8723b_config $PRODUCT_OUT/recovery/root/system/etc/firmware/rtlbt/rtlbt_config
    fi

    ############################################### bin/lib ##################################################
    
    cp -rf $PCBA_PATH/sbin/* $PRODUCT_OUT/recovery/root/system/bin/

    if [ -e "$PRODUCT_OUT/obj/lib/libc.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libc.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libcutils.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libcutils.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/liblog.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/liblog.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libm.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libm.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libstdc++.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libstdc++.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
if [ $BT_BLUEDROID = "true" ] ; then
    if [ -e "$PRODUCT_OUT/obj/lib/bluetooth.default.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/bluetooth.default.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libbluetooth_mtk.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libbluetooth_mtk.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libbt-hci.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libbt-hci.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libbt-utils.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libbt-utils.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/vendor/lib/libbt-vendor.so" ] ; then
    cp $PRODUCT_OUT/obj/vendor/lib/libbt-vendor.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libbt-vendor.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libbt-vendor.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libcorkscrew.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libcorkscrew.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libgccdemangle.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libgccdemangle.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libhardware_legacy.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libhardware_legacy.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libhardware.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libhardware.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libnetutils.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libnetutils.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libpower.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libpower.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libutils.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libutils.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libwpa_client.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libwpa_client.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
    if [ -e "$PRODUCT_OUT/obj/lib/libz.so" ] ; then
    cp $PRODUCT_OUT/obj/lib/libz.so $PRODUCT_OUT/recovery/root/system/lib/
    fi
fi
fi
