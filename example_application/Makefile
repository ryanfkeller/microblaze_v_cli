# Makefile for preparing example_application with platform BSP and XPFM

# Tools
VITIS := vitis
PYTHON := /usr/bin/python

# Names/Basenames
PLATFORM := arty_s7_riscv_platform
XSA := arty_s7_riscv.xsa
BIT := arty_s7_riscv.bit
APP := arty_s7_riscv_app
XPFM := arty_s7_riscv_platform.xpfm

# Paths
CLI_CORE_DIR := $(abspath ../cli_core)
CLI_INCLUDE_DIR := $(CLI_CORE_DIR)/include
CLI_ADAPTER_DIR := $(CLI_CORE_DIR)/platform_adapters

PLATFORM_DIR := $(abspath ../example_platform/build/$(PLATFORM))
PLATFORM_EXPORT_DIR := $(PLATFORM_DIR)/export/$(PLATFORM)
PLATFORM_XPFM := $(PLATFORM_EXPORT_DIR)/$(XPFM)
PLATFORM_XSA := $(PLATFORM_EXPORT_DIR)/hw/$(XSA)
PLATFORM_BIT := $(PLATFORM_EXPORT_DIR)/hw/$(BIT)
PLATFORM_BSP := $(PLATFORM_DIR)/microblaze_riscv_0/standalone_microblaze_riscv_0/bsp

APP_SRC_DIR := $(abspath src)
APP_BUILD_DIR := $(abspath build)
APP_SCRIPT_DIR := $(abspath scripts)
APP_BUILD_SCRIPT := $(APP_SCRIPT_DIR)/vitis_application_script.py
APP_RUN_SCRIPT := $(APP_SCRIPT_DIR)/xsdb_platform_script.py
APP_ELF := "$(APP_BUILD_DIR)/$(APP)/build/$(APP).elf" # Expected path of generated ELF files


.PHONY: all help app run clean check-env make-dirs

all: help

help:
	@echo "********************************"
	@echo "* Application Builder Makefile *"
	@echo "********************************"
	@echo ""
	@echo "Available targets:"
	@echo "  app         -- Builds the application component and ELF (.elf) file"
	@echo "  run		 -- Loads XSA and ELF onto the hardware and starts execution"
	@echo "  bar		 -- Builds and Runs the application on hardware"
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
	@$(VITIS) -s $(APP_BUILD_SCRIPT) --workspace_dir $(APP_BUILD_DIR) --platform_dir $(PLATFORM_DIR) --cli_core_dir $(CLI_CORE_DIR) --app_src_dir $(APP_SRC_DIR) --app_name $(APP)

run: check-env
	@$(PYTHON) $(APP_RUN_SCRIPT)
	
bar: app run

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
	@mkdir -p $(APP_BUILD_DIR)

clean:
	rm -rf $(APP_BUILD_DIR)