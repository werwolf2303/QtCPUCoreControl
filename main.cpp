#include <QVBoxLayout>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMenu>
#include <QDialog>
#include <QScreen>
#include <QCheckBox>
#include "core.h"


int main(int argc, char **argv) {
    QApplication app(argc, argv);

    core core;

    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon::fromTheme("cpu"));
    trayIcon.setToolTip("CPU Core Control");

    QMenu menu;
    menu.addAction("Exit", QApplication::quit);
    trayIcon.setContextMenu(&menu);

    QObject::connect(&trayIcon, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            QDialog *dialog = new QDialog;
            dialog->setWindowTitle("CPU Control");
            dialog->resize(300, 200);
            dialog->setAttribute(Qt::WA_DeleteOnClose);

            QVBoxLayout *layout = new QVBoxLayout(dialog);
            dialog->setLayout(layout);

            for (int i = 0; i < core.getCoreCount(); i++) {
                QCheckBox *checkBox = new QCheckBox(QString("Core ").append(QString::number(i + 1)), dialog);
                if (i == 0) {
                    checkBox->setDisabled(true);
                    checkBox->setChecked(true);
                }
                if (i != 0) checkBox->setChecked(core.getCoreState(i));

                auto updating = new bool(false);

                QObject::connect(checkBox, &QCheckBox::toggled, [checkBox, i, updating, &core, dialog](bool checked) {
                    if (*updating) return;
                    *updating = true;

                    core.setCoreState(i, checked, dialog);
                    checkBox->setChecked(core.getCoreState(i));

                    *updating = false;
                });


                layout->addWidget(checkBox);
            }

            dialog->show();
            dialog->raise();
            dialog->activateWindow();
        }
    });

    trayIcon.show();
    return app.exec();
}
