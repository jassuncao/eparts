#ifndef PARTATTRIBUTE_H
#define PARTATTRIBUTE_H

#include <QString>

namespace Models {

enum AttributeType
{
    ATTRIBUTE_GENERIC_NUMBER=0,
    ATTRIBUTE_RESISTANCE=1,
    ATTRIBUTE_CAPACITANCE=2,
    ATTRIBUTE_INDUCTANCE=3,
    ATTRIBUTE_POWER=4,
    ATTRIBUTE_PERCENTAGE=5,
    ATTRIBUTE_TEXT=1<<16
};


class PartAttribute
{
public:
    PartAttribute(const int id, const QString &name, const QString &description, const AttributeType type);
    inline int id() const {return _id;}
    inline QString name() const {return _name;}
    inline QString description() const {return _description;}
    inline AttributeType type() const {return _type;}

    static inline bool isTextAttribute(int attr)
    {
        return attr & ATTRIBUTE_TEXT;
    }

private:    
    const int _id;
    const QString _name;
    const QString _description;
    const AttributeType _type;
};

}//namespace
#endif // PARTATTRIBUTE_H
