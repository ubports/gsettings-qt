
import QtQuick 2.0
import GSettings 1.0

Item {
  width: 200
  height: 300

  GSettings {
    id: settings
    schema.id: "org.gnome.desktop.interface"
  }

  ListView {
    anchors.fill: parent
    model: settings.schema.choices('toolbarStyle')

    delegate {
      Text {
        text: modelData
      }
    }
  }
}
