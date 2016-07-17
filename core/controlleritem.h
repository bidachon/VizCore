#ifndef CONTROLLERITEM_H
#define CONTROLLERITEM_H

#include <QObject>
#include "view.h"

/**
 * @brief The ControllerItem class
 * Provides a abstraction for user interaction between the ViewItem and the Model.
 * An concrete implementation is the AnatomicLandmarkController.
 */
class ControllerItem : public QObject
{
    Q_OBJECT
public:
    explicit ControllerItem();
    virtual ~ControllerItem();
    void setView(QSharedPointer<View> v);
    void setModel(QSharedPointer<ModelItem> model);
    virtual void setEnabled(bool) = 0;

signals:


protected:
    virtual void doModelUpdated() = 0;
    virtual void doViewUpdated() = 0;

protected slots:

    virtual void doUpdated() = 0;
    virtual void doPositionUpdated(const PositionUpdate &pu) = 0;
    virtual void doVisibilityChanged(bool v) = 0;
    virtual void doPropertyUpdated(QSharedPointer<PropertyList> prop) = 0;
    virtual void doSelectionChanged(bool s) {}
    virtual void doIsSelectableChanged(bool s) {}

protected:
    QWeakPointer<View> m_view;
    QSharedPointer<ModelItem> m_model;
};

#endif // CONTROLLERITEM_H
