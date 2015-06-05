clear all
clc

% data=[10 14 18 8 20];
% theta = mle(data, 'distribution', 'exponential')

%3
year=1975:1:1987; 
lean=[2.9642,2.9644,2.9656,2.9667,2.9673,2.9688,2.9696,2.9698,2.9713,2.9717,2.9725,2.9742,2.9757];
figure(2);
scatter(year,lean);
hold on;
p=polyfit(year,lean,1);
yfit = polyval(p,year);
plot(year,yfit,'r');
hold off;
xlabel('Year');
ylabel('Lean');
legend('Data','Fit','Location','northwest');

%4
figure(3);
scatter(lean,year);
hold on;
p2=polyfit(lean,year,1);
xfit=polyval(p2,lean);
plot(yfit,year,'r--');
plot(lean,xfit,'k-');
hold off;
xlabel('Lean');
ylabel('Year');
legend('Data','Fit','Flipped Fit','Location','northwest');