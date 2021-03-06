/*  smplayer2, GUI front-end for mplayer2.
    Copyright (C) 2006-2010 Ricardo Villalba <rvm@escomposlinux.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _CORE_H_
#define _CORE_H_

#include <QObject>
#include <QProcess> // For QProcess::ProcessError
#include <QPoint>
#include "mediadata.h"
#include "mediasettings.h"
#include "mplayerprocess.h"
#include "config.h"

#ifndef NO_USE_INI_FILES
class FileSettingsBase;
#endif

class MplayerProcess;
class MplayerWindow;
class QSettings;

#ifdef Q_OS_WIN
#ifdef SCREENSAVER_OFF
class WinScreenSaver;
#endif
#endif

class Core : public QObject
{
    Q_OBJECT

public:
    enum State { Stopped = 0, Playing = 1, Paused = 2 };

    Core(MplayerWindow *mpw, QWidget *parent = 0);
    ~Core();

    MediaData mdat;
    MediaSettings mset;

    //! Return the current state
    State state() {
        return _state;
    };

    //! Return a string with the name of the current state,
    //! so it can be printed on debugging messages.
    QString stateToString();

protected:
    //! Change the current state (Stopped, Playing or Paused)
    //! And sends the stateChanged() signal.
    void setState(State s);

public slots:
    //! Generic open, with autodetection of type
    void open(QString file, int seek = -1);
    void openFile(QString filename, int seek = -1);
    void openStream(QString name);
    /*
    void openDVD( bool from_folder, QString directory = "");
    void openDVD(); // Plays title 1
    void openDVD(int title = 1);
    */
    void openDVD(QString dvd_url);
    void openVCD(int title = -1);
    void openAudioCD(int title = -1);
    void openTV(QString channel_id);

    void loadSub(const QString &sub);
    void unloadSub();

    //! Forces to use the specified subtitle file. It's not loaded immediately but stored
    //! and will be used for the next video. After that the variable is cleared.
    void setInitialSubtitle(const QString &subtitle_file) {
        initial_subtitle = subtitle_file;
    };

    void loadAudioFile(const QString &audiofile);
    void unloadAudioFile();

    void stop();
    void play();
    void play_or_pause();
    void pause_and_frame_step();
    void pause();
    void frameStep();
    void screenshot();	//!< Take a screenshot of current frame
    void screenshots();	//!< Start/stop taking screenshot of each frame

    //! Public restart, for the GUI.
    void restart();

    //! Reopens the file (no restart)
    void reload();

#ifdef SEEKBAR_RESOLUTION
    void goToPosition(int value);
    void goToPos(double perc);
#else
    void goToPos(int perc);
#endif
    void goToSec(double sec);

    void setAMarker(); //!< Set A marker to current sec
    void setAMarker(int sec);

    void setBMarker(); //!< Set B marker to current sec
    void setBMarker(int sec);

    void clearABMarkers();

    void toggleRepeat();
    void toggleRepeat(bool b);

    void toggleFlip();
    void toggleFlip(bool b);

    void toggleMirror();
    void toggleMirror(bool b);

    // Audio filters
    void toggleKaraoke();
    void toggleKaraoke(bool b);
    void toggleExtrastereo();
    void toggleExtrastereo(bool b);
    void toggleVolnorm();
    void toggleVolnorm(bool b);

    void setAudioChannels(int channels);
    void setStereoMode(int mode);

    // Video filters
    void toggleAutophase();
    void toggleAutophase(bool b);
    void toggleDeblock();
    void toggleDeblock(bool b);
    void toggleDering();
    void toggleDering(bool b);
    void toggleNoise();
    void toggleNoise(bool b);
    void changeDenoise(int);
    void changeLetterbox(bool);
    void changeUpscale(bool);

    void seek(int secs);
    void sforward(); 	// + 10 seconds
    void srewind(); 	// - 10 seconds
    void forward(); 	// + 1 minute
    void rewind(); 		// -1 minute
    void fastforward();	// + 10 minutes
    void fastrewind();	// - 10 minutes
    void forward(int secs);
    void rewind(int secs);
    void wheelUp();
    void wheelDown();

    void setSpeed(double value);
    void incSpeed10();	//!< Inc speed 10%
    void decSpeed10();	//!< Dec speed 10%
    void incSpeed4();	//!< Inc speed 4%
    void decSpeed4();	//!< Dec speed 4%
    void incSpeed1();	//!< Inc speed 1%
    void decSpeed1();	//!< Dec speed 1%
    void doubleSpeed();
    void halveSpeed();
    void normalSpeed();

    void setVolume(int volume, bool force = false);
    void switchMute();
    void mute(bool b);
    void incVolume();
    void decVolume();

    void setBrightness(int value);
    void setContrast(int value);
    void setGamma(int value);
    void setHue(int value);
    void setSaturation(int value);

    void incBrightness();
    void decBrightness();
    void incContrast();
    void decContrast();
    void incGamma();
    void decGamma();
    void incHue();
    void decHue();
    void incSaturation();
    void decSaturation();

    void setSubDelay(int delay);
    void incSubDelay();
    void decSubDelay();

    void setAudioDelay(int delay);
    void incAudioDelay();
    void decAudioDelay();

    void changeSubScale(double value);
    void incSubScale();
    void decSubScale();

    //! Select next line in subtitle file
    void incSubStep();
    //! Select previous line in subtitle file
    void decSubStep();

    void changeSubVisibility(bool visible);

    //! Audio equalizer
    void setAudioEqualizer(AudioEqualizerList values, bool restart = false);
    void setAudioAudioEqualizerRestart(AudioEqualizerList values) {
        setAudioEqualizer(values, true);
    };
    void updateAudioEqualizer();

    void setAudioEq0(int value);
    void setAudioEq1(int value);
    void setAudioEq2(int value);
    void setAudioEq3(int value);
    void setAudioEq4(int value);
    void setAudioEq5(int value);
    void setAudioEq6(int value);
    void setAudioEq7(int value);
    void setAudioEq8(int value);
    void setAudioEq9(int value);

    void changeDeinterlace(int);
    void changeSubtitle(int);
    void nextSubtitle();
    void changeAudio(int ID, bool allow_restart = true);
    void nextAudio();
    void changeVideo(int ID, bool allow_restart = true);
    void nextVideo();
#if PROGRAM_SWITCH
    void changeProgram(int ID);
    void nextProgram();
#endif
    void changeTitle(int);
    void changeChapter(int, bool);
    void changeChapter(int);
    void prevChapter();
    void nextChapter();
    void changeEdition(int);
    void changeAngle(int);
    void changeAspectRatio(int);
    void nextAspectRatio();
    void changeOSD(int);
    void nextOSD();
    void nextWheelFunction();

    void changeSize(int); // Size of the window
    void toggleDoubleSize();
    void changeZoom(double); // Zoom on mplayerwindow

    void changeRotate(int r);

#if USE_ADAPTER
    void changeAdapter(int n);
#endif

    void incZoom();
    void decZoom();
    void resetZoom();
    void autoZoom();
    void autoZoomFromLetterbox(double video_aspect);
    void autoZoomFor169();
    void autoZoomFor235();

    void changePanscan(double);
    void incPanscan();
    void decPanscan();

    void showFilenameOnOSD();
    void toggleDeinterlace();

    void changeSubUseMplayer2Defaults(bool);
    void toggleForcedSubsOnly(bool);

    void changeClosedCaptionChannel(int);
    /*
    void nextClosedCaptionChannel();
    void prevClosedCaptionChannel();
    */

    void visualizeMotionVectors(bool);

#if DVDNAV_SUPPORT
    // dvdnav buttons
    void dvdnavUp();
    void dvdnavDown();
    void dvdnavLeft();
    void dvdnavRight();
    void dvdnavMenu();
    void dvdnavSelect();
    void dvdnavPrev();
    void dvdnavMouse();
#endif

    // Pass a command to mplayer by stdin:
    void tellmp(const QString &command);

    //! Wrapper for the osd_show_text slave command
    void displayTextOnOSD(QString text, int duration = 3000, int level = 1,
                          QString prefix = QString::null);

public:
#ifndef NO_USE_INI_FILES
    void changeFileSettingsMethod(QString method);
#endif

protected:
    //! Returns the prefix to keep pausing on slave commands
    QString pausing_prefix();

protected slots:
    void changeCurrentSec(double sec);
    void changePause();
    void gotWindowResolution(int w, int h);
    void gotNoVideo();
    void gotVO(QString);
    void gotAO(QString);
    void gotStartingTime(double);

    void finishRestart();
    void processFinished();
    void fileReachedEnd();

    void displayMessage(QString text);
    void displayScreenshotName(QString filename);
    void displayUpdatingFontCache();

    void streamTitleChanged(QString);
    void streamTitleAndUrlChanged(QString, QString);

    // Catches mediaInfoChanged and sends mediaPlaying signal
    void sendMediaInfo();

    void watchState(Core::State state);

    //! Called when a video has just started to play.
    //! This function checks if the codec of video is ffh264 and if
    //! the resolution is HD
    void checkIfVideoIsHD();

#if NOTIFY_AUDIO_CHANGES
    void initAudioTrack(const Tracks &);
#endif
#if NOTIFY_SUB_CHANGES
    void initSubtitleTrack(const SubTracks &);
    void setSubtitleTrackAgain(const SubTracks &);
#endif
#if DVDNAV_SUPPORT
    void dvdTitleChanged(int);
    void updateChapter(int chapter);
    void updateEdition(int edition);
    void durationChanged(double);
    void askForInfo();
    void dvdnavUpdateMousePos(QPoint);
    void dvdTitleIsMenu();
    void dvdTitleIsMovie();
#endif

    void initializeOSD();

protected:
    void playNewFile(QString file, int seek = -1);
    void restartPlay();
    void initPlaying(int seek = -1);
    void newMediaPlaying();

    void startMplayer(QString file, double seek = -1);
    void stopMplayer();

#ifndef NO_USE_INI_FILES
    void saveMediaInfo();
#endif

    void initializeMenus();
    void updateWidgets();

signals:
    void aboutToStartPlaying(); // Signal emited just before to start mplayer
    void mediaLoaded();
    void mediaInfoChanged();
    //! Sends the filename and title of the stream playing in this moment
    void mediaPlaying(const QString &filename, const QString &title);
    void stateChanged(Core::State state);
    void mediaStartPlay();
    void mediaFinished(); // Media has arrived to the end.
    void mediaStoppedByUser();
    void showMessage(QString text);
    void menusNeedInitialize();
    void widgetsNeedUpdate();
    void videoEqualizerNeedsUpdate();
    void audioEqualizerNeedsUpdate();
    void showTime(double sec);
#ifdef SEEKBAR_RESOLUTION
    void positionChanged(int); // To connect a slider
#else
    void posChanged(int); // To connect a slider
#endif
    void showFrame(int frame);
    void ABMarkersChanged(int secs_a, int secs_b);
    void needResize(int w, int h, bool force);
    void noVideo();
    void volumeChanged(int);
#if NOTIFY_AUDIO_CHANGES
    void audioTracksChanged();
#endif

    //! mplayer2 started but finished with exit code != 0
    void mplayerFinishedWithError(int exitCode);

    //! mplayer didn't start or has crashed
    void mplayerFailed(QProcess::ProcessError error);

    // Resend signal from mplayerprocess:
    void failedToParseMplayerVersion(QString line_with_mplayer_version);

    //! A new line from the mplayer output is available
    void logLineAvailable(QString);

protected:
    MplayerProcess *proc;
    MplayerWindow *mplayerwindow;

#ifndef NO_USE_INI_FILES
    FileSettingsBase *file_settings;
    FileSettingsBase *tv_settings;
#endif

#ifdef Q_OS_WIN
#ifdef SCREENSAVER_OFF
    WinScreenSaver *win_screensaver;
#endif
#endif

private:
    // Some variables to proper restart
    bool we_are_restarting;

    bool just_loaded_external_subs;
    bool just_unloaded_external_subs;
    State _state;
    bool change_volume_after_unpause;

    QString initial_subtitle;

#if DVDNAV_SUPPORT
    bool dvdnav_title_is_menu;
#endif
};

#endif
