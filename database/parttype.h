#ifndef PARTTYPE_H
#define PARTTYPE_H

#include <dquest.h>
#include "category.h"

class PartType : public DQModel {
    DQ_MODEL
public:
    DQField<QString> name;
    DQField<QString> partTableName;
    DQField<QString> description;
    DQForeignKey<Category> category;
};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(PartType,
                 "part_type", // the table name.
                 DQ_FIELD(name, DQNotNull),
                 DQ_FIELD(partTableName, DQNotNull),
                 DQ_FIELD(description),
                 DQ_FIELD(category , DQNotNull)
                 )


#endif // PARTTYPE_H
