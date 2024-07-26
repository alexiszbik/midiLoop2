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
                        sequence[step].add(note);
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

    void sendPlayedNoteOff() {
      for (byte i = 0; i < playedNote.getSize(); i++) {
        byte note = playedNote.get(i);
        midiOut->sendNote(trackIndex, settings.channelOut, note, 0);
      }

      playedNote.clear();
      
    }
    
    virtual void didChangeStep (int newStep) override {
        sendPlayedNoteOff();

        if (eraserState) {
          sequence[newStep].clear();
        }

        if (settings.useArp() && !eraserState) {
            byte arpNote = arp.getNote();
            if (arpNote) {
              if (settings.isRecording) {
                sequence[newStep].set(arpNote);
              } else {
                midiOut->sendNote(trackIndex, settings.channelOut, arpNote, MAX_VELOCITY);
                playedNote.add(arpNote);
              }
            }
        }

        byte noteCount = sequence[newStep].getCount();
        
        for (byte i = 0; i < noteCount; i++) {
          byte noteValue = sequence[newStep].get(i);
          midiOut->sendNote(trackIndex, settings.channelOut, noteValue, MAX_VELOCITY);
          playedNote.add(noteValue);
        }
        
    }

    void clearAllSeq() {
        sequence.clearAll();
    }
    
    void setIsPlaying(bool isPlaying) {
        if (!isPlaying) {
            arp.eraseAll();
            sendPlayedNoteOff();
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
    NotePool playedNote = NotePool(10);
    byte trackIndex;
    
private:
    MidiOut* midiOut;
    TrackSettings settings;
    Transport transport = Transport(&settings, this);
    Sequence sequence;
    Arpeggiator arp;
    bool eraserState = false;
};
