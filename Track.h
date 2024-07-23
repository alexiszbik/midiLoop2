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
                        sequence[step] = note;
                    }
                } else {
                    midiOut->sendNote(trackIndex, settings.channelOut, note, velocity);
                }
            }
        }
    }
    
    virtual void didChangeStep (int newStep) override {
        if (lastNote) {
            midiOut->sendNote(trackIndex, settings.channelOut, lastNote, 0);
            lastNote = 0;
        }

        if (eraserState) {
          sequence[newStep] = 0;
        }
        
        byte noteValue = sequence[newStep];

        if (settings.useArp() && !eraserState) {
            byte arpNote  = arp.getNote();
            if (arpNote) {
              noteValue = arpNote;
            }

            if (settings.isRecording && noteValue) {
                sequence[newStep] = noteValue;
            }
        }
        
        if (noteValue) {
            midiOut->sendNote(trackIndex, settings.channelOut, noteValue, MAX_VELOCITY);
            lastNote = noteValue;
        }
    }

    void clearAllSeq() {
        sequence.clearAll();
    }
    
    void setIsPlaying(bool isPlaying) {
        if (!isPlaying) {
            arp.eraseAll();
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
    
private:
    byte lastNote = 0;
    byte trackIndex;
    
private:
    MidiOut* midiOut;
    TrackSettings settings;
    Transport transport = Transport(&settings, this);
    Sequence sequence;
    Arpeggiator arp;
    bool eraserState = false;
};
