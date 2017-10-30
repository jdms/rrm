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


function write_grid( X, Y, Z, filename ) 

if ( nargin < 4 ), 
    filename = 'grid.mat'; 
end 

if ( ~equal_sizes(X,Y) || ~equal_sizes(X,Z) ), 
    abort('Tensors size does not match.');
end 

[m, n, l] = size(X); 

x_linear = reshape(X, m*n*l, 1); 
y_linear = reshape(Y, m*n*l, 1); 
z_linear = reshape(Z, m*n*l, 1); 

save(filename, 'm', 'n', 'l', 'x_linear', 'y_linear', 'z_linear', '-ascii', '-double' );  

end

function [ result ] = equal_sizes(x, y) 

result = ~sum(size(x) ~= size(y));
end
