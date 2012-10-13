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
                           bool willingness)

    Connections {
        target: applicationLogic
        onAddDestEntry: {
            console.log("addDestEntry ", address, ", ", gateway, ", ", weight, ", ", interfaceName)

            routes.model.append({"destination":address, "gateway":gateway, "weight":weight, "interfaceName":interfaceName })
        }

        onAddNeighborEntry: {
            console.log("addNeighborEntry ", address, ", ", status, ", ", hysteresis, ", ", mPRSelect)
            neighbors.model.append({"address":address, "status":status, "hysteresis":hysteresis, "mPRSelect":mPRSelect })

        }

        onClearDestEntries: {
            console.log("clearDestEntries")
            routes.model.clear()
        }

        onClearNeighborEntries: {
            console.log("clearNeighborEntries")
            neighbors.model.clear()
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

    ListPage {
        id: routes
        delegate: RouteDelegate{}
        title: "Routes"

        function buttonClickedFunction() {
            updateRoutes()
        }
    }

    ListPage {
        id: neighbors
        delegate: NeighborDelegate{}
        title: "Neighbors"

        function buttonClickedFunction() {
            updateNeighbors()
        }
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
                onClicked: pageStack.replace(routes)
            }
            MenuItem {
                text: qsTr("Neighbors")
                onClicked: pageStack.replace(neighbors)
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.replace(mainPage) //getSettings()
            }
        }
    }
}
