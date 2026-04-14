#include <QCoreApplication>
#include <QDebug>
#include <QVariantMap>
#include "ginsdatamodel.h"
#include <google/protobuf/stubs/common.h>

static void printSeparator(const char *title)
{
    qInfo().noquote() << "\n========" << title << "========";
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    GinsDataModel model;

    // 监听信号
    QObject::connect(&model, &GinsDataModel::dataChanged, []() {
        qInfo() << "[signal] dataChanged";
    });
    QObject::connect(&model, &GinsDataModel::errorOccurred,
                     [](int type, const QString &msg) {
        qWarning() << "[signal] errorOccurred:" << type << msg;
    });

    /* ---------- 1. 通过 setter 逐字段写入 ---------- */
    printSeparator("1. setter 写入");

    model.setPort(8001);
    model.setUtcLocal(1712000000.123);
    model.setLatitude(31.230416);
    model.setLongitude(121.473701);
    model.setAltitude(4.5);
    model.setRollDeg(1.3);
    model.setPitchDeg(-0.5);
    model.setHeadingDeg(270.8);
    model.setVnMps(2.35);
    model.setVeMps(-1.12);
    model.setRoti(0.05);
    model.setAx(0.01);
    model.setAy(-0.02);
    model.setAaz(9.81);
    model.setGinsStatus(3);
    model.setHdop(1.2);
    model.setHeadStd(0.5);

    model.printData();

    /* ---------- 2. 通过 getter 读取验证 ---------- */
    printSeparator("2. getter 读取");

    qInfo() << "port       =" << model.port();
    qInfo() << "utcLocal   =" << model.utcLocal();
    qInfo() << "latitude   =" << model.latitude();
    qInfo() << "longitude  =" << model.longitude();
    qInfo() << "altitude   =" << model.altitude();
    qInfo() << "rollDeg    =" << model.rollDeg();
    qInfo() << "pitchDeg   =" << model.pitchDeg();
    qInfo() << "headingDeg =" << model.headingDeg();
    qInfo() << "vnMps      =" << model.vnMps();
    qInfo() << "veMps      =" << model.veMps();
    qInfo() << "roti       =" << model.roti();
    qInfo() << "ax         =" << model.ax();
    qInfo() << "ay         =" << model.ay();
    qInfo() << "aaz        =" << model.aaz();
    qInfo() << "ginsStatus =" << model.ginsStatus();
    qInfo() << "hdop       =" << model.hdop();
    qInfo() << "headStd    =" << model.headStd();

    /* ---------- 3. toMap 测试 ---------- */
    printSeparator("3. toMap");

    QVariantMap map = model.toMap();
    for (auto it = map.constBegin(); it != map.constEnd(); ++it)
        qInfo().noquote() << "  " << it.key() << "=" << it.value().toString();

    /* ---------- 4. 序列化 / 反序列化 ---------- */
    printSeparator("4. serialize -> deserialize");

    QByteArray bytes = model.serialize();
    qInfo() << "serialized size:" << bytes.size() << "bytes";

    GinsDataModel model2;
    bool ok = model2.deserialize(bytes);
    qInfo() << "deserialize ok:" << ok;
    model2.printData();

    /* ---------- 5. fromMap 测试 ---------- */
    printSeparator("5. fromMap");

    QVariantMap newMap;
    newMap["port"]       = 9002u;
    newMap["latitude"]   = 39.904202;
    newMap["longitude"]  = 116.407394;
    newMap["headingDeg"] = 180.0;
    newMap["ginsStatus"] = 1u;

    GinsDataModel model3;
    model3.fromMap(newMap);
    model3.printData();

    /* ---------- 6. setProto / proto 测试 ---------- */
    printSeparator("6. setProto / proto");

    gins::data::GINSData raw = model.proto();
    qInfo() << "proto().port() =" << raw.port();

    GinsDataModel model4;
    model4.setProto(raw);
    qInfo() << "model4.latitude() =" << model4.latitude();
    qInfo() << "model4.headingDeg() =" << model4.headingDeg();

    /* ---------- 7. clear 测试 ---------- */
    printSeparator("7. clear");

    model.clear();
    qInfo() << "after clear: port=" << model.port()
            << "lat=" << model.latitude()
            << "status=" << model.ginsStatus();

    /* ---------- 8. 反序列化错误数据 ---------- */
    printSeparator("8. deserialize bad data");

    QByteArray badBytes("not a valid protobuf");
    bool badOk = model.deserialize(badBytes);
    qInfo() << "bad deserialize ok:" << badOk;

    printSeparator("Done");

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
