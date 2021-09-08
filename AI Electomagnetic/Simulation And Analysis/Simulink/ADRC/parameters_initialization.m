%--参数初始化--%
h = 0.01;
%跟踪微分器参数
r = 100;%r表示跟踪快慢
h0 = 5 * h;%h0代表信号平滑度（滤波效果）
v1_last = 0;
v2_last = 0;
v0_last = 0;
%扩张状态观测器参数
beta01 = 100;
beta02 = 200;
beta03 = 300;
alpha1 = 0.5;%文献里给了值，就别动了
alpha2 = 0.25;%文献里给了值，就别动了
delta = 0.0025;
b=1;
z1_last = 0;
z2_last = 0;
z3_last = 0;
%非线性误差反馈
nlsef_alpha1 = 0.7;
nlsef_alpha2  = 0.95;