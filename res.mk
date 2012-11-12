ResFile := $(shell ls $(LOCAL_PATH)/res/)
PRODUCT_COPY_FILES += \
        $(foreach file, $(ResFile), $(LOCAL_PATH)/res/$(file):root/sbin/$(file))

# for wifi test
PRODUCT_COPY_FILES += \
        device/rockchip/$(TARGET_PRODUCT)/8188eu.ko:root/sbin/8188eu.ko \
        device/rockchip/$(TARGET_PRODUCT)/8192cu.ko:root/sbin/8192cu.ko \
        device/rockchip/$(TARGET_PRODUCT)/rda5890.ko:root/sbin/rda5890.ko \
        device/rockchip/$(TARGET_PRODUCT)/rt5370sta.ko:root/sbin/rt5370sta.ko \
        device/rockchip/$(TARGET_PRODUCT)/rkwifi.ko:root/sbin/rkwifi.ko \
        device/rockchip/$(TARGET_PRODUCT)/wlan.ko:root/sbin/wlan.ko


