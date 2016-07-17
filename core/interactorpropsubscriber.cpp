#include "interactorpropsubscriber.h"
#include <QMapIterator>
#include <QListIterator>
#include <QDebug>
#include "viewitem.h"
#include "IViewEventObserver.h"

InteractorPropSubscriber::InteractorPropSubscriber()
{
    QMap<vtkSmartPointer<vtkProp3D>, QSharedPointer<ViewItem> >::iterator it = m_actorBasedObservers.begin();
    for(;it != m_actorBasedObservers.end(); ++it) {
        it.value().clear();
    }
}

void InteractorPropSubscriber::broadcastEvent(vtkProp3D *actor, vtkSmartPointer<vtkCellPicker> picker, InteractorPropSubscriber::EventType e)
{
    //Notify Obervers that subscribed to all pick events that actually match a actor
    QList < QSharedPointer<ViewItem> >::iterator itl;
    if (!m_actorBasedObservers.contains(actor))
    {
        return;
    }
    for (itl = m_allEventsObservers.begin(); itl != m_allEventsObservers.end(); itl++)
    {
        switch (e)
        {
        case InteractorPropSubscriber::EventType::PICK:
            (*itl)->cellPickingEvent(actor, picker);
            break;
        case InteractorPropSubscriber::EventType::PAN:
           (*itl)->panEvent(actor, picker);
            break;
        }

    }

    if (actor)
    {

        QMapIterator<vtkSmartPointer<vtkProp3D>, QSharedPointer<ViewItem> > it(m_actorBasedObservers);
        while (it.hasNext()) {
            it.next();
            if (it.key().GetPointer() == actor)
            {
                QUuid modelId;
                switch (e)
                {
                case InteractorPropSubscriber::EventType::PICK:
                    it.value()->cellPickingEvent(actor,picker);
                    modelId = it.value()->model()->id();
                    if (m_customObservers.find(modelId) != m_customObservers.end())
                    {
                        QList<IViewEventObserver*> observers = m_customObservers.values(modelId);
                        for (int i = 0; i < observers.size(); ++i)
                            observers[i]->cellPickingEvent(actor,picker);
                    }
                    break;
                case InteractorPropSubscriber::EventType::PAN:
                    it.value()->panEvent(actor,picker);
                    modelId = it.value()->model()->id();
                    if (m_customObservers.find(modelId) != m_customObservers.end())
                    {
                        QList<IViewEventObserver*> observers = m_customObservers.values(modelId);
                        for (int i = 0; i < observers.size(); ++i)
                            observers[i]->panEvent(actor,picker);
                    }
                    break;
                case InteractorPropSubscriber::EventType::MOUSEMOVE:
                    //it.value()->panEvent(actor,picker); MouseMove event only broadcasted to custom obs to avoid
                    //un necessary traffic
                    modelId = it.value()->model()->id();
                    if (m_customObservers.find(modelId) != m_customObservers.end())
                    {
                        QList<IViewEventObserver*> observers = m_customObservers.values(modelId);
                        for (int i = 0; i < observers.size(); ++i)
                            observers[i]->mouseMoveEvent(actor,picker);
                    }
                    break;
                }
            }
        }
    }
}


void InteractorPropSubscriber::addEventObserver(vtkSmartPointer<vtkProp3D> actor, QSharedPointer<ViewItem> obs)
{
    m_actorBasedObservers[actor] = obs;
}

void InteractorPropSubscriber::addEventObserver(QSharedPointer<ViewItem> obs)
{
    m_allEventsObservers.push_back(obs);
}

void InteractorPropSubscriber::addEventObserver(IViewEventObserver *obs, QUuid modelId)
{
    //@todo prevent adding save model/eventobserver pair more than one.
    m_customObservers.insert(modelId, obs);
}


void InteractorPropSubscriber::removeObserver(QSharedPointer<ViewItem> obs)
{
    QMap<vtkSmartPointer<vtkProp3D>, QSharedPointer<ViewItem> >::iterator it = m_actorBasedObservers.begin();

    while (it != m_actorBasedObservers.end()) {
        QSharedPointer<ViewItem> tmpObs = it.value();
        if (tmpObs == obs)
        {
            it = m_actorBasedObservers.erase(it);
        }
        else
            it++;
    }

    QList< QSharedPointer<ViewItem> >::iterator itl = m_allEventsObservers.begin();

    while (itl != m_allEventsObservers.end()) {
        if (*itl == obs)
        {
            itl = m_allEventsObservers.erase(itl);
        }
        else
            itl++;
    }

}


void InteractorPropSubscriber::removeEventObserver(IViewEventObserver *obs)
{
    QMap<QUuid, IViewEventObserver* >::iterator it = m_customObservers.begin();
    while (it != m_customObservers.end()) {
        IViewEventObserver *tmpObs = it.value();
        if (tmpObs->id() == obs->id())
        {
            it = m_customObservers.erase(it);
        }
        else
            it++;
    }

}

