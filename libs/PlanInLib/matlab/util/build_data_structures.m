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


%% Base data structures

global xmin xmax ymin ymax tmin tmax 
global line_discretization grid_discretization number_of_planes selected_planes
global p x xg tg X T 

% Specifies the number of drawing planes
number_of_planes = Number_of_Planes;

selected_planes = Selected_Planes; 

%% Colormaps for visualization

cmap1 = diverging_map(linspace(0,1,1024), [0.085, 0.532, 0.201], [0.758, 0.214, 0.233]);
cmap2 = diverging_map(linspace(0,1,1024), [0.217, 0.525, 0.910], [0.677, 0.492, 0.093]);
cmap3 = diverging_map(linspace(0,1,1024), [0.085, 0.532, 0.201], [0.436, 0.308, 0.631]);
cmap4 = diverging_map(linspace(0,1,1024), [0.436, 0.308, 0.631], [0.759, 0.334, 0.046]);
cmap5 = diverging_map(linspace(0,1,1024), [0.230, 0.299, 0.754], [0.706, 0.016, 0.150]);


%% Definitions for the bounding box and the discretizations

% Bounding box limits
xmin = xMin; 
xmax = xMax; 

ymin = yMin; 
ymax = yMax; 

tmin = tMin; 
tmax = tMax; 

% Specifies how coarse the curves (i.e. sketchs) will be 
line_discretization = Line_Discretization; 

% Specifies how coarse the output mesh will be
grid_discretization = Grid_Discretization; 

% Sketch planes' indices
p = linspace(1, number_of_planes, number_of_planes);  

% x sampling of the sketchs
x = linspace(xmin, xmax, line_discretization); 

% x, t sampling for building meshes
xg = linspace(xmin, xmax, grid_discretization); 
tg = linspace(tmin, tmax, grid_discretization); 

% domain sampling to build visualization meshes 
[X, T] = meshgrid(xg, tg); 

% pass the base mesh data to the c++ program
write_grid(X, T, zeros(size(X)), strcat(tmp_dir, '/vertices.mat')); 

