#ifndef MYVIEWITEM_H
#define MYVIEWITEM_H


#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkPropPicker.h>
#include "IModelViewFactory.h"
#include "viewitem.h"
#include "commonMacros.h"

class MyViewItem : public ViewItem
{
    Q_OBJECT
public:

    MyViewItem(QSharedPointer<ModelItem> model):ViewItem(model){}

    void pick(double x, double y)
    {
        vtkSmartPointer<vtkPropPicker>  picker = vtkSmartPointer<vtkPropPicker>::New();
        picker->Pick(0, 0.0521216, 0.488104, m_ren);
    }

    virtual void cellPickingEvent(vtkSmartPointer<vtkProp3D>, vtkSmartPointer<vtkCellPicker> picker) {
        double *pos = picker->GetPickPosition();
    }


signals:
    void testDoUpdated();
    void testDoPositionUpdated(const PositionUpdate &pu);
    void testDoVisibilityChanged(bool v);
    void testDoPropertyChanged(int, bool, QString);
    void testDoSelectableChanged(bool);
    void testDoSelectedChanged(bool);

protected slots:


    virtual void doUpdated(){emit testDoUpdated();}
    virtual void doPositionUpdated(const PositionUpdate &pu){emit testDoPositionUpdated(pu);}
    virtual void doVisibilityChanged(bool v){emit testDoVisibilityChanged(v);}
    virtual void doPropertyUpdated(QSharedPointer<PropertyList> prop) {
        QSharedPointer<MyPropertyList> myProp = prop.dynamicCast<MyPropertyList>();
        Q_ASSERT(myProp);
        int i = myProp->myIntProperty();
        bool b = myProp->myBoolProperty();
        QString str = myProp->myStringProperty();
        emit testDoPropertyChanged(i,b,str);
    }
    virtual void doSelectionChanged(bool s) {emit testDoSelectedChanged(s);}
    void doIsSelectableChanged(bool s)
    {
        emit testDoSelectableChanged(s);
        if (!m_view)
            return;

        m_sphereActor->SetPickable(s);
        if (s)
        {
            m_view->addEventObserver(m_sphereActor, m_model->id());//listen to all picking events for all actors
            pick(0,0);
        }
        else
        {
            m_view->removeEventObserver(m_model->id());
        }


        return;
    }


protected:
    virtual void initActors() {
        m_sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        m_sphereSource->SetThetaResolution(8);
        m_sphereSource->SetPhiResolution(8);

        m_sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        m_sphereMapper->SetInputConnection(m_sphereSource->GetOutputPort());
        m_sphereActor = vtkSmartPointer<vtkActor>::New();
        m_sphereActor->SetMapper(m_sphereMapper);
        m_ren->AddActor(m_sphereActor);
    }

private:
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkSphereSource> m_sphereSource;
    vtkSmartPointer<vtkPolyDataMapper> m_sphereMapper;
    vtkSmartPointer<vtkActor> m_sphereActor;

};

class MyFactory : public IViewItemFactory
{
public:
    virtual QSharedPointer<ViewItem> createViewItem(QSharedPointer<ModelItem> model) const
    {
        QSharedPointer<ViewItem> myViewItem;
        QSharedPointer<MyModelItem> myModel = qSharedPointerDynamicCast<MyModelItem>(model);
        if (myModel)
            myViewItem = QSharedPointer<ViewItem>(new MyViewItem(myModel));

        return myViewItem;
    }
};


#endif // MYVIEWITEM_H
