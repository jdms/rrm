%% Load raw processed data

inside_raw = load('output_surface_inside_channel.txt', '-ascii');
channel_raw = load('output_surface_channel.txt', '-ascii'); 
top_raw = load('output_surface_top.txt', '-ascii' ); 

%% Obtain x, y, z coordinates from raw data

%get_coordinates = @(X, num)( [ X(2:num+1,:), X(num+2:2*num+1,:), X(2*num+2:3*num+1,:) ] );

numX = inside_raw(1,2);
numY = inside_raw(1,3);

[iX, iY, iZ ] = get_coordinates(inside_raw, numY);
[cX, cY, cZ ] = get_coordinates(channel_raw, numY);
[tX, tY, tZ ] = get_coordinates(top_raw, numY );

%% Visualiza model

figure
hold on

surf(iX, iY, iZ)
surf(cX, cY, cZ)
surf(tX, tY, tZ)

axis([ 0 1 0 1 0 1 ] ); axis square; axis off
shading interp

%% Helper function

function [x, y, z] = get_coordinates(data, num)
    x = data(2:num+1,:);
    y = data(num+2:2*num+1,:);
    z = data(2*num+2:3*num+1,:);
end
