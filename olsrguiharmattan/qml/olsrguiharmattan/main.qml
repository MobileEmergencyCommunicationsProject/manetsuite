import QtQuick 1.1
import com.nokia.meego 1.0
import QOLSRApp 1.0

PageStackWindow {
    id: appWindow

    initialPage: helpPage

    signal getSettings()
    signal reconnect()
    signal settingsChanged(bool al, bool fuzzy, bool slowdown,
                           double hi, double hj, double ht,
                           double tci, double tcj, double tct,
                           double hnai, double hnaj, double hnat,
                           double up, double down, double alpha,
                           int willingness)
    signal updateNeighbors()
    signal updateRoutes()

    Connections {
        target: applicationLogic
        onAddDestEntry: {
            console.log("addDestEntry ", address, ", ", gateway, ", ", weight, ", ", interfaceName)

            routesPage.model.append({"destination":address, "gateway":gateway, "weight":weight, "interfaceName":interfaceName })
        }

        onAddNeighborEntry: {
            console.log("addNeighborEntry ", address, ", ", status, ", ", hysteresis, ", ", mPRSelect)
            neighborsPage.model.append({"address":address, "status":status, "hysteresis":hysteresis, "mPRSelect":mPRSelect })

        }

        onClearDestEntries: {
            console.log("clearDestEntries")
            routesPage.model.clear()
        }

        onClearNeighborEntries: {
            console.log("clearNeighborEntries")
            neighborsPage.model.clear()
        }

        onSetSettings: {
            console.log("setSettings al ", al, " fuzzy ", fuzzy, " slowdown ", slowdown,
                        " hi ", hi, " hj ", hj, " ht ", ht,
                        " tci ", tci, " tcj ", tcj, " tct ", tct,
                        " hnai ", hnai, " hnaj ", hnaj, " hnat ", hnat,
                        " up ", up, " down ", down, " alpha ", alpha,
                        " willingness ", willingness)

            settingsPage.setValues(al, fuzzy, slowdown,
                                   hi, hj, ht,
                                   tci, tcj, tct,
                                   hnai, hnaj, hnat,
                                   up, down, alpha,
                                   willingness)
        }
    }

    HelpPage {
        id: helpPage
        title: qsTr("OLSR GUI Help")
        url: "qrc:/help/olsrguiharmattan.html"

        // HelpPage has a built-in update() function that reloads the
        // page from the URL.
    }

    ListPage {
        id: routesPage
        delegate: RouteDelegate{}
        title: qsTr("Routes")

        function update() {
            updateRoutes()
        }
    }

    ListPage {
        id: neighborsPage
        delegate: NeighborDelegate{}
        title: qsTr("Neighbors")

        function update() {
            updateNeighbors()
        }
    }

    SettingsPage {
        id:settingsPage
        title: qsTr("Settings")

        function okFunction() {
            settingsChanged(allLinks, fuzzyFlooding, tcSlowdown,
                            helloInterval, helloJitter, helloTimeoutFactor,
                            tcInterval, tcJitter, tcTimeoutFactor,
                            hnaInterval, hnaJitter, hnaTimeoutFactor,
                            hysteresisUp, hysteresisDown, hysteresisAlpha,
                            willingness)
        }

        // User presses the update button in the toolbar.
        // Ask olsrd for the current settings and throw away
        // the ones the user might have changed.
        function update() {
            settingsPage.restoreValues()
            getSettings()
        }
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-refresh"
            anchors.centerIn: (parent == undefined ) ? undefined : parent
            onClicked: pageStack.currentPage.update()
        }

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
                text: qsTr("Neighbors")
                onClicked: pageStack.replace(neighborsPage)
            }
            MenuItem {
                text: qsTr("Routes")
                onClicked: pageStack.replace(routesPage)
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.replace(settingsPage)
            }
            MenuItem {
                text: qsTr("Reconnect to olsrd")
                onClicked: {
                    console.debug("Reconnect")
                    appWindow.reconnect()
                }
            }

            MenuItem {
                text: qsTr("Help")
                onClicked: pageStack.replace(helpPage)
            }
        }
    }
}
