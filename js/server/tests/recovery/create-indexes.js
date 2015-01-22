
var db = require("org/arangodb").db;
var internal = require("internal");
var jsunity = require("jsunity");


function runSetup () {
  internal.debugClearFailAt();
  var i, j, c;
  for (i = 0; i < 5; ++i) {
    db._drop("UnitTestsRecovery" + i);
    c = db._create("UnitTestsRecovery" + i);

    for (j = 0; j < 100; ++j) { 
      c.save({ _key: "test" + j, value1: "foo" + j, value2: "bar" + j });
    }

    c.ensureUniqueConstraint("value1");
    c.ensureUniqueSkiplist("value2");
    c.ensureCapConstraint(50);
  }
 
  db._drop("test");
  c = db._create("test"); 
  c.save({ _key: "crashme" }, true);

  internal.debugSegfault("crashing server");
}

////////////////////////////////////////////////////////////////////////////////
/// @brief test suite
////////////////////////////////////////////////////////////////////////////////

function recoverySuite () {
  jsunity.jsUnity.attachAssertions();

  return {
    setUp: function () {
    },
    tearDown: function () {
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief test whether we can restore the trx data
////////////////////////////////////////////////////////////////////////////////
    
    testCreateIndexes : function () {
      var i, j, c, idx, doc;
      var hash = null, skip = null;
      for (i = 0; i < 5; ++i) {
        c = db._collection("UnitTestsRecovery" + i);
        assertEqual(50, c.count());
        idx = c.getIndexes();
        assertEqual(4, idx.length);

        for (j = 1; j < 4; ++j) {
          if (idx[j].type === 'hash') {
            hash = idx[j];
          }  
          else if (idx[j].type === 'skiplist') {
            skip = idx[j];
          }
        }

        assertNotNull(hash);
        assertNotNull(skip);

        for (j = 50; j < 100; ++j) {
          doc = c.document("test" + j);
          assertEqual("foo" + j, doc.value1);
          assertEqual("bar" + j, doc.value2);
        
          assertEqual(1, c.byExampleHash(hash.id, { value1: "foo" + j }).toArray().length);
          assertEqual(1, c.byExampleSkiplist(skip.id, { value2: "bar" + j }).toArray().length);
        }
      }

    }
        
  };
}

////////////////////////////////////////////////////////////////////////////////
/// @brief executes the test suite
////////////////////////////////////////////////////////////////////////////////

function main (argv) {
  if (argv[1] === "setup") {
    runSetup();
    return 0;
  }
  else {
    jsunity.run(recoverySuite);
    return jsunity.done().status ? 0 : 1;
  }
}

