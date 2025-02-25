class CopyPaste {
public:
    void toggle(Track* track) {
        if (!src) {
            src = track->getSequence();
        } else {
            Sequence* dest = track->getSequence();
            dest->copy(src);

            src = nullptr;
        }
    }
private:
    Sequence* src = nullptr;
};