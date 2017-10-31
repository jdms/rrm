#include <random>
#include <QHBoxLayout>

#include "color_picker.h"


ColorPicker::ColorPicker( QWidget* parent )
{
    createWidget();
    createActions();
}

void ColorPicker::createWidget()
{
    cd_picker_color = new QColorDialog( this );
    cd_picker_color->setWindowFlags( Qt::Widget );
    cd_picker_color->setCurrentColor( QColor( 255, 0, 0 ) );

    wa_picker_color = new QWidgetAction( this );
    wa_picker_color->setDefaultWidget( cd_picker_color );

    mn_picker_color = new QMenu( this );
    mn_picker_color->addAction( wa_picker_color );

    setPopupMode( QToolButton::MenuButtonPopup );
    setMenu( mn_picker_color );
    setCheckable( true );

    colorChanged( currentColor() );


}

void ColorPicker::createActions()
{
    connect( mn_picker_color, &QMenu::aboutToShow, cd_picker_color, &QColorDialog::show );
    connect( cd_picker_color, &QColorDialog::rejected, mn_picker_color, &QMenu::hide );
    connect( cd_picker_color, &QColorDialog::accepted, mn_picker_color, &QMenu::hide );

    connect( cd_picker_color, &QColorDialog::currentColorChanged, this, &ColorPicker::colorChanged );
    connect( cd_picker_color, &QColorDialog::colorSelected, [=]( const QColor& color_ )
                                                            { emit colorSelected( color_ ); } );

    connect( this, &ColorPicker::toggled, [=]( bool status_ ){
                                          if( status_ == true ) setColor( cd_picker_color->currentColor() );
                                          else defineRandomColor(); } );


}


void ColorPicker::setColor( const QColor& c )
{
    cd_picker_color->setCurrentColor( c );
    colorChanged( c );
}

QColor ColorPicker::currentColor() const
{
    return cd_picker_color->currentColor();
}


void ColorPicker::colorChanged( const QColor& color_ )
{

    QPixmap px( 15, 15 );
    px.fill( QColor( color_.red(), color_.green(), color_.blue() ) );
    setIcon( px );
}


void ColorPicker::defineRandomColor()
{
    QColor c;

    bool status = this->isChecked();

    if( status == false )
        c = randomColor();
    else
        c = cd_picker_color->currentColor();

    setColor( c );

    emit colorSelected( c );

}


QColor ColorPicker::randomColor()
{
    std::random_device rd;
    std::mt19937 eng( rd() );
    std::uniform_int_distribution< size_t > distr( 0, 255 );

    int r = distr( eng );
    int b = distr( eng );
    int g = distr( eng );

    return QColor( r, g, b );
}
