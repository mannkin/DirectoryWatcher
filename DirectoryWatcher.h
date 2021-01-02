#ifndef DIRECTORYWATCHER_H
#define DIRECTORYWATCHER_H

#include <QFileSystemWatcher>
#include <QJsonArray>
#include <QFileInfo>
#include <qqml.h>

class DirectoryWatcher : public QFileSystemWatcher{
    Q_OBJECT
    Q_PROPERTY(QStringList filesList MEMBER m_filesList NOTIFY filesListChanged)
    Q_PROPERTY(QStringList eventsList MEMBER m_eventsList NOTIFY eventsListChanged)
    QStringList m_filesList;
    QStringList m_eventsList;
public:
    DirectoryWatcher();
    ~DirectoryWatcher();
    QFileInfoList fileInfoList;
    QJsonArray jsonEventObjects;
public slots:
    bool setDirectoryPath(QString path);
    void handleChange(QString path);
    void printFilesList();
    void stopMonitoring();
signals:
    void filesListChanged();
    void eventsListChanged();
};

#endif // DIRECTORYWATCHER_H
