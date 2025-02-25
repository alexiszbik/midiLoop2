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
                    int step = transport.getRecStep();
                    if (velocity) {
                      if (settings.modeIsPoly()) {
                        sequence[step].add(note, settings.modeIsHold());
                      } else {
                        sequence[step].set(note);
                      }
                    }
                } else {
                    midiOut->sendNote(trackIndex, settings.channelOut, note, velocity);
                }
            }
        }
    }

    void sendNotesOff(NotePool* pool) {
        for (byte i = 0; i < pool->getSize(); i++) {
        byte note = pool->get(i);
        midiOut->sendNote(trackIndex, settings.channelOut, note, 0);
      }
      pool->clear();
    }
    
    virtual void didChangeStep (int newStep) override {
        sendNotesOff(&playedNotes);

        auto& step = sequence[newStep];

        if (eraserState) {
          step.clear();
        }

        if (settings.useArp() && !eraserState) {
            byte arpNote = arp.getNote();
            if (arpNote) {
              if (settings.isRecording) {
                step.set(arpNote);
              } else {
                if (!settings.isMuted) {
                    midiOut->sendNote(trackIndex, settings.channelOut, arpNote, MAX_VELOCITY);
                    playedNotes.add(arpNote);
                }
              }
            }
        }

        byte noteCount = step.getCount();

        bool isHold = step.isHold();

        if (noteCount > 0) {
            sendNotesOff(&holdedNotes);
        }

        for (byte i = 0; i < noteCount; i++) {
            byte noteValue = step.get(i);
            if (!settings.isMuted) {
                midiOut->sendNote(trackIndex, settings.channelOut, noteValue, MAX_VELOCITY);
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
        sendNotesOff(&holdedNotes);
    }
    
    void setIsPlaying(bool isPlaying) {
        if (!isPlaying) {
            arp.eraseAll();
            sendNotesOff(&playedNotes);
            sendNotesOff(&holdedNotes);
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

    void setEraserState(bool state) {
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
    NotePool playedNotes;
    NotePool holdedNotes;
    byte trackIndex;

    MidiOut* midiOut;
    TrackSettings settings;
    Transport transport = Transport(&settings, this);
    Sequence sequence;
    Arpeggiator arp;
    bool eraserState = false;
};
