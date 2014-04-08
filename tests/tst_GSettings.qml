
import QtTest 1.0
import "../GSettings" 1.0
import QtQuick 2.0

TestCase {
  id: testCase

  property var changes: []

  GSettings {
    id: settings
    schema.id: "com.canonical.gsettings.Test"

    onChanged: changes.push([key, value]);
  }

  GSettings {
    id: invalid_settings

    schema.id: "com.canonical.gsettings.NonExisting"
  }

  property string bindingTest: settings.testString

  // this test must run first (others overwrite keys), hence the 'aaa'
  function test_aaa_read_defaults() {
    compare(settings.schema.isValid, true);
    compare(settings.testInteger, 42);
    compare(settings.testDouble, 1.5);
    compare(settings.testBoolean, false);
    compare(settings.testString, 'hello');
    compare(settings.testStringList, ['one', 'two', 'three']);

    compare(testCase.bindingTest, 'hello');
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
    compare(testCase.bindingTest, 'bye');

    settings.testStringList = ['four', 'five']
    compare(settings.testStringList, ['four', 'five']);
    settings.testStringList = ['six']
    compare(settings.testStringList, ['six']);
    settings.testStringList = [];
    compare(settings.testStringList, []);

    settings.testEnum = 'two';
    compare(settings.testEnum, 'two');

    // test whether writing an out-of-range key doesn't work
    settings.testEnum = 'notanumber';
    compare(settings.testEnum, 'two');
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

  function test_reset() {
    settings.testInteger = 4;
    settings.schema.reset('testInteger');
    compare(settings.testInteger, 42);
  }

  function test_invalid_schema() {
    compare(invalid_settings.schema.isValid, false);
    compare(invalid_settings.schema.testInteger, undefined);
  }
}
