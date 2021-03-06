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

#ifndef __ctkDirectoryButton_h
#define __ctkDirectoryButton_h

// Qt includes
#include <QDir>
#include <QFileDialog>

// CTK includes
#include <ctkPimpl.h>
#include "CTKWidgetsExport.h"
class ctkDirectoryButtonPrivate;

class CTK_WIDGETS_EXPORT ctkDirectoryButton: public QWidget
{
  Q_OBJECT
  Q_PROPERTY(QString directory READ directory WRITE setDirectory)
  Q_PROPERTY(QString caption READ caption WRITE setCaption)
  Q_PROPERTY(Options options READ options WRITE setOptions)
  // QFileDialog::Options is not a meta-type, we need to create our own.
  Q_FLAGS(Option Options);

public:
  // QFileDialog::Options is not a meta-type, we need to create our own.
  enum Option
    {
      ShowDirsOnly          = 0x00000001,
      DontResolveSymlinks   = 0x00000002,
      DontConfirmOverwrite  = 0x00000004,
      DontUseSheet          = 0x00000008,
      DontUseNativeDialog   = 0x00000010,
      ReadOnly              = 0x00000020,
      HideNameFilterDetails = 0x00000040
    };
  Q_DECLARE_FLAGS(Options, Option)

  ctkDirectoryButton(QWidget * parent = 0);
  ctkDirectoryButton(const QString& directory, QWidget * parent = 0);
  ctkDirectoryButton(const QIcon& icon, const QString& directory, QWidget * parent = 0);

  void setDirectory(const QString& directory);
  QString directory()const;

  ///
  /// The title of the file dialog used to select a new directory
  /// If caption is not set, internally use QWidget::tooltip()
  void setCaption(const QString& caption);
  const QString& caption()const;

  void setOptions(const Options& options);
  const Options& options()const;

public slots:
  void browse();

signals:
  ///
  /// directoryChanged is emitted when the current directory changes
  ///if you want a directoryChanged signal as a utility. Feel free to add it
  void directoryChanged(const QString&);
  void directorySelected(const QString&);
private:
  CTK_DECLARE_PRIVATE(ctkDirectoryButton);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ctkDirectoryButton::Options);

#endif
