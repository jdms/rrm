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


utility_methods_path = strcat(char(eval('pwd')), '/util');
addpath( utility_methods_path )
clear

global cur_dir tmp_dir bin_dir

cur_dir = char(eval('pwd')); 
tmp_dir = strcat(char(eval('pwd')), '/tmp');
bin_dir = strcat(char(eval('pwd')), '/bin');

% close all

global delta
delta = 1;


%% Definitions 

% Specifies the number of drawing planes
Number_of_Planes = 10;

Selected_Planes = [ 1 2 3 4 5 6 7 8 9 10 ]; 

% Bounding box limits
xMin = 0; 
xMax = 1; 

yMin = 0; 
yMax = 1; 

tMin = 0; 
tMax = 1; 

% Specifies how coarse the curves (i.e. sketchs) will be 
Line_Discretization = 64; 

% Specifies how coarse the output mesh will be
Grid_Discretization = 1; 

%% Build necessary data structures

build_data_structures
