Name:
=====
 nemagfx_enhanced_stress_test


Description:
============
 NemaGFX example.

This example uses MSPI, IOM, DMA, GPU, and DC to read/write data from different
ram sections to put enormous pressure on the AXI bus.
The GUI task also demonstrates how to use the partial frame buffer and ping-pong
buffer features in a pipeline workflow for DMA, GPU, and DC. In GUI task,
DMA copy texture from external PSRAM to internal SSRAM or ExtendedRAM,
GPU render these textures to frame buffer, and DC transfer the framebuffer to
display panel, these three steps are organized in a pipeline to make DMA, GPU,
and DC work in parallel.

SMALLFB
undefine this to disable partial framebuffer features
SMALLFB_STRIPES
controls how many stripes to divide the whole framebuffer

Note: This example needs PSRAM devices connected to MSPI0, if you encounter
hardfault, please check your PSRAM setting.You are supposed to see a digital
Quartz clock if GUI task runs successfully.


******************************************************************************


