#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QtWidgets/QToolButton>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QMenuBar>
#include <QPixmap>


class ColorPicker: public QToolButton
{
    Q_OBJECT

    public:

        ColorPicker( QWidget* parent = 0 );

        void setColor( const QColor& c );
        QColor currentColor() const;

    protected:

        void createWidget();
        void createActions();


    protected slots:

        void colorChanged( const QColor& color_ );


    signals:

        void colorSelected( const QColor& );

    private:

        QColorDialog* cd_picker_color;
        QWidgetAction* wa_picker_color;
        QMenu* mn_picker_color;

};

#endif // COLORPICKER_H
