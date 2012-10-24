ResFile := $(shell ls $(LOCAL_PATH)/res/)
PRODUCT_COPY_FILES += \
        $(foreach file, $(ResFile), $(LOCAL_PATH)/res/$(file):root/sbin/$(file))



