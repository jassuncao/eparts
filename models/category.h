#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>

namespace Models {

class Category
{
public:
    Category();
private:
    int _id;
    int _parentCategory;
    QString _name;
};

}//namespace

#endif // CATEGORY_H
