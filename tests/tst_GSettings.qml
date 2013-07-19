
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

  // this test must run first (others overwrite keys), hence the 'aaa'
  function test_aaa_read_defaults() {
    compare(settings.testInteger, 42);
    compare(settings.testDouble, 1.5);
    compare(settings.testBoolean, false);
    compare(settings.testString, 'hello');
  }

  function test_write() {
    settings.testInteger = 2;
    compare(settings.testInteger, 2);
    settings.testDouble = 2.5;
    compare(settings.testDouble, 2.5);
    settings.testBoolean = true;
    compare(settings.testBoolean, true);
    settings.testString = 'bye';
    compare(settings.testString, 'bye');
  }

  function test_changed() {
    settings["testString"] = "goodbye";

    compare(testCase.changedKey, "testString", "changedKey not correct");
    compare(testCase.changedValue, "goodbye", "changedValue not correct");
  }

  function test_choices() {
    compare(settings.schema.choices('testEnum'), ['one', 'two', 'three']);
    compare(settings.schema.choices('testInteger'), []);
  }
}
