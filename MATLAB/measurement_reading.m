% Find a serial port object.
obj1 = instrfind('Type', 'serial', 'Port', 'COM3', 'Tag', '');

% Create the serial port object if it does not exist
% otherwise use the object that was found.
if isempty(obj1)
    obj1 = serial('COM3');
else
    fclose(obj1);
    obj1 = obj1(1);
end

SIZE_MEAS = 100;

data = zeros (SIZE_MEAS, 9);

% Connect to instrument object, obj1.
fopen(obj1);

% Communicating with instrument object, obj1.
reading_string = fscanf(obj1);
reading_num = str2num(reading_string);
reading_num_prev = reading_num;

for N = 1:SIZE_MEAS
    reading_string = fscanf(obj1);
    reading_num = str2num(reading_string);
    data(N,:) = reading_num;
end
N = 1:100;

   plot(N,data(:,1));  

   
fclose(obj1);