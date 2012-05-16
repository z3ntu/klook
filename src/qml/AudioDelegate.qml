import QtQuick 1.1
import Widgets 1.0

Component {
    id: audioDelegate
    
    Item
    {
        id: audioItem
        
        signal ready()
        
        Image {
            id: audioIcon
            anchors.left: parent.left
            clip: true
            source: "images/audio.png"
            opacity: 0
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            smooth: true;
            visible: albumWrapper.state === "fullscreen"
            height: getHeight( parent.height, 500 ) //parent.height * 3 / 5
            y: ( parent.height - panel.height - height ) / 2
            
            Behavior on opacity { NumberAnimation { duration: 500 } }
        }
        
        InfoItem {
            id : title
            font.pointSize: 15
            anchors.top: audioIcon.top
            anchors.left: audioIcon.right
            anchors.right: parent.right
        }
        
        InfoItem {
            id : artist
            anchors.top: title.bottom
            anchors.left: audioIcon.right
        }
        
        InfoItem {
            id : totalTime
            anchors.top: artist.bottom
            anchors.left: audioIcon.right
        }
        
        Binding { target: totalTime; property: "anchors.top"; value: artist.visible ? artist.bottom : title.bottom }
        
        Audio {
            id: audio
            
            onTicked:
            {
                if ( playing )
                {
                    panel.videoSlider.value = tick * 1000 / audio.totalTime; // tick and totalTime in msec
                }
            }
            
            onPlayFinished:
            {
                panel.playButtonState = 'Play'
                panel.videoSlider.value = 0
            }
            
            onReady:
            {
                title.text = "<b>" + audio.title + "</b>"
                artist.text = artistStr + " <b>" + audio.artist + "</b>"
                artist.visible = ( audio.artist != "" )
                
                var h = audio.totalTime / ( 1000 * 3600 )
                var strFmt = ( h >= 1 ) ? "hh:mm:ss" : "m:ss"
                
                totalTime.text = totalTimeStr + " <b>" + Qt.formatTime( audio.duration, strFmt ) + "</b>"
            }
        }
        
        Connections{
            target: panel.playItemBtn;
            onButtonClick:
            {
                if ( listItem.ListView.isCurrentItem )
                {
                    audio.play_or_pause();
                    if ( audio.playing )
                        panel.playButtonState = 'Play'
                    else
                        panel.playButtonState = 'Pause'
                }
            }
        }
        
        Connections{ target: panel.videoSlider; onPosChanged: audio.setPosition( panel.videoSlider.value * audio.totalTime / 1000 ) }
        
        Connections{
            target: albumWrapper;
            onStateChanged:
            {
                if ( albumWrapper.state === "inGrid" )
                    audio.pause()
                else
                    audio.play()
            }
        }
        
        Connections{
            target: photosListView;
            onCurrentIndexChanged: {
                if ( listItem.ListView.isCurrentItem )
                {
                    audioIcon.opacity = 1
                    audio.source = filePath
                    mainWindow.currentFileType = 5;
                    mainWindow.updatePanel()
                    if ( albumWrapper.state === "fullscreen" )
                        audio.play()
                    
                    if ( audio.playing )
                        panel.playButtonState = 'Play'
                    else
                        panel.playButtonState = 'Pause'
                }
                else
                    audioIcon.opacity = 0
            }
        }
    }
}
