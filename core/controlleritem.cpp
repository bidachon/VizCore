#include "controlleritem.h"

ControllerItem::ControllerItem() :
    QObject()
{
}

ControllerItem::~ControllerItem()
{
}

void ControllerItem::setView(QSharedPointer<View> view)
{
    m_view = view.toWeakRef();
    doViewUpdated();
}

void ControllerItem::setModel(QSharedPointer<ModelItem> model)
{
    if (m_model)
    {
        disconnect(&(m_model->events()),0,this,0);
    }
    m_model = model;
    if (!m_model)
        return;
    bool rslt = true;
    rslt &= connect(&(m_model->events()),SIGNAL(updated()),this,SLOT(doUpdated()));
    rslt &= connect(&(m_model->events()),SIGNAL(visibilityChanged(bool)),this,SLOT(doVisibilityChanged(bool)));
    rslt &= connect(&(m_model->events()),SIGNAL(positionUpdated(const PositionUpdate&)),this,SLOT(doPositionUpdated(const PositionUpdate&)));
    rslt &= connect(&(m_model->events()),SIGNAL(propertyUpdated(QSharedPointer<PropertyList>)),this,SLOT(doPropertyUpdated(QSharedPointer<PropertyList>)));
    rslt &= connect(&(m_model->events()),SIGNAL(selectionChanged(bool)),this,SLOT(doSelectionChanged(bool)));
    rslt &= connect(&(m_model->events()),SIGNAL(isSelectableChanged(bool)),this,SLOT(doIsSelectableChanged(bool)));
    assert(rslt == true);
    doModelUpdated();
}
