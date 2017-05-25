#include "ColorPicker.h"

ColorPicker::ColorPicker( QWidget* parent )
{
    createWidget();
    createActions();
}

void ColorPicker::createWidget()
{
    cd_picker_color = new QColorDialog();
    cd_picker_color->setWindowFlags( Qt::Widget );
    cd_picker_color->setCurrentColor( QColor( 255, 0, 0 ) );

    wa_picker_color = new QWidgetAction( this );
    wa_picker_color->setDefaultWidget( cd_picker_color );

    mn_picker_color = new QMenu();
    mn_picker_color->addAction( wa_picker_color );

    setPopupMode( QToolButton::MenuButtonPopup );
    setMenu( mn_picker_color );
    setCheckable( true );

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

void ColorPicker::colorChanged( const QColor& color_ )
{
    QPixmap px( 20, 20 );
    px.fill( QColor( color_.red(), color_.green(), color_.blue() ) );
    setIcon( px );
}

