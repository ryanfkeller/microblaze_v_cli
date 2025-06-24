import time
import os
import xsdb
import argparse


# === MAIN ===
if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Build a load and run the target XSA and ELF on the platform."
    )
    parser.add_argument(
        "--xsa_path",
        type=str,
        required=True,
        help="Path to XSA file to load into the FPGA"
    )
    parser.add_argument(
        "--elf_path",
        type=str,
        required=True,
        help="Path of the ELF file to load into the processor"
    )

    args = parser.parse_args()

    assert os.path.exists(args.xsa_path), f"XSA not found: {args.xsa_path}"
    assert os.path.exists(args.elf_path), f"ELF not found: {args.elf_path}"

    print(f"Loading and running application:")
    print(f"  XSA:      {args.xsa_path}")
    print(f"  ELF:      {args.elf_path}")

    print("Connecting to xsdb...")
    session = xsdb.start_debug_session()
    session.connect()

    # print("Available Targets:")
    # session.targets()

    fpga_target = session.ta('-s', filter="name =~ BSCAN JTAG*")
    print(f"Loading FPGA with {args.xsa_path}")
    fpga_target.loadhw('--regs', hw=args.xsa_path)

    riscv_target = None
    while (riscv_target == None) :
        # print("Available Targets:")
        # session.targets()
        try : 
            riscv_target = session.ta('-s', filter="name =~ RISC-V*")
            hart_target = session.ta('-s', filter="name =~ Hart #0")
        except KeyError :
            time.sleep(1)
            continue
        
    print(f"Loading Processor with {args.elf_path}")
    hart_target.dow(args.elf_path)
    hart_target.con()
    time.sleep(2)
    print("Done!")