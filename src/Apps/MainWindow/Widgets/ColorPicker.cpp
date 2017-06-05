#include "ColorPicker.h"

#include <QHBoxLayout>

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

}


void ColorPicker::setColor( const QColor& c )
{
    cd_picker_color->setCurrentColor( c );
    colorChanged( c );
}

QColor ColorPicker::currentColor() const
{
    return cd_picker_color->selectedColor();
}


void ColorPicker::colorChanged( const QColor& color_ )
{

    QPixmap px( 15, 15 );
    px.fill( QColor( color_.red(), color_.green(), color_.blue() ) );
    setIcon( px );
}

