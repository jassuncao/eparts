#ifndef ENTITIES_H
#define ENTITIES_H

#include <dquest.h>



class DQCategory : public DQModel {
    DQ_MODEL
public:
    DQField<int> parent;
    DQField<QString> name;
    DQField<QString> description;
};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(DQCategory,
                 "category", // the table name.
                 DQ_FIELD(parent),
                 DQ_FIELD(name , DQNotNull | DQUnique), // The field can not be null and must be unique
                 DQ_FIELD(description)
                 )

class DQPart : public DQModel {
    DQ_MODEL
public:
    DQForeignKey<DQCategory> category;
    DQField<int> quantity;
    DQField<int> minimumQuantity;
    DQField<QString> partNumber;
    DQField<QString> description;
    //DQForeignKey<DQPartBin> partBin;
};

DQ_DECLARE_MODEL(DQPart,
                 "part", // the table name.
                 DQ_FIELD(category, DQNotNull),
                 DQ_FIELD(quantity , DQNotNull),
                 DQ_FIELD(minimumQuantity),
                 DQ_FIELD(partNumber),
                 DQ_FIELD(description)
                 )

class DQUnit : public DQModel {
    DQ_MODEL
public:
    DQField<QString> symbol;
    DQField<QString> name;
};

DQ_DECLARE_MODEL(DQUnit,
                 "unit", // the table name.
                 DQ_FIELD(symbol, DQNotNull),
                 DQ_FIELD(name , DQNotNull)
                 )

class DQAttribute : public DQModel {
    DQ_MODEL
public:
    DQField<int> type;
    DQForeignKey<DQUnit> unit;
    DQField<QString> name;
    DQField<QString> description;    
};

DQ_DECLARE_MODEL(DQAttribute,
                 "attribute", // the table name.
                 DQ_FIELD(type, DQNotNull),
                 DQ_FIELD(unit),
                 DQ_FIELD(name),
                 DQ_FIELD(description)
                 )


class DQFloatValue : public DQModel {
    DQ_MODEL
public:
    DQForeignKey<DQPart> part;
    DQForeignKey<DQAttribute> attribute;
    //DQField<int> category;
    DQField<qreal> value;
};

DQ_DECLARE_MODEL(DQFloatValue,
                 "float_value", // the table name.
                 DQ_FIELD(part, DQNotNull),
                 DQ_FIELD(attribute, DQNotNull),
                 //DQ_FIELD(cat_id),
                 DQ_FIELD(value)
                 )

class DQTextValue : public DQModel {
    DQ_MODEL
public:
    DQForeignKey<DQPart> part;
    DQForeignKey<DQAttribute> attribute;
    //DQField<int> category;
    DQField<QString> value;
};

DQ_DECLARE_MODEL(DQTextValue,
                 "text_value", // the table name.
                 DQ_FIELD(part, DQNotNull),
                 DQ_FIELD(attribute, DQNotNull),
                 //DQ_FIELD(cat_id),
                 DQ_FIELD(value)
                 )

class DQCategoryActiveAttributes : public DQModel {
    DQ_MODEL
public:
    DQForeignKey<DQCategory> category;
    DQForeignKey<DQAttribute> attribute;
    DQField<int> idx;
};

DQ_DECLARE_MODEL(DQCategoryActiveAttributes,
                 "category_active_attributes", // the table name.
                 DQ_FIELD(category, DQNotNull),
                 DQ_FIELD(attribute, DQNotNull),
                 DQ_FIELD(idx)
                 )

//} //namespace
#endif // ENTITIES_H
