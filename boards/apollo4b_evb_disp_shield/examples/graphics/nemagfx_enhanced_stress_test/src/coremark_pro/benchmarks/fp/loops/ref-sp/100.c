/**** START DATASET ****/
#include "th_lib.h"
#include "../loops.h"
static intparts ref_data_3[]={
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,0,0x00000000,0x0080b182}/*1.005417108535766602e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{1,-2,0x00000000,0x00d357b4}/*-4.127784967422485352e-01*/,
	{1,0,0x00000000,0x00a78f8c}/*-1.309068202972412109e+00*/,
	{0,3,0x00000000,0x00bde177}/*1.186754512786865234e+01*/,
	{0,0,0x00000000,0x00832240}/*1.024482727050781250e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,2,0x00000000,0x00d7a000}/*6.738281250000000000e+00*/,
	{0,-2,0x00000000,0x00df3619}/*4.359596073627471924e-01*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,0,0x00000000,0x0080d886}/*1.006607770919799805e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{1,0,0x00000000,0x00d33333}/*-1.649999976158142090e+00*/,
	{0,0,0x00000000,0x008cfff8}/*1.101561546325683594e+00*/,
	{0,0,0x00000000,0x00000000}/*0.000000000000000000e+00*/,
	{0,12,0x00000000,0x009c4000}/*5.000000000000000000e+03*/,
	{0,12,0x00000000,0x009cde77}/*5.019808105468750000e+03*/
}; /* ref_data */

void init_preset_3_100() {
presets_loops[3].seed=0x4645ca3;
presets_loops[3].N=0x64;
presets_loops[3].tests=0x0fe8e785;
presets_loops[3].Loop=0x64;
presets_loops[3].minbits=14;
presets_loops[3].rtype=0;
presets_loops[3].limit_int_input=0;
presets_loops[3].reinit_step=0x1;
presets_loops[3].ref_data=ref_data_3;
}
/**** END DATASET ****/

