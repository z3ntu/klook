/* KLook
 * Copyright (c) 2011-2012 ROSA  <support@rosalab.ru>
 * Authors: Julia Mineeva, Evgeniy Auzhin, Sergey Borovkov.
 * License: GPLv3
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef DECLARATIVEVIEWER_H
#define DECLARATIVEVIEWER_H

#include <KDE/Phonon/MediaObject>
#include <KDE/Phonon/VideoWidget>

#include <QtDeclarative/QDeclarativeView>
#include <QVariant>

class QRect;
class PreviewGenerator;
class File;
class FileModel;
class QPoint;

typedef enum WidgetRegion
{
    FRAME_REGION,
    HEADER_REGION,
    RIGHT_BORDER_REGION,
    LEFT_BORDER_REGION,
    TOP_BORDER_REGION,
    BOTTOM_BORDER_REGION,
    TOP_LEFT_CORNER_REGION,
    TOP_RIGHT_CORNER_REGION,
    BOTTOM_LEFT_CORNER_REGION,
    BOTTOM_RIGHT_CORNER_REGION,
    ARROW_NULL_REGION

} WidgetRegion;

typedef enum ArrowPosition
{
    TOP,
    BOTTOM,
    LEFT,
    RIGHT

} ArrowPosition;


class DeclarativeViewer : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit DeclarativeViewer( QWidget *parent = 0 );
    virtual ~DeclarativeViewer();

    void updateSize(File* file);

    void init(QStringList urls, bool embedded = false, const QRect& rc = QRect( 0, 0, 0, 0 ), int indexToShow = 0 );

signals:
    void sizeChanged();

    /**
     * @brief Informs QML to set fullscreen state
     */
    void setFullScreenState();
    void setEmbeddedState();
    void setStartWindow();

public slots:
    void updateContent(int index);
    void setActualSize();
    void setFullScreen();
    void onSetGallery(bool );

    void onMetaDataChanged();

    void setEmbedded(bool);

private slots:
    void focusChanged(QWidget*, QWidget*);

protected:
    void mousePressEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );
    void mouseReleaseEvent( QMouseEvent* event );

private:
    enum ViewMode
    {
        Single,
        Multi
    };

    QSize getActualSize() const;
    WidgetRegion calculateWindowRegion( const QPoint& mousePos );

    void initModel(QStringList urls);
    void setViewMode( ViewMode mode );
    void changeContent();
    void registerTypes();
    QSize getTextWindowSize( QString url ) const;
    void createVideoObject( QUrl url );
    QSize calculateViewSize( const QSize& sz ) const;

    QSize inscribedRectToRect( const QSize& sz1, const QSize& sz2 ) const;
    void centerWidget( const QSize& sz );
    void showWidget( const QSize& sz );

    void skipTaskBar();
    bool checkComposite();

    QPoint          m_lastMousePosition;
    bool            m_isSingleMode;
    bool            m_moving;
    bool            m_resize;
    bool            m_startFullScreen;
    bool            m_isEmbedded;
    bool            m_isGallery;
    QRect           m_rcIcon;
    QRect           m_rcArrow;

    File*     m_currentFile;

    bool            m_isActualSize;
    WidgetRegion    m_region;

    Phonon::MediaObject* m_mediaObject;
    Phonon::VideoWidget* m_videoWidget;

    bool                m_compositing;
    PreviewGenerator*   m_previewGenerator;
    FileModel*          m_fileModel;
    ArrowPosition       m_posArrow;
    int                 m_indexToShow;
};

#endif // DECLARATIVEVIEWER_H
