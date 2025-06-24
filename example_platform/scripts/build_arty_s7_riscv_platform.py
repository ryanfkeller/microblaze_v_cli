# scripts/build_arty_s7_riscv_platform.py
# Usage: vitis -p this_script.py -- <xsa_path> <workspace_dir> <platform_name>

import argparse
import os
import shutil
import vitis

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Build a Vitis platform from an XSA file."
    )
    parser.add_argument(
        "--xsa_path",
        type=str,
        help="Path to the XSA file",
        required=True
    )
    parser.add_argument(
        "--workspace_dir",
        type=str,
        help="Directory for the Vitis workspace",
        required=True
    )
    parser.add_argument(
        "--platform_name",
        type=str,
        help="Name of the platform to create",
        required=True
    )

    args = parser.parse_args()

    xsa_path = os.path.abspath(args.xsa_path)
    workspace_dir = os.path.abspath(args.workspace_dir)
    platform_name = args.platform_name

    print(f"Building Vitis platform:")
    print(f"  XSA:        {xsa_path}")
    print(f"  Workspace:  {workspace_dir}")
    print(f"  Platform:   {platform_name}")

    client = vitis.create_client()

    # Create workspace and platform
    client.set_workspace(workspace_dir)

    platform_obj = client.create_platform_component(
        name=platform_name,
        hw_design=xsa_path,
        cpu="microblaze_riscv_0",
        os="standalone")
    
    platform_obj.report()
    platform_obj.build()

    # Move platform xpfm to out directory
    platform_xpfm_gen=client.find_platform_in_repos(platform_name)


    print(f"Done. Exported to: {platform_xpfm_gen}")