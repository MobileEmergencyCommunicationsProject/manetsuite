import QtQuick 1.0
import QtWebKit 1.0

Rectangle {
    id: container
    property alias clip: flickable.clip
    property alias url : webView.url

    function reload() {
        // Magic! This isn't documented well.
        webView.reload.trigger()
    }

    Flickable {
        id: flickable
        height: parent.height
        width: parent.width
        contentWidth: Math.max(parent.width,webView.width)
        contentHeight: Math.max(parent.height,webView.height)
        //	anchors.fill: parent
        opacity: 0.5
        onWidthChanged : {
            // Expand (but not above 1:1) if otherwise would be smaller that available width.
            if (width > webView.width*webView.contentsScale && webView.contentsScale < 1.0)
                webView.contentsScale = width / webView.width * webView.contentsScale;
        }

        Component.onCompleted: {
            console.debug("HelpViewer: Flickable: height", height, "width", width)
        }

        WebView {
            id: webView
            contentsScale: 1
            preferredHeight: flickable.height
            preferredWidth: flickable.width
            transformOrigin: Item.TopLeft

            Component.onCompleted: {
                console.debug("HelpViewer: WebView: height", height, "width", width)

            }

            onLoadFailed: {
                console.debug("HelpViewer: failed to load", url)
            }

            onLoadFinished: {
                console.debug("HelpViewer: loaded", url)
            }

            onLoadStarted: {
                console.debug("HelpViewer: loading", url)
            }

            function doZoom(zoom,centerX,centerY)
            {
                if (centerX) {
                    var sc = zoom*contentsScale;
                    scaleAnim.to = sc;
                    flickVX.from = flickable.contentX
                    flickVX.to = Math.max(0,Math.min(centerX-flickable.width/2,webView.width*sc-flickable.width))
                    finalX.value = flickVX.to
                    flickVY.from = flickable.contentY
                    flickVY.to = Math.max(0,Math.min(centerY-flickable.height/2,webView.height*sc-flickable.height))
                    finalY.value = flickVY.to
                    quickZoom.start()
                }
            }

            onContentsSizeChanged: {
                // zoom out
                contentsScale = Math.min(1,flickable.width / contentsSize.width)
            }
            SequentialAnimation {
                id: quickZoom

                PropertyAction {
                    target: webView
                    property: "renderingEnabled"
                    value: false
                }
                ParallelAnimation {
                    NumberAnimation {
                        id: scaleAnim
                        target: webView
                        property: "contentsScale"
                        // the to property is set before calling
                        easing.type: Easing.Linear
                        duration: 200
                    }
                    NumberAnimation {
                        id: flickVX
                        target: flickable
                        property: "contentX"
                        easing.type: Easing.Linear
                        duration: 200
                        from: 0 // set before calling
                        to: 0 // set before calling
                    }
                    NumberAnimation {
                        id: flickVY
                        target: flickable
                        property: "contentY"
                        easing.type: Easing.Linear
                        duration: 200
                        from: 0 // set before calling
                        to: 0 // set before calling
                    }
                }
                // Have to set the contentXY, since the above 2
                // size changes may have started a correction if
                // contentsScale < 1.0.
                PropertyAction {
                    id: finalX
                    target: flickable
                    property: "contentX"
                    value: 0 // set before calling
                }
                PropertyAction {
                    id: finalY
                    target: flickable
                    property: "contentY"
                    value: 0 // set before calling
                }
                PropertyAction {
                    target: webView
                    property: "renderingEnabled"
                    value: true
                }
            }
            onZoomTo: doZoom(zoom,centerX,centerY)
        }
    }
}
