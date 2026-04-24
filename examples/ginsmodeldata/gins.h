#ifndef GINS_H
#define GINS_H

#include "data.pb.h"
#include <string>
#include <stdexcept>

// GinsModel 封装 gins::data::OutMsg，提供构造、赋值、序列化与反序列化的示例接口。
class GinsModel
{
public:
    GinsModel() = default;

    // 从各传感器字段构造一条 OutMsg（展示层单位与 GinsDataModel 一致）。
    // roll_deg10 / pitch_deg10 单位为 0.1°；vn_mps100 / ve_mps100 单位为 0.01 m/s；hdop10 单位为 0.1。
    GinsModel(uint32_t port,
              double   utc_local,
              double   latitude,
              double   longitude,
              float    altitude,
              int32_t  roll_deg10,
              int32_t  pitch_deg10,
              float    heading_deg,
              int32_t  vn_mps100,
              int32_t  ve_mps100,
              float    roti,
              float    ax,
              float    ay,
              float    aaz,
              uint32_t gins_status,
              uint32_t hdop10,
              float    head_std)
    {
        m_data.set_udp_port(static_cast<::gins::port::Gins>(port));
        m_data.set_utc_local100(static_cast<int32_t>(utc_local * 100.0));
        m_data.set_latitude1e7(static_cast<int32_t>(latitude * 1e7));
        m_data.set_longitude1e7(static_cast<int32_t>(longitude * 1e7));
        m_data.set_altitude100(static_cast<int32_t>(altitude * 1000.0f));
        m_data.set_roll10(roll_deg10);
        m_data.set_pitch10(pitch_deg10);
        m_data.set_heading100(static_cast<int32_t>(heading_deg * 100.0f));
        m_data.set_vn_mps100(vn_mps100);
        m_data.set_ve_mps100(ve_mps100);
        m_data.set_roti100(static_cast<int32_t>(roti * 100.0f));
        m_data.set_ax100(static_cast<int32_t>(ax * 100.0f));
        m_data.set_ay100(static_cast<int32_t>(ay * 100.0f));
        m_data.set_aaz100(static_cast<int32_t>(aaz * 100.0f));
        m_data.set_gins_status(gins_status);
        m_data.set_hdop10(hdop10);
        m_data.set_head_std100(static_cast<int32_t>(head_std * 100.0f));
    }

    // 从已有的 OutMsg 构造（拷贝）。
    explicit GinsModel(const gins::data::OutMsg &data) : m_data(data) {}

    // ---------------------------------------------------------------
    // 只读访问器：将整数缩放字段还原为物理单位
    // ---------------------------------------------------------------
    uint32_t port()        const { return static_cast<uint32_t>(m_data.udp_port()); }
    double   utcLocal()    const { return m_data.utc_local100() / 100.0; }
    double   latitude()    const { return m_data.latitude1e7() / 1e7; }
    double   longitude()   const { return m_data.longitude1e7() / 1e7; }
    float    altitude()    const { return m_data.altitude100() / 1000.0f; }

    // roll / pitch 以 0.1° 为单位存储，此处转换为度
    double   rollDeg()     const { return m_data.roll10()  / 10.0; }
    double   pitchDeg()    const { return m_data.pitch10() / 10.0; }
    float    headingDeg()  const { return m_data.heading100() / 100.0f; }

    // 北向 / 东向速度以 0.01 m/s 为单位存储，此处转换为 m/s
    double   vnMps()       const { return m_data.vn_mps100() / 100.0; }
    double   veMps()       const { return m_data.ve_mps100() / 100.0; }

    float    roti()        const { return m_data.roti100() / 100.0f; }
    float    ax()          const { return m_data.ax100() / 100.0f; }
    float    ay()          const { return m_data.ay100() / 100.0f; }
    float    aaz()         const { return m_data.aaz100() / 100.0f; }
    uint32_t ginsStatus()  const { return m_data.gins_status(); }

    // HDOP 以 0.1 为单位存储，此处转换为实际值
    double   hdop()        const { return m_data.hdop10() / 10.0; }
    float    headStd()     const { return m_data.head_std100() / 100.0f; }

    // ---------------------------------------------------------------
    // 序列化：将消息编码为二进制字节串
    // ---------------------------------------------------------------
    std::string serialize() const
    {
        std::string buffer;
        if (!m_data.SerializeToString(&buffer)) {
            throw std::runtime_error("GinsModel: 序列化失败");
        }
        return buffer;
    }

    // ---------------------------------------------------------------
    // 反序列化：从二进制字节串解码，失败时抛出异常
    // ---------------------------------------------------------------
    void deserialize(const std::string &buffer)
    {
        if (!m_data.ParseFromString(buffer)) {
            throw std::runtime_error("GinsModel: 反序列化失败，数据格式不正确");
        }
    }

    // 工厂函数：从字节串构造 GinsModel
    static GinsModel fromBytes(const std::string &buffer)
    {
        GinsModel model;
        model.deserialize(buffer);
        return model;
    }

    // 底层消息的直接访问（供需要原始 API 的场景使用）
    const gins::data::OutMsg &raw() const { return m_data; }
    gins::data::OutMsg       &raw()       { return m_data; }

private:
    gins::data::OutMsg m_data;
};

#endif // GINS_H
