#include "commands.h"  // 包含 AddCommand 和 DeleteCommand 类的头文件

// AddCommand 构造函数，初始化成员变量
AddCommand::AddCommand(const ModelDataPoint &modelData, PointsListModel *model,
                       QUndoCommand *parent)
    : QUndoCommand(parent),   // 调用父类构造函数
      mModelData(modelData),  // 初始化 mModelData 成员变量
      mModel(model)           // 初始化 mModel 成员变量
{
  Q_ASSERT(mModel);             // 断言 mModel 不为空
  mIndex = mModel->rowCount();  // 获取模型中的行数，作为新数据的索引
}

// 撤销添加操作
void AddCommand::undo() {
  mModel->remove(mIndex);  // 从模型中移除指定索引位置的数据
}

// 重做添加操作
void AddCommand::redo() {
  Q_ASSERT(mModel);                    // 断言 mModel 不为空
  mModel->insert(mIndex, mModelData);  // 在模型中插入数据到指定索引位置
}

// DeleteCommand 构造函数，初始化成员变量
DeleteCommand::DeleteCommand(int index, PointsListModel *model,
                             QUndoCommand *parent)
    : QUndoCommand(parent),  // 调用父类构造函数
      mIndex(index),         // 初始化 mIndex 成员变量
      mModel(model)          // 初始化 mModel 成员变量
{
  Q_ASSERT(mModel);                       // 断言 mModel 不为空
  mModel->modelData(mIndex, mModelData);  // 获取指定索引位置的 ModelData
}

// 撤销删除操作
void DeleteCommand::undo() {
  Q_ASSERT(mModel);                    // 断言 mModel 不为空
  mModel->insert(mIndex, mModelData);  // 在模型中插入数据到指定索引位置
}

// 重做删除操作
void DeleteCommand::redo() {
  Q_ASSERT(mModel);        // 断言 mModel 不为空
  mModel->remove(mIndex);  // 从模型中移除指定索引位置的数据
}

// SetCommand 构造函数，初始化成员变量
SetCommand::SetCommand(int index, QVariantMap newPoint, QVariantMap oldPoint,
                       PointsListModel *model,
                       QUndoCommand *parent)
    : QUndoCommand(parent),  // 调用父类构造函数
      mIndex(index),         // 初始化 mIndex 成员变量
      mNewPoint(newPoint),   // 初始化 mNewPoint 成员变量
      mOldPoint(oldPoint),   // 初始化 mOldPoint 成员变量
      mModel(model)          // 初始化 mModel 成员变量
{}

// 撤销修改操作
void SetCommand::undo() {
  Q_ASSERT(mModel);  // 断言 mModel 不为空
  // 设置为旧的轨迹点数据
  mModel->set(mIndex, mOldPoint["latitude"].toDouble(),
              mOldPoint["longitude"].toDouble());
}

// 重做修改操作
void SetCommand::redo() {
  Q_ASSERT(mModel);  // 断言 mModel 不为空
  // 设置为新的轨迹点数据
  mModel->set(mIndex, mNewPoint["latitude"].toDouble(),
              mNewPoint["longitude"].toDouble());
}
