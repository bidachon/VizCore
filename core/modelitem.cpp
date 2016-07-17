#include "modelitem.h"

void ModelEvents::broadcastUpdate()
{
    emit this->updated();
}

void ModelEvents::broadcastPositionUpdate(const PositionUpdate &pu)
{
    emit this->positionUpdated(pu);
}

void ModelEvents::broadcastVisibilityChange(bool v)
{
    emit this->visibilityChanged(v);
}

void ModelEvents::broadCastPropertyUpdate(QSharedPointer<PropertyList> prop)
{
    emit this->propertyUpdated(prop);
}

void ModelEvents::broadCastSelectionChanged(bool s)
{
    emit this->selectionChanged(s);
}

void ModelEvents::broadCastIsSelectableChanged(bool s)
{
    emit this->isSelectableChanged(s);
}

ModelItem::ModelItem():m_xfm(vtkSmartPointer<vtkTransform>::New()),m_layer(RenderLayer::REFERENCE)
{
    m_uuid = QUuid::createUuid();
    m_isVisible = true;
    m_selectable = false;
    m_selected = false;
}

ModelItem::~ModelItem()
{
}

void ModelItem::setProperties(QSharedPointer<PropertyList> prop)
{
    m_properties = prop;
    this->m_events.broadCastPropertyUpdate(prop);
}

void ModelItem::setParent(QSharedPointer<ModelItem> parent)
{
    QSharedPointer<ModelItem> currentParent = m_parent.toStrongRef();
    if (currentParent)
        disconnect(&(currentParent->events()),0,this,0);

    m_parent = parent;
    if (parent)
    {
        connect(&(parent->events()),SIGNAL(updated()),this,SLOT(doParentUpdated()));
        connect(&(parent->events()),SIGNAL(visibilityChanged(bool)),this,SLOT(doParentVisibilityChanged(bool)));
        connect(&(parent->events()),SIGNAL(positionUpdated(const PositionUpdate&)),this,SLOT(doParentPositionUpdated(const PositionUpdate&)));
        connect(&(parent->events()),SIGNAL(propertyUpdated(QSharedPointer<PropertyList>)),this,SLOT(doParentPropertyUpdated(QSharedPointer<PropertyList>)));
    }
}

QSharedPointer<PropertyList> ModelItem::properties()
{
    if (!m_properties)
        m_properties = this->defaultProperties();
    return m_properties;
}
