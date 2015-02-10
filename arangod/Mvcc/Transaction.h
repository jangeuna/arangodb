////////////////////////////////////////////////////////////////////////////////
/// @brief MVCC transaction class
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2014 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
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
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Jan Steemann
/// @author Copyright 2015, ArangoDB GmbH, Cologne, Germany
/// @author Copyright 2011-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGODB_MVCC_TRANSACTION_H
#define ARANGODB_MVCC_TRANSACTION_H 1

#include "Basics/Common.h"
#include "Basics/FlagsType.h"
#include "Mvcc/CollectionStats.h"
#include "Mvcc/TransactionId.h"
#include "VocBase/voc-types.h"

struct TRI_vocbase_s;

namespace triagens {
  namespace mvcc {

    class TopLevelTransaction;
    class TransactionCollection;
    class TransactionManager;

// -----------------------------------------------------------------------------
// --SECTION--                                            struct TransactionInfo
// -----------------------------------------------------------------------------

    struct TransactionInfo {
      TransactionInfo (TransactionId const& id,
                       double startTime) 
        : own(id.own()),
          startTime(startTime) {
      }
      
      TransactionInfo (TransactionId::InternalType id,
                       double startTime) 
        : own(id),
          startTime(startTime) {
      }
      
      TransactionInfo (TransactionInfo const& other) 
        : own(other.own),
          startTime(other.startTime) {
      }
      
      TransactionInfo& operator= (TransactionInfo const& other) {
        own = other.own;
        startTime = other.startTime;
        return *this;
      }

      TransactionId::InternalType  own;
      double                       startTime;
    };

// -----------------------------------------------------------------------------
// --SECTION--                                                 class Transaction
// -----------------------------------------------------------------------------

    class Transaction {

      friend class TransactionManager;
      friend class LocalTransactionManager;
      friend class SubTransaction;

      public:

// -----------------------------------------------------------------------------
// --SECTION--                                                      public types
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief statuses that a transaction can have
////////////////////////////////////////////////////////////////////////////////

        enum class StatusType : uint16_t {
          ONGOING      = 1,
          COMMITTED    = 2,
          ROLLED_BACK  = 3
        };

////////////////////////////////////////////////////////////////////////////////
/// @brief statuses that a transaction can have
////////////////////////////////////////////////////////////////////////////////

        enum class VisibilityType : uint16_t {
          INVISIBLE  = 1,
          CONCURRENT = 2,
          VISIBLE    = 3
        };

// -----------------------------------------------------------------------------
// --SECTION--                                        constructors / destructors
// -----------------------------------------------------------------------------

      public:

        Transaction (Transaction const&) = delete;
        Transaction& operator= (Transaction const&) = delete;
      
      protected:

////////////////////////////////////////////////////////////////////////////////
/// @brief create a new transaction, with id provided by the transaction
/// manager. creating a transaction is only allowed by the transaction manager
////////////////////////////////////////////////////////////////////////////////

        Transaction (TransactionManager*,
                     TransactionId const&,
                     struct TRI_vocbase_s*,
                     double);

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief destroy the transaction
////////////////////////////////////////////////////////////////////////////////

        virtual ~Transaction ();

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief return the transaction manager
////////////////////////////////////////////////////////////////////////////////

        inline TransactionManager* transactionManager () const {
          return _transactionManager;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief return the transaction id
////////////////////////////////////////////////////////////////////////////////

        inline TransactionId const& id () const {
          return _id;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief return the transaction's own id
////////////////////////////////////////////////////////////////////////////////

        inline TransactionId::InternalType own () const {
          return _id.own();
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief return the database used by the transaction
////////////////////////////////////////////////////////////////////////////////

        inline struct TRI_vocbase_s* vocbase () const {
          return _vocbase;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief return the transaction start time
////////////////////////////////////////////////////////////////////////////////

        inline double startTime () const {
          return _startTime;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief return the transaction expiration stamp (if 0.0 then it is not set)
////////////////////////////////////////////////////////////////////////////////

        inline double expireTime () const {
          return _expireTime;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief whether or not the transaction is currently ongoing
////////////////////////////////////////////////////////////////////////////////

        inline bool isOngoing () const {
          return (_status == Transaction::StatusType::ONGOING);
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief whether or not the transaction has committed
////////////////////////////////////////////////////////////////////////////////

        inline bool isCommitted () const {
          return (_status == Transaction::StatusType::COMMITTED);
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief whether or not the transaction has rolled back
////////////////////////////////////////////////////////////////////////////////

        inline bool isRolledBack () const {
          return (_status == Transaction::StatusType::ROLLED_BACK);
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief whether or not the transaction has an ongoing subtransaction
////////////////////////////////////////////////////////////////////////////////

        inline bool hasOngoingSubTransaction () const {
          return (_ongoingSubTransaction != nullptr);
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief whether or not the transaction contains successful data-modifying 
/// operations (failed operations are not included)
////////////////////////////////////////////////////////////////////////////////

        bool hasModifications () const;  

////////////////////////////////////////////////////////////////////////////////
/// @brief whether or not the transaction must sync on commit
////////////////////////////////////////////////////////////////////////////////

        bool hasWaitForSync () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief prepare the statistics so updating them later is guaranteed to
/// succeed
////////////////////////////////////////////////////////////////////////////////

        void prepareStats (TransactionCollection const*);

////////////////////////////////////////////////////////////////////////////////
/// @brief update the number of inserted documents
/// this is guaranteed to not fail if prepareStats() was called before
////////////////////////////////////////////////////////////////////////////////

        void incNumInserted (TransactionCollection const*,
                             bool);

////////////////////////////////////////////////////////////////////////////////
/// @brief update the number of removed documents
/// this is guaranteed to not fail if prepareStats() was called before
////////////////////////////////////////////////////////////////////////////////

        void incNumRemoved (TransactionCollection const*,
                            bool);

////////////////////////////////////////////////////////////////////////////////
/// @brief return the transaction status
////////////////////////////////////////////////////////////////////////////////

        Transaction::StatusType status () const {
          return _status;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief commit the transaction
////////////////////////////////////////////////////////////////////////////////

        virtual void commit () = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief roll back the transaction
////////////////////////////////////////////////////////////////////////////////

        virtual void rollback () = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns aggregated transaction statistics
////////////////////////////////////////////////////////////////////////////////

        virtual CollectionStats aggregatedStats (TRI_voc_cid_t) = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns a collection used in the transaction
/// this registers the collection in the transaction if not yet present
////////////////////////////////////////////////////////////////////////////////
        
        virtual TransactionCollection* collection (std::string const&) = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns a collection used in the transaction
/// this registers the collection in the transaction if not yet present
////////////////////////////////////////////////////////////////////////////////
        
        virtual TransactionCollection* collection (TRI_voc_cid_t) = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief get the transaction's top-level transaction
////////////////////////////////////////////////////////////////////////////////

        virtual TopLevelTransaction* topLevelTransaction () = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief get the transaction's top-level transaction
////////////////////////////////////////////////////////////////////////////////

        virtual TopLevelTransaction const* topLevelTransaction () const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief get the transaction's direct parent transaction
////////////////////////////////////////////////////////////////////////////////

        virtual Transaction* parentTransaction () = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief whether or not the transaction is a top-level transaction
////////////////////////////////////////////////////////////////////////////////

        virtual bool isTopLevel () const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief get a string representation of the transaction
////////////////////////////////////////////////////////////////////////////////

        virtual std::string toString () const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief append the transaction to an output stream
////////////////////////////////////////////////////////////////////////////////
    
        friend std::ostream& operator<< (std::ostream&, Transaction const*);
    
        friend std::ostream& operator<< (std::ostream&, Transaction const&);
        
        friend std::ostream& operator<< (std::ostream&, VisibilityType);

        friend std::ostream& operator<< (std::ostream&, StatusType);

////////////////////////////////////////////////////////////////////////////////
/// @brief visibility, this implements the MVCC logic of what this transaction
/// can see, returns the visibility of the other transaction for this one.
/// The result can be INVISIBLE, CONCURRENT or VISIBLE. We guarantee
/// INVISIBLE < CONCURRENT < VISIBLE, such that one can do things like
/// "visibility(other) < VISIBLE" legally.
////////////////////////////////////////////////////////////////////////////////
    
         VisibilityType visibility (TransactionId const& other) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief checks if another document revision (identified by its _from and _to 
/// values) is visible to the current transaction for read-only purposes
////////////////////////////////////////////////////////////////////////////////

         bool isVisibleForRead (TransactionId const& from,
                                TransactionId const& to) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the status of a sub-transaction of us
/// will only be called if sub is an iterated sub-transaction of us and us
/// is still ongoing
////////////////////////////////////////////////////////////////////////////////
        
         StatusType statusSubTransaction (TransactionId const& sub) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief isNotAborted, this is a pure optimisation method. It checks
/// whether another transaction was aborted. It is only legal to call
/// this on transactions that were already committed or aborted before
/// this transaction has started. If the method returns true, then other
/// is guaranteed to be a transaction that has committed before this
/// transaction was started. The method may return false even if the
/// transaction with id other has committed. This is a method of the
/// Transaction class to allow for clever optimisations using a bloom
/// filter.
////////////////////////////////////////////////////////////////////////////////

#if 0
         bool isNotAborted(TransactionId const& other) {
           return false;  // Optimise later
         }
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief set the killed flag
////////////////////////////////////////////////////////////////////////////////

        void killed (bool) {
          _killed = true;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief check the killed flag
////////////////////////////////////////////////////////////////////////////////

        bool killed () const {
          return _killed;
        }

// -----------------------------------------------------------------------------
// --SECTION--                                                 protected methods
// -----------------------------------------------------------------------------

      protected:

////////////////////////////////////////////////////////////////////////////////
/// @brief this is called when a subtransaction is finished
////////////////////////////////////////////////////////////////////////////////

        void subTransactionFinished (Transaction*);

// -----------------------------------------------------------------------------
// --SECTION--                                               protected variables
// -----------------------------------------------------------------------------

      protected:

////////////////////////////////////////////////////////////////////////////////
/// @brief the transaction manager
////////////////////////////////////////////////////////////////////////////////

        TransactionManager* _transactionManager;

////////////////////////////////////////////////////////////////////////////////
/// @brief the transaction's id
////////////////////////////////////////////////////////////////////////////////

        TransactionId const _id;

////////////////////////////////////////////////////////////////////////////////
/// @brief the database used in the transaction
////////////////////////////////////////////////////////////////////////////////

        struct TRI_vocbase_s* const _vocbase;

////////////////////////////////////////////////////////////////////////////////
/// @brief the transaction's start time
////////////////////////////////////////////////////////////////////////////////

        double const _startTime;

////////////////////////////////////////////////////////////////////////////////
/// @brief the transaction's status
////////////////////////////////////////////////////////////////////////////////

        StatusType _status;

////////////////////////////////////////////////////////////////////////////////
/// @brief pointer to currently ongoing sub-transaction (might be null)
////////////////////////////////////////////////////////////////////////////////

        Transaction* _ongoingSubTransaction;

////////////////////////////////////////////////////////////////////////////////
/// @brief ids of all committed sub transaction
////////////////////////////////////////////////////////////////////////////////

        std::unordered_set<TRI_voc_cid_t> _committedSubTransactions;

////////////////////////////////////////////////////////////////////////////////
/// @brief collection modification statistics
////////////////////////////////////////////////////////////////////////////////

        std::unordered_map<TRI_voc_cid_t, CollectionStats> _stats;

////////////////////////////////////////////////////////////////////////////////
/// @brief optional expiration time
////////////////////////////////////////////////////////////////////////////////

        double _expireTime;

////////////////////////////////////////////////////////////////////////////////
/// @brief whether or not the transaction was killed by another thread
////////////////////////////////////////////////////////////////////////////////

        bool _killed;

    };
  }
}

#endif