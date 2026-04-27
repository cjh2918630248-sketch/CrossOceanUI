#include "ginsdatamodel.h"
#include <QDebug>

GinsDataModel::GinsDataModel(QObject *parent)
    : QObject(parent)
{
}

/* ========================== 属性读取 ========================== */

uint   GinsDataModel::port()       const { return static_cast<uint>(m_data.udp_port()); }
double GinsDataModel::utcLocal()   const { return m_data.utc_local100() / 100.0; }
double GinsDataModel::latitude()   const { return m_data.latitude1e7() / 1e7; }
double GinsDataModel::longitude()  const { return m_data.longitude1e7() / 1e7; }
double GinsDataModel::altitude()   const { return m_data.altitude1e3() / 1000.0; }
double GinsDataModel::rollDeg()    const { return m_data.roll100() / 100.0; }
double GinsDataModel::pitchDeg()   const { return m_data.pitch100() / 100.0; }
double GinsDataModel::headingDeg() const { return m_data.heading100() / 100.0; }
double GinsDataModel::courseDeg()  const { return m_data.course100() / 100.0; }
double GinsDataModel::vnMps()      const { return m_data.vn_mps100() / 100.0; }
double GinsDataModel::veMps()      const { return m_data.ve_mps100() / 100.0; }
double GinsDataModel::roti()       const { return m_data.roti1e5() / 1e5; }
double GinsDataModel::ax()         const { return m_data.ax1e5() / 1e5; }
double GinsDataModel::ay()         const { return m_data.ay1e5() / 1e5; }
double GinsDataModel::aaz()        const { return m_data.aaz1e5() / 1e5; }
uint   GinsDataModel::ginsStatus() const { return m_data.gins_status(); }
double GinsDataModel::hdop()       const { return m_data.hdop10() / 10.0; }
double GinsDataModel::headStd()    const { return m_data.head_std1e5() / 1e5; }

/* ========================== 属性写入 ========================== */

void GinsDataModel::setPort(uint value)
{
    const auto ev = static_cast<::gins::port::Gins>(value);
    if (m_data.udp_port() == ev) return;
    m_data.set_udp_port(ev);
    emit portChanged();
    emit dataChanged();
}

void GinsDataModel::setUtcLocal(double value)
{
    const int32_t scaled = static_cast<int32_t>(qRound64(value * 100.0));
    if (m_data.utc_local100() == scaled) return;
    m_data.set_utc_local100(scaled);
    emit utcLocalChanged();
    emit dataChanged();
}

void GinsDataModel::setLatitude(double value)
{
    const int32_t scaled = static_cast<int32_t>(qRound64(value * 1e7));
    if (m_data.latitude1e7() == scaled) return;
    m_data.set_latitude1e7(scaled);
    emit latitudeChanged();
    emit dataChanged();
}

void GinsDataModel::setLongitude(double value)
{
    const int32_t scaled = static_cast<int32_t>(qRound64(value * 1e7));
    if (m_data.longitude1e7() == scaled) return;
    m_data.set_longitude1e7(scaled);
    emit longitudeChanged();
    emit dataChanged();
}

void GinsDataModel::setAltitude(double value)
{
    const int32_t scaled = static_cast<int32_t>(qRound64(value * 1000.0));
    if (m_data.altitude1e3() == scaled) return;
    m_data.set_altitude1e3(scaled);
    emit altitudeChanged();
    emit dataChanged();
}

void GinsDataModel::setRollDeg(double value)
{
    const int32_t scaled = static_cast<int32_t>(qRound64(value * 100.0));
    if (m_data.roll100() == scaled) return;
    m_data.set_roll100(scaled);
    emit rollDegChanged();
    emit dataChanged();
}

void GinsDataModel::setPitchDeg(double value)
{
    const int32_t scaled = static_cast<int32_t>(qRound64(value * 100.0));
    if (m_data.pitch100() == scaled) return;
    m_data.set_pitch100(scaled);
    emit pitchDegChanged();
    emit dataChanged();
}

void GinsDataModel::setHeadingDeg(double value)
{
    const uint32_t scaled = static_cast<uint32_t>(qRound64(value * 100.0));
    if (m_data.heading100() == scaled) return;
    m_data.set_heading100(scaled);
    emit headingDegChanged();
    emit dataChanged();
}

void GinsDataModel::setCourseDeg(double value)
{
    const uint32_t scaled = static_cast<uint32_t>(qRound64(value * 100.0));
    if (m_data.course100() == scaled) return;
    m_data.set_course100(scaled);
    emit courseDegChanged();
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
    const int32_t scaled = static_cast<int32_t>(qRound64(value * 1e5));
    if (m_data.roti1e5() == scaled) return;
    m_data.set_roti1e5(scaled);
    emit rotiChanged();
    emit dataChanged();
}

void GinsDataModel::setAx(double value)
{
    const int32_t scaled = static_cast<int32_t>(qRound64(value * 1e5));
    if (m_data.ax1e5() == scaled) return;
    m_data.set_ax1e5(scaled);
    emit axChanged();
    emit dataChanged();
}

void GinsDataModel::setAy(double value)
{
    const int32_t scaled = static_cast<int32_t>(qRound64(value * 1e5));
    if (m_data.ay1e5() == scaled) return;
    m_data.set_ay1e5(scaled);
    emit ayChanged();
    emit dataChanged();
}

void GinsDataModel::setAaz(double value)
{
    const int32_t scaled = static_cast<int32_t>(qRound64(value * 1e5));
    if (m_data.aaz1e5() == scaled) return;
    m_data.set_aaz1e5(scaled);
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
    const int32_t scaled = static_cast<int32_t>(qRound64(value * 1e5));
    if (m_data.head_std1e5() == scaled) return;
    m_data.set_head_std1e5(scaled);
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
    emit courseDegChanged();
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

void GinsDataModel::setProto(const gins::data::OutMsg &data)
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
    emit courseDegChanged();
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

gins::data::OutMsg GinsDataModel::proto() const
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
    gins::data::OutMsg d;
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
    emit courseDegChanged();
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
        << " port=" << static_cast<uint>(m_data.udp_port())
        << " lat=" << QString::number(m_data.latitude1e7() / 1e7, 'f', 6)
        << " lon=" << QString::number(m_data.longitude1e7() / 1e7, 'f', 6)
        << " alt=" << QString::number(m_data.altitude1e3() / 1000.0, 'f', 2)
        << " heading=" << QString::number(m_data.heading100() / 100.0, 'f', 2)
        << " course=" << QString::number(m_data.course100() / 100.0, 'f', 2)
        << " roll=" << m_data.roll100() / 100.0
        << " pitch=" << m_data.pitch100() / 100.0
        << " vn=" << m_data.vn_mps100() / 100.0
        << " ve=" << m_data.ve_mps100() / 100.0
        << " roti=" << m_data.roti1e5() / 1e5
        << " ax=" << m_data.ax1e5() / 1e5
        << " ay=" << m_data.ay1e5() / 1e5
        << " aaz=" << m_data.aaz1e5() / 1e5
        << " status=" << m_data.gins_status()
        << " hdop=" << m_data.hdop10() / 10.0
        << " headStd=" << m_data.head_std1e5() / 1e5;
}

/* ========================== 内部工具 ========================== */

QVariantMap GinsDataModel::protoToMap(const gins::data::OutMsg &d)
{
    return {
        { "timestampMs", static_cast<qlonglong>(d.timestamp_ms()) },
        { "port",        static_cast<uint>(d.udp_port())           },
        { "utcLocal",    d.utc_local100() / 100.0                   },
        { "latitude",    d.latitude1e7() / 1e7                      },
        { "longitude",   d.longitude1e7() / 1e7                     },
        { "altitude",    d.altitude1e3() / 1000.0                  },
        { "rollDeg",     d.roll100() / 100.0                      },
        { "pitchDeg",    d.pitch100() / 100.0                     },
        { "headingDeg",  d.heading100() / 100.0                   },
        { "courseDeg",   d.course100() / 100.0                    },
        { "vnMps",       d.vn_mps100() / 100.0                    },
        { "veMps",       d.ve_mps100() / 100.0                    },
        { "roti",        d.roti1e5() / 1e5                        },
        { "ax",          d.ax1e5() / 1e5                          },
        { "ay",          d.ay1e5() / 1e5                          },
        { "aaz",         d.aaz1e5() / 1e5                         },
        { "ginsStatus",  d.gins_status()                          },
        { "hdop",        d.hdop10() / 10.0                        },
        { "headStd",     d.head_std1e5() / 1e5                    },
    };
}

void GinsDataModel::mapToProto(const QVariantMap &map, gins::data::OutMsg &d)
{
    if (map.contains("timestampMs"))
        d.set_timestamp_ms(map["timestampMs"].toLongLong());
    if (map.contains("port"))
        d.set_udp_port(static_cast<::gins::port::Gins>(map["port"].toUInt()));
    if (map.contains("utcLocal"))
        d.set_utc_local100(static_cast<int32_t>(qRound64(map["utcLocal"].toDouble() * 100.0)));
    if (map.contains("latitude"))
        d.set_latitude1e7(static_cast<int32_t>(qRound64(map["latitude"].toDouble() * 1e7)));
    if (map.contains("longitude"))
        d.set_longitude1e7(static_cast<int32_t>(qRound64(map["longitude"].toDouble() * 1e7)));
    if (map.contains("altitude"))
        d.set_altitude1e3(static_cast<int32_t>(qRound64(map["altitude"].toDouble() * 1000.0)));
    if (map.contains("rollDeg"))
        d.set_roll100(static_cast<int32_t>(qRound64(map["rollDeg"].toDouble() * 100.0)));
    if (map.contains("pitchDeg"))
        d.set_pitch100(static_cast<int32_t>(qRound64(map["pitchDeg"].toDouble() * 100.0)));
    if (map.contains("headingDeg"))
        d.set_heading100(static_cast<uint32_t>(qRound64(map["headingDeg"].toDouble() * 100.0)));
    if (map.contains("courseDeg"))
        d.set_course100(static_cast<uint32_t>(qRound64(map["courseDeg"].toDouble() * 100.0)));
    if (map.contains("vnMps"))
        d.set_vn_mps100(static_cast<int32_t>(map["vnMps"].toDouble() * 100.0));
    if (map.contains("veMps"))
        d.set_ve_mps100(static_cast<int32_t>(map["veMps"].toDouble() * 100.0));
    if (map.contains("roti"))
        d.set_roti1e5(static_cast<int32_t>(qRound64(map["roti"].toDouble() * 1e5)));
    if (map.contains("ax"))
        d.set_ax1e5(static_cast<int32_t>(qRound64(map["ax"].toDouble() * 1e5)));
    if (map.contains("ay"))
        d.set_ay1e5(static_cast<int32_t>(qRound64(map["ay"].toDouble() * 1e5)));
    if (map.contains("aaz"))
        d.set_aaz1e5(static_cast<int32_t>(qRound64(map["aaz"].toDouble() * 1e5)));
    if (map.contains("ginsStatus"))
        d.set_gins_status(map["ginsStatus"].toUInt());
    if (map.contains("hdop"))
        d.set_hdop10(static_cast<uint32_t>(map["hdop"].toDouble() * 10.0));
    if (map.contains("headStd"))
        d.set_head_std1e5(static_cast<int32_t>(qRound64(map["headStd"].toDouble() * 1e5)));
}
