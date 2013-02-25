#ifndef PARTATTRIBUTE_H
#define PARTATTRIBUTE_H

#include <QString>
#include <QObject>


namespace Models {

class AbstractPartAttribute : public QObject
{
    Q_OBJECT
public:    
    inline int id() const {return _id;}
    //inline void setId(int id) {_id=id;}

    inline QString name() const {return _name;}
    //inline void setName(const QString &name) {_name = name;}

    inline QString description() const {return _description;}
    //inline void setDescription(const QString &description) {_description = description;}

    /*
    inline QString unit() const {return _unit;}
    inline void setUnit(const QString &unit) {_unit = unit;}
    */
    //inline AttributeType type() const {return _type;}
    //inline void setType() {_type=type;}

    //virtual AttributeType type() const = 0;
    virtual void accept(class PartAttributeVisitor &visitor) const = 0;
    virtual bool isText() const = 0;

protected:
    AbstractPartAttribute(const int id, const QString &name, const QString &description, QObject *parent = 0);
private:    
    const int _id;
    const QString _name;
    const QString _description;
    //QString _unit;
    //AttributeType _type;
};

class FloatAttribute : public AbstractPartAttribute {    
    Q_OBJECT
public:
    FloatAttribute(const int id, const QString &name, const QString &description, QObject *parent = 0);
    void accept(PartAttributeVisitor &visitor) const;
    bool isText() const {return false;}
};

class IntegerAttribute : public AbstractPartAttribute {
    Q_OBJECT
public:
    IntegerAttribute(const int id, const QString &name, const QString &description, QObject *parent = 0);
    void accept(PartAttributeVisitor &visitor) const;
    bool isText() const {return false;}
};

class UnitAttribute : public AbstractPartAttribute {
    Q_OBJECT
public:
    UnitAttribute(const int id, const QString &name, const QString &description, const QString &unitName, const QString &unitSymbol, QObject *parent = 0);
    void accept(PartAttributeVisitor &visitor) const;
    inline QString unitSymbol() const {return _unitSymbol;}
    inline QString unitName() const {return _unitName;}
    bool isText() const {return false;}
private:    
    const QString _unitName;
    const QString _unitSymbol;
};

class PercentageAttribute : public AbstractPartAttribute {
    Q_OBJECT
public:
    PercentageAttribute(const int id, const QString &name, const QString &description, QObject *parent = 0);
    void accept(PartAttributeVisitor &visitor) const;
    bool isText() const {return false;}
};

class TextAttribute : public AbstractPartAttribute {    
    Q_OBJECT
public:
    TextAttribute(const int id, const QString &name, const QString &description, QObject *parent = 0);
    void accept(PartAttributeVisitor &visitor) const;
    bool isText() const {return true;}
};


class PartAttributeVisitor {
public:
    virtual void visit(const FloatAttribute *) = 0;
    virtual void visit(const IntegerAttribute *) = 0;
    virtual void visit(const UnitAttribute *) = 0;
    virtual void visit(const PercentageAttribute *) = 0;
    virtual void visit(const TextAttribute *) = 0;
};


}//namespace
#endif // PARTATTRIBUTE_H
