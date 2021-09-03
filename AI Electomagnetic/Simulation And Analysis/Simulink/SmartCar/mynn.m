%1.25 * exp(0.0012 * abs(x)) * x

x = -210.0:0.01:210.0;

lenX = length(x);

y = zeros(lenX,1);

for i = 1 : lenX
    
   y(i) = 1.25 * exp(0.0012 * abs(x(i))) * x(i);
   
   %y(i) = 260 * sin(0.1 * x(i));

   if y(i) >= 210.0
       y(i) = 210.0;
   end
   
   if y(i) <= -210.0
       y(i) = -210.0;
   end
    
end

subplot(1,1,1);

plot(x,y,"b");

text(80,100,"y - 1.25 * exp(0.0012 * abs(x)) * x");

set(gca,'Units', 'normalized', 'Position', [0.25 0.1 0.55 0.8])
set(gca,'XAxisLocation','origin','YAxisLocation','origin')

grid on;
set(gca, 'GridLineStyle', ':');  % 设置为虚线
set(gca, 'GridAlpha', 1);  % 设置透明度
set(gca, 'XTick', -240:20:240); 
N = 2;  
a = get(gca,'XTickLabel');  
b = cell(size(a));
b(mod(1:size(a,1),N)==1,:) = a(mod(1:size(a,1),N)==1,:);
set(gca,'XTickLabel',b);

set(gca, 'YTick', -240:20:240); 
N = 2;  
a = get(gca,'YTickLabel');  
b = cell(size(a));
b(mod(1:size(a,1),N)==1,:) = a(mod(1:size(a,1),N)==1,:);
set(gca,'YTickLabel',b);

axis([-240 240 -240 240]);

title("");

xlabel("NN Output");
ylabel("AI Angle");




