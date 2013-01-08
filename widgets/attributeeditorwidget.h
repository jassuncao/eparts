#ifndef ATTRIBUTEEDITORWIDGET_H
#define ATTRIBUTEEDITORWIDGET_H

#include <QWidget>
#include "models/partattribute.h"

using namespace Models;

class QLineEdit;
class QHBoxLayout;
class QDoubleSpinBox;


class AbstractAttributeEditorWidget : public QWidget
{
    Q_OBJECT
public:
    virtual QVariant value() const = 0;
    virtual void setValue(QVariant value) = 0;
    //inline int attributeId() const {return _attributeId;}
signals:
    void removeAttributeClicked(const AbstractPartAttribute * attribute);
protected:
    explicit AbstractAttributeEditorWidget(const AbstractPartAttribute * attribute, QWidget *parent = 0);
    inline QHBoxLayout * boxLayout() const {return _boxLayout;}
protected slots:
    void removeButtonClicked();
protected:
    const AbstractPartAttribute * _attribute;
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
    explicit TextAttributeEditor(const TextAttribute * attribute, QWidget *parent = 0);
    QVariant value() const;
    void setValue(QVariant value);
private:
    QLineEdit * _lineEdit;
};

class PercentageAttributeEditor : public AbstractAttributeEditorWidget
{
    Q_OBJECT
public:
    explicit PercentageAttributeEditor(const PercentageAttribute * attribute, QWidget *parent = 0);
    QVariant value() const;
    void setValue(QVariant value);
private:
    QDoubleSpinBox * _spinbox;
};

class FloatAttributeEditor : public AbstractAttributeEditorWidget
{
    Q_OBJECT
public:
    explicit FloatAttributeEditor(const FloatAttribute * attribute, QWidget *parent = 0);
    QVariant value() const;
    void setValue(QVariant value);
private:
    QLineEdit * _lineEdit;
};

class IntegerAttributeEditor : public AbstractAttributeEditorWidget
{
    Q_OBJECT
public:
    explicit IntegerAttributeEditor(const IntegerAttribute * attribute, QWidget *parent = 0);
    QVariant value() const;
    void setValue(QVariant value);
private:
    QLineEdit * _lineEdit;
};

class UnitAttributeEditor : public AbstractAttributeEditorWidget
{
    Q_OBJECT
public:
    explicit UnitAttributeEditor(const UnitAttribute * attribute, QWidget *parent = 0);
    QVariant value() const;
    void setValue(QVariant value);
private slots:
    void prettyPrint();
private:
    QLineEdit * _lineEdit;
};

#endif // ATTRIBUTEEDITORWIDGET_H
