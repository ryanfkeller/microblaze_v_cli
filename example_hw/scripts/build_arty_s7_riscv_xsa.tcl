# ====== Project Setup ======
set proj_name microblaze_v_hw_proj
set part xc7s50csga324-1
set board_part digilentinc.com:arty-s7-50:part0:1.1

# Get the directory where this script lives (scripts/)
set script_dir [file dirname [info script]]

# Define directories relative to script_dir
set constraint_dir [file normalize "$script_dir/../constraints"]
set out_dir [file normalize "$script_dir/../build/out"]
set proj_dir [file normalize "$script_dir/../build/vivado_proj"]

# Create, set up, and cd into the new project
create_project $proj_name $proj_dir -part $part
set_property board_part $board_part [current_project]
cd $proj_dir

# === Block Diagram + Wrapper Setup ===
# Generate block diagram
source $script_dir/build_arty_s7_riscv_bd.tcl

# Generate HDL wrapper and add to project
set wrapper_path [make_wrapper -files [get_files *.bd] -top -force]
add_files $wrapper_path
set_property top arty_s7_riscv_bd_wrapper [current_fileset]

# === Constraints ===
add_files -fileset constrs_1 $constraint_dir/arty_s7_riscv_handgen.xdc

# === Synthesis & Implementation ===
launch_runs impl_1 -jobs 6 -to_step write_bitstream
wait_on_run impl_1

# === Export Hardware Platform (.xsa with bitstream) ===
write_hw_platform -include_bit -fixed -force "$out_dir/arty_s7_riscv.xsa"