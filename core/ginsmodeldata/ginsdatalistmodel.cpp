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

    gins::data::GINSData d;
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

void GinsDataListModel::appendProto(const gins::data::GINSData &data)
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

    const gins::data::GINSData &d = m_list.at(index.row());

    // 读取时统一做“展示层单位转换”，避免 UI 到处手动除以 10/100。
    switch (role) {
    case Qt::DisplayRole:
        return QStringLiteral("[%1] lat=%2 lon=%3 hdg=%4")
            .arg(index.row())
            .arg(d.latitude(), 0, 'f', 6)
            .arg(d.longitude(), 0, 'f', 6)
            .arg(d.heading_deg(), 0, 'f', 1);
    case PortRole:       return d.port();
    case UtcLocalRole:   return d.utc_local();
    case LatitudeRole:   return d.latitude();
    case LongitudeRole:  return d.longitude();
    case AltitudeRole:   return static_cast<double>(d.altitude());
    case RollDegRole:    return d.roll_deg10() / 10.0;
    case PitchDegRole:   return d.pitch_deg10() / 10.0;
    case HeadingDegRole: return static_cast<double>(d.heading_deg());
    case VnMpsRole:      return d.vn_mps100() / 100.0;
    case VeMpsRole:      return d.ve_mps100() / 100.0;
    case RotiRole:       return static_cast<double>(d.roti());
    case AxRole:         return static_cast<double>(d.ax());
    case AyRole:         return static_cast<double>(d.ay());
    case AazRole:        return static_cast<double>(d.aaz());
    case GinsStatusRole: return d.gins_status();
    case HdopRole:       return d.hdop10() / 10.0;
    case HeadStdRole:    return static_cast<double>(d.head_std());
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

bool GinsDataListModel::protoData(int index, gins::data::GINSData &out) const
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
        const gins::data::GINSData &d = m_list.at(i);
        info.noquote()
            << "\n  [" << i << "]"
            << " port=" << d.port()
            << " lat=" << QString::number(d.latitude(), 'f', 6)
            << " lon=" << QString::number(d.longitude(), 'f', 6)
            << " alt=" << QString::number(d.altitude(), 'f', 2)
            << " heading=" << QString::number(d.heading_deg(), 'f', 2)
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

    gins::data::GINSData &d = m_list[index];
    mapToProto(map, d);

    // 仅刷新一行，避免整表重绘。
    QModelIndex mi = this->index(index);
    emit dataChanged(mi, mi);
    emit dataUpdated();
    emit itemChanged(index);
}

void GinsDataListModel::updateProto(int index, const gins::data::GINSData &data)
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
    gins::data::GINSData d;
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

    gins::data::GINSData d;
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

QVariantMap GinsDataListModel::protoToMap(const gins::data::GINSData &d)
{
    // proto -> map：恢复为“人可读单位”，方便 UI 直接显示。
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

void GinsDataListModel::mapToProto(const QVariantMap &map, gins::data::GINSData &d)
{
    // map -> proto：将浮点展示值按协议要求转换回缩放整数。
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
