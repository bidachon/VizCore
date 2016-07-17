#ifndef MODEL_H
#define MODEL_H

#include <QUuid>
#include <QSharedPointer>
#include <QDebug>
#include <QMetaType>

#include <vtkTransform.h>
#include <vtkSmartPointer.h>
#include <assert.h>

/**
 * @brief The PropertyList class provide an abstract interface to store PropertyList.
 */
class PropertyList
{
public:
    PropertyList(){}
    virtual ~PropertyList(){}
    virtual void setDefault() = 0;
};


/**
 * Allows the use of QObject::connect(...) with QSharedPointer.
 * Qt connect() does not support passing either a QSharedPoitner or even QSharedPointer::data()
 */
template<class T> bool
sharedconnect(const QSharedPointer<T> &sender,
           const char *signal,
           const QObject *receiver,
           const char *method,
           Qt::ConnectionType type = Qt::AutoConnection)
{
    return QObject::connect(sender.data(), signal, receiver, method, type);
}


/**
 * @brief The PositionUpdate class is a wrapper around a vtkTransform to
 * support sending position updated through Qt signal/slot mechanism.
 * PositionUpdate is also declared as a meta-type using Q_DECLARE_METATYPE.
 */
class PositionUpdate
{
public:

    /** default constructor for the Q_DECLARE_TYPE macro */
    PositionUpdate()
        :m_transform(vtkTransform::New())
    {
    }

  PositionUpdate(vtkTransform* aTransform)
    : m_transform(vtkTransform::New())
    {
      assert(aTransform != NULL);
      m_transform->DeepCopy(aTransform);
    }

  PositionUpdate(vtkSmartPointer<vtkTransform> aTransform)
    : m_transform(vtkTransform::New())
    {
      assert(aTransform);
      m_transform->DeepCopy(aTransform.GetPointer());
    }

  /** copy constructor
   * qt signal will make a copy of PositionUPdate if the signal emitted is queued.
   * In this case, we keep the same vtkTransform, no need to create another one,
   * as a DeepCopy is done when createing the PositionUpdate for teh first time anyways.
  */
  PositionUpdate(const PositionUpdate &pu)
      :m_transform(pu.transform())
  {
  }

  virtual ~PositionUpdate() {}

  vtkSmartPointer<vtkTransform> transform() const { return m_transform; }

private:
  vtkSmartPointer<vtkTransform> m_transform;
};


Q_DECLARE_METATYPE(PositionUpdate)

Q_DECLARE_METATYPE(QSharedPointer<PropertyList>)

/**
 * @brief The ModelEvents class provide Qt signal/slot mechanism to the ModelItem class
 * without having ModelItem inherits from QObject. This is made available via the ModelItem interface to the ViewItem
 * paired object.
 */
class ModelEvents: public QObject
{
    Q_OBJECT
public:
    ModelEvents():QObject() {}

    virtual ~ModelEvents(){}

signals:
    /** in queued signal connection, Qt will call the copy constructor anyways(even with const&).
     * the Copy constructor of PositionUpdate will keep a reference to the same vtkMatrix in order to
     * avoid copying the matrix again.
     */
    void positionUpdated(const PositionUpdate&);
    void updated();
    void visibilityChanged(bool);
    void propertyUpdated(QSharedPointer<PropertyList>);
    void selectionChanged(bool);
    void isSelectableChanged(bool);

public:
    /** This is a helper method for qt 4.8
     * In qt 4.8 signals are protected, in qt 5, signals are public and thus can
     * be emitted from outside.
     */
    void broadcastUpdate();
    void broadcastPositionUpdate(const PositionUpdate &pu);
    void broadcastVisibilityChange(bool v);
    void broadCastPropertyUpdate(QSharedPointer<PropertyList> prop);
    void broadCastSelectionChanged(bool s);
    void broadCastIsSelectableChanged(bool p);
};

/**
 * @brief The ModelItem class is the base class for all model to be added to a scene.
 */
class ModelItem: public QObject
{
    Q_OBJECT
public:

    enum class RenderLayer {
        REFERENCE = 0,
        VOLUME,
        SLICE,
        UNDERLAY,
        MARKER,
        OVERLAY,
        ANNOTATION // should always be last
    };

    ModelItem();

    virtual ~ModelItem();

    /**
     * @brief events provides signals when the ModelItem changes:
     * - the position is updated
     * - the visibility is udpated
     * - the model simply emits updated()
     * @return
     */
    ModelEvents& events()
    {
        return this->m_events;
    }

    void setRenderLayer(RenderLayer layer)   { m_layer = layer; }

    RenderLayer layer() const  {  return m_layer; }

    /**
     * @brief setPosition updated the modelItem internal transform and then emits the
     * positionUpdated() signal of the ModelEvents
     * @param position
     */
    void setTransform(vtkSmartPointer<vtkTransform> xfm)
    {
        m_xfm->DeepCopy(xfm);
        this->doPositionUpdate();
    }

    vtkSmartPointer<vtkTransform> transform() {return m_xfm;}

    virtual void setVisible(bool v)
    {
        m_isVisible = v;
        emit m_events.broadcastVisibilityChange(v);
    }

    virtual bool isVisible() const {return m_isVisible;}

    QUuid id() const {return m_uuid;}

    bool isSelectable() const {return m_selectable;}

    void setSelectable(bool s)
    {
        m_selectable = s;
        emit m_events.broadCastIsSelectableChanged(s);        
    }

    bool isSelected() const {return m_selected;}

    void setSelected(bool s) {
        m_selected = s;
        emit m_events.broadCastSelectionChanged(s);
    }

    void setProperties(QSharedPointer<PropertyList> prop);

    QSharedPointer<PropertyList> properties();

    virtual QSharedPointer<PropertyList> defaultProperties() const = 0;

    void setParent(QSharedPointer<ModelItem> parent);

    QSharedPointer<ModelItem> parent() const { return m_parent;}

protected slots:
    virtual void doParentUpdated() {}
    virtual void doParentPositionUpdated(const PositionUpdate &pu) {}
    virtual void doParentVisibilityChanged(bool v) {}
    virtual void doParentPropertyUpdated(QSharedPointer<PropertyList> prop) {}

protected:
    ModelEvents m_events;
    vtkSmartPointer<vtkTransform> m_xfm;
    QWeakPointer<ModelItem> m_parent;
    QSharedPointer<PropertyList> m_properties;
    RenderLayer m_layer;
    bool m_isVisible;


    /**
     * @brief doPositionUpdate Default implementation just broadcast an update.
     * This can be overloaded if something has to be done with the new position.
     * In this case m_events.broadcastPositionUpdate(PositionUpdate(m_position))
     * must be called to signal the ViewItem of the position update.
     */
    virtual void doPositionUpdate() {m_events.broadcastPositionUpdate(PositionUpdate(m_xfm));}

private:

    QUuid m_uuid;
    bool m_selectable;
    bool m_selected;



};

#endif // MODEL_H

