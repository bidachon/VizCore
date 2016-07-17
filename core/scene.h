#ifndef SCENE_H
#define SCENE_H

#include <QSharedPointer>
#include <QMap>
#include <QList>
#include <QUuid>
#include <QSharedPointer>

#include "modelitem.h"


Q_DECLARE_METATYPE(QUuid)

class Scene: public QObject
{
    Q_OBJECT
public:
    Scene();
    virtual ~Scene();

    void addItem(QSharedPointer<ModelItem> model);

    void removeItem(const QUuid &id);

    QUuid id() const  {return m_uuid; }

    void clear();

    QSharedPointer<ModelItem> modelItem(QUuid) const;
    QList < QSharedPointer<ModelItem> > modelItems() const;

signals:
    void modelItemAdded(QUuid id);
    void modelItemRemoved(QUuid id);

private:
    QUuid m_uuid;
    QMap< QUuid, QSharedPointer<ModelItem> > m_models;
    //QList< QSharedPointer<PropertyList> > m_properties;

};

#endif // SCENE_H

