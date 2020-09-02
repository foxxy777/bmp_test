#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
using namespace std;
constexpr auto BMP_OFFSET = 1078;


int main() {
    FILE* img_pFile;
    size_t img_size;
    uint8_t* buffer;
    size_t result;

    img_pFile = fopen("C:\\Users\\foxxy\\Desktop\\1.bin", "rb");
    if (img_pFile == NULL) { fputs("File error", stderr); exit(1); }

    // 读bmp大小
    fseek(img_pFile, 0, SEEK_END);
    img_size = ftell(img_pFile);
    rewind(img_pFile);

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

    //输出内容
    uint8_t* ptr_buffer_flip;
    ptr_buffer_flip = new uint8_t[img_size - BMP_OFFSET];

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



// terminate
fclose(img_pFile);
delete[] ptr_buffer;
return 0;
}