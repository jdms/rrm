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


function [ Points ] = surfacev(Channel_Discretization, position, BaseSurface)

global cur_dir tmp_dir bin_dir
global delta
global xmin xmax ymin ymax tmin tmax 
global line_discretization grid_discretization number_of_planes selected_planes
global p x xg tg X T 

%% Create the canyon's input data (a)
% perturbation = sin( 4*pi*p/number_of_planes);
% 
% Coeffs = 0.4 + 0.1*perturbation; 
% Positions = linspace( (1/3+1/4)/2, (2/3+3/4)/2, number_of_planes) + 0.1*perturbation; 

if ( nargin < 1 ), 
    Channel_Discretization = 10; 
end

t = linspace(0,1,256)'; 
tc = linspace(0,1,Channel_Discretization); 

W_Coeffs = (2*rand(size(t)) - 1); 
W_Positions =  2*(2*rand(size(t)) - 1); 

Coeffs = f(p/number_of_planes, zeros(size(p)), zeros(size(p)), [t, zeros(size(t)), zeros(size(t))], W_Coeffs, 1 );
% Coeffs = 0.4 + 0.25 * (Coeffs - min(Coeffs))/(max(Coeffs) - min(Coeffs)) - 1/8;
Coeffs = 0.4 + 0.1 * (Coeffs - min(Coeffs))/(max(Coeffs) - min(Coeffs));

Positions = f(p/number_of_planes, zeros(size(p)), zeros(size(p)), [t, zeros(size(t)), zeros(size(t))], W_Positions, 1 );
Positions = (1/3+1/4)/2 + 0.25 * (Positions - min(Positions))/(max(Positions) - min(Positions));

%% Create the canyon's input data (b)
Planes = zeros(line_discretization, 3, number_of_planes); 
t0 = zeros(size(x)); 

figure(1)
hold off
for j = 1 : numel(selected_planes),
    i = selected_planes(j); 
    y = 1 - Coeffs(i) * phi( x - Positions(i), zeros(size(x)), zeros(size(x)), 1/2);
    plane = [ x; t0 + (p(i)-1)/(number_of_planes-1); y ]'; 
    plot3(plane(:,1), plane(:,2), plane(:,3), 'xb')
    hold on
    Planes(:,:,i) = plane; 
end

Coeffs = f(tc, zeros(size(tc)), zeros(size(tc)), [t, zeros(size(t)), zeros(size(t))], W_Coeffs, 1 );
Coeffs = 0.4 + 0.1 * (Coeffs - min(Coeffs))/(max(Coeffs) - min(Coeffs));

Positions = f(tc, zeros(size(tc)), zeros(size(tc)), [t, zeros(size(t)), zeros(size(t))], W_Positions, 1 );
Positions = (1/3+1/4)/2 + 0.25 * (Positions - min(Positions))/(max(Positions) - min(Positions));

valley_bottom = [ Positions', tc', (1 - Coeffs)' ]; 

Coeffs = f(t, zeros(size(t)), zeros(size(t)), [t, zeros(size(t)), zeros(size(t))], W_Coeffs, 1 );
Coeffs = 0.4 + 0.1 * (Coeffs - min(Coeffs))/(max(Coeffs) - min(Coeffs));

Positions = f(t, zeros(size(t)), zeros(size(t)), [t, zeros(size(t)), zeros(size(t))], W_Positions, 1 );
Positions = (1/3+1/4)/2 + 0.25 * (Positions - min(Positions))/(max(Positions) - min(Positions));

valley_bottom_complete = [ Positions, t, (1 - Coeffs) ]; 
plot3(valley_bottom_complete(:,1), valley_bottom_complete(:,2), valley_bottom_complete(:,3), '.g'); 
    
%% Create the canyon's input data (b)
Points = []; 
for i = 1 : numel(selected_planes), 
    index = selected_planes(i); 
    Points = [ Points; Planes(:,:,index) ]; 
end

Points = [Points; valley_bottom]; 

if ( nargin == 3 )
    if ( strcmp(position, 'below') == 1 )
        Points = remove_above(BaseSurface, Points);
    elseif ( strcmp(position, 'above') == 1 )
        Points = remove_below(BaseSurface, Points);
    end
end

figure(1)
hold on
plot3(Points(:,1), Points(:,2), Points(:,3),'or')
axis equal
axis off
axis equal
title('Input curves for horizon 1 (selected curves in red)'); 
% legend('Original Points', 'Selected Points');
xlabel('x'); 
ylabel('t'); 
zlabel('y'); 

end
