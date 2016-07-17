#include "scene.h"
#include <QDebug>

Scene::Scene():QObject()
{
    m_uuid = QUuid::createUuid();
}

Scene::~Scene(){

}

void Scene::addItem(QSharedPointer<ModelItem> model)
{
    QUuid id = model->id();
    if (!m_models.contains(id))
    {
        m_models[id] = model;
        emit modelItemAdded(id);
    }
    else
        qDebug() << "Model with uuid" +id.toString() + " Already inserted";
}


QSharedPointer<ModelItem> Scene::modelItem(QUuid id) const
{
    QSharedPointer<ModelItem> item;
    QMap< QUuid, QSharedPointer<ModelItem> >::const_iterator it =  m_models.find(id);
    if (it != m_models.end())
        item = *it;
    else
        qDebug() << "Item "+ id.toString() + " could not be found";

    return item;

}

QList < QSharedPointer<ModelItem> > Scene::modelItems() const
{
    QList<QSharedPointer<ModelItem> > list;
    for(QMap< QUuid, QSharedPointer<ModelItem> >::const_iterator it = m_models.begin(); it != m_models.end(); ++it) {
        list.push_back(it.value());
    }
    return list;
}

/*void Scene::addModels(QList<QSharedPointer<ModelItem> > models)
{
    QList<QSharedPointer<ModelItem> >::iterator it;
    for (it = models.begin(); it != models.end(); it++)
        addModel(*it);
}*/

void Scene::removeItem(const QUuid &id)
{
    if(m_models.contains(id))
    {
        m_models.erase(id);
        emit modelItemRemoved(id);
    }
    else
        qDebug() << "Could not find model id " + id.toString();

}

void Scene::clear()
{
    QList<QUuid> keys = m_models.keys();
    for (int i = 0 ; i < keys.size(); i++)
        this->removeItem(keys[i]);
}

/**
 * @brief returns a PropertyList of type T. If it cannot be found, a new property will be created and Stored.
 */
/*template<class T>
QSharedPointer<T> Scene::properties() const
{
    for (int i = 0; i< m_properties.size(); i++)
    {
        T *prop = dynamic_cast<T*>(m_properties.at(i));
        if (prop)
            return prop;
    }

    QSharedPointer<T> newDefaultProp(new T);
    m_properties.append(newDefaultProp);
    return newDefaultProp;


}*/



/*void Scene::removeModels(QList<QSharedPointer<ModelItem> > models)
{
    QList<QSharedPointer<ModelItem> >::iterator it;
    for (it = models.begin(); it != models.end(); it++)
        removeModel((*it)->id());
}*/
