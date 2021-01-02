#include "DirectoryWatcher.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <iostream>

DirectoryWatcher::DirectoryWatcher() {

    connect(this, SIGNAL(directoryChanged(QString)),this, SLOT(handleChange(QString)));
    connect(this, SIGNAL(fileChanged(QString)),this, SLOT(handleChange(QString)));

}

DirectoryWatcher::~DirectoryWatcher() {
    QJsonDocument jsonDocument = QJsonDocument(jsonEventObjects);
    QFile jsonFile("log.json");
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(jsonDocument.toJson());
}

bool DirectoryWatcher::setDirectoryPath(QString path) {

    if(path.isEmpty()) {
        qDebug().noquote().nospace() << "Path is empty\n";
        return false;
    }
    #ifdef _WIN32
        path = path.remove(0,8); //URL to string correction
    #else
        path = path.remove(0,7); //URL to string correction
    #endif
    addPath(path);

    QStringList files = QDir(path).entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
    fileInfoList = QDir(path).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    std::sort(fileInfoList.begin(), fileInfoList.end(), [](QFileInfo a, QFileInfo b) {return a.lastModified() > b.lastModified();});
    m_filesList = files;
    emit filesListChanged();
    return true;
}

void DirectoryWatcher::handleChange(QString path) {
    QString event;
    QJsonObject jsonEvent;
    QStringList files = QDir(path).entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
    QFileInfoList tempInfoList = QDir(path).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    std::sort(tempInfoList.begin(), tempInfoList.end(), [](QFileInfo a, QFileInfo b) {return a.lastModified() > b.lastModified();});
    if(tempInfoList.size() > fileInfoList.size()) {
        //Entry created
        qDebug().noquote().nospace() << tempInfoList[0];
        event = path + " Entry created: " + tempInfoList[0].fileName();
        jsonEvent["Path"] = path;
        jsonEvent["Event Type"] = "Entry creation";
        jsonEvent["File"] = tempInfoList[0].fileName();
    }
    else if (tempInfoList.size() < fileInfoList.size()) {
        //Entry deleted
        for(int i =0; fileInfoList.length() > i; i++) {
            QFileInfo a = fileInfoList[i];
            auto deletedFile = std::find_if_not(
              tempInfoList.begin(), tempInfoList.end(),
              [&a](QFileInfo fi) { return fi == a; }
            );
            fileInfoList.removeOne((*deletedFile));
        }
        qDebug().noquote().nospace() << fileInfoList[0];
        event = path + " Entry deleted: " + fileInfoList[0].fileName();
        jsonEvent["Path"] = path;
        jsonEvent["Event Type"] = "Entry deletion";
        jsonEvent["File"] = fileInfoList[0].fileName();
    }
    else if(tempInfoList[0] != fileInfoList[0]) {
        //Entry modified
        for(int i =0; tempInfoList.length() > i; i++) {
            QFileInfo a = tempInfoList[i];
            auto deletedFile = std::find_if_not(
              tempInfoList.begin(), tempInfoList.end(),
              [&a](QFileInfo fi) { return fi == a; }
            );
            fileInfoList.removeOne((*deletedFile));
        }
        qDebug().noquote().nospace() << fileInfoList[0] << " > " << tempInfoList[0];
        event = path + " Entry modified: " + fileInfoList[0].fileName() + " > " + tempInfoList[0].fileName();
        jsonEvent["Path"] = path;
        jsonEvent["Event Type"] = "Entry deletion";
        jsonEvent["File"] = fileInfoList[0].fileName();
        jsonEvent["Modified File"] = tempInfoList[0].fileName();
    }
    fileInfoList = tempInfoList;
    m_filesList = files;
    m_eventsList += event;
    jsonEventObjects << jsonEvent;
    emit filesListChanged();
    emit eventsListChanged();
}

void DirectoryWatcher::printFilesList() {
    qDebug().noquote().nospace() << m_filesList << "\n";
}

void DirectoryWatcher::stopMonitoring() {
    removePaths(directories());
    m_filesList.clear();
    emit filesListChanged();
}
