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

class Track {
public:
    void initialize(byte trackIndex, MidiOut* midiOut, Transport* trans) {
        this->midiOut = midiOut;
        this->trackIndex = trackIndex;
        this->transport = trans;

        playedNotes.initialize(midiOut, trackIndex, &settings);
        holdedNotes.initialize(midiOut, trackIndex, &settings);
    }
    
    void playNote(byte note, byte velocity) {
        if (settings.isSelected) {
            bool isPlaying = transport->getIsPlaying();
            if (settings.useArp() && isPlaying) {
                if (velocity) {
                    arp.addNote(note);
                } else {
                    arp.removeNote(note);
                }
            } else {
                if (transport->getRecordingState() && isPlaying && !eraserState) {
                    bool playNote = false;
                    if (velocity) {
                        int step = transport->getRecStep(&playNote); 
                        if (settings.modeIsPoly()) {
                            sequence[step].add(note, settings.modeIsHold());
                        } else {
                            sequence[step].set(note);
                        }
                        if (playNote) {
                            playedNotes.load(note);
                        }
                    }
                    
                    
                } else {
                    midiOut->sendNote(trackIndex, settings.channelOut, note, velocity); //Send note right now
                }
            }
        }
    }
    
    void didChangeStep (int newStep) {
        playedNotes.sendNotesOff();

        auto& step = sequence[newStep];

        if (eraserState) {
          step.clear();
        }

        //play notes from arpeggiator
        if (settings.useArp() && !eraserState) {
            byte arpNote = arp.getNote();
            if (arpNote) {
              if (transport->getRecordingState()) {
                step.set(arpNote);
              } else {
                if (!settings.isMuted) {
                    playedNotes.load(arpNote);
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
                    playedNotes.load(noteValue);
                } else {
                    holdedNotes.load(noteValue);
                }
            }
        }
    }

    void clearAllSeq() {
        sequence.clearAll();
        holdedNotes.sendNotesOff();
    }

    void processNotesOn() {
        playedNotes.processNotesOn();
        holdedNotes.processNotesOn();
    }
    
    void setIsPlaying(bool isPlaying) {
        if (!isPlaying) {
            arp.eraseAll();
            holdedNotes.sendNotesOff();
            playedNotes.sendNotesOff();
        }
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
    
    void setTrackMode(TrackMode trackMode) {
        arp.eraseAll();
        settings.trackMode = trackMode;
    }

    inline void setEraserState(bool state) {
        eraserState = state;
    }

    TrackSettings* getSettings() {
        return &settings;
    }

    Sequence* getSequence() {
        return &sequence;
    }

    void fill() {
        sequence.fill(transport->getStepCount());
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

    Transport* transport = nullptr;
    Sequence sequence;
    Arpeggiator arp;
    bool eraserState = false;
};
