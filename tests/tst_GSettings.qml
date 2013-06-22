
import QtTest 1.0
import GSettings 1.0

TestCase {
  GSettings {
    id: settings
    schema: "com.canonical.gsettings.Test"
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
}
