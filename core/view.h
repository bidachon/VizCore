#ifndef VIEW_H
#define VIEW_H

#include "IModelViewFactory.h"
#include "modelitem.h"
#include "scene.h"

#include "pickinginteractorstyle.h"

#include "interactorpropsubscriber.h"

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkTransform.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>


#include <QVTKWidget.h>
#include <QSharedPointer>
#include <QUuid>

#include <QList>
#include <map>
#include <QTimer>


class View: public QObject
{
    Q_OBJECT
public:

    View();
    virtual ~View();
    void addFactory(QSharedPointer<IViewItemFactory> factory);
    QVTKWidget * qvtkwidget() const;
    void setScene(QSharedPointer<Scene> scene);
    void removeScene();

    void takeScreenshot(const QString &filename);

    void setCamera(vtkSmartPointer<vtkCamera> cam);
    vtkSmartPointer<vtkCamera> camera(){return m_camera;}

    /**
     * @brief setUpdateRate set the vtkRenderWindow update rate.
     * @param fps frame per second
     */
    void setUpdateRate(int fps);

    /**
     * @brief setWorldTransform set a transform the current View.
     * This transform will be applied to all current ViewItem and to all ViewItem to be added.
     */
    void setWorldTransform(vtkSmartPointer<vtkTransform>);

    /**
     * @brief addEventObserver
     * @param obs
     * @param id
     */
    void addEventObserver(IViewEventObserver *obs, QUuid id);
    /**
     * @brief addEventObserver registers a ViewItem for interaction events.
     * @param self the id of the ModelItem belonging to that ViewItem.
     * the id() is used to identify the IViewEventObservers among the ViewItems.
     */
    void addEventObserver(QUuid self);

    /**
     * @brief addEventObserver
     * @param actor for which we want to receive events. Only events for that specific
     * actor will be transmitted.
     * @param uid of the id of the ModelItem belonging to that ViewItem.
     */
    void addEventObserver(vtkSmartPointer<vtkProp3D> actor, QUuid uid);

    void removeEventObserver(QUuid self);

    void removeEventObserver(IViewEventObserver *obs);

    void setInteractorStyle(QSharedPointer<IViewInteractorStyle> style);

private slots:
    void createViewItem(QUuid);
    void deleteViewItem(QUuid);
    void update();

private:



    int m_fps;
    QTimer *m_timer;
    QVTKWidget* m_qvtkwidget;
    QSharedPointer<Scene> m_scene;
    std::map<ModelItem::RenderLayer, vtkSmartPointer<vtkRenderer> > m_renderers;
    QList<QSharedPointer<IViewItemFactory> > m_factories;
    QMap< QUuid , QSharedPointer<ViewItem> > m_viewItems;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkTransform> m_worldXfm;
    QSharedPointer<IViewInteractorStyle> m_interatorStyle;
    QSharedPointer<InteractorPropSubscriber> m_subscriber;
    vtkSmartPointer<vtkCamera> m_camera;





};

#endif // VIEW_H
