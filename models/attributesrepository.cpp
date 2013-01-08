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

void AttributesRepository::load()
{
    DQQuery<DQAttribute> query;
    if(query.exec()){
        DQAttribute attr;
        while(query.next()){
            query.recordTo(attr);
            AbstractPartAttribute * partAttr;
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
        }
    }
}

const AbstractPartAttribute* AttributesRepository::findById(int id) const
{
    return _attributes.value(id, 0);
}

const QList<const AbstractPartAttribute*> AttributesRepository::listMostUsedAttributes(int max) const
{
    QString statement("SELECT attribute, COUNT(attribute) FROM %1 GROUP BY attribute LIMIT %2");
    QSqlQuery query;
    //First we account float attributes
    query.prepare(statement.arg("float_value").arg(max));
    QList<AuxItem> auxList;
    if(query.exec()){
        while(query.next())
        {
            int attrId = query.value(0).toInt();
            int count = query.value(1).toInt();
            auxList.append(AuxItem(attrId,count));
        }
        query.finish();
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    //Now we account text attributes
    query.prepare(statement.arg("text_value").arg(max));
    if(query.exec()){
        while(query.next())
        {
            int attrId = query.value(0).toInt();
            int count = query.value(1).toInt();
            auxList.append(AuxItem(attrId,count));
        }
        query.finish();
    }

    //Sort by hits
    qSort(auxList);
    QList<const AbstractPartAttribute*> attributes;
    QList<AuxItem>::const_iterator it;
    //Add to the result list up to max attributes
    for(it=auxList.constBegin(); it!=auxList.constEnd() && attributes.count()<max; ++it){
        attributes.append(findById((*it).attributeId));
    }
    return attributes;
}

int AttributesRepository::count() const
{
    return _attributes.count();
}

QList<AbstractPartAttribute *> AttributesRepository::attributes() const
{
    return _attributes.values();
}

}//namespace
