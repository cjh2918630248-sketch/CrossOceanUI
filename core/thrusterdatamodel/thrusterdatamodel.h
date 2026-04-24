#ifndef THRUSTERDATAMODEL_H
#define THRUSTERDATAMODEL_H

#include <QObject>
#include <QVariantMap>
#include "thruster.pb.h"

// 单个推进器数据模型：
// - 内部存储一条 protobuf mcu::thruster::Data；
// - 附带 port / thruster_num 元信息；
// - 每个字段对外暴露 Q_PROPERTY，方便 QWidget/QML 绑定；
// - 提供序列化/反序列化与 QVariantMap 互转能力。
class ThrusterDataModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(uint   status       READ status       WRITE setStatus       NOTIFY statusChanged)
    Q_PROPERTY(uint   errorCode    READ errorCode    WRITE setErrorCode    NOTIFY errorCodeChanged)
    Q_PROPERTY(int    rpm          READ rpm          WRITE setRpm          NOTIFY rpmChanged)
    Q_PROPERTY(int    angle        READ angle        WRITE setAngle        NOTIFY angleChanged)
    Q_PROPERTY(int    power        READ power        WRITE setPower        NOTIFY powerChanged)
    Q_PROPERTY(uint   busVoltage   READ busVoltage   WRITE setBusVoltage   NOTIFY busVoltageChanged)
    Q_PROPERTY(int    busCurrent   READ busCurrent   WRITE setBusCurrent   NOTIFY busCurrentChanged)
    Q_PROPERTY(int    phaseCurrent READ phaseCurrent WRITE setPhaseCurrent NOTIFY phaseCurrentChanged)
    Q_PROPERTY(int    motorTemp    READ motorTemp    WRITE setMotorTemp    NOTIFY motorTempChanged)
    Q_PROPERTY(int    mosTemp      READ mosTemp      WRITE setMosTemp      NOTIFY mosTempChanged)
    Q_PROPERTY(int    temp         READ temp         WRITE setTemp         NOTIFY tempChanged)
    Q_PROPERTY(uint   runTime      READ runTime      WRITE setRunTime      NOTIFY runTimeChanged)
    Q_PROPERTY(uint   sumRunTime   READ sumRunTime   WRITE setSumRunTime   NOTIFY sumRunTimeChanged)
    Q_PROPERTY(QString statusText  READ statusText   NOTIFY statusChanged)

    Q_PROPERTY(uint port         READ port         WRITE setPort         NOTIFY portChanged)
    Q_PROPERTY(uint thrusterNum  READ thrusterNum   WRITE setThrusterNum  NOTIFY thrusterNumChanged)

public:
    explicit ThrusterDataModel(QObject *parent = nullptr);

    enum ErrorType {
        NoError = 0,
        InvalidData,
        SerializeError,
        DeserializeError,
        UnknownError
    };
    Q_ENUM(ErrorType)

    /* ---- 字段读取 ---- */
    uint    status()       const;
    uint    errorCode()    const;
    int     rpm()          const;
    int     angle()        const;
    int     power()        const;
    uint    busVoltage()   const;
    int     busCurrent()   const;
    int     phaseCurrent() const;
    int     motorTemp()    const;
    int     mosTemp()      const;
    int     temp()         const;
    uint    runTime()      const;
    uint    sumRunTime()   const;
    QString statusText()   const;

    /* ---- 字段写入 ---- */
    void setStatus(uint val);
    void setErrorCode(uint val);
    void setRpm(int val);
    void setAngle(int val);
    void setPower(int val);
    void setBusVoltage(uint val);
    void setBusCurrent(int val);
    void setPhaseCurrent(int val);
    void setMotorTemp(int val);
    void setMosTemp(int val);
    void setTemp(int val);
    void setRunTime(uint val);
    void setSumRunTime(uint val);

    /* ---- QVariantMap 互转 ---- */
    Q_INVOKABLE QVariantMap toMap() const;
    Q_INVOKABLE void fromMap(const QVariantMap &map);

    /* ---- Proto 直接操作 ---- */
    void loadFromProto(const mcu::thruster::Data &data);
    void exportToProto(mcu::thruster::Data &out) const;

    /* ---- 序列化 ---- */
    Q_INVOKABLE QByteArray serialize() const;
    Q_INVOKABLE bool deserialize(const QByteArray &bytes);

    /* ---- 元信息 ---- */
    uint port() const;
    void setPort(uint port);
    uint thrusterNum() const;
    void setThrusterNum(uint num);

    /* ---- 工具 ---- */
    static QString statusToString(uint32_t status);
    Q_INVOKABLE void printData() const;

signals:
    void statusChanged();
    void errorCodeChanged();
    void rpmChanged();
    void angleChanged();
    void powerChanged();
    void busVoltageChanged();
    void busCurrentChanged();
    void phaseCurrentChanged();
    void motorTempChanged();
    void mosTempChanged();
    void tempChanged();
    void runTimeChanged();
    void sumRunTimeChanged();
    void portChanged();
    void thrusterNumChanged();
    void dataUpdated();
    void errorOccurred(int errorType, const QString &errorMessage);

private:
    mcu::thruster::Data m_data;
    uint32_t m_port        = 0;
    uint32_t m_thrusterNum = 0;
};

#endif // THRUSTERDATAMODEL_H
