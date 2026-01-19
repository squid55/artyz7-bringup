###############################################################################
## Arty Z7-10 Rev.B - Bring-up Minimal XDC
## Purpose:
##  - System clock verification
##  - Single LED (LD0) output verification
##  - JTAG / bitstream download sanity check
###############################################################################

########################
## Clock Signal (SYSCLK)
########################
# Arty Z7-10 system clock: 125 MHz
set_property -dict { PACKAGE_PIN H16 IOSTANDARD LVCMOS33 } [get_ports { clk }]
create_clock -add -name sys_clk_pin -period 8.000 -waveform {0 4} [get_ports { clk }]

########################
## User LED (LD0)
########################
# LD0
set_property -dict { PACKAGE_PIN R14 IOSTANDARD LVCMOS33 } [get_ports { led[0] }]

###############################################################################
## End of file
###############################################################################
