%****************************************************************************
%                                                                            
% This file is part of the "Planar Interpolation Library" (PlanInLib)        
% Copyright (C) 2016, Julio Daniel Machado Silva.                            
%                                                                            
% PlanInLib is free software; you can redistribute it and/or                 
% modify it under the terms of the GNU Lesser General Public                 
% License as published by the Free Software Foundation; either               
% version 2.1 of the License, or (at your option) any later version.         
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


function [ Points ] = surface3(position, BaseSurface)

global cur_dir tmp_dir bin_dir
global delta
global xmin xmax ymin ymax tmin tmax 
global line_discretization grid_discretization number_of_planes selected_planes
global p x xg tg X T 

%% Create the canyon's input data (a)
perturbation = sin( 4*pi*p/number_of_planes);

Coeffs = 0.01*perturbation; 
Positions = linspace( (1/3+1/4)/2, (2/3+3/4)/2, number_of_planes) + 0.1*perturbation; 

%% Create the canyon's input data (b)
Planes = zeros(line_discretization, 3, number_of_planes); 
t0 = zeros(size(x)); 

figure(3)
hold off
for i = 1 : number_of_planes, 
    y = ((1/2+3/4)/2 + 3/4)/2 + 0.01*sin( 10*pi*(x - Positions(i)) );
    plane = [ x; t0 + (p(i)-1)/(number_of_planes-1); y ]'; 
    plot3(plane(:,1), plane(:,2), plane(:,3), 'xb')
    hold on
    Planes(:,:,i) = plane; 
end

%% Create the canyon's input data (b)
Points = []; 
for i = 1 : numel(selected_planes), 
    index = selected_planes(i); 
    Points = [ Points; Planes(:,:,index) ]; 
end

if ( nargin == 2 )
    if ( strcmp(position, 'below') == 1 )
        Points = remove_above(BaseSurface, Points);
    elseif ( strcmp(position, 'above') == 1 )
        Points = remove_below(BaseSurface, Points);
    end
end

figure(3)
hold on
plot3(Points(:,1), Points(:,2), Points(:,3),'or')
axis off
axis equal
title('Input curves for horizon 3 (selected curves in red)'); 
% legend('Original Points', 'Selected Points');
xlabel('x'); 
ylabel('t'); 
zlabel('y'); 

end
