#include "Scene3D.h"

Scene3D::Scene3D()
{
    input_volume = nullptr;

    context = nullptr;
    surfacegl = nullptr;

    current_color = QColor( 255, 0, 0 );
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

    auto search = object_list.find( object_->getId() );
    if( search != object_list.end() ) return false;

    ObjectOpenGLWrapper* obj_wrapper_ = new ObjectOpenGLWrapper();
    obj_wrapper_->setShaderDirectory( current_directory );
    obj_wrapper_->init();
    obj_wrapper_->setObjectRaw( object_ );
//    obj_wrapper_->setConstantColor( current_color.red(), current_color.green(), current_color.blue() );

    double ox_ = 0.0f, oy_ = 0.0f, oz_ = 0.0f;
    double w_ = 0.0f, h_ = 0.0f, d_ = 0.0f;

    input_volume->getOrigin( ox_, oy_, oz_ );
    input_volume->getDimensions( w_, h_, d_ );

    obj_wrapper_->setBoundingBox( ( float ) ox_, ( float ) (ox_ + w_), ( float ) oy_, ( float ) (oy_ + h_),
                                  ( float ) oz_, ( float ) (oz_ + d_) );

    object_list[ object_->getId() ] = obj_wrapper_;

    return true;

}


void Scene3D::draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                                                                          const int& h_ )
{

    for( auto& it : object_list )
        (it.second)->draw( V_, P_, w_, h_ );

    if( input_volume != nullptr )
        input_volume->draw( V_, P_, w_, h_ );

}


void Scene3D::updateScene()
{
    input_volume->update();

    for( auto &it_: object_list )
    {
        ObjectOpenGLWrapper* obj_ = it_.second;
        obj_->reloadBuffers();
    }

    emit updateCanvas();
}


void Scene3D::clearScene()
{

    input_volume->clear();
    delete input_volume;

    for( auto &it: object_list )
    {
        (it.second)->clear();
        delete (it.second);
    }
    object_list.clear();

    current_color = QColor( 255, 0, 0 );
}
