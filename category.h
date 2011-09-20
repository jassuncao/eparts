#ifndef CATEGORY_H
#define CATEGORY_H

#include <dquest.h>

class Category : public DQModel {
    DQ_MODEL
public:
    DQField<QString> name;
    DQField<QString> description;

};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(Category,
                 "category", // the table name.
                 DQ_FIELD(name , DQNotNull | DQUnique), // The field can not be null and must be unique
                 DQ_FIELD(description)
                 )

#endif // CATEGORY_H
