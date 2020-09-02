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

    // ��bmp��С
    fseek(img_pFile, 0, SEEK_END);
    img_size = ftell(img_pFile);
    rewind(img_pFile);

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

    //�������
    uint8_t* ptr_buffer_flip;
    ptr_buffer_flip = new uint8_t[img_size - BMP_OFFSET];

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



// terminate
fclose(img_pFile);
delete[] ptr_buffer;
return 0;
}