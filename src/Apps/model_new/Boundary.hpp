#ifndef BOUNDARY_HPP
#define BOUNDARY_HPP

class Boundary
{
	public: 
	
        Boundary():origin_x(0), origin_y(0), width(0), height(0), depth(0){}

        void edit( int x, int y, int w, int h ){ origin_x = x; origin_y = y;
                                                 width = w; height = h;      }
		

        inline int getX(){ return origin_x; }
        inline int getY(){ return origin_y; }
        inline int getWidth(){ return width; }
        inline int getHeight(){ return height; }
        inline int getDepth(){ return depth; }
        inline void getOrigin( int& x, int &y ){ x = origin_x; y = origin_y; }


    protected:

        int origin_x;
        int origin_y;

        int width;
        int height;
        int depth;
		
};


#endif
