import QtQuick 1.1
import com.nokia.meego 1.0
import QOLSRApp 1.0

PageStackWindow {
    id: appWindow

    initialPage: mainPage

    signal updateRoutes()
    signal updateNeighbors()
    signal getSettings()
    signal settingsChanged(bool al, bool fuzzy, bool slowdown,
                           double hi, double hj, double ht,
                           double tci, double tcj, double tct,
                           double hnai, double hnaj, double hnat,
                           double up, double down, double alpha,
                           int willingness)

    Connections {
        target: applicationLogic
        onAddDestEntry: {
            console.log("addDestEntry ", dest, ", ", gw, ", ", weight, ", ", interfaceName)
        }

        onAddNeighborEntry: {
            console.log("addNeighborEntry ", neighbor, ", ", type, ", ", hysterisis, ", ", mPRSelect)
        }

        onClearDestEntries: {
            console.log("clearDestEntries")
        }

        onClearNeighborEntries: {
            console.log("clearNeighborEntries")
        }

        onSetSettings: {
            console.log("setSettings al ", al, " fuzzy ", fuzzy, " slowdown ", slowdown,
                        " hi ", hi, " hj ", hj, " ht ", ht,
                        " tci ", tci, " tcj ", tcj, " tct ", tct,
                        " hnai ", hnai, " hnaj ", hnaj, " hnat ", hnat,
                        " up ", up, " down ", down, " alpha ", alpha,
                        " willingness ", willingness)
        }
    }

    MainPage {
        id: mainPage
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Routes")
                onClicked: updateRoutes()
            }
            MenuItem {
                text: qsTr("Neighbors")
                onClicked: updateNeighbors()
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: getSettings()
            }
        }
    }
}
