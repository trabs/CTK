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

// Qt includes
#include <QApplication>
#include <QTextStream>

// CTK includes
#include <ctkDICOMQuery.h>
#include <ctkDICOM.h>
#include "ctkLogger.h"

// STD includes
#include <cstdlib>
#include <iostream>
#include <fstream>

void print_usage()
{
  std::cerr << "Usage:\n";
  std::cerr << "  ctkDICOMQuery database callingAETitle calledAETitle host port\n";
  return;
}


/**
  *
*/
int main(int argc, char** argv)
{
  ctkLogger::configure();
  ctkLogger logger ( "org.commontk.core.Logger" );
  logger.setDebug();

  if (argc < 5)
  {
    print_usage();
    return EXIT_FAILURE;
  }

  QApplication app(argc, argv);
  QTextStream out(stdout);

  ctkDICOM myCTK;
  logger.debug ( "Opening database " + QString ( argv[1] ) );
  myCTK.openDatabase ( argv[1] );
  logger.debug ( "Last error: " + myCTK.GetLastError() );
  if ( myCTK.database().isOpen() ) 
    {
    logger.debug ( "Database is open" );
    }
  else
    {
    logger.debug ( "Database is not open" );
    }

  

  ctkDICOMQuery query;
  query.setCallingAETitle ( QString ( argv[2] ) );
  query.setCalledAETitle ( QString ( argv[3] ) );
  query.setHost ( QString ( argv[4] ) );
  int port;
  bool ok;
  port = QString ( argv[5] ).toInt ( &ok );
  if ( !ok )
    {
    std::cerr << "Could not convert " << argv[5] << " to an integer" << std::endl;
    print_usage();
    return EXIT_FAILURE;
    }
  query.setPort ( port );

  try
    {
    query.query ( myCTK.database() );
    }
  catch (std::exception e)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
