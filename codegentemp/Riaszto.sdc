# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\David\Documents\PSoC Creator\Keypadtest\Riaszto.cydsn\Riaszto.cyprj
# Date: Fri, 14 May 2021 18:15:46 GMT
#set_units -time ns
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {SPIM_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 13 25} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {ADC_SAR_PR_theACLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 23 47} -nominal_period 958.33333333333326 [list [get_pins {ClockBlock/aclk_glb_0}]]
create_generated_clock -name {ADC_SAR_RADAR_theACLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 25 49} [list [get_pins {ClockBlock/aclk_glb_1}]]
create_generated_clock -name {UART_GSM_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 313 627} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {UART_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 313 627} [list [get_pins {ClockBlock/dclk_glb_2}]]
create_generated_clock -name {pwn1_clk} -source [get_pins {ClockBlock/clk_sync}] -edges {1 401 799} -nominal_period 16666.666666666664 [list [get_pins {ClockBlock/dclk_glb_3}]]
create_generated_clock -name {timer_clock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 401 799} -nominal_period 16666.666666666664 [list [get_pins {ClockBlock/dclk_glb_4}]]
create_generated_clock -name {beeper_clk} -source [get_pins {ClockBlock/clk_sync}] -edges {1 1201 2401} [list [get_pins {ClockBlock/dclk_glb_5}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Users\David\Documents\PSoC Creator\Keypadtest\Riaszto.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\David\Documents\PSoC Creator\Keypadtest\Riaszto.cydsn\Riaszto.cyprj
# Date: Fri, 14 May 2021 18:15:34 GMT
