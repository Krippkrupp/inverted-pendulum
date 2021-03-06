%% Powerbox3000, 5 V, Fluke 4.89 V, MWA=100
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
title('Uppmätt spänning efter ADC och MWA=100, fluke 4.89 V')
ylabel('Spänning, V')
xlabel('Vinkel, deg')

%% Powerbox3000, 5 V, Fluke 5 V, MWA=100
clc%, close all
load('arduino_5V_5V.mat')
H1 = Arduino_5V_5V0(:,1)%.*1024/(3.7)
H2 = Arduino_5V_5V0(:,2)%.*1024/(3.7)
deg = Arduino_5V_5V0(:,3)
% Grov graf
hold on;
plot(deg, H1,'o')
plot(deg, H2, 'o');

% Polyfit
p1 = polyfit(deg, H1, 2)    
p2 = polyfit(deg, H2, 2)
x = linspace(-25, 25, 500);
H1_poly = polyval(p1, x);
H2_poly = polyval(p2, x);

%plot(x, H1_poly);
%plot(x, H2_poly);
%legend('H1', 'H2', 'H1 poly', 'H2 poly');
legend('H1', 'H2');
grid on;
title('Uppmätt spänning efter ADC och MWA=100, fluke 5 V')
ylabel('Spänning, V')
xlabel('Vinkel, deg')

%% test till rapporten
%% Powerbox3000, 5 V, Fluke 5 V, MWA=100
clc, close all
load('arduino_5V_5V.mat')
H1 = Arduino_5V_5V0(:,1).*1024/(3.7)-761
H2 = Arduino_5V_5V0(:,2).*1024/(3.7)-761
deg = Arduino_5V_5V0(:,3)
% Grov graf
hold on, grid on
plot(deg, H1,'o')
plot(deg, H2, 'o');
title({'Mätning reglerfel'},'Interpreter','latex')
legend({'$e_1(t)$', '$e_2(t)$'},'Interpreter','latex')
xlabel({'Grader, $^\circ$'},'Interpreter','latex')
%ylabel({'Sensorvärde-stationärt'},'Interpreter','latex')
%legend({'$e_2(t)$'},'Interpreter','latex')

%% Powerbox3000, 5 V, Fluke 5 V, MWA=100, -10<deg<10, linjärt
clc, close all, clear
load('degPoints.mat')
H1 = degPoints(:,1)
H2 = degPoints(:,2)
deg = degPoints(:,3)
% Grov graf
hold on
plot(deg, H1,'o')
plot(deg, H2, 'o');


% Polyfit
p1 = polyfit(deg, H1, 2)
p2 = polyfit(deg, H2, 2)
x = linspace(-25, 25, 500);
H1_poly = polyval(p1, x);
H2_poly = polyval(p2, x);

plot(x, H1_poly);
plot(x, H2_poly);
legend('H1', 'H2', 'H1 poly', 'H2 poly');
grid on;
title('Uppmätt spänning efter ADC och MWA=100, fluke 5 V')
ylabel('Spänning, V')
xlabel('Vinkel, deg')

%% Powerbox3000, 5 V, Fluke 5 V, MWA=100, -10<deg<10, linjärt, modifierad
clc, close all, clear
load('degPoints.mat')
H1 = degPoints(:,1).*(1024/4.22)
H2 = degPoints(:,2).*(1024/4.22)
deg = degPoints(:,3)
% Grov graf
hold on;
plot(deg, H1,'o')
plot(deg, H2, 'o');


% Polyfit
p1 = polyfit(deg, H1, 1)
p2 = polyfit(deg, H2, 1)
x = linspace(-25, 25, 500);
H1_poly = polyval(p1, x);
H2_poly = polyval(p2, x);

plot(x, H1_poly);
plot(x, H2_poly);
legend('H1', 'H2', 'H1 poly', 'H2 poly');
grid on;
title('Uppmätt spänning efter ADC och MWA=100, fluke 5 V')
ylabel('Spänning, V')
xlabel('Vinkel, deg')


