CFG_TEE_TA_LOG_LEVEL ?= 4

CPPFLAGS += -DCFG_TEE_TA_LOG_LEVEL=$(CFG_TEE_TA_LOG_LEVEL) -I$(TEEC_EXPORT)/include

# The UUID for the Trusted Application
BINARY=d6f85138-131c-4f86-b746-a066b2536e69

include $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk
