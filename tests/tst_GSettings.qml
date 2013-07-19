
import QtTest 1.0
import GSettings 1.0

TestCase {
  id: testCase

  property var changes: []

  GSettings {
    id: settings
    schema.id: "com.canonical.gsettings.Test"

    onChanged: changes.push([key, value]);
  }

  // this test must run first (others overwrite keys), hence the 'aaa'
  function test_aaa_read_defaults() {
    compare(settings.testInteger, 42);
    compare(settings.testDouble, 1.5);
    compare(settings.testBoolean, false);
    compare(settings.testString, 'hello');
    compare(settings.testStringList, ['one', 'two', 'three']);
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

    settings.testStringList = ['four', 'five']
    compare(settings.testStringList, ['four', 'five']);
    settings.testStringList = ['six']
    compare(settings.testStringList, ['six']);
    settings.testStringList = [];
    compare(settings.testStringList, []);
  }

  function test_changed() {
    changes = []

    settings.testInteger = 4;
    settings.testDouble = 3.14
    settings.testString = 'goodbye';

    compare(changes, [['testInteger', 4], ['testDouble', 3.14], ['testString', 'goodbye']]);
  }

  function test_choices() {
    compare(settings.schema.choices('testEnum'), ['one', 'two', 'three']);
    compare(settings.schema.choices('testInteger'), []);
  }

  function test_non_existing() {
    compare(settings.schema.aKeyThatsNotInTheSchema, undefined);
    compare(settings.schema.choices('aKeyThatsNotInTheSchema'), []);
  }
}
