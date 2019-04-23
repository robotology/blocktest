#ifndef NUMBERITEMDELEGATE_H
#define NUMBERITEMDELEGATE_H

#include <QStyledItemDelegate>

class SpinBoxDelegate : public QStyledItemDelegate
{
    //Q_OBJECT

public:
    SpinBoxDelegate(QObject *parent,int max,int min);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    int max_{0};
    int min_{0};
};


#endif // NUMBERITEMDELEGATE_H
