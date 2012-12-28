#ifndef PART_H
#define PART_H

#include <QString>
#include "partattribute.h"
#include "category.h"

namespace Models {

class Part
{    
public:
    Part();
private:
    int _categoryId;
    int _id;
    int _quantity;
    int _minimumQuantity;
    QString _partNumber;
    QString _description;
};

} //namespace
#endif // PART_H
