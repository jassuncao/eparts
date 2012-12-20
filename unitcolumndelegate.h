#ifndef UNITCOLUMNDELEGATE_H
#define UNITCOLUMNDELEGATE_H

#include <QStyledItemDelegate>
#include "database/partparameter.h"

class QChar;

class UnitColumnDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit UnitColumnDelegate(PartParameter::ParameterType paramType, QObject *parent = 0);
    QString displayText ( const QVariant & value, const QLocale & locale ) const;
    //void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    PartParameter::ParameterType _paramType;
    QChar _unit;
};

class DoubleSpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DoubleSpinBoxDelegate(QChar unit, double singleStep=1.0, QObject *parent = 0);
    QString displayText ( const QVariant & value, const QLocale & locale ) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QChar _unit;
    double _singleStep;
};

/*
class PowerColumnDelegate : public DoubleSpinBoxDelegate
{
    Q_OBJECT
public:
    explicit PowerColumnDelegate(QObject *parent = 0);
};
*/

#endif // UNITCOLUMNDELEGATE_H
