# scripts/build_arty_s7_riscv_application.py
# Usage: vitis -p this_script.py -- --workspace_dir <workspace_dir> --platform_name <platform_name> --app_name <app_name> --app_src_dir <app_src_dir>

import argparse
import os
import vitis

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Build a Vitis application from a platform."
    )
    parser.add_argument(
        "--workspace_dir",
        type=str,
        required=True,
        help="Directory for the Vitis workspace"
    )
    parser.add_argument(
        "--platform_dir",
        type=str,
        required=True,
        help="Path to platform component directory to use"
    )
    parser.add_argument(
        "--cli_core_dir",
        type=str,
        required=True,
        help="Path to CLI core directory to use"
    )
    parser.add_argument(
        "--app_name",
        type=str,
        required=True,
        help="Name of the application component to create"
    )

    args = parser.parse_args()

    workspace_dir = os.path.abspath(args.workspace_dir)
    platform_dir = args.platform_dir
    platform_name = os.path.basename(platform_dir)
    cli_core_dir = args.cli_core_dir
    app_name = args.app_name

    print(f"Building Vitis application:")
    print(f"  Workspace:  {workspace_dir}")
    print(f"  Platform:   {platform_dir}")
    print(f"  CLI Core:   {cli_core_dir}")
    print(f"  App Name:   {app_name}\n")

    client = vitis.create_client()

    # Set the workspace directory
    client.set_workspace(workspace_dir)

    # Find the platform component to base the app on
    client.add_platform_repos(platform_dir)

    platform_xpfm=client.find_platform_in_repos(platform_name)

    # Create application component from the platform
    app_comp = client.create_app_component(
        name=app_name,
        platform=platform_xpfm
    )

    app_comp.import_files(from_loc=cli_core_dir)
    app_comp.generate_build_files()

    app_comp.build()

    print(f"Done! ELF exported to build directory!")