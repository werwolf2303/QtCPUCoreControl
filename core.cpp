//
// Created by werwolf2303 on 8/3/25.
//

#include "core.h"

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <QApplication>
#include <QInputDialog>
#include <QProcess>

bool startsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() &&
           str.compare(0, prefix.size(), prefix) == 0;
}


bool containsDigit(const std::string& str) {
    return std::any_of(str.begin(), str.end(), ::isdigit);
}

core::core(): coreCount(0) {
    auto files = std::filesystem::directory_iterator("/sys/devices/system/cpu");

    for (const auto &entry: files) {
        if (entry.is_directory() && startsWith(entry.path().filename(), "cpu") &&
            containsDigit(entry.path().filename())) {
            coreCount += 1;
        }
    }
}

void core::setCoreState(int core, int state, QDialog *parent) {
    QString helperPath = QCoreApplication::applicationDirPath() + "/cpu_toggle_helper";
    QStringList args = { helperPath, QString::number(core), QString::number(state) };
    QString stdoutOutput, stderrOutput;

    {
        QProcess process;
        QStringList sudoArgs = QStringList() << "-n" << args;
        process.start("sudo", sudoArgs);

        if (process.waitForFinished(5000) && process.exitCode() == 0) {
            stdoutOutput = process.readAllStandardOutput();
            stderrOutput = process.readAllStandardError();
            qDebug() << "Ran successfully without password prompt.";
            qDebug() << "stdout:" << stdoutOutput;
            qDebug() << "stderr:" << stderrOutput;
            return;
        }
    }

    bool ok;
    QString password = QInputDialog::getText(parent,
                                             "Authentication Required",
                                             "Enter your password:",
                                             QLineEdit::Password,
                                             QString(),
                                             &ok);
    if (!ok || password.isEmpty()) {
        qDebug() << "Password input cancelled or empty";
        return;
    }

    {
        QProcess process;
        QStringList sudoArgs = QStringList() << "-S" << args;
        process.start("sudo", sudoArgs);

        if (!process.waitForStarted()) {
            qDebug() << "Failed to start sudo process";
            return;
        }

        process.write(password.toUtf8() + "\n");
        process.closeWriteChannel();

        if (!process.waitForFinished()) {
            qDebug() << "Process did not finish properly";
            return;
        }

        stdoutOutput = process.readAllStandardOutput();
        stderrOutput = process.readAllStandardError();

        qDebug() << "stdout:" << stdoutOutput;
        qDebug() << "stderr:" << stderrOutput;
    }
}

int core::getCoreState(int core) {
    if (core == 0) return 1;

    auto path = "/sys/devices/system/cpu/cpu" + std::to_string(core) + "/online";
    std::ifstream file(path);
    std::string value;
    std::getline(file, value);
    return value == "1";
}
