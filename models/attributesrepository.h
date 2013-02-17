#ifndef ATTRIBUTESREPOSITORY_H
#define ATTRIBUTESREPOSITORY_H
#include <QList>
#include <QMap>
#include "partattribute.h"

class DQAttribute;

namespace Models {

struct AuxItem;

class AttributesRepository : public QObject
{
public:
    explicit AttributesRepository(QObject *parent=0);
    ~AttributesRepository();
    AbstractPartAttribute* addAttribute(const QString &name, const QString &description,int type, int unitId);
    void load();
    const AbstractPartAttribute* findById(int id) const;
    const QList<const AbstractPartAttribute*> listMostUsedAttributes(int max) const;
    int count() const;
    QList<AbstractPartAttribute*> attributes() const;
    QList<const AbstractPartAttribute*> listCategoryAttributes(int categoryId) const;
private:
    QList<const AbstractPartAttribute*> mapToAttributes(QList<AuxItem> & list, int max=INT_MAX) const;
    AbstractPartAttribute* readAttribute(DQAttribute &attr);
    QMap<int,AbstractPartAttribute*> _attributes;
};


}//namespace

#endif // ATTRIBUTESREPOSITORY_H
