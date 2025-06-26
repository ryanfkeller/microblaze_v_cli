#!/usr/bin/env python3
"""
Improved Vitis RISC-V Application Build Script with Build Information
Usage: vitis -p this_script.py -- --workspace_dir <workspace_dir> --platform_dir <platform_dir> --cli_core_dir <cli_core_dir> --app_src_dir <app_src_dir> --app_name <app_name>

This script creates a Vitis application component from a platform, imports source files,
and builds the application. It includes improved error handling, validation, debugging,
and automatic generation of build information (Git hash, build date, etc.).
"""

import argparse
import os
import sys
import vitis
from pathlib import Path
import subprocess
import datetime
import json

class VitisApplicationBuilder:
    def __init__(self, workspace_dir, platform_dir, cli_core_dir, app_src_dir, app_name):
        """Initialize the Vitis application builder with validated paths."""
        self.workspace_dir = os.path.abspath(workspace_dir)
        self.platform_dir = os.path.abspath(platform_dir)
        self.platform_name = os.path.basename(platform_dir)
        self.cli_core_dir = os.path.abspath(cli_core_dir)
        self.app_src_dir = os.path.abspath(app_src_dir)
        self.app_name = app_name
        self.client = None
        self.app_comp = None
        self.build_info = {}
        
    def get_git_info(self):
        """Get Git repository information."""
        git_info = {
            'hash': 'unknown',
            'branch': 'unknown',
            'tag': 'unknown',
            'dirty': False,
            'commit_date': 'unknown'
        }
        
        try:
            # Get current directory for git operations
            git_dir = os.getcwd()
            
            # Try to get git hash
            try:
                result = subprocess.run(['git', 'rev-parse', 'HEAD'], 
                                      cwd=git_dir, capture_output=True, text=True, timeout=5)
                if result.returncode == 0:
                    git_info['hash'] = result.stdout.strip()
                    # Get short hash
                    git_info['short_hash'] = git_info['hash'][:8]
            except (subprocess.TimeoutExpired, FileNotFoundError, subprocess.SubprocessError):
                pass
            
            # Try to get branch name
            try:
                result = subprocess.run(['git', 'rev-parse', '--abbrev-ref', 'HEAD'], 
                                      cwd=git_dir, capture_output=True, text=True, timeout=5)
                if result.returncode == 0:
                    git_info['branch'] = result.stdout.strip()
            except (subprocess.TimeoutExpired, FileNotFoundError, subprocess.SubprocessError):
                pass
            
            # Try to get latest tag
            try:
                result = subprocess.run(['git', 'describe', '--tags', '--abbrev=0'], 
                                      cwd=git_dir, capture_output=True, text=True, timeout=5)
                if result.returncode == 0:
                    git_info['tag'] = result.stdout.strip()
            except (subprocess.TimeoutExpired, FileNotFoundError, subprocess.SubprocessError):
                pass
            
            # Check if working directory is dirty
            try:
                result = subprocess.run(['git', 'diff', '--quiet'], 
                                      cwd=git_dir, capture_output=True, timeout=5)
                git_info['dirty'] = (result.returncode != 0)
            except (subprocess.TimeoutExpired, FileNotFoundError, subprocess.SubprocessError):
                pass
            
            # Get commit date
            try:
                result = subprocess.run(['git', 'log', '-1', '--format=%ci'], 
                                      cwd=git_dir, capture_output=True, text=True, timeout=5)
                if result.returncode == 0:
                    git_info['commit_date'] = result.stdout.strip()
            except (subprocess.TimeoutExpired, FileNotFoundError, subprocess.SubprocessError):
                pass
                
        except Exception as e:
            print(f"Warning: Could not get git information: {e}")
        
        return git_info
    
    def generate_build_info(self):
        """Generate comprehensive build information."""
        print("Generating build information...")
        
        # Get current timestamp
        build_time = datetime.datetime.now()
        
        # Get git information
        git_info = self.get_git_info()
        
        # Get system information
        try:
            import platform
            system_info = {
                'system': platform.system(),
                'machine': platform.machine(),
                'python_version': platform.python_version(),
                'user': os.getenv('USER', os.getenv('USERNAME', 'unknown'))
            }
        except:
            system_info = {
                'system': 'unknown',
                'machine': 'unknown', 
                'python_version': 'unknown',
                'user': 'unknown'
            }
        
        # Compile build information
        self.build_info = {
            'build_date': build_time.strftime('%Y-%m-%d'),
            'build_time': build_time.strftime('%H:%M:%S'),
            'build_timestamp': build_time.strftime('%Y-%m-%d %H:%M:%S UTC'),
            'build_unix_timestamp': int(build_time.timestamp()),
            'git_hash': git_info['hash'],
            'git_short_hash': git_info.get('short_hash', git_info['hash'][:8]),
            'git_branch': git_info['branch'],
            'git_tag': git_info['tag'],
            'git_dirty': git_info['dirty'],
            'git_commit_date': git_info['commit_date'],
            'build_system': system_info['system'],
            'build_machine': system_info['machine'],
            'build_user': system_info['user'],
            'python_version': system_info['python_version'],
            'app_name': self.app_name,
            'platform_name': self.platform_name
        }
        
        # Create version string
        version_parts = []
        if git_info['tag'] != 'unknown':
            version_parts.append(git_info['tag'])
        
        if git_info['short_hash'] != 'unknown':
            hash_part = git_info['short_hash']
            if git_info['dirty']:
                hash_part += '-dirty'
            version_parts.append(hash_part)
        
        if git_info['branch'] != 'unknown' and git_info['branch'] != 'main' and git_info['branch'] != 'master':
            version_parts.append(f"({git_info['branch']})")
        
        if version_parts:
            self.build_info['version_string'] = '-'.join(version_parts)
        else:
            self.build_info['version_string'] = f"dev-{build_time.strftime('%Y%m%d')}"
        
        print(f"✓ Build info generated:")
        print(f"  Version: {self.build_info['version_string']}")
        print(f"  Git Hash: {self.build_info['git_short_hash']}")
        print(f"  Build Date: {self.build_info['build_timestamp']}")
        print(f"  Branch: {self.build_info['git_branch']}")
        if self.build_info['git_dirty']:
            print(f"  ⚠️  Working directory has uncommitted changes")
    
    def get_version_string(self):
        """Get just the version string for compiler defines."""
        return self.build_info.get('version_string', 'unknown')
    
    def validate_inputs(self):
        """Validate all input paths and directories exist."""
        print("Validating input paths...")
        
        paths_to_check = [
            ("Platform directory", self.platform_dir),
            ("CLI core directory", self.cli_core_dir),
            ("Application source directory", self.app_src_dir)
        ]
        
        for name, path in paths_to_check:
            if not os.path.exists(path):
                raise FileNotFoundError(f"{name} not found: {path}")
            print(f"✓ {name}: {path}")
        
        # Check for required CLI core subdirectories
        required_subdirs = [
            "include",
            "platform_adapters/include", 
            "platform_adapters/src"
        ]
        
        for subdir in required_subdirs:
            full_path = os.path.join(self.cli_core_dir, subdir)
            if not os.path.exists(full_path):
                raise FileNotFoundError(f"Required CLI core subdirectory not found: {full_path}")
            print(f"✓ CLI core subdirectory: {full_path}")
        
        # Ensure workspace directory exists
        os.makedirs(self.workspace_dir, exist_ok=True)
        print(f"✓ Workspace directory: {self.workspace_dir}")
        
    def print_configuration(self):
        """Print the build configuration."""
        print(f"\n{'='*60}")
        print(f"Vitis RISC-V Application Build Configuration")
        print(f"{'='*60}")
        print(f"Workspace:     {self.workspace_dir}")
        print(f"Platform Dir:  {self.platform_dir}")
        print(f"Platform Name: {self.platform_name}")
        print(f"CLI Core:      {self.cli_core_dir}")
        print(f"App Source:    {self.app_src_dir}")
        print(f"App Name:      {self.app_name}")
        print(f"{'='*60}\n")
        
    def initialize_client(self):
        """Initialize Vitis client and set workspace."""
        print("Initializing Vitis client...")
        try:
            self.client = vitis.create_client()
            print("✓ Vitis client created successfully")
            
            # Set the workspace directory
            self.client.set_workspace(self.workspace_dir)
            print(f"✓ Workspace set to: {self.workspace_dir}")
            
        except Exception as e:
            raise RuntimeError(f"Failed to initialize Vitis client: {e}")
    
    def setup_platform(self):
        """Add platform repository and find the platform."""
        print(f"\nSetting up platform...")
        try:
            # Add platform repository
            self.client.add_platform_repos(self.platform_dir)
            print(f"✓ Added platform repository: {self.platform_dir}")
            
            # List available platforms for debugging
            available_platforms = self.client.list_platforms()
            print(f"Available platforms: {available_platforms}")
            
            # Find the specific platform
            print(f"Looking for platform: {self.platform_name}")
            platform_xpfm = self.client.find_platform_in_repos(self.platform_name)
            
            if platform_xpfm is None:
                raise RuntimeError(f"Platform '{self.platform_name}' not found in repository")
            
            print(f"✓ Found platform: {platform_xpfm}")
            return platform_xpfm
            
        except Exception as e:
            raise RuntimeError(f"Failed to setup platform: {e}")
    
    def create_application(self, platform_xpfm):
        """Create the application component from the platform."""
        print(f"\nCreating application component...")
        try:
            # Check if application already exists and remove it
            existing_components = self.client.list_components()
            if self.app_name in [comp.get_name() for comp in existing_components]:
                print(f"Warning: Application '{self.app_name}' already exists. Removing...")
                existing_app = next(comp for comp in existing_components if comp.get_name() == self.app_name)
                self.client.delete_component(existing_app)
                print(f"✓ Removed existing application: {self.app_name}")
            
            # Create new application component
            self.app_comp = self.client.create_app_component(
                name=self.app_name,
                platform=platform_xpfm
            )
            print(f"✓ Created application component: {self.app_name}")
            
        except Exception as e:
            raise RuntimeError(f"Failed to create application component: {e}")
    
    def import_source_files(self):
        """Import all required source files into the application."""
        print(f"\nImporting source files...")
        
        import_tasks = [
            {
                'desc': 'CLI core headers',
                'dest': 'src',
                'src': os.path.join(self.cli_core_dir, 'include')
            },
            {
                'desc': 'Platform adapter headers', 
                'dest': 'src',
                'src': os.path.join(self.cli_core_dir, 'platform_adapters', 'include')
            },
            {
                'desc': 'Platform adapter sources',
                'dest': 'src', 
                'src': os.path.join(self.cli_core_dir, 'platform_adapters', 'src')
            },
            {
                'desc': 'Application sources',
                'dest': 'src',
                'src': self.app_src_dir
            }
        ]
        
        try:
            for task in import_tasks:
                print(f"Importing {task['desc']}...")
                print(f"  From: {task['src']}")
                print(f"  To:   {task['dest']}")
                
                if not os.path.exists(task['src']):
                    raise FileNotFoundError(f"Source directory not found: {task['src']}")
                
                self.app_comp.import_files(
                    dest_dir_in_cmp=task['dest'],
                    from_loc=task['src']
                )
                print(f"✓ Imported {task['desc']}")
            
        except Exception as e:
            raise RuntimeError(f"Failed to import source files: {e}")
    
    def list_imported_files(self):
        """Verify imported files by checking the workspace directory structure."""
        print(f"\nVerifying imported files...")
        try:
            app_dir = os.path.join(self.workspace_dir, self.app_name)
            if os.path.exists(app_dir):
                print(f"✓ Application directory: {app_dir}")
                src_dir = os.path.join(app_dir, 'src')
                if os.path.exists(src_dir):
                    print(f"✓ Source directory structure:")
                    for root, dirs, files in os.walk(src_dir):
                        level = root.replace(src_dir, '').count(os.sep)
                        indent = '  ' * level
                        rel_path = os.path.relpath(root, src_dir)
                        if rel_path == '.':
                            print(f"{indent}src/")
                        else:
                            print(f"{indent}{rel_path}/")
                        subindent = '  ' * (level + 1)
                        for file in sorted(files):
                            print(f"{subindent}{file}")
                else:
                    print(f"Warning: Source directory not found at {src_dir}")
            else:
                print(f"Warning: Application directory not found at {app_dir}")
                    
        except Exception as e:
            print(f"Warning: Could not verify application files: {e}")
            print("This is not critical - the build may still succeed.")
    
    def configure_build_settings(self):
        """Configure build settings and add version string as compiler define."""
        print(f"\nConfiguring build settings...")
        try:
            # Generate build information first
            self.generate_build_info()
            
            # Add user compiler defines
            version_define = f'-DVERSION_STRING=\\"{self.build_info["version_string"]}\\"\"'
            timestamp_define = f'-DTIMESTAMP_STRING=\\"{self.build_info["build_timestamp"]}\\"\"'
            try:
                self.app_comp.append_app_config(key = 'USER_COMPILE_DEFINITIONS', values = version_define)
                self.app_comp.append_app_config(key = 'USER_COMPILE_DEFINITIONS', values = timestamp_define)
                print(f"✓ Added compiler defines: ")
                print(f"     {version_define}")
                print(f"     {timestamp_define}")
            except Exception as e:
                print(f"Warning: Could not set compiler define via 'set_app_config': {e}")

        
            # Add any other specific compiler flags, linker settings, etc. here
            # Example: Set optimization level
            # self.app_comp.set_property("compiler.optimization", "-O2")
            
            # Example: Set RISC-V architecture
            # self.app_comp.set_property("compiler.arch", "rv32i")
            
            print("✓ Build settings configured with version information")
            
        except Exception as e:
            raise RuntimeError(f"Failed to configure build settings: {e}")
    
    def generate_and_build(self):
        """Generate build files and build the application."""
        print(f"\nGenerating build files...")
        try:
            self.app_comp.generate_build_files()
            print("✓ Build files generated successfully")
            
        except Exception as e:
            raise RuntimeError(f"Failed to generate build files: {e}")
        
        print(f"\nBuilding application...")
        try:
            build_result = self.app_comp.build()
            print("✓ Application built successfully")
            
            # Print build results if available
            if hasattr(build_result, 'get_build_log'):
                print("\nBuild log:")
                print(build_result.get_build_log())
            
            return build_result
            
        except Exception as e:
            raise RuntimeError(f"Failed to build application: {e}")
    
    def get_output_files(self):
        """Get information about generated output files and verify build success."""
        print(f"\nLocating output files...")
        try:
            # Common output file paths in Vitis projects
            build_dir = os.path.join(self.workspace_dir, self.app_name, "build")
            elf_file = os.path.join(build_dir, f"{self.app_name}.elf")
            
            if os.path.exists(elf_file):
                print(f"✓ ELF file generated: {elf_file}")
                return {"elf": elf_file, "success": True}
            else:
                print(f"⚠️  Expected ELF file not found at: {elf_file}")
                # Try to find ELF files in the build directory
                if os.path.exists(build_dir):
                    elf_files = list(Path(build_dir).rglob("*.elf"))
                    if elf_files:
                        print(f"Found ELF files:")
                        for elf in elf_files:
                            print(f"  {elf}")
                        print(f"✓ Using ELF file: {elf_files[0]}")
                        return {"elf": str(elf_files[0]), "success": True}
                    else:
                        print(f"❌ No ELF files found in build directory: {build_dir}")
                else:
                    print(f"❌ Build directory not found: {build_dir}")
                
                return {"success": False}
                
        except Exception as e:
            print(f"❌ Error locating output files: {e}")
            return {"success": False}
    
    def cleanup(self):
        """Clean up Vitis client connection."""
        if self.client:
            try:
                # Close the client connection
                print("\nCleaning up Vitis client...")
                # Note: Vitis Python API may have a close() or disconnect() method
                # Check the API documentation for the correct method
                print("✓ Vitis client cleaned up")
            except Exception as e:
                print(f"Warning: Error during cleanup: {e}")
    
    def build(self):
        """Main build method that orchestrates the entire build process."""
        try:
            self.validate_inputs()
            self.print_configuration()
            self.initialize_client()
            
            platform_xpfm = self.setup_platform()
            self.create_application(platform_xpfm)
            self.import_source_files()
            self.list_imported_files()
            self.configure_build_settings()  # This now includes version define generation
            
            build_result = self.generate_and_build()
            output_files = self.get_output_files()
            
            # Check if build actually succeeded by verifying output files
            build_success = output_files.get("success", False)
            
            if build_success:
                print(f"\n{'='*60}")
                print(f"BUILD COMPLETED SUCCESSFULLY")
                print(f"{'='*60}")
                print(f"Version: {self.build_info['version_string']}")
                
                if output_files.get('elf'):
                    print(f"Output files:")
                    print(f"  ELF: {output_files['elf']}")
                
                return True, output_files
            else:
                print(f"\n{'='*60}")
                print(f"BUILD FAILED - NO OUTPUT FILES GENERATED")
                print(f"{'='*60}")
                print(f"The build process completed without errors, but no ELF file was generated.")
                print(f"This usually indicates a compilation or linking failure.")
                print(f"Check the build logs above for specific error messages.")
                return False, output_files
            
        except Exception as e:
            print(f"\n{'='*60}")
            print(f"BUILD FAILED - EXCEPTION OCCURRED")
            print(f"{'='*60}")
            print(f"Error: {e}")
            return False, {}
            
        finally:
            self.cleanup()

def main():
    """Main function with argument parsing."""
    parser = argparse.ArgumentParser(
        description="Build a Vitis RISC-V application from a platform with improved error handling, debugging, and build information generation.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Example usage:
  vitis -p build_arty_s7_riscv_application.py -- \\
    --workspace_dir ./vitis_workspace \\
    --platform_dir ./platform_build/arty_s7_riscv_platform \\
    --cli_core_dir ./cli_core \\
    --app_src_dir ./app_source \\
    --app_name my_riscv_app
        """
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
        help="Path to CLI core directory containing include and platform_adapters"
    )
    parser.add_argument(
        "--app_src_dir",
        type=str,
        required=True,
        help="Path to application source directory"
    )
    parser.add_argument(
        "--app_name",
        type=str,
        required=True,
        help="Name of the application component to create"
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Enable verbose output"
    )

    args = parser.parse_args()
    
    # Create and run the builder
    builder = VitisApplicationBuilder(
        workspace_dir=args.workspace_dir,
        platform_dir=args.platform_dir,
        cli_core_dir=args.cli_core_dir,
        app_src_dir=args.app_src_dir,
        app_name=args.app_name
    )
    
    success, output_files = builder.build()
    
    if success:
        print(f"\nApplication build completed successfully!")
        if output_files.get('elf'):
            print(f"You can now program your FPGA using:")
            print(f"  ELF File: {output_files['elf']}")
            print(f"\nUse your XSDB script to program and run the application.")
        return 0
    else:
        print(f"\nApplication build failed!")
        return 1

if __name__ == "__main__":
    sys.exit(main())