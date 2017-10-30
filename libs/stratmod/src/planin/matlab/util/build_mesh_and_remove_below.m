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


function [ vL, fL, cL ] = build_mesh_and_remove_below(Surface, BaseSurface)

global cur_dir tmp_dir bin_dir
global delta
global xmin xmax ymin ymax tmin tmax 
global line_discretization grid_discretization number_of_planes selected_planes
global p x xg tg X T 

%% Write data to the c++ program

% pass the lines data to the c++ program
write_vector3(Surface, strcat(tmp_dir, '/points.mat')); 
write_vector3(BaseSurface, strcat(tmp_dir, '/base_surface_points.mat')); 

%% Run the c++ program

cd(tmp_dir)
!./remove_below
cd(cur_dir)

%% Reads the c++ program' output data

% reads the output mesh generated by the c++ program
[vL, fL] = load_patch( strcat(tmp_dir, '/vlist.mat'), strcat(tmp_dir, '/flist.mat') ); 
fL = fL + 1;
cL = vL(:,3);  

end
