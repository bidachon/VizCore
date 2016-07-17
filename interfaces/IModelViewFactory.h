#ifndef IMODELVIEWFACTORY_H
#define IMODELVIEWFACTORY_H

#include <QSharedPointer>
#include "modelitem.h"
#include "viewitem.h"

class IViewItemFactory
{
public:

    virtual ~IViewItemFactory() {}

    /**
     * @brief createViewItem
     * @param model
     * @return a new ViewItem or NULL if the factory does not deal with the give
     * Model.
     */
    virtual QSharedPointer<ViewItem> createViewItem(QSharedPointer<ModelItem> model) const = 0;
};

#endif // IMODELVIEWFACTORY_H
