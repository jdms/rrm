#include "Scene3D.h"

Scene3D::Scene3D()
{
    input_volume = nullptr;
}


bool Scene3D::addVolume(  Volume* const& volume_ )
{
    context->makeCurrent( surfacegl );

    input_volume = new VolumeOpenGLWrap();
    input_volume->setShaderDirectory( current_directory );
    input_volume->init();
    input_volume->setVolumeRaw( volume_ );

    return true;
}

bool Scene3D::addObject( Object* const& object_ )
{
    context->makeCurrent( surfacegl );

    ObjectOpenGLWrapper* obj_ = new ObjectOpenGLWrapper();
    obj_->setShaderDirectory( current_directory );
    obj_->init();
    obj_->setObjectRaw( object_ );

    object_list[ object_->getId() ] = obj_;

    std::cout << "Number of wrappers of objects created so far: " << object_list.size() <<
                 "\n\n" << std::flush;

    return true;

}


void Scene3D::draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                                                                          const int& h_ )
{
    if( input_volume != nullptr )
        input_volume->draw( V_, P_, w_, h_ );

    for( auto& it : object_list )
        (it.second)->draw( V_, P_, w_, h_ );

}
