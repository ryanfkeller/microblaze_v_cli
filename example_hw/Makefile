# Makefile for building example Vivado Hardware .xsa for microblaze_v_cli project

VIVADO := vivado

SCRIPT_DIR := $(abspath scripts)
CONSTRAINT_DIR := $(abspath constraints)
BUILD_DIR := $(abspath build)
OUT_DIR := $(BUILD_DIR)/out

BD_BUILD_SCRIPT := $(SCRIPT_DIR)/build_arty_s7_riscv_bd.tcl
XSA_BUILD_SCRIPT := $(SCRIPT_DIR)/build_arty_s7_riscv_xsa.tcl
HW_CONSTRAINTS := $(CONSTRAINT_DIR)/arty_s7_riscv_handgen.xdc

XSA := $(BUILD_DIR)/$(OUT_DIR)/arty_s7_riscv.xsa

.PHONY: all help xsa clean check-env make-dirs

all: help

help:
	@echo "*******************************"
	@echo "*     XSA Builder Makefile    *"
	@echo "*******************************"
	@echo ""
	@echo "Available targets:"
	@echo "  hw          -- Build the hardware (.xsa) file"
	@echo "  clean       -- Remove all build artifacts and outputs"
	@echo ""
	@echo "Internal helper targets:"
	@echo "  check-env   -- (internal) Ensure Vivado is available in your PATH"
	@echo "  make-dirs   -- (internal) Create required build output directories"
	@echo ""
	@echo "Usage:"
	@echo "  make [target]"
	@echo "  For example, run 'make xsa' to build the hardware platform."
	
hw: clean check-env make-dirs $(XSA)

clean: 
	rm -rf $(OUT_DIR)
	rm -rf $(BUILD_DIR)

check-env:
	@command -v $(VIVADO) > /dev/null 2>&1 || (echo "ERROR: Vivado not found in PATH. Please source the Xilinx Vivado settings script before running make."; exit 1)

make-dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OUT_DIR)

# Assuming dependencies are defined properly 
$(XSA):$(XSA_BUILD_SCRIPT) $(BD_BUILD_SCRIPT) $(HW_CONSTRAINTS)
	@echo "Building hardware platform (.xsa)..."
	@cd $(BUILD_DIR) && $(VIVADO) -mode batch -source $(XSA_BUILD_SCRIPT) -notrace




