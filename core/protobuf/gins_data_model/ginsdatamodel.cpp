#include "ginsdatamodel.h"
#include <QDebug>

GinsDataModel::GinsDataModel(QObject *parent)
    : QObject(parent)
{
}

/* ========================== 属性读取 ========================== */

uint   GinsDataModel::port()       const { return m_data.port(); }
double GinsDataModel::utcLocal()   const { return m_data.utc_local(); }
double GinsDataModel::latitude()   const { return m_data.latitude(); }
double GinsDataModel::longitude()  const { return m_data.longitude(); }
double GinsDataModel::altitude()   const { return static_cast<double>(m_data.altitude()); }
double GinsDataModel::rollDeg()    const { return m_data.roll_deg10() / 10.0; }
double GinsDataModel::pitchDeg()   const { return m_data.pitch_deg10() / 10.0; }
double GinsDataModel::headingDeg() const { return static_cast<double>(m_data.heading_deg()); }
double GinsDataModel::vnMps()      const { return m_data.vn_mps100() / 100.0; }
double GinsDataModel::veMps()      const { return m_data.ve_mps100() / 100.0; }
double GinsDataModel::roti()       const { return static_cast<double>(m_data.roti()); }
double GinsDataModel::ax()         const { return static_cast<double>(m_data.ax()); }
double GinsDataModel::ay()         const { return static_cast<double>(m_data.ay()); }
double GinsDataModel::aaz()        const { return static_cast<double>(m_data.aaz()); }
uint   GinsDataModel::ginsStatus() const { return m_data.gins_status(); }
double GinsDataModel::hdop()       const { return m_data.hdop10() / 10.0; }
double GinsDataModel::headStd()    const { return static_cast<double>(m_data.head_std()); }

/* ========================== 属性写入 ========================== */

void GinsDataModel::setPort(uint value)
{
    if (m_data.port() == value) return;
    m_data.set_port(value);
    emit portChanged();
    emit dataChanged();
}

void GinsDataModel::setUtcLocal(double value)
{
    if (qFuzzyCompare(m_data.utc_local(), value)) return;
    m_data.set_utc_local(value);
    emit utcLocalChanged();
    emit dataChanged();
}

void GinsDataModel::setLatitude(double value)
{
    if (qFuzzyCompare(m_data.latitude(), value)) return;
    m_data.set_latitude(value);
    emit latitudeChanged();
    emit dataChanged();
}

void GinsDataModel::setLongitude(double value)
{
    if (qFuzzyCompare(m_data.longitude(), value)) return;
    m_data.set_longitude(value);
    emit longitudeChanged();
    emit dataChanged();
}

void GinsDataModel::setAltitude(double value)
{
    if (qFuzzyCompare(static_cast<double>(m_data.altitude()), value)) return;
    m_data.set_altitude(static_cast<float>(value));
    emit altitudeChanged();
    emit dataChanged();
}

void GinsDataModel::setRollDeg(double value)
{
    int32_t scaled = static_cast<int32_t>(value * 10.0);
    if (m_data.roll_deg10() == scaled) return;
    m_data.set_roll_deg10(scaled);
    emit rollDegChanged();
    emit dataChanged();
}

void GinsDataModel::setPitchDeg(double value)
{
    int32_t scaled = static_cast<int32_t>(value * 10.0);
    if (m_data.pitch_deg10() == scaled) return;
    m_data.set_pitch_deg10(scaled);
    emit pitchDegChanged();
    emit dataChanged();
}

void GinsDataModel::setHeadingDeg(double value)
{
    if (qFuzzyCompare(static_cast<double>(m_data.heading_deg()), value)) return;
    m_data.set_heading_deg(static_cast<float>(value));
    emit headingDegChanged();
    emit dataChanged();
}

void GinsDataModel::setVnMps(double value)
{
    int32_t scaled = static_cast<int32_t>(value * 100.0);
    if (m_data.vn_mps100() == scaled) return;
    m_data.set_vn_mps100(scaled);
    emit vnMpsChanged();
    emit dataChanged();
}

void GinsDataModel::setVeMps(double value)
{
    int32_t scaled = static_cast<int32_t>(value * 100.0);
    if (m_data.ve_mps100() == scaled) return;
    m_data.set_ve_mps100(scaled);
    emit veMpsChanged();
    emit dataChanged();
}

void GinsDataModel::setRoti(double value)
{
    if (qFuzzyCompare(static_cast<double>(m_data.roti()), value)) return;
    m_data.set_roti(static_cast<float>(value));
    emit rotiChanged();
    emit dataChanged();
}

void GinsDataModel::setAx(double value)
{
    if (qFuzzyCompare(static_cast<double>(m_data.ax()), value)) return;
    m_data.set_ax(static_cast<float>(value));
    emit axChanged();
    emit dataChanged();
}

void GinsDataModel::setAy(double value)
{
    if (qFuzzyCompare(static_cast<double>(m_data.ay()), value)) return;
    m_data.set_ay(static_cast<float>(value));
    emit ayChanged();
    emit dataChanged();
}

void GinsDataModel::setAaz(double value)
{
    if (qFuzzyCompare(static_cast<double>(m_data.aaz()), value)) return;
    m_data.set_aaz(static_cast<float>(value));
    emit aazChanged();
    emit dataChanged();
}

void GinsDataModel::setGinsStatus(uint value)
{
    if (m_data.gins_status() == value) return;
    m_data.set_gins_status(value);
    emit ginsStatusChanged();
    emit dataChanged();
}

void GinsDataModel::setHdop(double value)
{
    uint32_t scaled = static_cast<uint32_t>(value * 10.0);
    if (m_data.hdop10() == scaled) return;
    m_data.set_hdop10(scaled);
    emit hdopChanged();
    emit dataChanged();
}

void GinsDataModel::setHeadStd(double value)
{
    if (qFuzzyCompare(static_cast<double>(m_data.head_std()), value)) return;
    m_data.set_head_std(static_cast<float>(value));
    emit headStdChanged();
    emit dataChanged();
}

/* ========================== QVariantMap 接口 ========================== */

QVariantMap GinsDataModel::toMap() const
{
    return protoToMap(m_data);
}

void GinsDataModel::fromMap(const QVariantMap &map)
{
    mapToProto(map, m_data);
    emit portChanged();
    emit utcLocalChanged();
    emit latitudeChanged();
    emit longitudeChanged();
    emit altitudeChanged();
    emit rollDegChanged();
    emit pitchDegChanged();
    emit headingDegChanged();
    emit vnMpsChanged();
    emit veMpsChanged();
    emit rotiChanged();
    emit axChanged();
    emit ayChanged();
    emit aazChanged();
    emit ginsStatusChanged();
    emit hdopChanged();
    emit headStdChanged();
    emit dataChanged();
}

/* ========================== Proto 接口 ========================== */

void GinsDataModel::setProto(const gins::data::GINSData &data)
{
    m_data = data;
    emit portChanged();
    emit utcLocalChanged();
    emit latitudeChanged();
    emit longitudeChanged();
    emit altitudeChanged();
    emit rollDegChanged();
    emit pitchDegChanged();
    emit headingDegChanged();
    emit vnMpsChanged();
    emit veMpsChanged();
    emit rotiChanged();
    emit axChanged();
    emit ayChanged();
    emit aazChanged();
    emit ginsStatusChanged();
    emit hdopChanged();
    emit headStdChanged();
    emit dataChanged();
}

gins::data::GINSData GinsDataModel::proto() const
{
    return m_data;
}

/* ========================== 序列化 ========================== */

QByteArray GinsDataModel::serialize() const
{
    std::string buf;
    if (!m_data.SerializeToString(&buf)) {
        const_cast<GinsDataModel *>(this)->errorOccurred(
            SerializeError, QStringLiteral("序列化失败"));
        return QByteArray();
    }
    return QByteArray(buf.data(), static_cast<int>(buf.size()));
}

bool GinsDataModel::deserialize(const QByteArray &bytes)
{
    gins::data::GINSData d;
    if (!d.ParseFromArray(bytes.constData(), bytes.size())) {
        emit errorOccurred(DeserializeError,
                           QStringLiteral("反序列化失败，数据格式不正确"));
        return false;
    }
    setProto(d);
    return true;
}

/* ========================== 重置 ========================== */

void GinsDataModel::clear()
{
    m_data.Clear();
    emit portChanged();
    emit utcLocalChanged();
    emit latitudeChanged();
    emit longitudeChanged();
    emit altitudeChanged();
    emit rollDegChanged();
    emit pitchDegChanged();
    emit headingDegChanged();
    emit vnMpsChanged();
    emit veMpsChanged();
    emit rotiChanged();
    emit axChanged();
    emit ayChanged();
    emit aazChanged();
    emit ginsStatusChanged();
    emit hdopChanged();
    emit headStdChanged();
    emit dataChanged();
}

/* ========================== 调试 ========================== */

void GinsDataModel::printData() const
{
    qInfo().noquote()
        << "GinsDataModel:"
        << " port=" << m_data.port()
        << " lat=" << QString::number(m_data.latitude(), 'f', 6)
        << " lon=" << QString::number(m_data.longitude(), 'f', 6)
        << " alt=" << QString::number(m_data.altitude(), 'f', 2)
        << " heading=" << QString::number(m_data.heading_deg(), 'f', 2)
        << " roll=" << m_data.roll_deg10() / 10.0
        << " pitch=" << m_data.pitch_deg10() / 10.0
        << " vn=" << m_data.vn_mps100() / 100.0
        << " ve=" << m_data.ve_mps100() / 100.0
        << " roti=" << m_data.roti()
        << " ax=" << m_data.ax()
        << " ay=" << m_data.ay()
        << " aaz=" << m_data.aaz()
        << " status=" << m_data.gins_status()
        << " hdop=" << m_data.hdop10() / 10.0
        << " headStd=" << m_data.head_std();
}

/* ========================== 内部工具 ========================== */

QVariantMap GinsDataModel::protoToMap(const gins::data::GINSData &d)
{
    return {
        { "port",       d.port()                              },
        { "utcLocal",   d.utc_local()                         },
        { "latitude",   d.latitude()                          },
        { "longitude",  d.longitude()                         },
        { "altitude",   static_cast<double>(d.altitude())     },
        { "rollDeg",    d.roll_deg10() / 10.0                 },
        { "pitchDeg",   d.pitch_deg10() / 10.0                },
        { "headingDeg", static_cast<double>(d.heading_deg())  },
        { "vnMps",      d.vn_mps100() / 100.0                 },
        { "veMps",      d.ve_mps100() / 100.0                 },
        { "roti",       static_cast<double>(d.roti())         },
        { "ax",         static_cast<double>(d.ax())           },
        { "ay",         static_cast<double>(d.ay())           },
        { "aaz",        static_cast<double>(d.aaz())          },
        { "ginsStatus", d.gins_status()                       },
        { "hdop",       d.hdop10() / 10.0                     },
        { "headStd",    static_cast<double>(d.head_std())     },
    };
}

void GinsDataModel::mapToProto(const QVariantMap &map, gins::data::GINSData &d)
{
    if (map.contains("port"))
        d.set_port(map["port"].toUInt());
    if (map.contains("utcLocal"))
        d.set_utc_local(map["utcLocal"].toDouble());
    if (map.contains("latitude"))
        d.set_latitude(map["latitude"].toDouble());
    if (map.contains("longitude"))
        d.set_longitude(map["longitude"].toDouble());
    if (map.contains("altitude"))
        d.set_altitude(map["altitude"].toFloat());
    if (map.contains("rollDeg"))
        d.set_roll_deg10(static_cast<int32_t>(map["rollDeg"].toDouble() * 10.0));
    if (map.contains("pitchDeg"))
        d.set_pitch_deg10(static_cast<int32_t>(map["pitchDeg"].toDouble() * 10.0));
    if (map.contains("headingDeg"))
        d.set_heading_deg(map["headingDeg"].toFloat());
    if (map.contains("vnMps"))
        d.set_vn_mps100(static_cast<int32_t>(map["vnMps"].toDouble() * 100.0));
    if (map.contains("veMps"))
        d.set_ve_mps100(static_cast<int32_t>(map["veMps"].toDouble() * 100.0));
    if (map.contains("roti"))
        d.set_roti(map["roti"].toFloat());
    if (map.contains("ax"))
        d.set_ax(map["ax"].toFloat());
    if (map.contains("ay"))
        d.set_ay(map["ay"].toFloat());
    if (map.contains("aaz"))
        d.set_aaz(map["aaz"].toFloat());
    if (map.contains("ginsStatus"))
        d.set_gins_status(map["ginsStatus"].toUInt());
    if (map.contains("hdop"))
        d.set_hdop10(static_cast<uint32_t>(map["hdop"].toDouble() * 10.0));
    if (map.contains("headStd"))
        d.set_head_std(map["headStd"].toFloat());
}
