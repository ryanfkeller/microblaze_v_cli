# Makefile for building example Vitis Platform .xpfm for microblaze_v_cli project

VITIS := vitis

BUILD_DIR := $(abspath build)
SCRIPT_DIR := $(abspath scripts)
XSA_DIR := $(abspath xsa)

PLATFORM_NAME := arty_s7_riscv_platform
PLATFORM_SCRIPT := $(SCRIPT_DIR)/build_arty_s7_riscv_platform.py
STABLE_XSA := xsa/arty_s7_riscv.xsa

XPFM := $(BUILD_DIR)/export/xilinx_platforms/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm


.PHONY: all help platform clean check-env make-dirs

all: help

help:
	@echo "*******************************"
	@echo "*  Platform Builder Makefile  *"
	@echo "*******************************"
	@echo ""
	@echo "Available targets:"
	@echo "  platform    -- Build the platform component and BSP from stable XSA"
	@echo "  clean       -- Remove all build artifacts and outputs"
	@echo ""
	@echo "Internal helper targets:"
	@echo "  check-env   -- (internal) Ensure Vitis is available in your PATH"
	@echo "  make-dirs   -- (internal) Create required build output directories"
	@echo ""
	@echo "Usage:"
	@echo "  make [target]"
	@echo "  For example, run 'make platform' to build the platform component and BSP."
	
platform: clean check-env make-dirs $(XPFM)
	
clean: 
	rm -rf $(BUILD_DIR)

check-env:
	@command -v $(VITIS) > /dev/null 2>&1 || (echo "ERROR: Vitis not found in PATH. Please source the Xilinx Vitis settings script before running make."; exit 1)

make-dirs:
	@mkdir -p $(BUILD_DIR)

$(XPFM): $(PLATFORM_SCRIPT) $(STABLE_XSA) 
	@echo "Creating platform '$(PLATFORM_NAME)' from XSA: $(STABLE_XSA)"
	@mkdir -p $(BUILD_DIR)
	@$(VITIS) -s $(PLATFORM_SCRIPT) --xsa_path $(STABLE_XSA) --workspace_dir $(BUILD_DIR) --platform_name $(PLATFORM_NAME)

