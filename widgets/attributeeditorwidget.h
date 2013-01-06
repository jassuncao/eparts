#ifndef ATTRIBUTEEDITORWIDGET_H
#define ATTRIBUTEEDITORWIDGET_H

#include <QWidget>
#include "models/partattribute.h"

class QLineEdit;
class QHBoxLayout;
class QDoubleSpinBox;


class AbstractAttributeEditorWidget : public QWidget
{
    Q_OBJECT
public:
    virtual QVariant value() const = 0;
    virtual void setValue(QVariant value) = 0;
    inline int attributeId() const {return _attributeId;}
signals:
    void removeAttributeClicked(int attributeId);
protected:
    explicit AbstractAttributeEditorWidget(int attributeId, QWidget *parent = 0);
    inline QHBoxLayout * boxLayout() const {return _boxLayout;}
protected slots:
    void removeButtonClicked();
private:
    int _attributeId;
    QHBoxLayout * _boxLayout;
};

/*
class AttributeEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AttributeEditorWidget(int attributeId, Models::AttributeType attributeType, QWidget *parent = 0);
    QVariant value() const;
    QVariant setValue(QVariant value);
signals:
    void removeAttributeClicked(int attributeId);
public slots:

private slots:
    void removeButtonClicked();
    void prettyPrint();
private:
    QLineEdit * _lineEdit;
    int _attributeId;
    Models::AttributeType _attributeType;
};
*/

class TextAttributeEditor : public AbstractAttributeEditorWidget
{
    Q_OBJECT
public:
    explicit TextAttributeEditor(int attributeId, QWidget *parent = 0);
    QVariant value() const;
    void setValue(QVariant value);
private:
    QLineEdit * _lineEdit;
};

class PercentageAttributeEditor : public AbstractAttributeEditorWidget
{
    Q_OBJECT
public:
    explicit PercentageAttributeEditor(int attributeId, QWidget *parent = 0);
    QVariant value() const;
    void setValue(QVariant value);
private:
    QDoubleSpinBox * _spinbox;
};

class GenericNumberAttributeEditor : public AbstractAttributeEditorWidget
{
    Q_OBJECT
public:
    explicit GenericNumberAttributeEditor(int attributeId, QWidget *parent = 0);
    QVariant value() const;
    void setValue(QVariant value);
private:
    QLineEdit * _lineEdit;
};

class UnitAttributeEditor : public AbstractAttributeEditorWidget
{
    Q_OBJECT
public:
    explicit UnitAttributeEditor(int attributeId, QChar suffix, QWidget *parent = 0);
    QVariant value() const;
    void setValue(QVariant value);
private slots:
    void prettyPrint();
private:
    QLineEdit * _lineEdit;
};

#endif // ATTRIBUTEEDITORWIDGET_H
