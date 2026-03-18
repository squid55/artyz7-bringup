connect -url tcp:127.0.0.1:3121

targets -set -filter {name =~ "xc7z010"}
fpga -file {/home/hyeonjun/AMD/Vivado/source/test_/test_platform/export/test_platform/hw/design_1_wrapper.bit}

targets -set -filter {name =~ "APU*"}
loadhw -hw /home/hyeonjun/AMD/Vivado/source/test_/test_platform/export/test_platform/hw/design_1_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
source /home/hyeonjun/AMD/Vivado/source/test_/test_app/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config

targets -set -filter {name =~ "*A9*#0"}
rst -processor
dow /home/hyeonjun/AMD/Vivado/source/test_/test_app/build/test_app.elf
configparams force-mem-access 0
con

targets
