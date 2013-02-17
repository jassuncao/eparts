#ifndef READONLYATTRIBUTESLISTMODEL_H
#define READONLYATTRIBUTESLISTMODEL_H

#include <QAbstractListModel>
#include "partattribute.h"

namespace Models {

class ReadOnlyAttributesListModel : public QAbstractListModel
{
public:
    ReadOnlyAttributesListModel(QObject * parent=0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setAttributes(const QList<const AbstractPartAttribute *> attributes);
    const AbstractPartAttribute* getAttribute(int row) const;
private:    
    QList<const AbstractPartAttribute*> m_attributes;
};

} //namespace

#endif // READONLYATTRIBUTESLISTMODEL_H
