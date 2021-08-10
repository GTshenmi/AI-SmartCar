/*
 * attitude.c
 *
 *  Created on: 2021骞�4鏈�18鏃�
 *      Author: 936305695
 */
#include "attitude.h"
#include "math.h"
#include "sysmath.h"
#include "arm_math.h"
#include "include.h"

/*宸ョ▼鍏ㄥ眬鍙橀噺*/

//濮挎�佹洿鏂版爣蹇椕椕嬅屄该济惷偮泵柭�
unsigned char IMU_Flag;

//濮挎�佽
float Roll  , Pitch  , Yaw  ;
float Roll_0, Pitch_0, Yaw_0;

//纾佸満鍚戦噺
static float M[3],  M_cx[3] ,M_cxy[3];
static arm_matrix_instance_f32 M_matrix,M_cx_matrix,M_cxy_matrix;

//缁曡x杞寸殑鏃嬭浆鐭╅樀
arm_matrix_instance_f32 Rx_matrix;
float               Rx[9];

//缁晊杞寸殑鏃嬭浆鐭╅樀
arm_matrix_instance_f32 Ry_matrix;
float               Ry[9];

/*鏂囦欢鍏ㄥ眬鍙橀噺*/

//閲囨牱鍛ㄦ湡
static float dt=0.02;

//
#define Qs   0.000001
static arm_matrix_instance_f32 Q_matrix;
static float               Q[16]={Qs ,0  ,0  ,0  ,
                                      0  ,Qs ,0  ,0  ,
                                      0  ,0  ,Qs ,0  ,
                                      0  ,0  ,0  ,Qs };

//鹿脹虏芒脨颅路陆虏卯戮脴脮贸
#define Rs_a 5000
static arm_matrix_instance_f32 R_matrix;
static float               R[9]= {Rs_a ,0    ,0    ,
                                                            0    ,Rs_a ,0    ,
                                                            0    ,0    ,Rs_a ,};

//脣脛陆脳碌楼脦禄戮脴脮贸
static arm_matrix_instance_f32 I_matrix;
static float                I[16]={1  ,0  ,0  ,0  ,
                                        0  ,1  ,0  ,0  ,
                                        0  ,0  ,1  ,0  ,
                                        0  ,0  ,0  ,1  };

//脳麓脤卢路陆鲁脤脩脜驴脣卤脠戮脴脮贸录掳脝盲脳陋脰脙
static float               F[16],   F_t[16];
static arm_matrix_instance_f32 F_matrix,F_t_matrix;

//鹿脹虏芒路陆鲁脤脩脜驴脣卤脠戮脴脮贸录掳脝盲脳陋脰脙
static float               H[12],   H_t[12];
static arm_matrix_instance_f32 H_matrix, H_t_matrix;

//脳麓脤卢脨颅路陆虏卯脧脿鹿脴戮脴脮贸
static float               P[16],   P_p[16],   P_temp1[16],   P_temp2[16];
static arm_matrix_instance_f32 P_matrix,P_p_matrix,P_temp1_matrix,P_temp2_matrix;

//脌漏脮鹿驴篓露没脗眉脭枚脪忙脧脿鹿脴戮脴脮贸
static float               K[12]   ,K_temp1[12]   ,K_temp2[12]   ,K_temp3[9],               K_temp4[9];
static arm_matrix_instance_f32 K_matrix,K_temp1_matrix,K_temp2_matrix,K_temp3_matrix,       K_temp4_matrix;

//脣脛脭陋脢媒脧脿鹿脴脧貌脕驴
static float               q[4]={1,0,0,0},q_p[4]={1,0,0,0},q_e[4]={0,0,0,0};
static arm_matrix_instance_f32 q_matrix,      q_p_matrix,      q_e_matrix;

//脰脴脕娄脧脿鹿脴脧貌脕驴
static float               Z[3],    Z_p[3],    Z_e[3];
static arm_matrix_instance_f32 Z_matrix,Z_p_matrix,Z_e_matrix;

/**/
float ax=1,ay=1,az=1;
/**/

//鍗″皵鏇艰瀺鍚�
//static float Hignt_Kalman(float X_P,float X_V)
//{
//    static float  P0_0=0,Kg=0.0,P1_0=0;
//    float X;
//    //绯荤粺鍗忔柟宸�
//    P1_0=P0_0+0.0001f;
//    //瑙傛祴鍗忔柟宸�
//    Kg=P1_0/(P1_0+200.0f);
//    X=X_P+(X_V-X_P)*Kg;
//    P0_0=(1-Kg)*P1_0;
//    return X;
//}

//static float Hignt_Speed_Kalman(float X_P,float X_V)//鍙﹀涓�涓崱灏旀浖铻嶅悎
//{
//    static float  P0_0=0,Kg=0.0,P1_0=0;
//    float X;
//    //绯荤粺鍗忔柟宸�
//    P1_0=P0_0+0.0002f;
//    //瑙傛祴鍗忔柟宸�
//    Kg=P1_0/(P1_0+100.0f);
//    X=X_P+(X_V-X_P)*Kg;
//    P0_0=(1-Kg)*P1_0;
//    return X;
//}

void IMU_Update_Init(void)
{
    //float norm;
    //杩囩▼鍗忔柟宸煩闃�
    Q_matrix.numRows=4;
    Q_matrix.numCols=4;
    Q_matrix.pData  =Q;

    //瑙傛祴鍗忔柟宸煩闃�
    R_matrix.numRows=3;
    R_matrix.numCols=3;
    R_matrix.pData  =R;

    //鍥涢樁鍗曚綅鐭╅樀
    I_matrix.numRows=4;
    I_matrix.numCols=4;
    I_matrix.pData  =I;

    //X脨媒脳陋戮脴脮贸
    Rx_matrix.numRows=3;
    Rx_matrix.numCols=3;
    Rx_matrix.pData  =Rx;

    //Y脨媒脳陋戮脴脮贸
    Ry_matrix.numRows=3;
    Ry_matrix.numCols=3;
    Ry_matrix.pData  =Ry;

    //脳麓脤卢路陆鲁脤脩脜驴脣卤脠戮脴脮贸录掳脝盲脳陋脰脙
    F_matrix.numRows=4;        F_t_matrix.numRows=4;
    F_matrix.numCols=4;        F_t_matrix.numCols=4;
    F_matrix.pData  =F;        F_t_matrix.pData  =F_t;

    //鹿脹虏芒路陆鲁脤脩脜驴脣卤脠戮脴脮贸录掳脝盲脳陋脰脙
    H_matrix.numRows=3;        H_t_matrix.numRows=4;
    H_matrix.numCols=4;        H_t_matrix.numCols=3;
    H_matrix.pData  =H;        H_t_matrix.pData  =H_t;

    //绯荤粺鍗忔柟宸煩闃靛強绯荤粺鍗忔柟宸娴嬬煩闃�
    P_matrix.numRows=4;         P_p_matrix.numRows=4;          P_temp1_matrix.numRows=4;        P_temp2_matrix.numRows=4;
    P_matrix.numCols=4;         P_p_matrix.numCols=4;          P_temp1_matrix.numCols=4;        P_temp2_matrix.numCols=4;
    P_matrix.pData  =P;         P_p_matrix.pData  =P_p;        P_temp1_matrix.pData  =P_temp1;  P_temp2_matrix.pData  =P_temp2;

    //脌漏脮鹿驴篓露没脗眉脭枚脪忙
    K_matrix.numRows=4;         K_temp1_matrix.numRows=4;      K_temp2_matrix.numRows=3;        K_temp3_matrix.numRows=3;       K_temp4_matrix.numRows=3;
    K_matrix.numCols=3;         K_temp1_matrix.numCols=3;      K_temp2_matrix.numCols=4;        K_temp3_matrix.numCols=3;       K_temp4_matrix.numCols=3;
    K_matrix.pData  =K;         K_temp1_matrix.pData  =K_temp1;K_temp2_matrix.pData  =K_temp2;  K_temp3_matrix.pData  =K_temp3; K_temp4_matrix.pData  =K_temp4;

    //鍥涘厓鏁扮姸鎬佸悜閲忓強鍏堕娴嬪悜閲�
    q_matrix.numRows=4;       q_p_matrix.numRows=4;            q_e_matrix.numRows=4;
    q_matrix.numCols=1;       q_p_matrix.numCols=1;            q_e_matrix.numCols=1;
    q_matrix.pData  =q;       q_p_matrix.pData  =q_p;          q_e_matrix.pData  =q_e;

    //閲嶅姏瑙傛祴鍚戦噺鍙婅娴嬮噺鐨勯娴嬪悜閲�
    Z_matrix.numRows=3;        Z_p_matrix.numRows=3;           Z_e_matrix.numRows=3;
    Z_matrix.numCols=1;        Z_p_matrix.numCols=1;           Z_e_matrix.numCols=1;
    Z_matrix.pData  =Z;        Z_p_matrix.pData  =Z_p;         Z_e_matrix.pData  =Z_e;

    //鍦扮瑙傛祴鍚戦噺鍙婅娴嬮噺鐨勯娴嬪悜閲�
    M_matrix.numRows=3;        M_cx_matrix.numRows=3;       M_cxy_matrix.numRows=3;
    M_matrix.numCols=1;        M_cx_matrix.numCols=1;       M_cxy_matrix.numCols=1;
    M_matrix.pData  =M;        M_cx_matrix.pData  =M_cx;    M_cxy_matrix.pData  =M_cxy;

    //鍦扮瑙傛祴鍚戦噺鍙婅娴嬮噺鐨勯娴嬪悜閲�
    Rx_matrix.numRows=3;
    Rx_matrix.numCols=3;
    Rx_matrix.pData  =Rx;

    //鍦扮瑙傛祴鍚戦噺鍙婅娴嬮噺鐨勯娴嬪悜閲�
    Ry_matrix.numRows=3;
    Ry_matrix.numCols=3;
    Ry_matrix.pData  =Ry;

//    //鏈轰綋鍔犻�熷害鍚戦噺
//    a_b_matrix.numRows=3;        a_b_temp_matrix.numRows=3;
//    a_b_matrix.numCols=1;        a_b_temp_matrix.numCols=1;
//    a_b_matrix.pData  =a_b;      a_b_temp_matrix.pData  =a_b_temp;

    //瑙傛祴閲忚鍙栧強鍗曚綅鍖�****************************************************************************
//    norm = sqrt(ax*ax+ay*ay+az*az);
//    //鍔犻�熷害璇诲彇鍙婂崟浣嶅寲
//    Z[0]=ax/norm;
//    Z[1]=ay/norm;
//    Z[2]=az/norm;
//
//    //鍒濆淇话缈绘粴瑙掑害璁＄畻***************************************************************************
//    Roll_0 =atanf(Z[1]/Z[2]);
//    Pitch_0=asinf(Z[0]);
//    Yaw_0 =0;
//
//    q[0]=cosf(Pitch_0/2)*cosf(Roll_0/2)*cosf(Yaw_0/2)  +sinf(Pitch_0/2)*sinf(Roll_0/2) *sinf(Yaw_0/2);
//    q[1]=cosf(Pitch_0/2)*cosf(Yaw_0/2) *sinf(Roll_0/2) -cosf(Roll_0/2) *sinf(Pitch_0/2)*sinf(Yaw_0/2);
//    q[2]=cosf(Roll_0/2) *cosf(Yaw_0/2) *sinf(Pitch_0/2)+cosf(Pitch_0/2)*sinf(Roll_0/2) *sinf(Yaw_0/2);
//    q[3]=cosf(Pitch_0/2)*cosf(Roll_0/2)*sinf(Yaw_0/2)  -cosf(Yaw_0/2)  *sinf(Pitch_0/2)*sinf(Roll_0/2);

    //Console.WriteLine("Roll_0:%f,Pitch_0:%f,Yaw_0:%f",Roll_0,Pitch_0,Yaw_0);

    //Console.WriteLine("q[0]:%f,q[1]%f,q[2]%f,q[3]",q[0],q[1],q[2],q[3]);

}
//

void IMU_Update0(float gx, float gy, float gz, float ax, float ay, float az,float mx,float my,float mz,float *rollout,float *pitchout,float *yawout)
{
    float norm;

    norm=sqrt(mx*mx+my*my+mz*mz);
    M[0]=mx/norm;
    M[1]=my/norm;
    M[2]=mz/norm;
//    Console.WriteLine("M[0]:%f,M[1]%f,M[2]%f",M[0],M[1],M[2]);
    norm=sqrt(ax*ax+ay*ay+az*az);
    Z[0]=ax/norm;
    Z[1]=ay/norm;
    Z[2]=az/norm;
//    Console.WriteLine("Z[0]:%f,Z[1]%f,Z[2]%f",Z[0],Z[1],Z[2]);
    F[0] = 1        ;F[1] =-gx*dt*0.5f;F[2] =-gy*dt*0.5f;F[3] =-gz*dt*0.5f;
    F[4] =gx*dt*0.5f;F[5] =  1        ;F[6] = gz*dt*0.5f;F[7] =-gy*dt*0.5f;
    F[8] =gy*dt*0.5f;F[9] =-gz*dt*0.5f;F[10]= 1         ;F[11]= gx*dt*0.5f;
    F[12]=gz*dt*0.5f;F[13]= gy*dt*0.5f;F[14]=-gx*dt*0.5f;F[15]= 1         ;

//    Console.WriteLine("------------------1---------------------");
//    Console.WriteArray("float",F,4);
//    Console.WriteArray("float",F_matrix.pData,4);
//    Console.WriteLine(" ");



    arm_mat_trans_f32(&F_matrix,&F_t_matrix);
//    Console.WriteLine("------------------2---------------------");
//    Console.WriteArray("float",F_t_matrix.pData,4);
//    Console.WriteLine(" ");


    arm_mat_mult_f32(&F_matrix,&q_matrix,&q_p_matrix);
//    Console.WriteLine("------------------3---------------------");
//    Console.WriteArray("float",F_matrix.pData,4);
//    Console.WriteArray("float",q_matrix.pData,4);
//    Console.WriteArray("float",q_p_matrix.pData,4);
//    Console.WriteLine(" ");



    norm=sqrt(q_p[0]*q_p[0]+q_p[1]*q_p[1]+q_p[2]*q_p[2]+q_p[3]*q_p[3]);
    q_p[0]/=norm;
    q_p[1]/=norm;
    q_p[2]/=norm;
    q_p[3]/=norm;

    for(int i = 0 ; i <= 3; i++)
    {
        q[i] = q_p[i];
    }

//    Console.WriteLine("------------------4---------------------");
//    Console.WriteLine("q_p[0]:%f,q_p[1]%f,q_p[2]%f,q_p[3]%f",q_p[0],q_p[1],q_p[2],q_p[3]);
//    Console.WriteLine(" ");

//    //璁＄畻棰勬祴閲忓強鍏堕泤鍏嬫瘮鐭╅樀***********************************************************************
//    Z_p[0]= 2*(q_p[0]*q_p[2] -q_p[1]*q_p[3]);                            //閲嶅姏鍔犻�熷害棰勬祴脰脴脕娄录脫脣脵露脠脭陇虏芒
//    Z_p[1]=-2*(q_p[0]*q_p[1] +q_p[2]*q_p[3]);
//    Z_p[2]=    q_p[1]*q_p[1] +q_p[2]*q_p[2] -(q_p[0]*q_p[0]) -(q_p[3]*q_p[3]);
//    norm=sqrt(Z_p[0]*Z_p[0]+Z_p[1]*Z_p[1]+Z_p[2]*Z_p[2]);      //碌楼脦禄禄炉
//    Z_p[0]/=norm;
//    Z_p[1]/=norm;
//    Z_p[2]/=norm;
//
//    Console.WriteLine("------------------5---------------------");
//    Console.WriteLine("Z_p[0]:%f,Z_p[1]%f,Z_p[2]%f",Z_p[0],Z_p[1],Z_p[2]);
//    Console.WriteLine(" ");
//
//
//    H[0 ]= 2*q_p[2];   H[ 1]=-2*q_p[3];   H[ 2]= 2*q_p[0];   H[ 3]=-2*q_p[1];//脟贸H
//    H[4 ]=-2*q_p[1];   H[ 5]=-2*q_p[0];   H[ 6]=-2*q_p[3];   H[ 7]=-2*q_p[2];
//    H[8 ]=-2*q_p[0];   H[ 9]= 2*q_p[1];   H[10]= 2*q_p[2];   H[11]=-2*q_p[3];
//
//
//    arm_mat_trans_f32(&H_matrix,&H_t_matrix);//脟贸H碌脛脳陋脰脙H_t
//
////
//    Console.WriteLine("------------------6---------------------");
//    Console.WriteArray("float",H_matrix.pData,12);
//    Console.WriteLine("------------------6T---------------------");
//    Console.WriteArray("float",H_t_matrix.pData,12);
//    Console.WriteLine(" ");
//
//
////    //鎵╁睍鍗″皵鏇兼护娉㈠櫒鍙傛暟鏇存柊***********************************************************************
//    arm_mat_mult_f32(&F_matrix,&P_matrix,&P_temp1_matrix);        //脭陇虏芒P_p
//
//    Console.WriteLine("------------------7mult11---------------------");
//    Console.WriteArray("float",F_matrix.pData,4);
//    Console.WriteLine(" ");
//    Console.WriteLine("------------------7mult12---------------------");
//    Console.WriteArray("float",P_matrix.pData,4);
//    Console.WriteLine(" ");
//    Console.WriteLine("------------------7mult13---------------------");
//    Console.WriteArray("float",P_temp1_matrix.pData,4);
//    Console.WriteLine(" ");
//
//    arm_mat_mult_f32(&P_temp1_matrix,&F_t_matrix,&P_temp2_matrix);
//
//    Console.WriteLine("------------------7mult2---------------------");
//    Console.WriteArray("float",P_temp2_matrix.pData,4);
//    Console.WriteLine(" ");
//
//    arm_mat_add_f32(&P_temp2_matrix,&Q_matrix,&P_p_matrix);
//
//    Console.WriteLine("------------------7add1---------------------");
//    Console.WriteArray("float",P_p_matrix.pData,4);
//    Console.WriteLine(" ");
//
//    arm_mat_mult_f32(&P_p_matrix,&H_t_matrix,&K_temp1_matrix);    //脟贸K
//    Console.WriteLine("------------------7mult3---------------------");
//    Console.WriteArray("float",K_temp1_matrix.pData,4);
//    Console.WriteLine(" ");
//    arm_mat_mult_f32(&H_matrix,&P_p_matrix,&K_temp2_matrix);
//    Console.WriteLine("------------------7mult4---------------------");
//    Console.WriteArray("float",K_temp2_matrix.pData,4);
//    Console.WriteLine(" ");
//    arm_mat_mult_f32(&K_temp2_matrix,&H_t_matrix,&K_temp3_matrix);
//    Console.WriteLine("------------------7mult5---------------------");
//    Console.WriteArray("float",K_temp3_matrix.pData,4);
//    Console.WriteLine(" ");
//    arm_mat_add_f32(&K_temp3_matrix,&R_matrix,&K_temp3_matrix);
//    Console.WriteLine("------------------7add2---------------------");
//    Console.WriteArray("float",K_temp3_matrix.pData,4);
//    Console.WriteLine(" ");
//
//
//    Console.WriteLine("Value: %d",arm_mat_inverse_f32(&K_temp3_matrix,&K_temp4_matrix));
//
//    Console.WriteLine("------------------8inverse---------------------");
//    Console.WriteArray("float",K_temp3_matrix.pData,16);
//    Console.WriteArray("float",K_temp4_matrix.pData,16);
//    Console.WriteLine(" ");
//
//    arm_mat_mult_f32(&K_temp1_matrix,&K_temp3_matrix,&K_matrix);
//    arm_mat_mult_f32(&K_matrix,&H_matrix,&P_temp1_matrix);       //脨颅路陆虏卯赂眉脨脗
//    arm_mat_sub_f32(&I_matrix,&P_temp1_matrix,&P_temp2_matrix);
//    arm_mat_mult_f32(&P_temp2_matrix,&P_p_matrix,&P_matrix);
//    Console.WriteLine("------------------9---------------------");
//    Console.WriteArray("float",P_matrix.pData,4);
//    Console.WriteLine(" ");
//
//
//
////    //脳卯脫脜脣脛脭陋脢媒鹿脌录脝录掳脝盲碌楼脦禄禄炉***********************************************************************
//    arm_mat_sub_f32(&Z_matrix,&Z_p_matrix,&Z_e_matrix); //鹿脹虏芒脕驴脳梅虏卯
//    arm_mat_mult_f32(&K_matrix,&Z_e_matrix,&q_e_matrix);//录脝脣茫脦贸虏卯
//    arm_mat_add_f32(&q_p_matrix,&q_e_matrix,&q_matrix); //脳卯脫脜鹿脌录脝
//
//    Console.WriteLine("------------------10--------------------");
//    Console.WriteArray("float",q_matrix.pData,4);
//    Console.WriteLine(" ");

    norm=sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);//碌楼脦禄禄炉脣脛脭陋脢媒
    q[0]/=norm;
    q[1]/=norm;
    q[2]/=norm;
    q[3]/=norm;
//    Console.WriteLine("------------------11---------------------");
//    Console.WriteLine("q[0]:%f,q[1]%f,q[2]%f,q[3]%f",q[0],q[1],q[2],q[3]);
//    Console.WriteLine(" ");
//
    Roll=atan2f(2.0f*(q[2]*q[3]+q[0]*q[1]),q[0]*q[0]-q[1]*q[1]-q[2]*q[2]+q[3]*q[3]);
    Pitch=asinf(2.0f*(q[0]*q[2]-q[1]*q[3]))                                        ;


//    Console.WriteLine("------------------12---------------------");
//    Console.WriteLine("Roll:%f,Pitch:%f",Roll,Pitch);
//    Console.WriteLine(" ");

    //Console.WriteLine("q[0]:%f,q[1]%f,q[2]%f,q[3]",q[0],q[1],q[2],q[3]);
    //*********************************************************
    Rx[0]=1;              Rx[1]=0                ;  Rx[2]=0                  ;
    Rx[3]=0;              Rx[4]= cosf(-Roll);  Rx[5]= sinf(-Roll)  ;
    Rx[6]=0;              Rx[7]=-sinf(-Roll);  Rx[8]= cosf(-Roll)  ;
    Ry[0]=cosf(-Pitch);   Ry[1]=0;             Ry[2]=-sinf(-Pitch) ;//
    Ry[3]=0           ;        Ry[4]=1;                  Ry[5]= 0            ;
    Ry[6]=sinf(-Pitch);   Ry[7]=0;             Ry[8]= cosf(-Pitch) ;

//    Console.WriteLine("------------------13x--------------------");
//    Console.WriteArray("float",Rx,3);
//    Console.WriteLine("------------------13y--------------------");
//    Console.WriteArray("float",Ry,3);
//    Console.WriteLine(" ");
//
    arm_mat_mult_f32(&Rx_matrix,&M_matrix,&M_cx_matrix);
    arm_mat_mult_f32(&Ry_matrix,&M_cx_matrix,&M_cxy_matrix);

//    Console.WriteLine("------------------14--------------------");
//    Console.WriteArray("float",M_cx_matrix.pData,3);
//    Console.WriteLine(" ");
//
//
//    Console.WriteLine("------------------15--------------------");
//    Console.WriteArray("float",M_cxy_matrix.pData,3);
//    Console.WriteLine(" ");
//
//
    Yaw=atan2f(M_cxy[0],M_cxy[1]);
    //**********************************************************************
    Pitch=Pitch*57.3f;
    Roll =Roll*57.3f;
    Yaw  =Yaw*57.3f;

//    Console.WriteLine("------------------16--------------------");
//    Console.WriteLine("Pitch:%f,Roll:%f,Yaw:%f",Pitch,Roll,Yaw);
//    Console.WriteLine(" ");

    *pitchout=Pitch;
    *rollout=Roll;
    *yawout=Yaw;


}


