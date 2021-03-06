
// Qt includes
#include <QDebug>
#include <QApplication>

// CTKVTK includes
#include "ctkVTKObjectTestHelper.h"

// VTK includes
#include <vtkObject.h>

//------------------------------------------------------------------------------
class ctkVTKObjectTestPrivate : public ctkPrivate<ctkVTKObjectTest>
{
public:
  ctkVTKObjectTestPrivate();
  int PublicSlotCalled ;
  int ProtectedSlotCalled;
  int PrivateSlotCalled;
};

//------------------------------------------------------------------------------
ctkVTKObjectTestPrivate::ctkVTKObjectTestPrivate()
{
  this->PublicSlotCalled = 0;
  this->ProtectedSlotCalled = 0;
  this->PrivateSlotCalled = 0;
}

//------------------------------------------------------------------------------
ctkVTKObjectTest::ctkVTKObjectTest()
{
  CTK_INIT_PRIVATE(ctkVTKObjectTest);
}

//------------------------------------------------------------------------------
bool ctkVTKObjectTest::test()
{
  CTK_D(ctkVTKObjectTest);
  // should do nothing but shouldn't fail neither
  qDebug() << "The following can generate error message.";
  qDebug() << "Disconnect:";
  this->qvtkDisconnect(0, static_cast<unsigned long>(-1), this, SLOT(onVTKObjectModifiedPublic()));
  qDebug() << "Connect:";
  QString connection = this->qvtkConnect(0, static_cast<unsigned long>(-1), this, SLOT(onVTKObjectModifiedPublic()));
  if (!connection.isEmpty())
    {
    qDebug() << "ctkVTKObject::qvtkConnect() failed: "<< connection;
    return false;
    }
  qDebug() << "Reconnect:";
  connection = this->qvtkReconnect(0, 0, static_cast<unsigned long>(-1), this, SLOT(onVTKObjectModifiedPublic()));
  if (!connection.isEmpty())
    {
    qDebug() << "ctkVTKObject::qvtkReconnect() failed: "<< connection;
    return false;
    }
  qDebug() << "End of possible error messages.";
  
  vtkObject* object = vtkObject::New();

  connection = this->qvtkConnect(object, vtkCommand::ModifiedEvent, 
                                 this, SLOT(onVTKObjectModifiedPublic()));
  if (connection.isEmpty() || object->GetReferenceCount() != 1)
    {
    qDebug() << "ctkVTKObject::qvtkConnect() failed: "<< connection;
    return false;
    }

  object->Modified();

  if (d->PublicSlotCalled != 1)
    {
    qDebug() << "qvtkConnect failed";
    return false;
    }
  
  this->resetSlotCalls();
  // should do nothing...
  connection = this->qvtkConnect(object, vtkCommand::ModifiedEvent, 
                                 this, SLOT(onVTKObjectModifiedPublic()));
  if (!connection.isEmpty())
    {
    qDebug() << __LINE__ << "ctkVTKObject::qvtkConnect() failed: "<< connection;
    return false;
    }

  object->Modified();
  
  if (d->PublicSlotCalled != 1)
    {
    qDebug() << __LINE__ << "qvtkConnect failed";
    return false;
    }

  this->resetSlotCalls();

  this->qvtkDisconnect(object, vtkCommand::WarningEvent, 
                       this, SLOT(onVTKObjectModifiedPublic()));
  object->Modified();

  if (d->PublicSlotCalled != 1)
    {
    qDebug() << __LINE__ << "qvtkDisconnect failed" << d->PublicSlotCalled;
    return false;
    }
  this->resetSlotCalls();

  this->qvtkDisconnect(object, vtkCommand::ModifiedEvent, 
                       this, SLOT(onVTKObjectModifiedPublic()));
  QCoreApplication::instance()->processEvents();
  object->Modified();

  if (d->PublicSlotCalled != 0)
    {
    qDebug() << __LINE__ << "qvtkDisconnect failed" << d->PublicSlotCalled;
    return false;
    }
  this->resetSlotCalls();
  
  // Set a new connection (protected)
  connection = this->qvtkConnect(object, vtkCommand::ModifiedEvent, 
                                 this, SLOT( onVTKObjectModifiedProtected ( ) ));
  if (connection.isEmpty())
    {
    qDebug() << __LINE__ << "ctkVTKObject::qvtkConnect() failed: "<< connection;
    return false;
    }

  object->Modified();
  
  if (d->ProtectedSlotCalled != 1)
    {
    qDebug() << __LINE__ << "ctkVTKObject::qvtkConnect failed" << d->ProtectedSlotCalled;
    return false;
    }
  this->resetSlotCalls();

  // remove the connection using flags, 0 means any event, qt object or slot
  this->qvtkDisconnect(object, vtkCommand::NoEvent, 0, 0);
  object->Modified();

  if (d->ProtectedSlotCalled != 0)
    {
    qDebug() << __LINE__ << "qvtkDisconnect failed" << d->ProtectedSlotCalled;
    return false;
    }
  this->resetSlotCalls();

  // Set new connections
  this->qvtkConnect(object, vtkCommand::ModifiedEvent, 
                    this, SLOT(onVTKObjectModifiedProtected()));
  this->qvtkConnect(object, vtkCommand::ModifiedEvent, 
                    this, SLOT(onVTKObjectModifiedPrivate()));
  object->Modified();
  if (d->ProtectedSlotCalled != 1 || 
      d->PrivateSlotCalled != 1)
    {
    qDebug() << __LINE__ << "qvtkConnect failed" 
             << d->ProtectedSlotCalled 
             << d->PrivateSlotCalled;
    return false;
    }
  this->resetSlotCalls();

  // remove the connection using flags, 0 means any event, qt object or slot
  this->qvtkDisconnect(object, vtkCommand::ModifiedEvent, this, 0);
  object->Modified();
  if (d->ProtectedSlotCalled != 0 || d->PrivateSlotCalled != 0)
    {
    qDebug() << __LINE__ << "qvtkDisconnect failed" 
             << d->ProtectedSlotCalled
             << d->PrivateSlotCalled;
    return false;
    }
  this->resetSlotCalls();

  // Set new connections
  this->qvtkConnect(object, vtkCommand::ModifiedEvent, 
                    this, SLOT(onVTKObjectModifiedPublic ()));
  this->qvtkConnect(object, vtkCommand::WarningEvent, 
                    this, SLOT(onVTKObjectModifiedPublic( )));
  int disconnected = this->qvtkDisconnect(object, vtkCommand::NoEvent, 
                                          this, SLOT(onVTKObjectModifiedPublic() ));
  if (disconnected != 2)
    {
    qDebug() << __LINE__ << "qvtkDisconnect failed" << disconnected;
    return false;
    }

  object->InvokeEvent(vtkCommand::ModifiedEvent, 0);
  object->InvokeEvent(vtkCommand::WarningEvent, 0);

  if (d->PublicSlotCalled != 0)
    {
    qDebug() << __LINE__ << "qvtkConnect failed" 
             << d->PublicSlotCalled;
    return false;
    }
  this->resetSlotCalls();

  disconnected = this->qvtkDisconnectAll();
  if (disconnected != 0)
    {
    qDebug() << __LINE__ << "qvtkDisconnectAll failed" << disconnected;
    return false;
    }
  this->qvtkConnect(object, vtkCommand::ModifiedEvent, 
                    this, SLOT(deleteConnection()));
  object->InvokeEvent(vtkCommand::ModifiedEvent, 0);

  object->Delete();
  
  return true;
}

//------------------------------------------------------------------------------
void ctkVTKObjectTest::resetSlotCalls()
{
  CTK_D(ctkVTKObjectTest);
  d->PublicSlotCalled = 0;
  d->ProtectedSlotCalled = 0;
  d->PrivateSlotCalled = 0;
}

//------------------------------------------------------------------------------
void ctkVTKObjectTest::onVTKObjectModifiedPublic()
{
  qDebug() << __FUNCTION__;
  ctk_d()->PublicSlotCalled = true;
}

//------------------------------------------------------------------------------
void ctkVTKObjectTest::deleteConnection()
{
  qDebug() << __FUNCTION__;
  this->qvtkDisconnect(0, vtkCommand::NoEvent, 0, 0);
}

//------------------------------------------------------------------------------
void ctkVTKObjectTest::onVTKObjectModifiedProtected()
{
  qDebug() << __FUNCTION__;
  ctk_d()->ProtectedSlotCalled = true;
}

//------------------------------------------------------------------------------
void ctkVTKObjectTest::onVTKObjectModifiedPrivate()
{
  qDebug() << __FUNCTION__;
  ctk_d()->PrivateSlotCalled = true;
}
