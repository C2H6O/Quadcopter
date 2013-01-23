
clear all;
close all;
y = zeros(5000);

s1 = instrfind('Type','serial','Port','COM3','Tag','');

if isempty(s1)
    s1 = serial('COM3');
else
    fclose(s1);
    s1 = s1(1);
end
     
%open serial port
fopen(s1);
s1.Terminator = '\n';
%clear data;
%for i= 1:2000                                %acquisition of 100 points
%    data=fscanf(s1);%read sensor
%    y(i) = str2double(data);

%    plot(i,y(i),'+-b')
%    title('Dados de LDR Aquisitados');
%    xlabel('Pontos');
%    ylabel('Valores');
%    drawnow;
%    hold on%
%
%    if y(i) > 10
%        fprintf(s1, '1');
%    end
%    if y(i) < 10
%        fprintf(s1, '0');
%    end
%    
%    
%end
% close the serial port
fclose(s1);