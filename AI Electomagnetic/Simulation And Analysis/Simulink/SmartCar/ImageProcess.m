
file_name='baboon.bmp';
H=imread(file_name);
H=double(H);
Grgb = 0.2990*H,1)+0.5870*.H.,2)+0.1140*H,3);
NbColors=255;
%对矩阵进行量化编码
G=wcodemat(Grgb,NbColors);
%gray线性的灰阶色调
map2=gray(NbColors);
%建立图形窗口1
figure(1);
%建立图像G
image(G);
%应用调色板
colormap(map2);
title('原图像的灰度图');
%显示workplace的变量的详细信息
whos(G);
%转换成为灰度级索引图像
%dwt2单尺度二维离散小波变换
[CA1,CH1,CV1,CD1]=dwt2(G,bior3.7);
%从分解系数中提取近似和细节

% upcoef2二维系数的直接小波重构
A1=upcoef2(a',CA1,'bior3.7',1);

H1=upcoef2('h',CHl,'bior3.7',1);
V1=upcoef2(v',CV1,'bior3.7',1);
D1=upcoef2('d',CD1,'bior3.7',1);

