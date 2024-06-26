/*
 ==============================================================================
 
 Arpeggiator.h
 Created: 19 Jun 2024 11:26:35am
 Author:  Alexis ZBIK
 
 ==============================================================================
 */

#pragma once

#define MAX_KEY_PRESSED 10

class Arpeggiator {
public:
    byte list[MAX_KEY_PRESSED];
    byte count = 0;
    byte arpPos = 0;
    
    void eraseAll() {
        count = 0;
    }
    
    void addNote(byte note) {
        bool alreadyExists = false;
        if (count < MAX_KEY_PRESSED) {
            for (byte i = 0 ; i < count; i++) {
                if (list[i] == note) {
                    alreadyExists = true;
                }
            }
            if (!alreadyExists) {
                list[count] = note;
                count++;
            }
        }
    }
    
    void removeNote(byte note) {
        bool pop = false;
        for (byte i = 0 ; i < count; i++) {
            if (list[i] == note) {
                pop = true;
            }
            if (pop && (i+1) < count) {
                list[i] = list[i+1];
            }
        }
        if (pop) {
            count--;
        }
    }
    
    byte getNote() {
        if (count == 0) {
            return 0;
        }
        byte pos = arpPos;
        arpPos++;
        if (arpPos >= count) {
            arpPos = 0;
        }
        while (pos >= count && pos > 0) {
            pos--;
        }
        return list[pos];
    }
};
