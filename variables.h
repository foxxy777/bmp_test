/**********annotation**********
 * ����һ�ξ���Ĳ���,MODE1=1:Ҫpadding��RELU_EN1=1:Ҫrelu
 * ********************/
//Conv1
#define    IN_WIDTH1 28
#define    IN_HEIGHT1 28
#define    IN_CH1 1
#define    KERNEL_WIDTH1 3
#define    KERNEL_HEIGHT1 3
#define    X_STRIDE1 1
#define    Y_STRIDE1 1
#define    RELU_EN1 1 //relu ���ǰѾ������õ���ÿ����������ϵ�Ԫ�أ�������0����������
#define    MODE1 1  //0:��Ҫpadding, 1 :Ҫpadding��padding������Ϻ��Ϊ�˵�һ�볤�� 

#if MODE1 //��MODE1��ֵ����PADINGG��ֵ
#define X_PADDING1 = ((KERNEL_WIDTH1 - 1) / 2)//�������1
#define Y_PADDING1 = ((KERNEL_HEIGHT1 - 1) / 2)
#else
#define X_PADDING1 = 0
#define Y_PADDING1 = 0
#endif 

#define    OUT_WIDTH1  ((IN_WIDTH1 + 2 * X_PADDING1 - KERNEL_WIDTH1) / X_STRIDE1 + 1) // ����������������С��padding��ȣ�����һ�������//�������28
#define    OUT_HEIGHT1  ((IN_HEIGHT1 + 2 * Y_PADDING1 - KERNEL_HEIGHT1) / Y_STRIDE1 + 1)
#define    OUT_CH1  16

/**********annotation**********
 * ����һ�γػ��Ĳ�������ȡmax��Ϊ�ػ�����
 * ********************/
//Pool1
#define    MODE11  2  //mode: 0 : MEAN, 1 : MIN, 2 : MAX
#define    IN_WIDTH11  OUT_WIDTH1
#define    IN_HEIGHT11  OUT_HEIGHT1
#define    IN_CH11  OUT_CH1
#define    KERNEL_WIDTH11  2
#define    KERNEL_HEIGHT11  2
#define    OUT_CH11  IN_CH11//16
#define    OUT_WIDTH11  (IN_WIDTH11 / KERNEL_WIDTH11)//14            //16��28*28�������ȡ���ֵ�ػ������Ϊ16��14*14�ľ���
#define    OUT_HEIGHT11  (IN_HEIGHT11 / KERNEL_HEIGHT11)
/**********annotation**********
 * ���ڶ��ξ���Ĳ�����MODE2=1:Ҫpadding��RELU_EN2=1:Ҫrelu
 * ********************/
//Conv2
#define    IN_WIDTH2  OUT_WIDTH11
#define    IN_HEIGHT2  OUT_HEIGHT11
#define    IN_CH2  OUT_CH11//�ڶ��ξ�����ý�16������
#define    KERNEL_WIDTH2  3
#define    KERNEL_HEIGHT2  3
#define    X_STRIDE2  1
#define    Y_STRIDE2  1
#define    RELU_EN2  1
#define    MODE2  1

#if MODE2 
#define X_PADDING2 = ((KERNEL_WIDTH2 - 1) / 2)//�������1
#define Y_PADDING2 = ((KERNEL_HEIGHT2 - 1) / 2)
#else
#define X_PADDING2 = 0
#define Y_PADDING2 = 0
#endif 

#define    OUT_WIDTH2  ((IN_WIDTH2 + 2 * X_PADDING2 - KERNEL_WIDTH2) / X_STRIDE2 + 1)//���ﻹ�ǵ���14
#define    OUT_HEIGHT2  ((IN_HEIGHT2 + 2 * Y_PADDING2 - KERNEL_HEIGHT2) / Y_STRIDE2 + 1)
#define    OUT_CH2  32//�ڶ��ξ�����32������

/**********annotation**********
 * ���ڶ��γػ��Ĳ��� ����ȡmax��Ϊ�ػ�����
 * ********************/
//Pool2
#define    MODE21  2  //mode: 0 : MEAN, 1 : MIN, 2 : MAX
#define    IN_WIDTH21  OUT_WIDTH2
#define    IN_HEIGHT21  OUT_HEIGHT2
#define    IN_CH21  OUT_CH2//32
#define    KERNEL_WIDTH21  2
#define    KERNEL_HEIGHT21  2
#define    OUT_CH21  IN_CH21//32
#define    OUT_WIDTH21  (IN_WIDTH21 / KERNEL_WIDTH21)//7            //һ��16*32=512��14*14������󣬵ڶ��γػ�֮�󣬵õ�512��7*7�ľ���
#define    OUT_HEIGHT21  (IN_HEIGHT21 / KERNEL_HEIGHT21)
/**********annotation**********
 * �� ��һ��ȫ���ӵĲ�����ȫ���ӣ�����Ȼ�ģ�����padding��Ҫrelu
 * ********************/
//Fc1
#define    IN_WIDTH3  OUT_WIDTH21//7
#define    IN_HEIGHT3  OUT_HEIGHT21
#define    IN_CH3  OUT_CH21//32
#define    KERNEL_WIDTH3  7
#define    KERNEL_HEIGHT3  7
#define    X_STRIDE3  1
#define    Y_STRIDE3  1
#define    RELU_EN3  1
#define    MODE3  0

#if MODE3 
#define X_PADDING4 = ((KERNEL_WIDTH4 - 1) / 2)
#define Y_PADDING4 = ((KERNEL_HEIGHT4 - 1) / 2)
#else
#define X_PADDING3 = 0
#define Y_PADDING3 = 0
#endif

#define    OUT_WIDTH3  ((IN_WIDTH3 + 2 * X_PADDING3 - KERNEL_WIDTH3) / X_STRIDE3 + 1)//1
#define    OUT_HEIGHT3  ((IN_HEIGHT3 + 2 * Y_PADDING3 - KERNEL_HEIGHT3) / Y_STRIDE3 + 1)
#define    OUT_CH3  128//

/**********annotation**********
 * �� �ڶ���ȫ���ӵĲ�����ȫ���ӣ�����Ȼ�ģ�����padding��Ҫrelu
 * ********************/
//Fc2
#define     IN_WIDTH4  OUT_WIDTH3//1
#define     IN_HEIGHT4  OUT_HEIGHT3
#define     IN_CH4  OUT_CH3//128
#define     KERNEL_WIDTH4  1
#define     KERNEL_HEIGHT4  1
#define     X_STRIDE4  1
#define     Y_STRIDE4  1
#define     RELU_EN4  1
#define     MODE4  0

#if MODE4 
#define X_PADDING4 = ((KERNEL_WIDTH4 - 1) / 2)
#define Y_PADDING4 = ((KERNEL_HEIGHT4 - 1) / 2)
#else
#define X_PADDING4 = 0
#define Y_PADDING4 = 0
#endif 

#define     OUT_WIDTH4  ((IN_WIDTH4 + 2 * 0 - KERNEL_WIDTH4) / X_STRIDE4 + 1)   //  1
#define     OUT_HEIGHT4  ((IN_HEIGHT4 + 2 * 0 - KERNEL_HEIGHT4) / Y_STRIDE4 + 1)
#define     OUT_CH4  10
