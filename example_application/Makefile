# Makefile for preparing example_application with platform BSP and XPFM

VITIS := vitis

BUILD_DIR := $(abspath build)
SCRIPT_DIR := $(abspath scripts)

PLATFORM_NAME := arty_s7_riscv_platform
APP_NAME := arty_s7_riscv_app

PLATFORM_DIR := $(abspath ../example_platform/build/$(PLATFORM_NAME))
CLI_CORE_DIR := $(abspath ../cli_core)


PLATFORM_XPFM := $(PLATFORM_DIR)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
PLATFORM_BSP := $(PLATFORM_DIR)/microblaze_riscv_0/standalone_microblaze_riscv_0/bsp

APP_SCRIPT := $(SCRIPT_DIR)/build_arty_s7_riscv_application.py


.PHONY: all help application clean check-env make-dirs

all: help

help:
	@echo "********************************"
	@echo "* Application Builder Makefile *"
	@echo "********************************"
	@echo ""
	@echo "Available targets:"
	@echo "  app         -- Builds the application component and ELF (.elf) file"
	@echo "  clean       -- Remove all build artifacts and outputs"
	@echo ""
	@echo "Internal helper targets:"
	@echo "  check-env   -- (internal) Ensure Vivado is available in your PATH"
	@echo "  make-dirs   -- (internal) Create required build output directories"
	@echo ""
	@echo "Usage:"
	@echo "  make [target]"
	@echo "  For example, run 'make app' to build the application component and ELF file."

app: check-env clean make-dirs
	@$(VITIS) -s $(APP_SCRIPT) --workspace_dir $(BUILD_DIR) --platform_dir $(PLATFORM_DIR) --cli_core_dir $(CLI_CORE_DIR) --app_name $(APP_NAME)


check-env:
	@command -v $(VITIS) > /dev/null 2>&1 || (echo "ERROR: Vitis not found in PATH. Please source the Xilinx Vitis settings script before running make."; exit 1)
	@if [ ! -f "$(PLATFORM_XPFM)" ]; then \
		echo "ERROR: Platform XPFM not found at $(PLATFORM_XPFM). Please build the platform first."; \
		exit 1; \
	fi
	@if [ ! -d "$(PLATFORM_BSP)" ]; then \
		echo "ERROR: Platform BSP not found at $(PLATFORM_BSP). Please build the platform first."; \
		exit 1; \
	fi

make-dirs:
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)