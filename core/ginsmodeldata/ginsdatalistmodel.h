#ifndef GINSDATALISTMODEL_H
#define GINSDATALISTMODEL_H

#include <QAbstractListModel>
#include <QVariantMap>
#include "msg_gins_data.pb.h"

// QAbstractListModel 封装：
// - 内部存储 protobuf 的 GINSData；
// - 对外提供 QVariantMap 接口，方便 QWidget/QML 使用；
// - 同时提供序列化/反序列化能力，便于网络传输或存储。
class GinsDataListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    explicit GinsDataListModel(QObject *parent = nullptr);

    // 角色名用于视图层访问字段（QML 或自定义 delegate）。
    enum DisplayRoles {
        PortRole = Qt::UserRole + 1,
        UtcLocalRole,
        LatitudeRole,
        LongitudeRole,
        AltitudeRole,
        RollDegRole,
        PitchDegRole,
        HeadingDegRole,
        VnMpsRole,
        VeMpsRole,
        RotiRole,
        AxRole,
        AyRole,
        AazRole,
        GinsStatusRole,
        HdopRole,
        HeadStdRole
    };

    // 统一错误类型，供 UI 通过 errorOccurred 展示问题来源。
    enum ErrorType {
        NoError = 0,
        InvalidIndex,
        InvalidData,
        OutOfRange,
        SerializeError,
        DeserializeError,
        UnknownError
    };
    Q_ENUM(ErrorType)

    /* 增 */
    Q_INVOKABLE void insert(int index, const QVariantMap &map);
    Q_INVOKABLE void append(const QVariantMap &map);

    /* 删 */
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void clear();

    /* 查 */
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int count() const;
    Q_INVOKABLE QVariantMap get(int index) const;
    Q_INVOKABLE void printAllData() const;

    /* 改 */
    Q_INVOKABLE void set(int index, const QVariantMap &map);

    /* 序列化 */
    Q_INVOKABLE QByteArray serializeItem(int index) const;
    Q_INVOKABLE bool deserializeAndAppend(const QByteArray &bytes);
    Q_INVOKABLE bool deserializeAndUpdate(int index, const QByteArray &bytes);

    /* 批量操作 */
    void appendProto(const gins::data::GINSData &data);
    void updateProto(int index, const gins::data::GINSData &data);
    bool protoData(int index, gins::data::GINSData &out) const;

signals:
    void countChanged();
    void dataUpdated();
    void itemAdded(int index);
    void itemRemoved(int index);
    void itemChanged(int index);
    void errorOccurred(int errorType, const QString &errorMessage);

protected:
    // 将 role id 映射为可读字符串键名。
    QHash<int, QByteArray> roleNames() const override;

private:
    // map/proto 双向转换：
    // - mapToProto: UI 输入 -> protobuf（含单位缩放）；
    // - protoToMap: protobuf -> UI 友好值（恢复真实单位）。
    static QVariantMap protoToMap(const gins::data::GINSData &d);
    static void mapToProto(const QVariantMap &map, gins::data::GINSData &d);

    QList<gins::data::GINSData> m_list;
};

#endif // GINSDATALISTMODEL_H
