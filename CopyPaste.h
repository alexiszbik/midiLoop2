class CopyPaste {
public:
    void toggle(Track* track) {
        if (!src) {
            copy(track);
        } else {
            paste(track);

            src = nullptr;
        }
    }

    void copy(Track* track) {
        src = track->getSequence();
    }

    void paste(Track* track) {
        if (src) {
            Sequence* dest = track->getSequence();
            dest->copy(src);
        }
    }

private:
    Sequence* src = nullptr;
};