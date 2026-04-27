#include "ginsdatalistmodel.h"
#include <QDebug>

// 构造函数保持轻量，模型内容由 append/deserialize 等接口驱动。
GinsDataListModel::GinsDataListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

/* ---------------------------------- 增 ---------------------------------- */

void GinsDataListModel::insert(int index, const QVariantMap &map)
{
    if (index < 0 || index > m_list.count()) {
        emit errorOccurred(InvalidIndex,
                           QString("插入数据时索引无效: %1").arg(index));
        return;
    }

    gins::data::OutMsg d;
    mapToProto(map, d);

    beginInsertRows(QModelIndex(), index, index);
    m_list.insert(index, d);
    endInsertRows();

    emit countChanged();
    emit dataUpdated();
    emit itemAdded(index);
}

void GinsDataListModel::append(const QVariantMap &map)
{
    insert(m_list.count(), map);
}

void GinsDataListModel::appendProto(const gins::data::OutMsg &data)
{
    int index = m_list.count();
    beginInsertRows(QModelIndex(), index, index);
    m_list.append(data);
    endInsertRows();

    emit countChanged();
    emit dataUpdated();
    emit itemAdded(index);
}

/* ---------------------------------- 删 ---------------------------------- */

void GinsDataListModel::remove(int index)
{
    if (index < 0 || index >= m_list.count()) {
        emit errorOccurred(InvalidIndex,
                           QString("删除数据时索引无效: %1").arg(index));
        return;
    }

    beginRemoveRows(QModelIndex(), index, index);
    m_list.removeAt(index);
    endRemoveRows();

    emit countChanged();
    emit dataUpdated();
    emit itemRemoved(index);
}

void GinsDataListModel::clear()
{
    if (m_list.isEmpty())
        return;

    beginResetModel();
    m_list.clear();
    endResetModel();

    emit countChanged();
    emit dataUpdated();
}

/* ---------------------------------- 查 ---------------------------------- */

int GinsDataListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

int GinsDataListModel::count() const
{
    return m_list.count();
}

QVariant GinsDataListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_list.count())
        return QVariant();

    const gins::data::OutMsg &d = m_list.at(index.row());

    // 读取时统一做“展示层单位转换”，避免 UI 到处手动除以 10/100。
    switch (role) {
    case Qt::DisplayRole:
        return QStringLiteral("[%1] lat=%2 lon=%3 hdg=%4")
            .arg(index.row())
            .arg(d.latitude1e7() / 1e7, 0, 'f', 6)
            .arg(d.longitude1e7() / 1e7, 0, 'f', 6)
            .arg(d.heading100() / 100.0, 0, 'f', 1);
    case PortRole:       return static_cast<uint>(d.udp_port());
    case UtcLocalRole:   return d.utc_local100() / 100.0;
    case LatitudeRole:   return d.latitude1e7() / 1e7;
    case LongitudeRole:  return d.longitude1e7() / 1e7;
    case AltitudeRole:   return d.altitude1e3() / 1000.0;
    case RollDegRole:    return d.roll100() / 100.0;
    case PitchDegRole:   return d.pitch100() / 100.0;
    case HeadingDegRole: return d.heading100() / 100.0;
    case CourseDegRole:  return d.course100() / 100.0;
    case VnMpsRole:      return d.vn_mps100() / 100.0;
    case VeMpsRole:      return d.ve_mps100() / 100.0;
    case RotiRole:       return d.roti1e5() / 1e5;
    case AxRole:         return d.ax1e5() / 1e5;
    case AyRole:         return d.ay1e5() / 1e5;
    case AazRole:        return d.aaz1e5() / 1e5;
    case GinsStatusRole: return d.gins_status();
    case HdopRole:       return d.hdop10() / 10.0;
    case HeadStdRole:    return d.head_std1e5() / 1e5;
    default:             return QVariant();
    }
}

QVariantMap GinsDataListModel::get(int index) const
{
    if (index < 0 || index >= m_list.count()) {
        const_cast<GinsDataListModel *>(this)->errorOccurred(
            InvalidIndex,
            QString("获取数据时索引无效: %1").arg(index));
        return QVariantMap();
    }
    return protoToMap(m_list.at(index));
}

bool GinsDataListModel::protoData(int index, gins::data::OutMsg &out) const
{
    if (index < 0 || index >= m_list.count())
        return false;
    out = m_list.at(index);
    return true;
}

void GinsDataListModel::printAllData() const
{
    if (m_list.isEmpty())
        return;

    QDebug info = qInfo();
    info.noquote() << "GinsDataListModel (" << m_list.count() << " items):";
    for (int i = 0; i < m_list.count(); ++i) {
        const gins::data::OutMsg &d = m_list.at(i);
        info.noquote()
            << "\n  [" << i << "]"
            << " port=" << static_cast<uint>(d.udp_port())
            << " lat=" << QString::number(d.latitude1e7() / 1e7, 'f', 6)
            << " lon=" << QString::number(d.longitude1e7() / 1e7, 'f', 6)
            << " alt=" << QString::number(d.altitude1e3() / 1000.0, 'f', 2)
            << " heading=" << QString::number(d.heading100() / 100.0, 'f', 2)
            << " status=" << d.gins_status();
    }
}

/* ---------------------------------- 改 ---------------------------------- */

void GinsDataListModel::set(int index, const QVariantMap &map)
{
    if (index < 0 || index >= m_list.count()) {
        emit errorOccurred(InvalidIndex,
                           QString("修改数据时索引无效: %1").arg(index));
        return;
    }

    gins::data::OutMsg &d = m_list[index];
    mapToProto(map, d);

    // 仅刷新一行，避免整表重绘。
    QModelIndex mi = this->index(index);
    emit dataChanged(mi, mi);
    emit dataUpdated();
    emit itemChanged(index);
}

void GinsDataListModel::updateProto(int index, const gins::data::OutMsg &data)
{
    if (index < 0 || index >= m_list.count()) {
        emit errorOccurred(InvalidIndex,
                           QString("更新 proto 数据时索引无效: %1").arg(index));
        return;
    }

    m_list[index] = data;

    // 外部直接传 protobuf 时，仍保持一致的视图通知流程。
    QModelIndex mi = this->index(index);
    emit dataChanged(mi, mi);
    emit dataUpdated();
    emit itemChanged(index);
}

/* ---------------------------------- 序列化 ---------------------------------- */

QByteArray GinsDataListModel::serializeItem(int index) const
{
    if (index < 0 || index >= m_list.count()) {
        const_cast<GinsDataListModel *>(this)->errorOccurred(
            InvalidIndex,
            QString("序列化时索引无效: %1").arg(index));
        return QByteArray();
    }

    // protobuf 二进制序列化，体积小且跨语言一致。
    std::string buf;
    if (!m_list.at(index).SerializeToString(&buf)) {
        const_cast<GinsDataListModel *>(this)->errorOccurred(
            SerializeError, QStringLiteral("序列化失败"));
        return QByteArray();
    }
    return QByteArray(buf.data(), static_cast<int>(buf.size()));
}

bool GinsDataListModel::deserializeAndAppend(const QByteArray &bytes)
{
    gins::data::OutMsg d;
    if (!d.ParseFromArray(bytes.constData(), bytes.size())) {
        emit errorOccurred(DeserializeError,
                           QStringLiteral("反序列化失败，数据格式不正确"));
        return false;
    }
    appendProto(d);
    return true;
}

bool GinsDataListModel::deserializeAndUpdate(int index, const QByteArray &bytes)
{
    if (index < 0 || index >= m_list.count()) {
        emit errorOccurred(InvalidIndex,
                           QString("反序列化更新时索引无效: %1").arg(index));
        return false;
    }

    gins::data::OutMsg d;
    if (!d.ParseFromArray(bytes.constData(), bytes.size())) {
        emit errorOccurred(DeserializeError,
                           QStringLiteral("反序列化失败，数据格式不正确"));
        return false;
    }
    updateProto(index, d);
    return true;
}

/* ---------------------------------- 内部工具 ---------------------------------- */

QHash<int, QByteArray> GinsDataListModel::roleNames() const
{
    return {
        { PortRole,       "port"       },
        { UtcLocalRole,   "utcLocal"   },
        { LatitudeRole,   "latitude"   },
        { LongitudeRole,  "longitude"  },
        { AltitudeRole,   "altitude"   },
        { RollDegRole,    "rollDeg"    },
        { PitchDegRole,   "pitchDeg"   },
        { HeadingDegRole, "headingDeg" },
        { CourseDegRole,  "courseDeg"  },
        { VnMpsRole,      "vnMps"      },
        { VeMpsRole,      "veMps"      },
        { RotiRole,       "roti"       },
        { AxRole,         "ax"         },
        { AyRole,         "ay"         },
        { AazRole,        "aaz"        },
        { GinsStatusRole, "ginsStatus" },
        { HdopRole,       "hdop"       },
        { HeadStdRole,    "headStd"    },
    };
}

QVariantMap GinsDataListModel::protoToMap(const gins::data::OutMsg &d)
{
    // proto -> map：恢复为“人可读单位”，方便 UI 直接显示。
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

void GinsDataListModel::mapToProto(const QVariantMap &map, gins::data::OutMsg &d)
{
    // map -> proto：将浮点展示值按协议要求转换回缩放整数。
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
