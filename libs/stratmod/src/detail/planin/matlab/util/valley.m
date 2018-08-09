%****************************************************************************
%                                                                            
% This file is part of the "Planar Interpolation Library" (PlanInLib)        
% Copyright (C) 2016, Julio Daniel Machado Silva.                            
%                                                                            
% PlanInLib is free software; you can redistribute it and/or                 
% modify it under the terms of the GNU Lesser General Public                 
% License as published by the Free Software Foundation; either               
% version 3 of the License, or (at your option) any later version.           
%                                                                            
% PlanInLib is distributed in the hope that it will be useful,               
% but WITHOUT ANY WARRANTY; without even the implied warranty of             
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          
% Lesser General Public License for more details.                            
%                                                                            
% You should have received a copy of the GNU Lesser General Public           
% License along with PlanInLib.  If not, see <http://www.gnu.org/licenses/>, 
% or write to the Free Software Foundation, Inc., 51 Franklin Street,        
% Fifth Floor, Boston, MA  02110-1301  USA.                                  
%                                                                            
%****************************************************************************


function [ Points, valley_bottom ] = valley(mean_, position, BaseSurface)

global cur_dir tmp_dir bin_dir
global delta
global xmin xmax ymin ymax tmin tmax 
global line_discretization grid_discretization number_of_planes selected_planes
global p x xg tg X T 

%% Create the canyon's input data (a)

% Simple function to yield the channel's path
getPosition = @(t)( 7/24*(1-t) + 17/24*t + 0.1*sin(4*pi*t) ); 

Positions = getPosition((p-1)/(number_of_planes-1)); 
vbottom = 0.4; 


%% Create the canyon's input data (b)
Planes = zeros(line_discretization, 3, number_of_planes); 
t0 = zeros(size(x)); 

% figure(1)
% hold off
for i = 1 : number_of_planes, 
    y = 1 - vbottom * phi( x - Positions(i), zeros(size(x)), zeros(size(x)), 1/2);
    plane = [ x; t0 + (p(i)-1)/(number_of_planes-1); y ]'; 
%     plot3(plane(:,1), plane(:,2), plane(:,3), 'xb')
    hold on
    Planes(:,:,i) = plane; 
end

channel_mean = mean(mean(Planes(:,3,:))); 
if ( nargin >= 1 ), 
    Planes(:,3,:) = Planes(:,3,:) - mean(mean(Planes(:,3,:))) + mean_; 
else 
    mean_ = channel_mean; 
end

%% Create the canyon's input data (b)
Points = []; 
for i = 1 : numel(selected_planes), 
    index = selected_planes(i); 
    Points = [ Points; Planes(:,:,index) ]; 
end

if ( nargin == 3 )
    if ( strcmp(position, 'below') == 1 )
        Points = remove_above(BaseSurface, Points);
    elseif ( strcmp(position, 'above') == 1 )
        Points = remove_below(BaseSurface, Points);
    end
end

% Now proccess the Canyon's bottom. 

channel_bottom_discretization = 64; 
t = linspace(0, 1, channel_bottom_discretization); 

Positions_high_res = getPosition(t); 
valley_bottom = [ Positions_high_res; t; ( (1 - vbottom) - channel_mean + mean_)  * ones(size(t) ) ]';

% figure(1)
% hold on
% plot3(Points(:,1), Points(:,2), Points(:,3),'or')
% axis equal
% axis off
% axis equal
% title('Input curves for horizon 1 (selected curves in red)'); 
% % legend('Original Points', 'Selected Points');
% xlabel('x'); 
% ylabel('t'); 
% zlabel('y'); 

end
