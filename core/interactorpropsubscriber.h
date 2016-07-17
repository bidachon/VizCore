#ifndef INTERACTORPROPSUBSCRIBER_H
#define INTERACTORPROPSUBSCRIBER_H

#include <QSharedPointer>
#include "IViewInteractorStyle.h"
#include <QMap>
#include <QList>

class ViewItem;
class IViewEventObserver;

class InteractorPropSubscriber : public IInteractorPropSubscriber
{
public:

    InteractorPropSubscriber();
    virtual ~InteractorPropSubscriber(){}


    void addEventObserver(vtkSmartPointer<vtkProp3D> actor, QSharedPointer<ViewItem> obs);
    void addEventObserver(QSharedPointer<ViewItem> obs);
    void addEventObserver(IViewEventObserver *obs, QUuid modelId);

    void removeObserver(QSharedPointer<ViewItem> obs);
    void removeEventObserver(IViewEventObserver *obs);
    void broadcastEvent(vtkProp3D *actor, vtkSmartPointer<vtkCellPicker> picker,InteractorPropSubscriber::EventType e);

private:
    QList<QSharedPointer<ViewItem> > m_allEventsObservers;
    QMap<vtkSmartPointer<vtkProp3D>, QSharedPointer<ViewItem> > m_actorBasedObservers;
    QMultiMap<QUuid, IViewEventObserver*> m_customObservers;
};

#endif // INTERACTORPROPSUBSCRIBER_H
