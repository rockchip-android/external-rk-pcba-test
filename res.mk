ResFile := $(shell ls $(LOCAL_PATH)/res/)
PRODUCT_COPY_FILES += \
        $(foreach file, $(ResFile), $(LOCAL_PATH)/res/$(file):root/sbin/$(file))

# for wifi test
PRODUCT_COPY_FILES += \
        $(call add-to-product-copy-files-if-exists,device/rockchip/$(TARGET_PRODUCT)/8188eu.ko:root/sbin/8188eu.ko) \
        $(call add-to-product-copy-files-if-exists,device/rockchip/$(TARGET_PRODUCT)/8192cu.ko:root/sbin/8192cu.ko) \
        $(call add-to-product-copy-files-if-exists,device/rockchip/$(TARGET_PRODUCT)/rda5890.ko:root/sbin/rda5890.ko) \
        $(call add-to-product-copy-files-if-exists,device/rockchip/$(TARGET_PRODUCT)/rt5370sta.ko:root/sbin/rt5370sta.ko) \
        $(call add-to-product-copy-files-if-exists,device/rockchip/$(TARGET_PRODUCT)/rkwifi.ko:root/sbin/rkwifi.ko)

