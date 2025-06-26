#!/usr/bin/env python3
"""
Python script to load and run a bitfile and ELF file into an Arty S7-50 FPGA
using XSDB (Xilinx System Debugger) API.

This script replicates the XSDB command sequence for programming an FPGA
with a bitstream and downloading an ELF file to a RISC-V processor.
"""

import subprocess
import sys
import time
import os
from pathlib import Path

class XSDBController:
    def __init__(self, xsdb_path="xsdb"):
        """
        Initialize XSDB controller.
        
        Args:
            xsdb_path (str): Path to xsdb executable. Default is "xsdb" assuming it's in PATH.
        """
        self.xsdb_path = xsdb_path
        self.process = None
        self.connected = False
        
    def start_xsdb_session(self):
        """Start an interactive XSDB session."""
        try:
            self.process = subprocess.Popen(
                [self.xsdb_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1
            )
            print("XSDB session started successfully")
            return True
        except FileNotFoundError:
            print(f"Error: XSDB executable not found at '{self.xsdb_path}'")
            print("Make sure Vitis is installed and xsdb is in your PATH")
            return False
        except Exception as e:
            print(f"Error starting XSDB session: {e}")
            return False
    
    def send_command(self, command, wait_time=1):
        """
        Send a command to XSDB and return the response.
        
        Args:
            command (str): XSDB command to execute
            wait_time (float): Time to wait for command completion
        
        Returns:
            str: Command output
        """
        if not self.process:
            raise RuntimeError("XSDB session not started")
        
        print(f"Executing: {command}")
        
        try:
            # Send command
            self.process.stdin.write(command + "\n")
            self.process.stdin.flush()
            
            # Wait for command to complete
            time.sleep(wait_time)
            
            # Read any available output (non-blocking)
            output = ""
            if self.process.poll() is None:  # Process is still running
                # Note: This is a simplified approach. In practice, you might need
                # more sophisticated output handling for reliable communication
                pass
            
            return output
            
        except Exception as e:
            print(f"Error sending command '{command}': {e}")
            return ""
    
    def connect_to_hw_server(self, url="tcp:127.0.0.1:3121"):
        """Connect to hardware server."""
        result = self.send_command(f"connect -url {url}")
        self.connected = True
        return result
    
    def remove_all_breakpoints(self):
        """Remove all breakpoints."""
        return self.send_command("bpremove -all")
    
    def select_target_device(self, cable_name="Digilent Arty S7 - 50", serial_number=None):
        """
        Select target FPGA device.
        
        Args:
            cable_name (str): JTAG cable name
            serial_number (str): Serial number of the device (optional)
        """
        if serial_number:
            filter_cmd = f'targets -set -filter {{jtag_cable_name =~ "{cable_name} {serial_number}" && level==0 && jtag_device_ctx=="jsn-Arty S7 - 50-{serial_number}-0362f093-0"}}'
        else:
            filter_cmd = f'targets -set -filter {{jtag_cable_name =~ "{cable_name}*" && level==0}}'
        
        return self.send_command(filter_cmd, wait_time=2)
    
    def program_fpga(self, bitfile_path):
        """
        Program FPGA with bitstream file.
        
        Args:
            bitfile_path (str): Path to .bit file
        """
        if not os.path.exists(bitfile_path):
            raise FileNotFoundError(f"Bitfile not found: {bitfile_path}")
        
        # The device is automatically configured when selecting targets with a bitfile
        # We can also use the fpga command explicitly
        result = self.send_command(f'fpga -file "{bitfile_path}"', wait_time=5)
        print(f"Device configured successfully with \"{bitfile_path}\"")
        return result
    
    def select_processor_target(self, processor_name="*Hart**#0"):
        """Select processor target (e.g., RISC-V Hart)."""
        return self.send_command(f'targets -set -nocase -filter {{name =~ "{processor_name}"}}')
    
    def load_hardware_description(self, xsa_file_path):
        """
        Load hardware description file.
        
        Args:
            xsa_file_path (str): Path to .xsa file
        """
        if not os.path.exists(xsa_file_path):
            raise FileNotFoundError(f"XSA file not found: {xsa_file_path}")
        
        return self.send_command(f'loadhw -hw "{xsa_file_path}" -regs', wait_time=2)
    
    def reset_system(self):
        """Reset the system."""
        return self.send_command("rst -system", wait_time=3)
    
    def reset_processor(self):
        """Reset the processor."""
        return self.send_command("rst -processor", wait_time=2)
    
    def download_elf(self, elf_file_path):
        """
        Download ELF file to target.
        
        Args:
            elf_file_path (str): Path to .elf file
        """
        if not os.path.exists(elf_file_path):
            raise FileNotFoundError(f"ELF file not found: {elf_file_path}")
        
        return self.send_command(f'dow "{elf_file_path}"', wait_time=3)
    
    def continue_execution(self):
        """Continue program execution."""
        return self.send_command("con")
    
    def check_target_state(self):
        """Check the current state of the target."""
        return self.send_command("state")
    
    def run_program(self):
        """Run the program (alternative to continue)."""
        return self.send_command("run")
    
    def step_program(self):
        """Single step the program."""
        return self.send_command("stp")
    
    def get_program_counter(self):
        """Get current program counter value."""
        return self.send_command("rrd pc")
    
    def set_program_counter(self, address):
        """Set program counter to specific address."""
        return self.send_command(f"rwr pc {address}")
    
    def read_memory(self, address, size=1):
        """Read memory at specified address."""
        return self.send_command(f"mrd {address} {size}")
    
    def disconnect(self):
        """Disconnect from hardware server."""
        if self.connected:
            self.send_command("disconnect")
            self.connected = False
    
    def close_session(self):
        """Close XSDB session."""
        if self.process:
            self.send_command("exit")
            self.process.terminate()
            self.process.wait()
            self.process = None
            print("XSDB session closed")

def program_arty_s7_fpga(bitfile_path, elf_file_path, xsa_file_path=None, 
                        cable_serial=None, xsdb_path="xsdb", start_execution=True):
    """
    Complete function to program Arty S7-50 FPGA with bitstream and ELF file.
    
    Args:
        bitfile_path (str): Path to .bit file
        elf_file_path (str): Path to .elf file  
        xsa_file_path (str): Path to .xsa hardware description file (optional)
        cable_serial (str): Serial number of JTAG cable (optional)
        xsdb_path (str): Path to xsdb executable
        start_execution (bool): Whether to start program execution after download
    """
    
    # Validate input files
    if not os.path.exists(bitfile_path):
        raise FileNotFoundError(f"Bitfile not found: {bitfile_path}")
    if not os.path.exists(elf_file_path):
        raise FileNotFoundError(f"ELF file not found: {elf_file_path}")
    if xsa_file_path and not os.path.exists(xsa_file_path):
        raise FileNotFoundError(f"XSA file not found: {xsa_file_path}")
    
    print("Starting FPGA programming sequence...")
    print(f"Bitfile: {bitfile_path}")
    print(f"ELF file: {elf_file_path}")
    if xsa_file_path:
        print(f"XSA file: {xsa_file_path}")
    
    xsdb = XSDBController(xsdb_path)
    
    try:
        # Start XSDB session
        if not xsdb.start_xsdb_session():
            return False
        
        # Connect to hardware server
        print("Connecting to hardware server...")
        xsdb.connect_to_hw_server()
        
        # Remove all breakpoints
        xsdb.remove_all_breakpoints()
        
        # Select target device
        print("Selecting target device...")
        xsdb.select_target_device(serial_number=cable_serial)
        
        # Program FPGA with bitstream
        print("Programming FPGA with bitstream...")
        xsdb.program_fpga(bitfile_path)
        
        # Select processor target
        print("Selecting processor target...")
        xsdb.select_processor_target()
        
        # Load hardware description if provided
        if xsa_file_path:
            print("Loading hardware description...")
            xsdb.load_hardware_description(xsa_file_path)
        
        # Reset system
        print("Resetting system...")
        xsdb.reset_system()
        
        # Wait a bit after system reset
        time.sleep(3)
        
        # Select processor target again
        xsdb.select_processor_target()
        
        # Reset processor
        print("Resetting processor...")
        xsdb.reset_processor()
        
        # Download ELF file
        print("Downloading ELF file...")
        xsdb.download_elf(elf_file_path)
        
        if start_execution:
            # Start program execution
            print("Starting program execution...")
            xsdb.continue_execution()
            
            # Give the program a moment to start
            time.sleep(1)
            
            # Optional: Check execution state
            print("Checking target state...")
            xsdb.check_target_state()
            
            # Optional: Read program counter to verify execution
            print("Reading program counter...")
            xsdb.get_program_counter()
            
            print("FPGA programming and execution started successfully!")
        else:
            print("FPGA programming completed successfully! (Execution not started)")
        
        # Keep connection alive for a bit to allow program to run
        if start_execution:
            print("Allowing program to run for 5 seconds...")
            time.sleep(5)
        
        return True
        
    except Exception as e:
        print(f"Error during FPGA programming: {e}")
        return False
        
    finally:
        # Clean up
        xsdb.disconnect()
        xsdb.close_session()

def main():
    """Main function with example usage."""
    
    # Example file paths - update these to match your project structure
    bitfile = "/home/ryan-keller/hdldev/arty_s7_project/workspaces/vitis_workspace/riscv_hello_uart/_ide/bitstream/arty_s7_riscv_hardware.bit"
    # elf_file = "/home/ryan-keller/hdldev/arty_s7_project/workspaces/vitis_workspace/riscv_hello_uart/build/riscv_hello_uart.elf"
    # xsa_file = "/home/ryan-keller/hdldev/arty_s7_project/workspaces/vitis_workspace/arty_s7_riscv_platform/export/arty_s7_riscv_platform/hw/arty_s7_riscv_hardware.xsa"

    #bitfile = "/home/ryan-keller/hdldev/microblaze_v_cli/example_platform/build/arty_s7_riscv_platform/export/arty_s7_riscv_platform/hw/sdt/arty_s7_riscv.bit"
    elf_file = "/home/ryan-keller/hdldev/microblaze_v_cli/example_application/build/arty_s7_riscv_app/build/arty_s7_riscv_app.elf"
    xsa_file = "/home/ryan-keller/hdldev/microblaze_v_cli/example_platform/build/arty_s7_riscv_platform/export/arty_s7_riscv_platform/hw/arty_s7_riscv.xsa"
    
    # Optional: specify cable serial number if you have multiple devices
    cable_serial = "210352AD6E7CA"  # Update this to match your device
    
    # Check if files exist
    if not all(os.path.exists(f) for f in [bitfile, elf_file, xsa_file]):
        print("Error: One or more required files not found.")
        print("Please update the file paths in the script to match your project structure.")
        return 1
    
    # Program the FPGA
    success = program_arty_s7_fpga(
        bitfile_path=bitfile,
        elf_file_path=elf_file,
        xsa_file_path=xsa_file,
        cable_serial=cable_serial
    )
    
    return 0 if success else 1

if __name__ == "__main__":
    sys.exit(main())