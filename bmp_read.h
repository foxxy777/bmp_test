#pragma once

float* readkernelsfile_2_float(const char* filepath, int hight, int width, int channel_in, int channel_out);

float* readbiasfile_to_float(const char* filepath, int channel_out);

/*
void test_conv(
    int CHout,
    int Hout,
    int Wout,
    int Ky,
    int Kx,
    int Sy,
    int Sx,
    int pad_y,
    int pad_x,
    int Hin,
    int Win,
    int CHin
);
*/
