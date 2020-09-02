#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

//#include "conv_core.h"
#include "bmp_read.h"
#include "variables.h"

using namespace std;
constexpr auto BMP_OFFSET = 1078;




int main() {
    FILE* img_pFile;
    size_t img_size;
    size_t result;

    img_pFile = fopen("1.bmp", "rb");
    if (img_pFile == NULL) { fputs("File error", stderr); exit(1); }

    // 读bmp大小
    fseek(img_pFile, 0, SEEK_END);//把指针放去末尾
    img_size = ftell(img_pFile);//读出指针的位置，也就是这个文件的byte数量
    rewind(img_pFile);//将指针放回开头

    // 开块动态数组
    uint8_t* ptr_buffer;
    ptr_buffer = new uint8_t[img_size - BMP_OFFSET];
    if (ptr_buffer == NULL) { fputs("Memory error", stderr); exit(2); }

    //跳1078
    fseek(img_pFile, BMP_OFFSET, SEEK_SET);

    //将bmp从1078位以后的内容读去动态数组ptr_buffer
    result = fread(ptr_buffer, sizeof(uint8_t), img_size - BMP_OFFSET, img_pFile);//result是正确读到的元素数量
    if (result != img_size - BMP_OFFSET) { fputs("Reading error", stderr); exit(3); }
    cout << result << endl;

    //开辟个存正确顺序的图的空间
    uint8_t* ptr_buffer_flip;
    ptr_buffer_flip = new uint8_t[img_size - BMP_OFFSET];

    //输出内容
    for (int i = 0; i < 28; i++) {//row
        for (int j = 0; j < 28; j++) {//colume
            cout <<setw(5)<< (int)(ptr_buffer[i * 28 + j]);
    //正过来存
            ptr_buffer_flip[(26-i)*28+j] = ptr_buffer[i*28+j];
        }
        cout <<endl;
    }
        cout <<endl;

    //再输出一遍 
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28; j++) {
            cout << setw(5) << (int)(ptr_buffer_flip[i * 28 + j]);
        }
        cout << endl;
    }
    /**********annotation**********
    * ↓看一眼卷积和全连接用的核
    * ********************/
    //cout << "读取第一次用的卷积核" << endl;
    //readkernelsfile_2_float("W_conv1.bin",  KERNEL_HEIGHT1, KERNEL_WIDTH1,IN_CH1, OUT_CH1);
    //cout << "读取第二次用的卷积核" << endl;
    readkernelsfile_2_float("W_conv2.bin",  KERNEL_HEIGHT2, KERNEL_WIDTH2,IN_CH2, OUT_CH2);
    //cout << "读取第一次全连接用的卷积核" << endl;
    //readkernelsfile_2_float("W_fc1.bin",  KERNEL_HEIGHT3, KERNEL_WIDTH3, IN_CH3, OUT_CH3);
    //cout << "读取第二次全连接用的卷积核" << endl;
    //readkernelsfile_2_float("W_fc2.bin",  KERNEL_HEIGHT4, KERNEL_WIDTH4, IN_CH4, OUT_CH4);
    //test_conv(32,14,14,3,3,1,1,1,1,28,28,16);
   
    /**********annotation**********
* ↓看一眼偏置
* ********************/
    //cout << "读取第一次卷积用的偏置" << endl;
    //readbiasfile_to_float("b_conv1.bin", OUT_CH1);
    //cout << "读取第二次卷积用的偏置" << endl;
    //readbiasfile_to_float("b_conv2.bin", OUT_CH2);
    //cout << "读取第一次全连接用的偏置" << endl;
    //readbiasfile_to_float("b_fc1.bin", OUT_CH3);
    //cout << "读取第一次全连接用的偏置" << endl;
    //readbiasfile_to_float("b_fc2.bin", OUT_CH4);


/**********annotation**********
* ↓
* ********************/


/**********annotation**********
* ↓
* ********************/


// terminate
fclose(img_pFile);






delete[] ptr_buffer;
return 0;
}

/**********annotation**********
* ↓把python里面的readbinfile翻译成C++,就是从bin文件里每次读32bit，读出来按照float格式放到一个动态数组里面，C++函数返回动态数组的指针就好，
* ********************/
float* readkernelsfile_2_float(const char* filepath,int hight,int width, int channel_in,int channel_out) {
    FILE* bin_pFile;
    size_t bin_size;
    size_t bin_size_float;
    size_t result;

    //bin_pFile = fopen("\\data\\W_conv1.bin", "rb");
    bin_pFile = fopen(filepath, "rb");
    if (bin_pFile == NULL) { fputs("File error", stderr); exit(1); }

    // 读bin大小(byte个数)
    fseek(bin_pFile, 0, SEEK_END);
    bin_size = ftell(bin_pFile);
    rewind(bin_pFile);

    //算bin内容按float来算的个数 
    bin_size_float = bin_size / 4;

    // 开块动态数组
    float* ptr_buffer;
    ptr_buffer = new float[bin_size_float];//bin_size是以byte为单位的，要转成float型，就要/4
    if (ptr_buffer == NULL) { fputs("Memory error", stderr); exit(2); }

    //将bin内容读去动态数组ptr_buffer
    result = fread(ptr_buffer, sizeof(float), bin_size_float, bin_pFile);//result是正确读到的元素数量,(读去哪，一次读多少byte，共读多少次，用哪个指针去读)
    if (result != bin_size_float) { fputs("Reading error", stderr); exit(3); }
    cout << result << endl;

    //输出内容
    //Feature [H][W][CHin]
    //kernel: [Ky][Kx][CHin][CHout]
    //Dtype_mul tp=feature_in[h][w][cin]*w[ii][jj][cin][cout];

    for (int u = 0; u < channel_out; u++) {//channel out
        for (int k = 0; k < channel_in; k++) {//channel out
            for (int i = 0; i < hight; i++) {//row
                for (int j = 0; j < width; j++) {//colume
                    cout << setw(20) << (ptr_buffer[u*channel_in*hight*width + k * hight * width + i * hight + j]);
                }
                cout <<endl;
            }
            cout << "    CHin=" << k << "    CHout=" << u << endl;
                //cout << u * channel_in + k << endl;
        }
    }
    return ptr_buffer;
}

float* readbiasfile_to_float(const char* filepath, int channel_out) {
    FILE* bin_pFile;
    size_t bin_size;
    size_t bin_size_float;
    size_t result;

    //bin_pFile = fopen("\\data\\W_conv1.bin", "rb");
    bin_pFile = fopen(filepath, "rb");
    if (bin_pFile == NULL) { fputs("File error", stderr); exit(1); }

    // 读bin大小(byte个数)
    fseek(bin_pFile, 0, SEEK_END);
    bin_size = ftell(bin_pFile);
    rewind(bin_pFile);

    //算bin内容按float来算的个数 
    bin_size_float = bin_size / 4;

    // 开块动态数组
    float* ptr_buffer;
    ptr_buffer = new float[bin_size_float];//bin_size是以byte为单位的，要转成float型，就要/4
    if (ptr_buffer == NULL) { fputs("Memory error", stderr); exit(2); }

    //将bin内容读去动态数组ptr_buffer
    result = fread(ptr_buffer, sizeof(float), bin_size_float, bin_pFile);//result是正确读到的元素数量,(读去哪，一次读多少byte，共读多少次，用哪个指针去读)
    if (result != bin_size_float) { fputs("Reading error", stderr); exit(3); }
    cout << result << endl;

    //输出内容
    for (int u = 0; u < channel_out; u++) {//channel out
                    cout << setw(20) << (ptr_buffer[u])<<endl;
                }
    return ptr_buffer;


}


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
) 
{

    for (int cout = 0; cout < CHout; cout++)
        for (int i = 0; i < Hout; i++)//对输出图像H W做循环
            for (int j = 0; j < Wout; j++)
            {
                float sum = 0;
                for (int ii = 0; ii < Ky; ii++)//对核kx ky做循环
                    for (int jj = 0; jj < Kx; jj++)
                    {
                        int h = i * Sy - pad_y + ii;//定下在输出图像地盘上要计算的位置h 和 w
                        int w = j * Sx - pad_x + jj;
                        if (h >= 0 && w >= 0 && h < Hin && w < Win)
                        {
                            for (int cin = 0; cin < CHin; cin++)//多个核的时候，就要每个核对应每个CHin下标的同样图像做卷积
                            {
                                //Feature [H][W][C]
                                //kernel: [Ky][Kx][CHin][CHout]
                                //Dtype_mul tp=feature_in[h][w][cin]*w[ii][jj][cin][cout];
                                std::cout<<"h:"<<h<<",w"<<w<<",cin"<<cin<<"\n";
                                std::cout<<"feature_in["<<h*CHin*Win+w*CHin+cin<<"]*W["<<ii*Kx*CHin*CHout+jj*CHin*CHout+cin*CHout+cout<<"]\n";
                                //float tp = feature_in[h * CHin * Win + w * CHin + cin] * W[ii * Kx * CHin * CHout + jj * CHin * CHout + cin * CHout + cout];//算一次tp就是做一次卷积中的相乘
                                //sum += tp;//sum是卷积核和对应图像中小矩阵全部对应元素相乘后相加的结果
                            }
                        }
                    }

                //sum += bias[cout];
                //if (relu_en & sum < 0)//relu函数就是负数变0，正数保留，所以这里relu_en就是个开关
                //    sum = 0;
                ////feature_out[i][j][cout]=sum;
                //feature_out[i * Wout * CHout + j * CHout + cout] = sum;
            }


}
*/
