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
    ATTRIBUTE_TEXT=1024
};



class PartAttribute
{
public:
    PartAttribute(int id, const QString & name, AttributeType type, bool virtualAttr=false);
    inline int id() const {return _id;}
    inline const QString & name() const {return _name;}
    inline AttributeType type() const {return _type;}
    inline bool isVirtual() const {return _virtualAttr;}

    static inline bool isTextAttribute(int attr)
    {
        return attr & 1024;
    }

private:    
    const int _id;
    const QString _name;
    const AttributeType _type;
    const bool _virtualAttr;
};

}//namespace
#endif // PARTATTRIBUTE_H
