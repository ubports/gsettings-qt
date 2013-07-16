
import QtTest 1.0
import GSettings 1.0

TestCase {
  id: testCase

  property string changedKey
  property string changedValue

  GSettings {
    id: settings
    schema.id: "com.canonical.gsettings.Test"
    onChanged: {
        changedKey = key
        changedValue = value
    }
  }

  function readWriteKey(key, expectedValue, newValue) {
    compare(settings[key], expectedValue, "read " + key);
    settings[key] = newValue;
    compare(settings[key], newValue, "write " + key);
  }

  function test_types() {
    readWriteKey("testInteger", 42, 2);
    readWriteKey("testDouble", 1.5, 2.5);
    readWriteKey("testBoolean", false, true);
    readWriteKey("testString", "hello", "bye");
  }

  function test_changed() {
    settings["testString"] = "goodbye";

    compare(testCase.changedKey, "testString", "changedKey not correct");
    compare(testCase.changedValue, "goodbye", "changedValue not correct");

    // Clean up for test_types()
    settings["testString"] = "hello";
  }
}
