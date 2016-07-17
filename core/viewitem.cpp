#include "viewitem.h"
#include "view.h"

ViewItem::ViewItem(QSharedPointer<ModelItem> model)
{
    m_model = model;
    m_view  = NULL;
    connect(&(m_model->events()),SIGNAL(updated()),this,SLOT(doUpdated()));
    connect(&(m_model->events()),SIGNAL(visibilityChanged(bool)),this,SLOT(doVisibilityChanged(bool)));
    connect(&(m_model->events()),SIGNAL(positionUpdated(const PositionUpdate&)),this,SLOT(doPositionUpdated(const PositionUpdate&)));
    connect(&(m_model->events()),SIGNAL(propertyUpdated(QSharedPointer<PropertyList>)),this,SLOT(doPropertyUpdated(QSharedPointer<PropertyList>)));
    connect(&(m_model->events()),SIGNAL(selectionChanged(bool)),this,SLOT(doSelectionChanged(bool)));
    connect(&(m_model->events()),SIGNAL(isSelectableChanged(bool)),this,SLOT(doIsSelectableChanged(bool)));
    m_worldXfm = vtkSmartPointer<vtkTransform>::New();

}


ViewItem::~ViewItem()
{
    this->disconnect(&(m_model->events()),0,this,0);
}

void ViewItem::setWorldTransform(vtkSmartPointer<vtkTransform> xfm)
{
    m_worldXfm->DeepCopy(xfm.GetPointer());
}

