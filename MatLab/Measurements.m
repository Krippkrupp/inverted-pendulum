clc, clear, close all
load('arduino_5V_4V89.mat')
H1 = Arduino_5V_4V89(:,1)
H2 = Arduino_5V_4V89(:,2)
deg = Arduino_5V_4V89(:,3)
% Grov graf
hold on;
plot(deg, H1,'o')
plot(deg, H2, 'o');

% Polyfit
p1 = polyfit(deg, H1, 5)
p2 = polyfit(deg, H2, 6)
x = linspace(-25, 25, 500);
H1_poly = polyval(p1, x);
H2_poly = polyval(p2, x);

plot(x, H1_poly);
plot(x, H2_poly);
legend('H1', 'H2', 'H1 poly', 'H2 poly');
grid on;
title('Mätt spänning efter ADC och MWA=100')
ylabel('Spänning, V')
xlabel('Vinkel, deg')

