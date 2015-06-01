#ifndef SMBXCONFIG_H
#define SMBXCONFIG_H

#include <QObject>
#include <autostartconfig.h>
#include <QVariant>

class SMBXConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AutostartConfig* Autostart READ Autostart)
public:
    explicit SMBXConfig(QObject *parent = 0);
    ~SMBXConfig();

    AutostartConfig* Autostart() const
    {
        return m_Autostart;
    }

public slots:
    QVariantList getEpisodeInfo(const QString& subDirPerEpisode, const QString &jsonFileName);


signals:
    void runSMBX();
    void runSMBXEditor();
    void loadEpisodeWebpage(const QString& file);

private:

    QString m_Episode;
    QString m_SMBXExeFilename;
    AutostartConfig* m_Autostart;
};

#endif // SMBXCONFIG_H
