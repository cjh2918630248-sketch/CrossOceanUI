#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QVariantMap>
#include "pointslistmodel.h" // 包含用户自定义的 DataListModel 类的头文件

// 定义 AddCommand 类，继承自 QUndoCommand
class AddCommand : public QUndoCommand
{
public:
    // 构造函数，接受三个参数：modelData 是要添加的数据，model 是数据模型的指针，parent 是父命令，默认为空指针
    AddCommand(const ModelDataPoint &modelData, PointsListModel *model,
               QUndoCommand *parent = nullptr);

    // 重载的虚函数，用于实现撤销操作
    void undo() override;
    // 重载的虚函数，用于实现重做操作
    void redo() override;

private:
    int mIndex = -1; // 存储数据的位置，初始化为 -1
    ModelDataPoint mModelData; // 存储要添加的数据
    PointsListModel *mModel = nullptr; // 数据模型的指针，初始化为空指针
};

// 定义 DeleteCommand 类，继承自 QUndoCommand
class DeleteCommand : public QUndoCommand
{
public:
    // 构造函数，接受三个参数：index 是要删除的数据的位置，model 是数据模型的指针，parent 是父命令，默认为空指针
    DeleteCommand(int index, PointsListModel *model, QUndoCommand *parent = nullptr);

    // 重载的虚函数，用于实现撤销操作
    void undo() override;
    // 重载的虚函数，用于实现重做操作
    void redo() override;

private:
    int mIndex; // 存储数据的位置
    ModelDataPoint mModelData; // 存储要删除的数据
    PointsListModel *mModel = nullptr; // 数据模型的指针，初始化为空指针
};

// 定义 SetCommand 类，继承自 QUndoCommand
class SetCommand : public QUndoCommand
{
public:
    // 构造函数，接受四个参数：
    // index 是要修改的数据的位置，newPoint 是新的轨迹点数据，oldPoint 是旧的轨迹点数据，model 是数据模型的指针
    SetCommand(int index, QVariantMap newPoint, QVariantMap oldPoint, PointsListModel *model,
               QUndoCommand *parent = nullptr);

    // 重载的虚函数，用于实现撤销操作
    void undo() override;
    // 重载的虚函数，用于实现重做操作
    void redo() override;

private:
    int mIndex; // 存储数据的位置
    QVariantMap mNewPoint; // 存储要修改的数据（新的轨迹点数据）
    QVariantMap mOldPoint; // 存储要修改的数据（旧的轨迹点数据）
    PointsListModel *mModel = nullptr; // 数据模型的指针，初始化为空指针
};

#endif // COMMANDS_H
