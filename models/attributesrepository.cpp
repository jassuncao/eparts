#include "attributesrepository.h"
#include <dquest.h>
#include <database/entities.h>
#include <QSqlQuery>
#include <QSqlError>


namespace Models {

struct AuxItem {
    int attributeId;
    int count;
    AuxItem(int attributeId,int count) : attributeId(attributeId), count(count) {}

    bool operator< (AuxItem const& other) const
    {
        return count>other.count;//We want the element with most hits first
    }
};

AttributesRepository::AttributesRepository(QObject *parent) :
    QObject(parent)
{
}

AttributesRepository::~AttributesRepository()
{    
    _attributes.clear();
}

AbstractPartAttribute *AttributesRepository::addAttribute(const QString &name, const QString &description, int type, int unitId)
{
     DQAttribute newAttr;
     newAttr.name.set(name);
     newAttr.description.set(description);
     newAttr.type.set(type);
     newAttr.unit.set(unitId);
     if(newAttr.save()){
         return readAttribute(newAttr);
     }
     else{
         qErrnoWarning("Unable to save attribute");
         //TODO: SHow some error msg
         return 0;
     }
}

void AttributesRepository::load()
{
    DQQuery<DQAttribute> query;
    if(query.exec()){
        DQAttribute attr;
        while(query.next()){
            query.recordTo(attr);
            readAttribute(attr);
        }
    }
}

const AbstractPartAttribute* AttributesRepository::findById(int id) const
{
    return _attributes.value(id, 0);
}

void countAttributes(QSqlQuery query, QList<AuxItem> & dest)
{
    if(query.exec()){
        while(query.next())
        {
            int attrId = query.value(0).toInt();
            int count = query.value(1).toInt();
            dest.append(AuxItem(attrId,count));
        }
        query.finish();
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
}

QList<const AbstractPartAttribute*> AttributesRepository::mapToAttributes(QList<AuxItem> & list, int max) const
{
    QList<const AbstractPartAttribute*> attributes;
    QList<AuxItem>::const_iterator it;
    qDebug()<<"Mapping "<<list.count()<<" attributes";
    //Add to the result list up to max attributes
    for(it=list.constBegin(); it!=list.constEnd() && attributes.count()<max; ++it){
        const AbstractPartAttribute* attr = findById((*it).attributeId);
        if(attr)
            attributes.append(attr);
    }
    return attributes;
}

const QList<const AbstractPartAttribute*> AttributesRepository::listMostUsedAttributes(int max) const
{
    QList<AuxItem> auxList;
    QString statement("SELECT attribute, COUNT(attribute) FROM %1 GROUP BY attribute LIMIT %2");
    QSqlQuery query;
    //First we account float attributes
    query.prepare(statement.arg("float_value").arg(max));
    countAttributes(query,auxList);
    //Now we account text attributes
    query.prepare(statement.arg("text_value").arg(max));
    countAttributes(query,auxList);
    //Sort by hits
    qSort(auxList);
    return mapToAttributes(auxList,max);
}

int AttributesRepository::count() const
{
    return _attributes.count();
}

QList<AbstractPartAttribute *> AttributesRepository::attributes() const
{
    return _attributes.values();
}


QList<const AbstractPartAttribute *> AttributesRepository::listCategoryAttributes(int categoryId) const
{
    QList<AuxItem> auxList;
    QString statement("SELECT v.attribute, COUNT(v.attribute) FROM %1 v INNER JOIN part p WHERE v.part=p.id AND p.category=%2 GROUP BY v.attribute");
    QSqlQuery query;
    //First we account float attributes
    query.prepare(statement.arg("float_value").arg(categoryId));
    countAttributes(query,auxList);
    //Now we account text attributes
    query.prepare(statement.arg("text_value").arg(categoryId));
    countAttributes(query,auxList);
    //Sort by hits
    qSort(auxList);
    return mapToAttributes(auxList);
}

AbstractPartAttribute *AttributesRepository::readAttribute(DQAttribute &attr)
{
    AbstractPartAttribute * partAttr=0;
    int id = attr.id.get().toInt();
    QString name = attr.name.get().toString();
    QString description = attr.description.get().toString();
    switch(attr.type.get().toInt()){
    case Models::ATTRIBUTE_GENERIC_FLOAT:
        partAttr = new FloatAttribute(id,name,description, this);
        break;
    case Models::ATTRIBUTE_GENERIC_INTEGER:
        partAttr = new IntegerAttribute(id,name,description, this);
        break;
    case Models::ATTRIBUTE_PERCENTAGE:
        partAttr = new PercentageAttribute(id,name,description, this);
        break;
    case Models::ATTRIBUTE_TEXT:
        partAttr = new TextAttribute(id,name,description, this);
        break;
    case Models::ATTRIBUTE_UNIT:
        QString unitName = attr.unit->name.get().toString();
        QString unitSymbol = attr.unit->symbol.get().toString();
        partAttr = new UnitAttribute(id,name,description,unitName, unitSymbol, this);
        break;
    }
    _attributes[id]=partAttr;
    return partAttr;
}

}//namespace
