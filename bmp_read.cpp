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

    // ��bmp��С
    fseek(img_pFile, 0, SEEK_END);//��ָ���ȥĩβ
    img_size = ftell(img_pFile);//����ָ���λ�ã�Ҳ��������ļ���byte����
    rewind(img_pFile);//��ָ��Żؿ�ͷ

    // ���鶯̬����
    uint8_t* ptr_buffer;
    ptr_buffer = new uint8_t[img_size - BMP_OFFSET];
    if (ptr_buffer == NULL) { fputs("Memory error", stderr); exit(2); }

    //��1078
    fseek(img_pFile, BMP_OFFSET, SEEK_SET);

    //��bmp��1078λ�Ժ�����ݶ�ȥ��̬����ptr_buffer
    result = fread(ptr_buffer, sizeof(uint8_t), img_size - BMP_OFFSET, img_pFile);//result����ȷ������Ԫ������
    if (result != img_size - BMP_OFFSET) { fputs("Reading error", stderr); exit(3); }
    cout << result << endl;

    //���ٸ�����ȷ˳���ͼ�Ŀռ�
    uint8_t* ptr_buffer_flip;
    ptr_buffer_flip = new uint8_t[img_size - BMP_OFFSET];

    //�������
    for (int i = 0; i < 28; i++) {//row
        for (int j = 0; j < 28; j++) {//colume
            cout <<setw(5)<< (int)(ptr_buffer[i * 28 + j]);
    //��������
            ptr_buffer_flip[(26-i)*28+j] = ptr_buffer[i*28+j];
        }
        cout <<endl;
    }
        cout <<endl;

    //�����һ�� 
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28; j++) {
            cout << setw(5) << (int)(ptr_buffer_flip[i * 28 + j]);
        }
        cout << endl;
    }
    /**********annotation**********
    * ����һ�۾����ȫ�����õĺ�
    * ********************/
    //cout << "��ȡ��һ���õľ����" << endl;
    //readkernelsfile_2_float("W_conv1.bin",  KERNEL_HEIGHT1, KERNEL_WIDTH1,IN_CH1, OUT_CH1);
    //cout << "��ȡ�ڶ����õľ����" << endl;
    readkernelsfile_2_float("W_conv2.bin",  KERNEL_HEIGHT2, KERNEL_WIDTH2,IN_CH2, OUT_CH2);
    //cout << "��ȡ��һ��ȫ�����õľ����" << endl;
    //readkernelsfile_2_float("W_fc1.bin",  KERNEL_HEIGHT3, KERNEL_WIDTH3, IN_CH3, OUT_CH3);
    //cout << "��ȡ�ڶ���ȫ�����õľ����" << endl;
    //readkernelsfile_2_float("W_fc2.bin",  KERNEL_HEIGHT4, KERNEL_WIDTH4, IN_CH4, OUT_CH4);
    //test_conv(32,14,14,3,3,1,1,1,1,28,28,16);
   
    /**********annotation**********
* ����һ��ƫ��
* ********************/
    //cout << "��ȡ��һ�ξ���õ�ƫ��" << endl;
    //readbiasfile_to_float("b_conv1.bin", OUT_CH1);
    //cout << "��ȡ�ڶ��ξ���õ�ƫ��" << endl;
    //readbiasfile_to_float("b_conv2.bin", OUT_CH2);
    //cout << "��ȡ��һ��ȫ�����õ�ƫ��" << endl;
    //readbiasfile_to_float("b_fc1.bin", OUT_CH3);
    //cout << "��ȡ��һ��ȫ�����õ�ƫ��" << endl;
    //readbiasfile_to_float("b_fc2.bin", OUT_CH4);


/**********annotation**********
* ��
* ********************/


/**********annotation**********
* ��
* ********************/


// terminate
fclose(img_pFile);






delete[] ptr_buffer;
return 0;
}

/**********annotation**********
* ����python�����readbinfile�����C++,���Ǵ�bin�ļ���ÿ�ζ�32bit������������float��ʽ�ŵ�һ����̬�������棬C++�������ض�̬�����ָ��ͺã�
* ********************/
float* readkernelsfile_2_float(const char* filepath,int hight,int width, int channel_in,int channel_out) {
    FILE* bin_pFile;
    size_t bin_size;
    size_t bin_size_float;
    size_t result;

    //bin_pFile = fopen("\\data\\W_conv1.bin", "rb");
    bin_pFile = fopen(filepath, "rb");
    if (bin_pFile == NULL) { fputs("File error", stderr); exit(1); }

    // ��bin��С(byte����)
    fseek(bin_pFile, 0, SEEK_END);
    bin_size = ftell(bin_pFile);
    rewind(bin_pFile);

    //��bin���ݰ�float����ĸ��� 
    bin_size_float = bin_size / 4;

    // ���鶯̬����
    float* ptr_buffer;
    ptr_buffer = new float[bin_size_float];//bin_size����byteΪ��λ�ģ�Ҫת��float�ͣ���Ҫ/4
    if (ptr_buffer == NULL) { fputs("Memory error", stderr); exit(2); }

    //��bin���ݶ�ȥ��̬����ptr_buffer
    result = fread(ptr_buffer, sizeof(float), bin_size_float, bin_pFile);//result����ȷ������Ԫ������,(��ȥ�ģ�һ�ζ�����byte���������ٴΣ����ĸ�ָ��ȥ��)
    if (result != bin_size_float) { fputs("Reading error", stderr); exit(3); }
    cout << result << endl;

    //�������
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

    // ��bin��С(byte����)
    fseek(bin_pFile, 0, SEEK_END);
    bin_size = ftell(bin_pFile);
    rewind(bin_pFile);

    //��bin���ݰ�float����ĸ��� 
    bin_size_float = bin_size / 4;

    // ���鶯̬����
    float* ptr_buffer;
    ptr_buffer = new float[bin_size_float];//bin_size����byteΪ��λ�ģ�Ҫת��float�ͣ���Ҫ/4
    if (ptr_buffer == NULL) { fputs("Memory error", stderr); exit(2); }

    //��bin���ݶ�ȥ��̬����ptr_buffer
    result = fread(ptr_buffer, sizeof(float), bin_size_float, bin_pFile);//result����ȷ������Ԫ������,(��ȥ�ģ�һ�ζ�����byte���������ٴΣ����ĸ�ָ��ȥ��)
    if (result != bin_size_float) { fputs("Reading error", stderr); exit(3); }
    cout << result << endl;

    //�������
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
        for (int i = 0; i < Hout; i++)//�����ͼ��H W��ѭ��
            for (int j = 0; j < Wout; j++)
            {
                float sum = 0;
                for (int ii = 0; ii < Ky; ii++)//�Ժ�kx ky��ѭ��
                    for (int jj = 0; jj < Kx; jj++)
                    {
                        int h = i * Sy - pad_y + ii;//���������ͼ�������Ҫ�����λ��h �� w
                        int w = j * Sx - pad_x + jj;
                        if (h >= 0 && w >= 0 && h < Hin && w < Win)
                        {
                            for (int cin = 0; cin < CHin; cin++)//����˵�ʱ�򣬾�Ҫÿ���˶�Ӧÿ��CHin�±��ͬ��ͼ�������
                            {
                                //Feature [H][W][C]
                                //kernel: [Ky][Kx][CHin][CHout]
                                //Dtype_mul tp=feature_in[h][w][cin]*w[ii][jj][cin][cout];
                                std::cout<<"h:"<<h<<",w"<<w<<",cin"<<cin<<"\n";
                                std::cout<<"feature_in["<<h*CHin*Win+w*CHin+cin<<"]*W["<<ii*Kx*CHin*CHout+jj*CHin*CHout+cin*CHout+cout<<"]\n";
                                //float tp = feature_in[h * CHin * Win + w * CHin + cin] * W[ii * Kx * CHin * CHout + jj * CHin * CHout + cin * CHout + cout];//��һ��tp������һ�ξ���е����
                                //sum += tp;//sum�Ǿ���˺Ͷ�Ӧͼ����С����ȫ����ӦԪ����˺���ӵĽ��
                            }
                        }
                    }

                //sum += bias[cout];
                //if (relu_en & sum < 0)//relu�������Ǹ�����0��������������������relu_en���Ǹ�����
                //    sum = 0;
                ////feature_out[i][j][cout]=sum;
                //feature_out[i * Wout * CHout + j * CHout + cout] = sum;
            }


}
*/
