#include "Scene3D.h"

Scene3D::Scene3D()
{
    input_volume = nullptr;
    cross_section = nullptr;

    context = nullptr;
    surfacegl = nullptr;

    current_color = QColor( 255, 0, 0 );


}


void Scene3D::addVolume(  Volume* const& volume_ )
{
    context->makeCurrent( surfacegl );

    input_volume = new VolumeOpenGLWrap();
    input_volume->setShaderDirectory( current_directory );
    input_volume->init();
    input_volume->setVolumeRaw( volume_ );




}


void Scene3D::createCrossSection()
{
    double ox_ = 0.0f, oy_ = 0.0f, oz_ = 0.0f;
    double w_ = 0.0f, h_ = 0.0f, d_ = 0.0f;

    input_volume->getOrigin( ox_, oy_, oz_ );
    input_volume->getDimensions( w_, h_, d_ );

    cross_section = new CrossSectionOpenGLWrapper();
    cross_section->setShaderDirectory( current_directory );
    cross_section->init();

    updateCrossSection();
    cross_section->setShaderDirectory( current_directory );
    cross_section->init();
    cross_section->setBoundingBox( ( float ) ox_, ( float ) (ox_ + w_), ( float ) oy_, ( float ) (oy_ + h_),
                                  ( float ) oz_, ( float ) (oz_ + d_) );

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

    if( show_axis == true )
    {
        axes.draw( V_, P_, w_, h_ );
    }

    if( input_volume != nullptr )
        input_volume->draw( V_, P_, w_, h_ );

    for( auto& it : object_list )
        (it.second)->draw( V_, P_, w_, h_ );

    if( cross_section != nullptr )
        cross_section->draw( V_, P_, w_, h_ );

}


void Scene3D::updateScene()
{
    input_volume->update();
//    updateCrossSection();

    for( auto &it_: object_list )
    {
        ObjectOpenGLWrapper* obj_ = it_.second;
        obj_->reloadBuffers();
    }

    emit updateCanvas();
}


void Scene3D::clearScene()
{

    if( input_volume != nullptr )
    {
        input_volume->clear();
        delete input_volume;
        input_volume = nullptr;
    }

//    if( cross_section != nullptr )
//    {
////        cross_section->clear();
//        delete cross_section;
//        cross_section = nullptr;
//    }

    for( auto &it: object_list )
    {
        (it.second)->clear();
        delete (it.second);
    }
    object_list.clear();

    current_color = QColor( 255, 0, 0 );

}
