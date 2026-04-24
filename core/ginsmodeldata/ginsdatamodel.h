#ifndef GINSDATAMODEL_H
#define GINSDATAMODEL_H

#include <QObject>
#include <QVariantMap>
#include "data.pb.h"

// 单条船 GINS 数据模型：
// - 内部存储一个 protobuf gins::data::OutMsg；
// - 每个字段暴露为 Q_PROPERTY，支持 QWidget/QML 绑定；
// - 提供 QVariantMap 接口与序列化/反序列化能力。
class GinsDataModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(uint   port       READ port       WRITE setPort       NOTIFY portChanged)
    Q_PROPERTY(double utcLocal   READ utcLocal   WRITE setUtcLocal   NOTIFY utcLocalChanged)
    Q_PROPERTY(double latitude   READ latitude   WRITE setLatitude   NOTIFY latitudeChanged)
    Q_PROPERTY(double longitude  READ longitude  WRITE setLongitude  NOTIFY longitudeChanged)
    Q_PROPERTY(double altitude   READ altitude   WRITE setAltitude   NOTIFY altitudeChanged)
    Q_PROPERTY(double rollDeg    READ rollDeg    WRITE setRollDeg    NOTIFY rollDegChanged)
    Q_PROPERTY(double pitchDeg   READ pitchDeg   WRITE setPitchDeg   NOTIFY pitchDegChanged)
    Q_PROPERTY(double headingDeg READ headingDeg WRITE setHeadingDeg NOTIFY headingDegChanged)
    Q_PROPERTY(double vnMps      READ vnMps      WRITE setVnMps      NOTIFY vnMpsChanged)
    Q_PROPERTY(double veMps      READ veMps      WRITE setVeMps      NOTIFY veMpsChanged)
    Q_PROPERTY(double roti       READ roti       WRITE setRoti       NOTIFY rotiChanged)
    Q_PROPERTY(double ax         READ ax         WRITE setAx         NOTIFY axChanged)
    Q_PROPERTY(double ay         READ ay         WRITE setAy         NOTIFY ayChanged)
    Q_PROPERTY(double aaz        READ aaz        WRITE setAaz        NOTIFY aazChanged)
    Q_PROPERTY(uint   ginsStatus READ ginsStatus WRITE setGinsStatus NOTIFY ginsStatusChanged)
    Q_PROPERTY(double hdop       READ hdop       WRITE setHdop       NOTIFY hdopChanged)
    Q_PROPERTY(double headStd    READ headStd    WRITE setHeadStd    NOTIFY headStdChanged)

public:
    explicit GinsDataModel(QObject *parent = nullptr);

    enum ErrorType {
        NoError = 0,
        InvalidData,
        SerializeError,
        DeserializeError,
        UnknownError
    };
    Q_ENUM(ErrorType)

    /* ---------- 属性读取（展示层单位） ---------- */
    uint   port()       const;
    double utcLocal()   const;
    double latitude()   const;
    double longitude()  const;
    double altitude()   const;
    double rollDeg()    const;
    double pitchDeg()   const;
    double headingDeg() const;
    double vnMps()      const;
    double veMps()      const;
    double roti()       const;
    double ax()         const;
    double ay()         const;
    double aaz()        const;
    uint   ginsStatus() const;
    double hdop()       const;
    double headStd()    const;

    /* ---------- 属性写入 ---------- */
    void setPort(uint value);
    void setUtcLocal(double value);
    void setLatitude(double value);
    void setLongitude(double value);
    void setAltitude(double value);
    void setRollDeg(double value);
    void setPitchDeg(double value);
    void setHeadingDeg(double value);
    void setVnMps(double value);
    void setVeMps(double value);
    void setRoti(double value);
    void setAx(double value);
    void setAy(double value);
    void setAaz(double value);
    void setGinsStatus(uint value);
    void setHdop(double value);
    void setHeadStd(double value);

    /* ---------- QVariantMap 接口 ---------- */
    Q_INVOKABLE QVariantMap toMap() const;
    Q_INVOKABLE void fromMap(const QVariantMap &map);

    /* ---------- Proto 接口 ---------- */
    void setProto(const gins::data::OutMsg &data);
    gins::data::OutMsg proto() const;

    /* ---------- 序列化 ---------- */
    Q_INVOKABLE QByteArray serialize() const;
    Q_INVOKABLE bool deserialize(const QByteArray &bytes);

    /* ---------- 重置 ---------- */
    Q_INVOKABLE void clear();

    /* ---------- 调试 ---------- */
    Q_INVOKABLE void printData() const;

signals:
    void portChanged();
    void utcLocalChanged();
    void latitudeChanged();
    void longitudeChanged();
    void altitudeChanged();
    void rollDegChanged();
    void pitchDegChanged();
    void headingDegChanged();
    void vnMpsChanged();
    void veMpsChanged();
    void rotiChanged();
    void axChanged();
    void ayChanged();
    void aazChanged();
    void ginsStatusChanged();
    void hdopChanged();
    void headStdChanged();

    void dataChanged();
    void errorOccurred(int errorType, const QString &errorMessage);

private:
    static QVariantMap protoToMap(const gins::data::OutMsg &d);
    static void mapToProto(const QVariantMap &map, gins::data::OutMsg &d);

    gins::data::OutMsg m_data;
};

#endif // GINSDATAMODEL_H
