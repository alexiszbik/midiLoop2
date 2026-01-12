/*
 ==============================================================================
 
 Track.h
 Created: 16 Jun 2024 6:59:27pm
 Author:  Alexis ZBIK
 
 ==============================================================================
 */

#pragma once

#include "Transport.h"
#include "Sequence.h"
#include "MidiOut.h"
#include "Arpeggiator.h"
#include "NotePool.h"

class Track : public TransportDelegate {
public:
    void initialize(byte trackIndex, MidiOut* midiOut) {
        this->midiOut = midiOut;
        this->trackIndex = trackIndex;

        playedNotes.initialize(midiOut, trackIndex, &settings);
        holdedNotes.initialize(midiOut, trackIndex, &settings);
    }
    
    void tick() {
        transport.tick();
    }
    
    void playNote(byte note, byte velocity) {
        if (settings.isSelected) {
            bool isPlaying = transport.getIsPlaying();
            if (settings.useArp() && isPlaying) {
                if (velocity) {
                    arp.addNote(note);
                } else {
                    arp.removeNote(note);
                }
            } else {
                if (settings.isRecording && isPlaying && !eraserState) {
                    bool playNote = false;
                    if (velocity) {
                        int step = transport.getRecStep(&playNote); 
                        if (settings.modeIsPoly()) {
                            sequence[step].add(note, settings.modeIsHold());
                        } else {
                            sequence[step].set(note);
                        }
                        if (playNote) {
                            playedNotes.add(note);
                        }
                    }
                    
                    
                } else {
                    midiOut->sendNote(trackIndex, settings.channelOut, note, velocity);
                }
            }
        }
    }
    
    virtual void didChangeStep (int newStep) override {
        playedNotes.sendNotesOff();

        auto& step = sequence[newStep];

        if (eraserState) {
          step.clear();
        }

        //play notes from arpeggiator
        if (settings.useArp() && !eraserState) {
            byte arpNote = arp.getNote();
            if (arpNote) {
              if (settings.isRecording) {
                step.set(arpNote);
              } else {
                if (!settings.isMuted) {
                    playedNotes.add(arpNote);
                }
              }
            }
        }

        //play notes from sequence
        byte noteCount = step.getCount();

        bool isHold = step.isHold();

        if (noteCount > 0) {
            holdedNotes.sendNotesOff();
        }

        for (byte i = 0; i < noteCount; i++) {
            byte noteValue = step.get(i);
            
            if (!settings.isMuted) {
                if (!isHold) {
                    playedNotes.add(noteValue);
                } else {
                    holdedNotes.add(noteValue);
                }
            }
        }
    }

    void clearAllSeq() {
        sequence.clearAll();
        holdedNotes.sendNotesOff();
    }
    
    void setIsPlaying(bool isPlaying) {
        if (!isPlaying) {
            arp.eraseAll();
            holdedNotes.sendNotesOff();
            playedNotes.sendNotesOff();
        }
        transport.setIsPlaying(isPlaying);
    }
    
    void setIsRecording(bool isRecording) {
        settings.isRecording = isRecording;
    }

    void toggleIsRecording() {
        settings.isRecording = !settings.isRecording;
    }

    void resetTransport() {
        transport.willReset();
    }

    void setGroove(byte groove) {
        transport.setGroove(groove);
    }
    
    void setIsSelected(bool isSelected) {
        if (settings.isSelected && !isSelected) {
            arp.eraseAll();
            setModulationWheel(0);
        }
        if (settings.isSelected != isSelected) {
            eraserState = false;
            settings.isSelected = isSelected;
        }
    }
    
    void setChannelOut(byte channelOut) {
        settings.channelOut = channelOut;
    }
    
    void setBarCount(byte newBarCount) {
        settings.barCount = newBarCount;
    }
    
    void setStepCount(byte newStepCount) {
        settings.stepsPerBar = newStepCount;
    }
    
    void setTrackMode(TrackMode trackMode) {
        arp.eraseAll();
        settings.trackMode = trackMode;
    }

    inline void setEraserState(bool state) {
        eraserState = state;
    }

    Transport* getTransport() {
        return &transport;
    }

    TrackSettings* getSettings() {
        return &settings;
    }

    Sequence* getSequence() {
        return &sequence;
    }

    void fill() {
        sequence.fill(settings.getStepCount());
    }
    
    void setModulationWheel(byte value) {
        midiOut->sendModulationWheel(settings.channelOut, value);
    }

private:
    TrackSettings settings;
    NotePool playedNotes;
    NotePool holdedNotes;
    byte trackIndex;

    MidiOut* midiOut;

    Transport transport = Transport(&settings, this);
    Sequence sequence;
    Arpeggiator arp;
    bool eraserState = false;
};
