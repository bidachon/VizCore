#ifndef DISPLAYOBJECT
#define DISPLAYOBJECT


#include <QSharedPointer>
#include <QObject>
#include <QMetaType>
#include <vtkProp.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkCellPicker.h>

#include "modelitem.h"

class View;

class ViewItem : public QObject
{
    Q_OBJECT
public:
    ViewItem(QSharedPointer<ModelItem> model);


    virtual ~ViewItem();

    void setRenderer(vtkSmartPointer<vtkRenderer> renderer)
    {
        m_ren = renderer;
        initActors();
    }

    virtual void cellPickingEvent(vtkSmartPointer<vtkProp3D>, vtkSmartPointer<vtkCellPicker>) {}
    virtual void panEvent(vtkSmartPointer<vtkProp3D>, vtkSmartPointer<vtkCellPicker>) {}

    /**
     * @brief setWorldTransform base transform to be applied to all Actors.
     * ViewTem makes a DeepCopy of the given transform first.
     */
    void setWorldTransform(vtkSmartPointer<vtkTransform>);

    ModelItem::RenderLayer layer() const {return m_model->layer();}

    QSharedPointer<ModelItem> model() const {return m_model;}
    void setView(View* v) {m_view = v;}

protected slots:
    virtual void doUpdated() = 0;
    virtual void doPositionUpdated(const PositionUpdate &pu) = 0;
    virtual void doVisibilityChanged(bool v) = 0;
    virtual void doPropertyUpdated(QSharedPointer<PropertyList> prop) = 0;
    virtual void doWorldTransformChanged() {}
    virtual void doSelectionChanged(bool s) {}
    virtual void doIsSelectableChanged(bool s) {}



protected:

    virtual void initActors() = 0;

    QSharedPointer<ModelItem> m_model;
    View *m_view;
    vtkSmartPointer<vtkRenderer> m_ren;
    vtkSmartPointer<vtkTransform> m_worldXfm;

private:


};

#endif // DISPLAYOBJECT

