#include "conv_core.h"
#include "stdio.h"
//Feature: [H][W][C]
//kernel: [Ky][Kx][CHin][CHout]

void Conv(ap_uint<16> CHin,//这几个都是axi接口的输入
        ap_uint<16> Hin,
        ap_uint<16> Win,
        ap_uint<16> CHout,
		ap_uint<8> Kx,
        ap_uint<8> Ky,
        ap_uint<8> Sx,//这个是步长,输出图像的长宽是根据两个方向上的步长来算的
        ap_uint<8> Sy,
        ap_uint<1> mode,
        ap_uint<1> relu_en,
		Dtype_f feature_in[],//都是float数组,这几个是axi接口的输出，输入特征：就是输入图像
        Dtype_w W[],//卷积或全连接的权重
        Dtype_w bias[],//偏置
        Dtype_f feature_out[]//输出图像
	)//mode: 0:VALID, 1:SAME
{
	//#pragma HLS PIPELINE enable_flush
	/*2018版vivado用的depth
	#pragma HLS INTERFACE m_axi depth=4294967295 port=feature_out offset=slave
	#pragma HLS INTERFACE m_axi depth=4294967295 port=bias offset=slave
	#pragma HLS INTERFACE m_axi depth=4294967295 port=W offset=slave
	#pragma HLS INTERFACE m_axi depth=4294967295 port=feature_in offset=slave
	*/
	/*
    #pragma HLS INTERFACE m_axi depth=2147483647 port=feature_out offset=slave//最后生成IP的AXI的master端，把feature_out的内容存在外部存储器中，到时候综合出电路后就会从存储器中取东西
	#pragma HLS INTERFACE m_axi depth=2147483647 port=bias offset=slave//可以看到，在main中，事前把这三个参数写进了RAM，然后这个IP通过master端口主动地取数，这样方便搞多几层，虽然实际这里只用了一层
	#pragma HLS INTERFACE m_axi depth=2147483647 port=W offset=slave
	#pragma HLS INTERFACE m_axi depth=2147483647 port=feature_in offset=slave//offset=slave表示这个master端向外写入时的指针确定指向位置时用的offset参数用slave的offset参数

	#pragma HLS INTERFACE s_axilite port=relu_en//这里配成IP的slave端，意思是这些参数是被PS主动给到这个函数对应的电路的，这些参数相当于电路的input，纯被动地被PS控输入。
	#pragma HLS INTERFACE s_axilite port=CHout
	#pragma HLS INTERFACE s_axilite port=Sx
	#pragma HLS INTERFACE s_axilite port=Hin
	#pragma HLS INTERFACE s_axilite port=CHin
	#pragma HLS INTERFACE s_axilite port=Kx
	#pragma HLS INTERFACE s_axilite port=mode
	#pragma HLS INTERFACE s_axilite port=Sy
	#pragma HLS INTERFACE s_axilite port=Ky
	#pragma HLS INTERFACE s_axilite port=Win
	#pragma HLS INTERFACE s_axilite port=return//意思把return值做成一个端口
	*/

	ap_uint<8> pad_x,pad_y;//外围pad圈的大小，模式0就不要pad，模式1就要pad，pad长宽自然是卷积核的一半

	if(mode==0)
	{
		pad_x=0;pad_y=0;
	}
	else

	{
		pad_x=(Kx-1)/2;pad_y=(Ky-1)/2;
	}

	ap_uint<16> Hout,Wout;
	Wout=(Win+2*pad_x-Kx)/Sx+1;//这就是，输出的长宽由padding和步长来决定
	Hout=(Hin+2*pad_y-Ky)/Sy+1;
//以16CHin，
	for(int cout=0;cout<CHout;cout++)
		for(int i=0;i<Hout;i++)//对输出图像H W做循环
			for(int j=0;j<Wout;j++)
			{
				Dtype_acc sum=0;
				for(int ii=0;ii<Ky;ii++)//对核kx ky做循环
					for(int jj=0;jj<Kx;jj++)
					{
						ap_int<16> h=i*Sy-pad_y+ii;//定下在输出图像地盘上要计算的位置h 和 w
						ap_int<16> w=j*Sx-pad_x+jj;
						if(h>=0 && w>=0 && h<Hin && w<Win)
						{
							for(int cin=0;cin<CHin;cin++)//多个核的时候，就要每个核对应每个CHin下标的同样图像做卷积
							{
								//Feature [H][W][C]
								//kernel: [Ky][Kx][CHin][CHout]
								//Dtype_mul tp=feature_in[h][w][cin]*w[ii][jj][cin][cout];
								//std::cout<<"h:"<<h<<",w"<<w<<",cin"<<cin<<"\n";
								//std::cout<<"feature_in["<<h*CHin*Win+w*CHin+cin<<"]*W["<<ii*Kx*CHin*CHout+jj*CHin*CHout+cin*CHout+cout<<"]\n";
								Dtype_mul tp=feature_in[h*CHin*Win+w*CHin+cin]*W[ii*Kx*CHin*CHout+jj*CHin*CHout+cin*CHout+cout];//算一次tp就是做一次卷积中的相乘
								sum+=tp;//sum是卷积核和对应图像中小矩阵全部对应元素相乘后相加的结果
							}
						}
					}

				sum+=bias[cout];
				if(relu_en & sum<0)//relu函数就是负数变0，正数保留，所以这里relu_en就是个开关
					sum=0;
				//feature_out[i][j][cout]=sum;
				feature_out[i*Wout*CHout+j*CHout+cout]=sum;
			}
}
