////////////////////////////////////////////////////////////////////////////////
/// @brief avocado server
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2010-2011 triagens GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Copyright 2011, triagens GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_AVOCADODB_RESTHANDLER_REST_ACTION_HANDLER45_H
#define TRIAGENS_AVOCADODB_RESTHANDLER_REST_ACTION_HANDLER45_H 1

#include <Rest/AnyServer.h>

#include <Admin/ApplicationAdminServer.h>
#include <Rest/ApplicationHttpServer.h>
#include <VocBase/vocbase.h>

// -----------------------------------------------------------------------------
// --SECTION--                                               class AvocadoServer
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AvocadoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

namespace triagens {
  namespace avocado {

////////////////////////////////////////////////////////////////////////////////
/// @brief AvocadoDB server
////////////////////////////////////////////////////////////////////////////////

  class AvocadoServer : public rest::AnyServer {
      private:
        AvocadoServer (const AvocadoServer&);
        AvocadoServer& operator= (const AvocadoServer&);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AvocadoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief UnviversalVoc constructor
////////////////////////////////////////////////////////////////////////////////

        AvocadoServer (int argc, char** argv);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 AnyServer methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AvocadoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        void buildApplicationServer ();

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        int startupServer ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                   private methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AvocadoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief executes the shell
////////////////////////////////////////////////////////////////////////////////

        void executeShell ();

////////////////////////////////////////////////////////////////////////////////
/// @brief opens the database
////////////////////////////////////////////////////////////////////////////////

        void openDatabase ();

////////////////////////////////////////////////////////////////////////////////
/// @brief closes the database
////////////////////////////////////////////////////////////////////////////////

        void closeDatabase ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AvocadoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief number of command line arguments
////////////////////////////////////////////////////////////////////////////////

        int _argc;

////////////////////////////////////////////////////////////////////////////////
/// @brief command line arguments
////////////////////////////////////////////////////////////////////////////////

        char** _argv;

////////////////////////////////////////////////////////////////////////////////
/// @brief constructed admin server application
////////////////////////////////////////////////////////////////////////////////

        admin::ApplicationAdminServer* _applicationAdminServer;

////////////////////////////////////////////////////////////////////////////////
/// @brief constructed http server application
////////////////////////////////////////////////////////////////////////////////

        rest::ApplicationHttpServer* _applicationHttpServer;

////////////////////////////////////////////////////////////////////////////////
/// @brief constructed http server
////////////////////////////////////////////////////////////////////////////////

        rest::HttpServer* _httpServer;

////////////////////////////////////////////////////////////////////////////////
/// @brief list port for client requests
////////////////////////////////////////////////////////////////////////////////

        string _httpPort;

////////////////////////////////////////////////////////////////////////////////
/// @brief number of dispatcher threads
////////////////////////////////////////////////////////////////////////////////

        int _dispatcherThreads;

////////////////////////////////////////////////////////////////////////////////
/// @brief path to the directory containing the startup scripts
////////////////////////////////////////////////////////////////////////////////

        string _startupPath;

////////////////////////////////////////////////////////////////////////////////
/// @brief path to the action directory
////////////////////////////////////////////////////////////////////////////////

        string _actionPath;

////////////////////////////////////////////////////////////////////////////////
/// @brief number of action threads
////////////////////////////////////////////////////////////////////////////////

        int _actionThreads;

////////////////////////////////////////////////////////////////////////////////
/// @brief path to the database
///
/// @CMDOPT{--database.directory @CA{directory}}
///
/// The directory containing the collections and data-files. Defaults
/// to @CODE{/var/lib/avocado}.
////////////////////////////////////////////////////////////////////////////////

        string _databasePath;

////////////////////////////////////////////////////////////////////////////////
/// @brief vocbase
////////////////////////////////////////////////////////////////////////////////

        TRI_vocbase_t* _vocbase;
    };
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#endif

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\)"
// End:
