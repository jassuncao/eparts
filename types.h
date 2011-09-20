#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QSharedData>
#include <QList>
#include <QHash>

namespace EParts {

class Category
{
public:
    Category(const int id, const QString &name, const QString &description);

    int id () const;
    const QString & name() const;
    const QString & description() const;

    void setId(const int id);
    void setName(const QString &name);
    void setDescription(const QString &description);
private:
    int _id;
    QString _name;
    QString _description;
};

class Parameter;

class PartDefinition
{
public:
    PartDefinition(const int id, const QString &name, const QString &description, const int categoryId);

    inline int id () const { return _id;}
    inline const QString & name() const { return _name;}
    inline const QString & description() const { return _description;}
    inline int categoryId () const {return _categoryId;}
    inline const QList<Parameter> * parameters() const { return _parameters;}

    inline void setId(const int id) {_id = id;}
    inline void setName(const QString &name) {_name = name;}
    inline void setDescription(const QString &description) {_description = description;}
    inline void setCategoryId(const int categoryId) {_categoryId = categoryId;}
    inline void setParameter(QList<Parameter> * parameters) {_parameters = parameters;}
private:
    int _id;
    QString _name;
    QString _description;
    int _categoryId;
    QList<Parameter> * _parameters;
};

class Parameter
{
public:
    enum ParameterType
    {
        Text, Number
    };

    enum FormatType
    {
        Unformated, Resistance, Capacitance, Inductance, Power, Percentage
    } ;

    Parameter(const int id, const QString& name, const QString& description, const ParameterType type, const FormatType format, const int partDefinitionId, const bool isFixedValue, const int order);
    Parameter(const Parameter& other );
    Parameter& operator=(const Parameter& other );

    inline int id () const { return _id;}
    inline const QString & name() const { return _name;}
    inline const QString & description() const { return _description;}
    inline ParameterType type() const { return _type;}
    inline FormatType format() const { return _format;}
    inline int partId () const { return _partId;}
    inline bool isFixedValue() const { return _isFixedvalue;}
    inline int order() const { return _order;}

    inline void setId(const int id) {_id = id;}
    inline void setName(const QString &name) {_name = name;}
    inline void setDescription(const QString &description) { _description = description;}
    inline void setType(const ParameterType type) { _type = type;}
    inline void setFormat(const FormatType format) { _format = format;}
    inline void setPartId(const int partDefinitionId) { _partId = partDefinitionId;}
    inline void setIsFixedValue(bool isFixedValue) { _isFixedvalue = isFixedValue;}
    inline void setOrder(const int order) {_order = order;}

private:
    int _id;
    QString _name;
    QString _description;
    ParameterType _type;
    FormatType _format;
    int _partId;
    bool _isFixedvalue;
    int _order;
};


class PartValueData : public QSharedData
{
public:
    PartValueData()
        : id(-1), numericValue(0) {textValue.clear();}
    PartValueData(const int id, const double value)
        : id(id), numericValue(value) {textValue.clear();}
    PartValueData(const int id, const QString& value)
        : id(id), numericValue(0), textValue(value) {}
    PartValueData(const PartValueData &other)
        : QSharedData(other), id(other.id), numericValue(other.numericValue), textValue(other.textValue) { }
    ~PartValueData() { }
public:
    int id;
    double numericValue;
    QString textValue;
};

class PartValue
{
public:
    PartValue() {
        d = new PartValueData();
    }
    PartValue(const int id, const double value) {
        d = new PartValueData(id, value);
    }
    PartValue(const int id, const QString & value) {
        d = new PartValueData(id, value);
    }
    PartValue(const PartValue &other)
       : d (other.d)
    {
    }
    inline void setId(const int id) { d->id = id; }
    inline void setValue(const double value) { d->numericValue = value; }
    inline void setValue(const QString & value) { d->textValue = value; }

    inline int id() const { return d->id; }
    inline double numericValue() const { return d->numericValue; }
    inline const QString & textValue() const { return d->textValue; }
private:
    QSharedDataPointer<PartValueData> d;
};

/*

class NumericValueData : public PartValueData
{
public:
    NumericValueData() : PartValueData(), value(0) {}
    NumericValueData(const int id, const double value) : PartValueData(id), value(value) {}
    NumericValueData(const NumericValueData &other) : PartValueData(other), value(other.value) { }
    ~NumericValueData() { }

    double value;
};

class TextValueData : public PartValueData
{
public:
    TextValueData() : id(-1) { value.clear();}
    TextValueData(const int id, const QString & value) : id(id), value(value) {}
    TextValueData(const TextValueData &other) : PartValueData(other),value(other.value) { }
    ~TextValueData() { }
    int id;
    QString value;
};

class PartValue
{
protected:
     PartValue(){};
     virtual int id() const =0;
};

class NumericPartValue : PartValue
{
public:
    NumericPartValue() { d = new NumericValueData(); }
     NumericPartValue(const int id, const double value) {
         d = new NumericValueData(id, value);
     }
     NumericPartValue(const NumericPartValue &other)
           : d (other.d)
     {
     }
     inline void setId(const int id) { d->id = id; }
     inline void setValue(const double value) { d->value = value; }

     inline int id() const { return d->id; }
     inline double value() const { return d->value; }
private:
    QSharedDataPointer<NumericValueData> d;
};
*/
/*
class PartValue
{
protected:
    PartValue(const int id);
    inline int id () const { return _id;}
    inline void setId(const int id) {_id = id;}
protected:
    int _id;
};

class NumericValue : public PartValue {
public:
    NumericValue(const int id, double value);
    inline double numericValue () const { return _numericValue;}
    inline void setValue(const double value) {_numericValue = value;}
protected:
    double _numericValue;
};

class TextValue : public PartValue {
public:
    TextValue(const int id, const QString& value);   
    inline const QString & textValue () const { return _textValue;}
    inline void setValue(const QString &value) {_textValue = value;}
protected:
    QString _textValue;
};
*/

class Part
{
public:
    Part(const int id, const int quantity, const int minimumQuantity, const QString & notes);
    inline int id () const { return _id;}
    inline int quantity () const { return _quantity;}
    inline int minimumQuantity () const { return _minimumQuantity;}
    inline const QString & notes() const { return _notes;}
    //inline const QHash<int, PartValue> & paramValues() const { return _paramValues;}
    inline PartValue getParamValue(const int paramTypeId) const { return _paramValues.value(paramTypeId);}

    inline void setId(const int id) {_id = id;}
    inline void setQuantity(const int quantity) {_quantity = quantity;}
    inline void setMinimumQuantity(const int minimumQuantity) {_minimumQuantity = minimumQuantity;}
    inline void setNotes(const QString &notes) {_notes = notes;}
    inline void setParamValue(const int paramTypeId, const PartValue & partValue) {_paramValues[paramTypeId] = partValue;}

    //inline void setParamValues(const QList<PartValue> paramValues) {_paramValues = paramValues;}

private:
    int _id;
    int _quantity;
    int _minimumQuantity;
    QString _notes;
    QHash<int, PartValue> _paramValues;

};

}

Q_DECLARE_TYPEINFO(EParts::Parameter, Q_MOVABLE_TYPE);

#endif // TYPES_H
