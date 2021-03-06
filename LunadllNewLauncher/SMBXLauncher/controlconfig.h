#ifndef CONTROLCONFIG_H
#define CONTROLCONFIG_H

#include <QObject>
#include <PGE_File_Formats/file_formats.h>


class ControlConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool fullscreen READ fullscreen WRITE setFullscreen)
    Q_PROPERTY(unsigned int controllerType1 READ controllerType1 WRITE setControllerType1)
    Q_PROPERTY(unsigned int keyboardUp1 READ keyboardUp1 WRITE setKeyboardUp1)
    Q_PROPERTY(unsigned int keyboardDown1 READ keyboardDown1 WRITE setKeyboardDown1)
    Q_PROPERTY(unsigned int keyboardLeft1 READ keyboardLeft1 WRITE setKeyboardLeft1)
    Q_PROPERTY(unsigned int keyboardRight1 READ keyboardRight1 WRITE setKeyboardRight1)
    Q_PROPERTY(unsigned int keyboardRun1 READ keyboardRun1 WRITE setKeyboardRun1)
    Q_PROPERTY(unsigned int keyboardJump1 READ keyboardJump1 WRITE setKeyboardJump1)
    Q_PROPERTY(unsigned int keyboardDrop1 READ keyboardDrop1 WRITE setKeyboardDrop1)
    Q_PROPERTY(unsigned int keyboardPause1 READ keyboardPause1 WRITE setKeyboardPause1)
    Q_PROPERTY(unsigned int keyboardAltJump1 READ keyboardAltJump1 WRITE setKeyboardAltJump1)
    Q_PROPERTY(unsigned int keyboardAltRun1 READ keyboardAltRun1 WRITE setKeyboardAltRun1)
    Q_PROPERTY(unsigned int joystickRun1 READ joystickRun1 WRITE setJoystickRun1)
    Q_PROPERTY(unsigned int joystickJump1 READ joystickJump1 WRITE setJoystickJump1)
    Q_PROPERTY(unsigned int joystickDrop1 READ joystickDrop1 WRITE setJoystickDrop1)
    Q_PROPERTY(unsigned int joystickPause1 READ joystickPause1 WRITE setJoystickPause1)
    Q_PROPERTY(unsigned int joystickAltJump1 READ joystickAltJump1 WRITE setJoystickAltJump1)
    Q_PROPERTY(unsigned int joystickAltRun1 READ joystickAltRun1 WRITE setJoystickAltRun1)

    Q_PROPERTY(unsigned int controllerType2 READ controllerType2 WRITE setControllerType2)
    Q_PROPERTY(unsigned int keyboardUp2 READ keyboardUp2 WRITE setKeyboardUp2)
    Q_PROPERTY(unsigned int keyboardDown2 READ keyboardDown2 WRITE setKeyboardDown2)
    Q_PROPERTY(unsigned int keyboardLeft2 READ keyboardLeft2 WRITE setKeyboardLeft2)
    Q_PROPERTY(unsigned int keyboardRight2 READ keyboardRight2 WRITE setKeyboardRight2)
    Q_PROPERTY(unsigned int keyboardRun2 READ keyboardRun2 WRITE setKeyboardRun2)
    Q_PROPERTY(unsigned int keyboardJump2 READ keyboardJump2 WRITE setKeyboardJump2)
    Q_PROPERTY(unsigned int keyboardDrop2 READ keyboardDrop2 WRITE setKeyboardDrop2)
    Q_PROPERTY(unsigned int keyboardPause2 READ keyboardPause2 WRITE setKeyboardPause2)
    Q_PROPERTY(unsigned int keyboardAltJump2 READ keyboardAltJump2 WRITE setKeyboardAltJump2)
    Q_PROPERTY(unsigned int keyboardAltRun2 READ keyboardAltRun2 WRITE setKeyboardRun2)
    Q_PROPERTY(unsigned int joystickRun2 READ joystickRun2 WRITE setJoystickRun2)
    Q_PROPERTY(unsigned int joystickJump2 READ joystickJump2 WRITE setJoystickJump2)
    Q_PROPERTY(unsigned int joystickDrop2 READ joystickDrop2 WRITE setJoystickDrop2)
    Q_PROPERTY(unsigned int joystickPause2 READ joystickPause2 WRITE setJoystickPause2)
    Q_PROPERTY(unsigned int joystickAltJump2 READ joystickAltJump2 WRITE setJoystickAltJump2)
    Q_PROPERTY(unsigned int joystickAltRun2 READ joystickAltRun2 WRITE setJoystickAltRun2)

public:
    explicit ControlConfig(QObject *parent = 0);
    explicit ControlConfig(const QString& configFilename, QObject *parent = 0);



    bool fullscreen() const
    {
        return m_data.fullScreen;
    }

    unsigned int controllerType1() const
    {
        return m_data.players[0].controllerType;
    }

    unsigned int keyboardUp1() const
    {
        return m_data.players[0].k_up;
    }

    unsigned int keyboardDown1() const
    {
        return m_data.players[0].k_down;
    }

    unsigned int keyboardLeft1() const
    {
        return m_data.players[0].k_left;
    }

    unsigned int keyboardRight1() const
    {
        return m_data.players[0].k_right;
    }

    unsigned int keyboardRun1() const
    {
        return m_data.players[0].k_run;
    }

    unsigned int keyboardJump1() const
    {
        return m_data.players[0].k_jump;
    }

    unsigned int keyboardDrop1() const
    {
        return m_data.players[0].k_drop;
    }

    unsigned int keyboardPause1() const
    {
        return m_data.players[0].k_pause;
    }

    unsigned int keyboardAltJump1() const
    {
        return m_data.players[0].k_altjump;
    }

    unsigned int keyboardAltRun1() const
    {
        return m_data.players[0].k_altrun;
    }

    unsigned int joystickRun1() const
    {
        return m_data.players[0].j_run;
    }

    unsigned int joystickJump1() const
    {
        return m_data.players[0].j_jump;
    }

    unsigned int joystickDrop1() const
    {
        return m_data.players[0].j_drop;
    }

    unsigned int joystickPause1() const
    {
        return m_data.players[0].j_pause;
    }

    unsigned int joystickAltJump1() const
    {
        return m_data.players[0].j_jump;
    }

    unsigned int joystickAltRun1() const
    {
        return m_data.players[0].j_run;
    }



    unsigned int controllerType2() const
    {
        return m_data.players[1].controllerType;
    }

    unsigned int keyboardUp2() const
    {
        return m_data.players[1].k_up;
    }

    unsigned int keyboardDown2() const
    {
        return m_data.players[1].k_down;
    }

    unsigned int keyboardLeft2() const
    {
        return m_data.players[1].k_left;
    }

    unsigned int keyboardRight2() const
    {
        return m_data.players[1].k_right;
    }

    unsigned int keyboardRun2() const
    {
        return m_data.players[1].k_run;
    }

    unsigned int keyboardJump2() const
    {
        return m_data.players[1].k_jump;
    }

    unsigned int keyboardDrop2() const
    {
        return m_data.players[1].k_drop;
    }

    unsigned int keyboardPause2() const
    {
        return m_data.players[1].k_pause;
    }

    unsigned int keyboardAltJump2() const
    {
        return m_data.players[1].k_altjump;
    }

    unsigned int keyboardAltRun2() const
    {
        return m_data.players[1].k_altrun;
    }

    unsigned int joystickRun2() const
    {
        return m_data.players[1].j_run;
    }

    unsigned int joystickJump2() const
    {
        return m_data.players[1].j_jump;
    }

    unsigned int joystickDrop2() const
    {
        return m_data.players[1].j_drop;
    }

    unsigned int joystickPause2() const
    {
        return m_data.players[1].j_pause;
    }

    unsigned int joystickAltJump2() const
    {
        return m_data.players[1].j_jump;
    }

    unsigned int joystickAltRun2() const
    {
        return m_data.players[1].j_run;
    }

public slots:
    bool read();
    bool write();
    void setFullscreen(bool fullscreen)
    {
        m_data.fullScreen = fullscreen;
    }

    void setControllerType1(unsigned int controllerType1)
    {
        m_data.players[0].controllerType = controllerType1;
    }

    void setKeyboardUp1(unsigned int keyboardUp1)
    {
        m_data.players[0].k_up = keyboardUp1;
    }

    void setKeyboardDown1(unsigned int keyboardDown1)
    {
        m_data.players[0].k_down = keyboardDown1;
    }

    void setKeyboardLeft1(unsigned int keyboardLeft1)
    {
        m_data.players[0].k_left = keyboardLeft1;
    }

    void setKeyboardRight1(unsigned int keyboardRight1)
    {
        m_data.players[0].k_right = keyboardRight1;
    }

    void setKeyboardRun1(unsigned int keyboardRun1)
    {
        m_data.players[0].k_run = keyboardRun1;
    }

    void setKeyboardJump1(unsigned int keyboardJump1)
    {
        m_data.players[0].k_jump = keyboardJump1;
    }

    void setKeyboardDrop1(unsigned int keyboardDrop1)
    {
        m_data.players[0].k_drop = keyboardDrop1;
    }

    void setKeyboardPause1(unsigned int keyboardPause1)
    {
        m_data.players[0].k_pause = keyboardPause1;
    }

    void setKeyboardAltJump1(unsigned int keyboardAltJump1)
    {
        m_data.players[0].k_altjump = keyboardAltJump1;
    }

    void setKeyboardAltRun1(unsigned int keyboardAltRun1)
    {
        m_data.players[0].k_altrun = keyboardAltRun1;
    }

    void setJoystickRun1(unsigned int joystickRun1)
    {
        m_data.players[0].j_run = joystickRun1;
    }

    void setJoystickJump1(unsigned int joystickJump1)
    {
        m_data.players[0].j_jump = joystickJump1;
    }

    void setJoystickDrop1(unsigned int joystickDrop1)
    {
        m_data.players[0].j_drop = joystickDrop1;
    }

    void setJoystickPause1(unsigned int joystickPause1)
    {
        m_data.players[0].j_pause = joystickPause1;
    }

    void setJoystickAltJump1(unsigned int joystickAltJump1)
    {
        m_data.players[0].j_altjump = joystickAltJump1;
    }

    void setJoystickAltRun1(unsigned int joystickAltRun1)
    {
        m_data.players[0].j_altrun = joystickAltRun1;
    }



    void setControllerType2(unsigned int controllerType2)
    {
        m_data.players[1].controllerType = controllerType2;
    }

    void setKeyboardUp2(unsigned int keyboardUp2)
    {
        m_data.players[1].k_up = keyboardUp2;
    }

    void setKeyboardDown2(unsigned int keyboardDown2)
    {
        m_data.players[1].k_down = keyboardDown2;
    }

    void setKeyboardLeft2(unsigned int keyboardLeft2)
    {
        m_data.players[1].k_left = keyboardLeft2;
    }

    void setKeyboardRight2(unsigned int keyboardRight2)
    {
        m_data.players[1].k_right = keyboardRight2;
    }

    void setKeyboardRun2(unsigned int keyboardRun2)
    {
        m_data.players[1].k_run = keyboardRun2;
    }

    void setKeyboardJump2(unsigned int keyboardJump2)
    {
        m_data.players[1].k_jump = keyboardJump2;
    }

    void setKeyboardDrop2(unsigned int keyboardDrop2)
    {
        m_data.players[1].k_drop = keyboardDrop2;
    }

    void setKeyboardPause2(unsigned int keyboardPause2)
    {
        m_data.players[1].k_pause = keyboardPause2;
    }

    void setKeyboardAltJump2(unsigned int keyboardAltJump2)
    {
        m_data.players[1].k_altjump = keyboardAltJump2;
    }

    void setKeyboardAltRun2(unsigned int keyboardAltRun2)
    {
        m_data.players[1].k_altrun = keyboardAltRun2;
    }

    void setJoystickRun2(unsigned int joystickRun2)
    {
        m_data.players[1].j_run = joystickRun2;
    }

    void setJoystickJump2(unsigned int joystickJump2)
    {
        m_data.players[1].j_jump = joystickJump2;
    }

    void setJoystickDrop2(unsigned int joystickDrop2)
    {
        m_data.players[1].j_drop = joystickDrop2;
    }

    void setJoystickPause2(unsigned int joystickPause2)
    {
        m_data.players[1].j_pause = joystickPause2;
    }

    void setJoystickAltJump2(unsigned int joystickAltJump2)
    {
        m_data.players[1].j_altjump = joystickAltJump2;
    }

    void setJoystickAltRun2(unsigned int joystickAltRun2)
    {
        m_data.players[1].j_altrun = joystickAltRun2;
    }

private:
    QString m_configFilename;
    SMBX64_ConfigFile m_data;

    bool m_fullscreen;
    unsigned int m_controllerType1;
    unsigned int m_keyboardUp1;
    unsigned int m_keyboardDown1;
    unsigned int m_keyboardLeft1;
    unsigned int m_keyboardRight1;
    unsigned int m_keyboardRun1;
    unsigned int m_keyboardJump1;
    unsigned int m_keyboardDrop1;
    unsigned int m_keyboardPause1;
    unsigned int m_keyboardAltJump1;
    unsigned int m_keyboardAltRun1;
    unsigned int m_joystickRun1;
    unsigned int m_joystickJump1;
    unsigned int m_joystickDrop1;
    unsigned int m_joystickPause1;
    unsigned int m_joystickAltJump1;
    unsigned int m_joystickAltRun1;
    unsigned int m_controllerType2;
    unsigned int m_keyboardUp2;
    unsigned int m_keyboardDown2;
    unsigned int m_keyboardLeft2;
    unsigned int m_keyboardRight2;
    unsigned int m_keyboardRun2;
    unsigned int m_keyboardJump2;
    unsigned int m_keyboardDrop2;
    unsigned int m_keyboardPause2;
    unsigned int m_keyboardAltJump2;
    unsigned int m_keyboardAltRun2;
    unsigned int m_joystickRun2;
    unsigned int m_joystickJump2;
    unsigned int m_joystickDrop2;
    unsigned int m_joystickPause2;
    unsigned int m_joystickAltJump2;
    unsigned int m_joystickAltRun2;
};

#endif // CONTROLCONFIG_H
