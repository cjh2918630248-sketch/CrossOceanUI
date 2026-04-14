#include "thrusterdatamodel.h"
#include <QDebug>

ThrusterDataModel::ThrusterDataModel(QObject *parent)
    : QObject(parent)
{
}

/* ---------------------------------- 字段读取 ---------------------------------- */

uint    ThrusterDataModel::status()       const { return m_data.status();        }
uint    ThrusterDataModel::errorCode()    const { return m_data.error_code();    }
int     ThrusterDataModel::rpm()          const { return m_data.rpm();           }
int     ThrusterDataModel::angle()        const { return m_data.angle();         }
int     ThrusterDataModel::power()        const { return m_data.power();         }
uint    ThrusterDataModel::busVoltage()   const { return m_data.bus_voltage();   }
int     ThrusterDataModel::busCurrent()   const { return m_data.bus_current();   }
int     ThrusterDataModel::phaseCurrent() const { return m_data.phase_current(); }
int     ThrusterDataModel::motorTemp()    const { return m_data.motor_temp();    }
int     ThrusterDataModel::mosTemp()      const { return m_data.mos_temp();      }
int     ThrusterDataModel::temp()         const { return m_data._temp();         }
uint    ThrusterDataModel::runTime()      const { return m_data.run_time();      }
uint    ThrusterDataModel::sumRunTime()   const { return m_data.sum_run_time();  }
QString ThrusterDataModel::statusText()   const { return statusToString(m_data.status()); }

/* ---------------------------------- 字段写入 ---------------------------------- */

void ThrusterDataModel::setStatus(uint val)
{
    if (m_data.status() == val) return;
    m_data.set_status(val);
    emit statusChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setErrorCode(uint val)
{
    if (m_data.error_code() == val) return;
    m_data.set_error_code(val);
    emit errorCodeChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setRpm(int val)
{
    if (m_data.rpm() == val) return;
    m_data.set_rpm(val);
    emit rpmChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setAngle(int val)
{
    if (m_data.angle() == val) return;
    m_data.set_angle(val);
    emit angleChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setPower(int val)
{
    if (m_data.power() == val) return;
    m_data.set_power(val);
    emit powerChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setBusVoltage(uint val)
{
    if (m_data.bus_voltage() == val) return;
    m_data.set_bus_voltage(val);
    emit busVoltageChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setBusCurrent(int val)
{
    if (m_data.bus_current() == val) return;
    m_data.set_bus_current(val);
    emit busCurrentChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setPhaseCurrent(int val)
{
    if (m_data.phase_current() == val) return;
    m_data.set_phase_current(val);
    emit phaseCurrentChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setMotorTemp(int val)
{
    if (m_data.motor_temp() == val) return;
    m_data.set_motor_temp(val);
    emit motorTempChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setMosTemp(int val)
{
    if (m_data.mos_temp() == val) return;
    m_data.set_mos_temp(val);
    emit mosTempChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setTemp(int val)
{
    if (m_data._temp() == val) return;
    m_data.set__temp(val);
    emit tempChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setRunTime(uint val)
{
    if (m_data.run_time() == val) return;
    m_data.set_run_time(val);
    emit runTimeChanged();
    emit dataUpdated();
}

void ThrusterDataModel::setSumRunTime(uint val)
{
    if (m_data.sum_run_time() == val) return;
    m_data.set_sum_run_time(val);
    emit sumRunTimeChanged();
    emit dataUpdated();
}

/* ---------------------------------- QVariantMap 互转 ---------------------------------- */

QVariantMap ThrusterDataModel::toMap() const
{
    return {
        { "status",        m_data.status()        },
        { "errorCode",     m_data.error_code()    },
        { "rpm",           m_data.rpm()           },
        { "angle",         m_data.angle()         },
        { "power",         m_data.power()         },
        { "busVoltage",    m_data.bus_voltage()   },
        { "busCurrent",    m_data.bus_current()   },
        { "phaseCurrent",  m_data.phase_current() },
        { "motorTemp",     m_data.motor_temp()    },
        { "mosTemp",       m_data.mos_temp()      },
        { "temp",          m_data._temp()         },
        { "runTime",       m_data.run_time()      },
        { "sumRunTime",    m_data.sum_run_time()  },
        { "statusText",    statusToString(m_data.status()) },
    };
}

void ThrusterDataModel::fromMap(const QVariantMap &map)
{
    if (map.contains("status"))       setStatus(map["status"].toUInt());
    if (map.contains("errorCode"))    setErrorCode(map["errorCode"].toUInt());
    if (map.contains("rpm"))          setRpm(map["rpm"].toInt());
    if (map.contains("angle"))        setAngle(map["angle"].toInt());
    if (map.contains("power"))        setPower(map["power"].toInt());
    if (map.contains("busVoltage"))   setBusVoltage(map["busVoltage"].toUInt());
    if (map.contains("busCurrent"))   setBusCurrent(map["busCurrent"].toInt());
    if (map.contains("phaseCurrent")) setPhaseCurrent(map["phaseCurrent"].toInt());
    if (map.contains("motorTemp"))    setMotorTemp(map["motorTemp"].toInt());
    if (map.contains("mosTemp"))      setMosTemp(map["mosTemp"].toInt());
    if (map.contains("temp"))         setTemp(map["temp"].toInt());
    if (map.contains("runTime"))      setRunTime(map["runTime"].toUInt());
    if (map.contains("sumRunTime"))   setSumRunTime(map["sumRunTime"].toUInt());
}

/* ---------------------------------- Proto 直接操作 ---------------------------------- */

void ThrusterDataModel::loadFromProto(const thruster::Data &data)
{
    m_data = data;

    emit statusChanged();
    emit errorCodeChanged();
    emit rpmChanged();
    emit angleChanged();
    emit powerChanged();
    emit busVoltageChanged();
    emit busCurrentChanged();
    emit phaseCurrentChanged();
    emit motorTempChanged();
    emit mosTempChanged();
    emit tempChanged();
    emit runTimeChanged();
    emit sumRunTimeChanged();
    emit dataUpdated();
}

void ThrusterDataModel::exportToProto(thruster::Data &out) const
{
    out = m_data;
}

/* ---------------------------------- 序列化 ---------------------------------- */

QByteArray ThrusterDataModel::serialize() const
{
    std::string buf;
    if (!m_data.SerializeToString(&buf)) {
        const_cast<ThrusterDataModel *>(this)->errorOccurred(
            SerializeError, QStringLiteral("序列化失败"));
        return QByteArray();
    }
    return QByteArray(buf.data(), static_cast<int>(buf.size()));
}

bool ThrusterDataModel::deserialize(const QByteArray &bytes)
{
    thruster::Data d;
    if (!d.ParseFromArray(bytes.constData(), bytes.size())) {
        emit errorOccurred(DeserializeError,
                           QStringLiteral("反序列化失败，数据格式不正确"));
        return false;
    }
    loadFromProto(d);
    return true;
}

/* ---------------------------------- 元信息 ---------------------------------- */

uint ThrusterDataModel::port() const        { return m_port;        }
uint ThrusterDataModel::thrusterNum() const  { return m_thrusterNum; }

void ThrusterDataModel::setPort(uint port)
{
    if (m_port == port) return;
    m_port = static_cast<uint32_t>(port);
    emit portChanged();
}

void ThrusterDataModel::setThrusterNum(uint num)
{
    if (m_thrusterNum == num) return;
    m_thrusterNum = static_cast<uint32_t>(num);
    emit thrusterNumChanged();
}

/* ---------------------------------- 工具 ---------------------------------- */

QString ThrusterDataModel::statusToString(uint32_t status)
{
    if (status == thruster::NORMAL)
        return QStringLiteral("Normal");

    QStringList flags;
    if (status & thruster::STALL)            flags << QStringLiteral("Stall");
    if (status & thruster::MOTOR_OVERHEAT)   flags << QStringLiteral("MotorOverheat");
    if (status & thruster::MOS_OVERHEAT)     flags << QStringLiteral("MosOverheat");
    if (status & thruster::OVER_CURRENT)     flags << QStringLiteral("OverCurrent");
    if (status & thruster::COMM_FAULT)       flags << QStringLiteral("CommFault");
    if (status & thruster::MOTOR_TEMP_ERROR) flags << QStringLiteral("MotorTempErr");
    if (status & thruster::MOS_TEMP_ALARM)   flags << QStringLiteral("MosTempAlarm");
    if (status & thruster::OVER_VOLTAGE)     flags << QStringLiteral("OverVoltage");
    if (status & thruster::UNDER_VOLTAGE)    flags << QStringLiteral("UnderVoltage");
    if (status & thruster::CIRCUIT_FAULT)    flags << QStringLiteral("CircuitFault");
    if (status & thruster::CHARGING)         flags << QStringLiteral("Charging");
    if (status & thruster::FAN_FAULT)        flags << QStringLiteral("FanFault");
    if (status & thruster::OTHER)            flags << QStringLiteral("Other");

    return flags.isEmpty()
               ? QStringLiteral("Unknown(0x%1)").arg(status, 0, 16)
               : flags.join(QStringLiteral(", "));
}

void ThrusterDataModel::printData() const
{
    qInfo().noquote()
        << QStringLiteral("ThrusterDataModel (port=%1, num=%2):"
                          "\n  rpm=%3 angle=%4 power=%5W"
                          "\n  busV=%6 busCur=%7 phaseCur=%8"
                          "\n  motorTemp=%9°C mosTemp=%10°C"
                          "\n  status=%11 errorCode=%12"
                          "\n  runTime=%13 sumRunTime=%14")
               .arg(m_port)
               .arg(m_thrusterNum)
               .arg(m_data.rpm())
               .arg(m_data.angle())
               .arg(m_data.power())
               .arg(m_data.bus_voltage())
               .arg(m_data.bus_current())
               .arg(m_data.phase_current())
               .arg(m_data.motor_temp())
               .arg(m_data.mos_temp())
               .arg(statusToString(m_data.status()))
               .arg(m_data.error_code())
               .arg(m_data.run_time())
               .arg(m_data.sum_run_time());
}
