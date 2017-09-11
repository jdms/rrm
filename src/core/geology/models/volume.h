#ifndef VOLUME_H
#define VOLUME_H

#include <vector>
#include <cmath>


class Volume
{
    public:

        Volume()
        {
            initialize();
        }

        inline void setDimensions( double width_, double height_, double depth_ )
        {
            width = std::fabs( width_ );
            height = std::fabs( height_ );
            depth = std::fabs(depth_ );
        }

        inline void getDimensions( double& width_, double& height_, double& depth_ ) const
        {
            width_ = width;
            height_ = height;
            depth_ = depth;
        }


        inline void setWidth( double width_ )
        {
            width = std::fabs( width_ );
        }

        inline void setHeight( double height_ )
        {
            height = std::fabs( height_ );
        }

        inline void setDepth( double depth_ )
        {
            depth = std::fabs( depth_ );
        }


        inline double getWidth() const
        {
            return width;
        }

        inline double getHeight() const
        {
            return height;
        }

        inline double getDepth() const
        {
            return depth;
        }


        inline void setOrigin( double ox_, double oy_, double oz_ )
        {
            origin_x = ox_;
            origin_y = oy_;
            origin_z = oz_;
        }

        inline void getOrigin( double& ox_, double& oy_, double& oz_ ) const
        {
            ox_ = origin_x;
            oy_ = origin_y;
            oz_ = origin_z;
        }


        inline void setVisibility( bool option )
        {
            is_visible = option;
        }

        inline bool getVisibility() const
        {
            return is_visible;
        }


        inline void initialize()
        {
            setDefaultValues();
        }

        inline void clear()
        {
            setDefaultValues();
        }


        inline void setDefaultValues()
        {
            origin_x = 0.0f;
            origin_y = 0.0f;
            origin_z = 0.0f;

            width = 500.0f;
            height = 500.0f;
            depth = 500.0f;

            is_visible = true;
        }



    protected:

        double origin_x;
        double origin_y;
        double origin_z;

        double width;
        double height;
        double depth;

        bool is_visible;

        const std::size_t POINT_DIMENSION = 3;

};

#endif // VOLUME_H
