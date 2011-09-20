#ifndef PARTTYPE_H
#define PARTTYPE_H

#include <dquest.h>
#include "category.h"

class PartType : public DQModel {
    DQ_MODEL
public:
    DQField<QString> name;
    DQField<QString> description;
    DQForeignKey<Category> category;
    DQField<int> valueType;
};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(PartType,
                 "part_type", // the table name.
                 DQ_FIELD(name, DQNotNull),
                 DQ_FIELD(description),
                 DQ_FIELD(category , DQNotNull),
                 DQ_FIELD(valueType , DQNotNull)
                 )


#endif // PARTTYPE_H
