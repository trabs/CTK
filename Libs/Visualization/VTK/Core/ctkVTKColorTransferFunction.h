/*=========================================================================

  Library:   CTK
 
  Copyright (c) 2010  Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.commontk.org/LICENSE

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 
=========================================================================*/

#ifndef __ctkVTKColorTransferFunction_h
#define __ctkVTKColorTransferFunction_h

// CTK includes
#include "ctkTransferFunction.h"
#include "ctkPimpl.h"
#include "CTKVisualizationVTKCoreExport.h"
#include "ctkVTKObject.h"

class vtkColorTransferFunction;
class ctkVTKColorTransferFunctionPrivate;

///
/// Transfer function for a vtkColorTransferFunction. 
/// The value is an RGB QColor (no alpha supported)
class CTK_VISUALIZATION_VTK_CORE_EXPORT ctkVTKColorTransferFunction: public ctkTransferFunction
{
  Q_OBJECT;
  QVTK_OBJECT;
public:
  ctkVTKColorTransferFunction(QObject* parent = 0);
  ctkVTKColorTransferFunction(vtkColorTransferFunction* colorTransferFunction, 
                              QObject* parent = 0);
  virtual ~ctkVTKColorTransferFunction();
  
  virtual ctkControlPoint* controlPoint(int index)const;
  virtual QVariant value(qreal pos)const;
  virtual int count()const;
  virtual bool isDiscrete()const;
  virtual bool isEditable()const;

  virtual void range(qreal& minRange, qreal& maxRange)const;
  virtual QVariant minValue()const;
  virtual QVariant maxValue()const;

  virtual int insertControlPoint(const ctkControlPoint& cp);
  virtual int insertControlPoint(qreal pos);

  virtual void setControlPointPos(int index, qreal pos);
  virtual void setControlPointValue(int index, const QVariant& value);
  
  virtual void removeControlPoint( qreal pos );

  void setColorTransferFunction(vtkColorTransferFunction* colorTransferFunction);
  vtkColorTransferFunction* colorTransferFunction()const;
private:
  CTK_DECLARE_PRIVATE(ctkVTKColorTransferFunction);
};

#endif
