#include <QWidget>
#include <QMessageBox>

#include "qt_popup.hpp"


void displayNotice( std::string header, std::string info )
{
    class Widget : public QWidget {
        public:
            Widget(std::string &header, std::string &info) {
                QMessageBox::information( 
                        this, 
                        tr(header.c_str()),
                        tr(info.c_str()) ); 
            }
    } w(header, info);
}

void deprecationNotice( std::string deprecated_method, std::string new_method, bool &popup_is_enabled )
{
    std::string header, info;

    header = "Deprecation notice:";
    info   = "The following method is deprecated: \n >>> RulesProcessor::" + deprecated_method + "\n\n";
    info  += "Please, use method: \n >>> RulesProcessor::" + new_method + " instead.\n\n";
    info  += "This message appears only once per run.";

    if ( popup_is_enabled )
    {
        displayNotice(header, info);
    }

    popup_is_enabled = false;

    /* if ( !enable_popup ) */
    /* { */
    /*     class Widget : public QWidget { */
    /*         public: */
    /*             Widget(std::string &header, std::string &info) { */
    /*                 QMessageBox::information( */ 
    /*                         this, */ 
    /*                         tr(header.c_str()), */
    /*                         tr(info.c_str()) ); */ 
    /*             } */
    /*     } w(header, info); */
    /* } */

    /* enable_popup = true; */
}
