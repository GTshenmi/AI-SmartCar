
file_name='baboon.bmp';
H=imread(file_name);
H=double(H);
Grgb = 0.2990*H,1)+0.5870*.H.,2)+0.1140*H,3);
NbColors=255;
%�Ծ��������������
G=wcodemat(Grgb,NbColors);
%gray���ԵĻҽ�ɫ��
map2=gray(NbColors);
%����ͼ�δ���1
figure(1);
%����ͼ��G
image(G);
%Ӧ�õ�ɫ��
colormap(map2);
title('ԭͼ��ĻҶ�ͼ');
%��ʾworkplace�ı�������ϸ��Ϣ
whos(G);
%ת����Ϊ�Ҷȼ�����ͼ��
%dwt2���߶ȶ�ά��ɢС���任
[CA1,CH1,CV1,CD1]=dwt2(G,bior3.7);
%�ӷֽ�ϵ������ȡ���ƺ�ϸ��

% upcoef2��άϵ����ֱ��С���ع�
A1=upcoef2(a',CA1,'bior3.7',1);

H1=upcoef2('h',CHl,'bior3.7',1);
V1=upcoef2(v',CV1,'bior3.7',1);
D1=upcoef2('d',CD1,'bior3.7',1);

