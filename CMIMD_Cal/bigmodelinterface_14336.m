clear all

cd C:\Users\Administrator\Desktop\CuNi论文思路整理\20250708提交稿准备\正文\CuNi工作\NC投稿\code\CMIMD %改路径

for N=1:1
    
flag = 2;   

A=load(['Ni75Cu25_300K_order_',num2str(N),'.txt']);

if flag ==1
X=A(:,1);
else
X=A(:,1)*-1;
end

Y=A(:,2);

D=[];
 for i=0:80    

A=14336*i+1;     %原子的个数
B=14336*i+14336;

x=X(A:B,1);
y=Y(A:B,1);
z=[x,y];
w=sortrows(z);
x=w(:,1);
y=w(:,2);

 x=x(1:7168,1)
 y=y(1:7168,1)


% figure(1)  %（关闭绘图）
% 
% plot(x,y,'.');
% 
% hold on    %保证同时显示 
%z=beta(1)+beta(2).*tanh((x+beta(3))./beta(4));

 myfunc =inline('beta(1)+beta(2).*tanh((x+beta(3))./beta(4))','beta','x');
 beta0 = [5,5,10,5]';

beta = nlinfit(x,y,myfunc,beta0)%

beta1=beta(1,1);
beta2=beta(2,1);
beta3=beta(3,1);
beta4=beta(4,1);

D=[D,beta(3,1)];
E=D';

% x = -124:0.1:-63;      %0：1：402
 %y=beta(1)+beta(2).*tanh((x+beta(3))./beta(4));
 %figure(1)          %（关闭绘图）
 %plot(x,y)
%set(gca, 'FontSize',16,'Fontname','Times New Roman')%（改变图像输出的格式）
 %hold on
   
 end

E =E;
sheet = 1;
xlRange = 'B1';
if flag ==1
xlswrite(['interface_left_',num2str(N),'.xlsx'],E,sheet,xlRange);
else
    xlswrite(['interface_right_',num2str(N),'.xlsx'],E,sheet,xlRange);
end


F=[];
 for j=0:80 
J=j;
F=[F,J];
G=F';
 end


G =G;
sheet = 1;
xlRange = 'A1';

if flag ==1
xlswrite(['interface_left_',num2str(N),'.xlsx'],G,sheet,xlRange);
else
    xlswrite(['interface_right_',num2str(N),'.xlsx'],G,sheet,xlRange);
end

%  f=E;
%  q=G;
%  hold on
%  figure(2)
%  plot(q,f,'.');
%  hold on

%flag = 'interface_left_',num2str(N),'.xlsx';
if flag==1
M=xlsread(['interface_left_',num2str(N),'.xlsx']);
a = M;
fileID=fopen(['interface_left_',num2str(N),'.txt'],'w');
fprintf (fileID,'%f %f\n',a') ;
delete(['interface_left_',num2str(N),'.xlsx']);
else
M=xlsread(['interface_right_',num2str(N),'.xlsx']);
a = M;
fileID=fopen(['interface_right_',num2str(N),'.txt'],'w');
fprintf (fileID,'%f %f\n',a') ;
delete(['interface_right_',num2str(N),'.xlsx']);
end
    
end




for N=1:1
    
flag = 1;    

A=load(['Ni75Cu25_300K_order_',num2str(N),'.txt']);

if flag ==1
X=A(:,1);
else
X=A(:,1)*-1;
end

Y=A(:,2);

D=[];
 for i=0:80     

A=14336*i+1;    
B=14336*i+14336;

x=X(A:B,1);
y=Y(A:B,1);
z=[x,y];
w=sortrows(z);
x=w(:,1);
y=w(:,2);

 x=x(1:7168,1)
 y=y(1:7168,1)


%figure(1)  %（关闭绘图）

%plot(x,y,'.');

%hold on    %保证同时显示 
%z=beta(1)+beta(2).*tanh((x+beta(3))./beta(4));

myfunc =inline('beta(1)+beta(2).*tanh((x+beta(3))./beta(4))','beta','x');
beta0 = [5,5,10,5]';


beta = nlinfit(x,y,myfunc,beta0)%

beta1=beta(1,1);
beta2=beta(2,1);
beta3=beta(3,1);
beta4=beta(4,1);

D=[D,beta(3,1)];
E=D';

%x = 0:1:1000;      %0：1：402
%y=beta(1)+beta(2).*tanh((x+beta(3))./beta(4));
%figure(1)          %（关闭绘图）
%plot(x,y)
%set(gca, 'FontSize',16,'Fontname','Times New Roman')%（改变图像输出的格式）
%hold on

   
 end



%filename = 'interface_left.xls'; %打开
E =E;
sheet = 1;
xlRange = 'B1';
%xlswrite('F:\SecondEpan\.CuNi_test\移动控温\1nm单侧减薄实验\berendensen\400K\interface_right_',num2str(N),'.xlsx',E,sheet,xlRange);
if flag ==1
xlswrite(['interface_left_',num2str(N),'.xlsx'],E,sheet,xlRange);
else
    xlswrite(['interface_right_',num2str(N),'.xlsx'],E,sheet,xlRange);
end


F=[];
 for j=0:80
J=j;
F=[F,J];
G=F';
 end


G =G;
sheet = 1;
xlRange = 'A1';

if flag ==1
xlswrite(['interface_left_',num2str(N),'.xlsx'],G,sheet,xlRange);
else
    xlswrite(['interface_right_',num2str(N),'.xlsx'],G,sheet,xlRange);
end

% f=E;
% q=G;
% hold on
% figure(2)
% plot(q,f,'.');
% hold on

%flag = 'interface_left_',num2str(N),'.xlsx';
if flag==1
M=xlsread(['interface_left_',num2str(N),'.xlsx']);
a = M;
fileID=fopen(['interface_left_',num2str(N),'.txt'],'w');
fprintf (fileID,'%f %f\n',a') ;
delete(['interface_left_',num2str(N),'.xlsx']);
else
M=xlsread(['interface_right_',num2str(N),'.xlsx']);
a = M;
fileID=fopen(['interface_right_',num2str(N),'.txt'],'w');
fprintf (fileID,'%f %f\n',a') ;
delete(['interface_right_',num2str(N),'.xlsx']);
end
    
end

fclose all;
