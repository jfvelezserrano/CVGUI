#include "ViewerWindow.h"
#include <QApplication>

LogWindow *l = NULL;

class ViewerQApplication : public QApplication
{
public:
    ViewerQApplication(int &argc, char *argv[]):QApplication(argc,argv)
    {
    }

    bool notify(QObject *receiver, QEvent *event)
    {
        try
        {
            return QApplication::notify(receiver, event);
        }
        catch (std::exception &ex)
        {
            l->addLog("Catch exception at " + QTime::currentTime().toString() + " - " + QDate::currentDate().toString()+"\n");
            l->addLog("Receiver: " + receiver->objectName()+"\n");
            l->addLog("Event: " + QString::number((int)event->type())+"\n");
            l->addLog("Details: ");
            l->addLog(ex.what());
            l->addLog("\n");
        }
        catch (...)
        {
            l->addLog("Catch exception at " + QTime::currentTime().toString() + " - " + QDate::currentDate().toString()+"\n");
            l->addLog("Receiver: " + receiver->objectName()+"\n");
            l->addLog("Event: " + QString::number((int)event->type())+"\n");
            l->addLog("\n");
        }

        return false;
    }
};

int main(int argc, char *argv[])
{
    ViewerQApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/Resources/GavabIcon.ico"));
    ViewerWindow w;
    l = w.getLogWindow();
    w.show();

    return a.exec();
}
