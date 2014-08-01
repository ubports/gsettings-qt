
import GSettings 1.0
import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItems

Item {
  width: 400
  height: 300

  GSettings {
    id: settings
    schema.id: "com.canonical.Unity.Lenses"
  }

  Column {
    anchors.fill: parent

    ListItems.Standard {
      text: 'Dash search'
      control: Switch {
        checked: settings.remoteContentSearch == 'all'
        onClicked: settings.remoteContentSearch = checked ? 'all' : 'none'
      }
    }

    ListItems.SingleValue {
      text: 'Possible values'
      value: settings.schema.choices('remoteContentSearch').join(', ')
    }
  }
}

