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


function val = phi( x, y, z, delta )

if ( nargin == 3 ),
    global delta
end
    
r = sqrt( x.*x + y.*y + z.*z )/delta;

val = (1-r).^8 .* (32.0*r.^3 + 25.0*r.^2 + 8.0*r + 1.0 );
%val = (1-r).^4 .* ( 4.0*r + 1.0 ); 

val( r >= 1 ) = 0; 

end
