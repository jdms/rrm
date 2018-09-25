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


function [vertex_list, face_list, color_list] = get_patch_from_graph(X, Y, Z, C) 

if ( nargin < 4 ) 
    color_list = []; 
else
    color_list = reshape(C, size(C,1)*size(C,2), 1); 
end

[nY, nX] = size(X); 

%% 

XL = reshape(X, size(X,1)*size(X,2), 1);
YL = reshape(Y, size(Y,1)*size(Y,2), 1);
ZL = reshape(Z, size(Z,1)*size(Z,2), 1); 

vertex_list = [XL, YL, ZL];  
face_list = [];

get_vertex = @(i,j)( (i-1)*nY + (j-1) + 1);

for i = 1 : nX-1, 
    for j = 1 : nY-1, 

        face_list = [ face_list; 
                      get_vertex(i,j), get_vertex(i+1, j), get_vertex(i+1, j+1);
                      get_vertex(i,j), get_vertex(i+1, j+1), get_vertex(i, j+1) ]; 
    end
end

